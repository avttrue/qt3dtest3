#include "scene.h"
#include "sceneobject.h"
#include "light.h"
#include "material.h"
#include "helpers3d.h"
#include "cameracontroller.h"
#include "frameratecalculator.h"
#include "properties.h"

#include <QDir>
#include <QFileInfo>
#include <cmath>

#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QMesh>

Scene::Scene(Qt3DExtras::Qt3DWindow *window,
             float cell,
             float width, float height, float depth,
             const QString &name):
    Qt3DCore::QEntity(nullptr),
    m_SelectedEntity(nullptr),
    m_Box(nullptr),
    m_CellSize(abs(ceilf(cell))),
    m_Height(abs(ceilf(height))),
    m_Width(abs(ceilf(width))),
    m_Depth(abs(ceilf(depth)))
{
    applyEntityName(this, "scene", name);

    auto w = cell * width; auto h = cell * height; auto d = cell * depth;

    m_Camera = window->camera();
    m_CameraFarPlane = static_cast<float>(sqrt(pow(static_cast<double>(w), 2) +
                                               pow(static_cast<double>(h), 2) +
                                               pow(static_cast<double>(d), 2)));
    auto camera_aspect = static_cast<float>(window->width()) / window->height();
    m_Camera->lens()->setPerspectiveProjection(45.0f, camera_aspect, 0.1f, m_CameraFarPlane);

    m_Camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    m_Camera->setPosition(QVector3D(w, h, d) - BOX_EXCESS);
    m_Camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    m_CameraController = new CameraController(this);
    m_CameraController->setCamera(m_Camera);

    /* обещают исправить в 5.14
    m_SkyBox = new Qt3DExtras::QSkyboxEntity(this);
    m_SkyBox->setGammaCorrectEnabled(true);
    m_SkyBox->setBaseName(config->PathAssetsDir() + QDir::separator() + QStringLiteral("default_sky"));
    m_SkyBox->setExtension(QStringLiteral(".png"));
    auto skytrfm = new Qt3DCore::QTransform();
    skytrfm->setTranslation(QVector3D( 0.0f, 0.0f, 0.0f));
    skytrfm->setScale3D(QVector3D(SCENE_WIDTH, SCENE_HEIGHT, SCENE_DEPTH));
    m_SkyBox->addComponent(skytrfm);
    */

    loadMaterials();
    loadGeometries();

    m_FrameAction = new Qt3DLogic::QFrameAction(this);
    QObject::connect(m_FrameAction, &Qt3DLogic::QFrameAction::triggered, this, &Scene::slotFrameActionTriggered);
    QObject::connect(this, &QObject::destroyed, [=](QObject* o){ qDebug() << o->objectName() << ": destroyed"; });

    m_FRC = new FrameRateCalculator(FRAME_RATE_COUNT_CALC, this);
    qDebug() << objectName() << ": Scene created";
}

Light* Scene::addLight(Qt3DRender::QAbstractLight *light,
                       const QString &name)
{
    auto l = new Light(this, light);
    applyEntityName(l, "light", name);
    l->slotShowGeometry(config->DrawSceneBoxes());

    delLight(l->objectName());
    m_Lights.insert(l->objectName(), l);

    qDebug() << l->objectName() << ": Light added, count" << m_Lights.count();
    Q_EMIT signalLightChanged(name);

    return l;
}

bool Scene::delLight(const QString &name)
{
    auto l = m_Lights.take(name);
    if(l)
    {
        if(m_SelectedEntity == l) m_SelectedEntity = nullptr;
        l->deleteLater();
        Q_EMIT signalSelectedEntityChanged(m_SelectedEntity);
        Q_EMIT signalLightChanged(name);
        qDebug() << objectName() << ": Lights count" << m_Lights.count();
        return true;
    }
    return false;
}

bool Scene::delLight(SceneEntity* entity)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong light"; return false; }
    return delLight(entity->objectName());
}

SceneObject* Scene::addObject(Qt3DRender::QGeometryRenderer *geometry,
                              Qt3DRender::QMaterial *material,
                              const QString &name)
{
    auto entity = new SceneObject(this, geometry, material, name);

    delObject(entity->objectName());
    m_Objects.insert(entity->objectName(), entity);

    qDebug() << objectName() << ": Entity created, count" << m_Objects.count();
    Q_EMIT signalObjectChanged(name);

    return entity;
}

SceneObject *Scene::addObject(const QString &geometry,
                              const QString &material,
                              const QString &name)
{
    auto gr = m_Geometries.value(geometry);
    if(!gr) { qCritical() << objectName() << "Geonetry not exists:" << geometry; return nullptr; }

    auto mat = m_Materials.value(material);
    if(!mat) { qCritical() << objectName() << "Material not exists:" << material; return nullptr; }

    return addObject(gr, mat, name);
}

bool Scene::delObject(const QString &name)
{
    auto entity = m_Objects.take(name);
    if(entity)
    {
        if(m_SelectedEntity == entity) m_SelectedEntity = nullptr;
        entity->deleteLater();
        Q_EMIT signalSelectedEntityChanged(m_SelectedEntity);
        Q_EMIT signalObjectChanged(name);
        qDebug() << objectName() << ": Objects count" << m_Objects.count();
        return true;
    }
    return false;
}

bool Scene::delObject(SceneEntity *entity)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong Entity"; return false; }
    return delObject(entity->objectName());
}

void Scene::SelectEntity(SceneEntity *entity)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong Entity"; return; }
    if(!m_SelectedEntity)
    {
        entity->slotSelect(true);
        m_SelectedEntity = entity;
    }
    else
    {
        if(m_SelectedEntity == entity)
        {
            m_SelectedEntity->slotSelect(false);
            m_SelectedEntity = nullptr;
        }
        else
        {
            m_SelectedEntity->slotSelect(false);
            entity->slotSelect(true);
            m_SelectedEntity = entity;
        }
    }
    Q_EMIT signalSelectedEntityChanged(m_SelectedEntity);
}

QString Scene::EntityGeometry(SceneEntity *entity) const
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return ""; }
    return m_Geometries.key(entity->Geometry());
}

QString Scene::EntityMaterial(SceneEntity *entity) const
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return ""; }
    return m_Materials.key(entity->Material());
}

void Scene::setEntityPosition(SceneEntity *entity, const QVector3D &position)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return; }

    entity->applyPosition(entity->Size() + m_CellSize * position);
}

QVector3D Scene::EntityPosition(SceneEntity *entity) const
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return QVector3D(-1, -1, -1); }

    auto p = entity->Position() - entity->Size();
    return QVector3D(floorf(p.x() / m_CellSize),
                     floorf(p.y() / m_CellSize),
                     floorf(p.z() / m_CellSize));
}

void Scene::setEntitySize(SceneEntity *entity, const QVector3D &size)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return; }
    entity->applySize(m_CellSize * size / 2);
}

QVector3D Scene::EntitySize(SceneEntity *entity) const
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return QVector3D(0, 0, 0); }

    auto p = entity->Size() * 2;
    return  QVector3D(floorf(p.x() / m_CellSize),
                     floorf(p.y() / m_CellSize),
                     floorf(p.z() / m_CellSize));
}

void Scene::setEntityGeometry(SceneEntity* entity, const QString &name)
{
    auto gr = Geometries().value(name);
    if(!gr){ qCritical() << objectName() << "(" << __func__ << "): Wrong geometry name:" << name;  return; }

    entity->applyGeometry(gr);
}

void Scene::setEntityMaterial(SceneEntity *entity, const QString &name)
{
    auto m = Materials().value(name);
    if(!m) { qCritical() << objectName() << "(" << __func__ << "): Wrong material name" << name;  return; }

    entity->applyMaterial(m);
}

void Scene::slotFrameActionTriggered(float dt)
{
    Q_UNUSED(dt)
    m_FRC->calculate();
}

void Scene::slotShowBoxes(bool value)
{
    for(Light* l: m_Lights) l->slotShowGeometry(value);
    if(m_Box)
    {
        m_Box->setEnabled(false);
        m_Box->deleteLater();
        m_Box = nullptr;
    }
    if(!value) return;

    m_Box = createEntityBox(QVector3D(0.0, 0.0, 0.0) + BOX_EXCESS, RealSize() - BOX_EXCESS, COLOR_SCENE_BOX, this);
    applyEntityName(m_Box, "box", "scene_box");
    createEntityBottomGrid(QVector3D(0.0, 0.0, 0.0), QVector3D(RealSize().x(), 0.0, RealSize().z()), m_CellSize, COLOR_SCENE_GREED, m_Box);
}

void Scene::loadGeometry(const QString &path)
{
    QFileInfo fi(path);
    if(!fi.exists()){ qCritical() << objectName() << "(" << __func__ << "): Wrong path:" << path;  return; }

    auto name = fi.baseName();

    auto geometry = m_Geometries.take(name);
    if(geometry)
    {
        geometry->deleteLater();
        Q_EMIT signalGeometryChanged(geometry->objectName());
        qDebug() << objectName() << ": Geometries count" << m_Geometries.count();
    }

    Qt3DRender::QMesh* mesh = new Qt3DRender::QMesh(this);
    mesh->setObjectName(name);
    QObject::connect(mesh, &QObject::destroyed, [=](QObject* o){ qDebug() << objectName() << ": Geometry" << o->objectName() << "destroyed"; });
    mesh->setSource(QUrl::fromLocalFile(path));
    m_Geometries.insert(name, mesh);
    qDebug() << objectName() << ": Geometry loaded" << mesh->objectName();
    Q_EMIT signalGeometryChanged(name);
}

void Scene::loadGeometries()
{
    QDir resdir(config->PathAssetsDir());
    if(!resdir.exists()) { qCritical() << "Path not exist:" << config->PathAssetsDir(); return; }

    auto fileList = resdir.entryList({GEOMETRY_EXTENSION}, QDir::Files);
    if(fileList.count() <= 0) return;

    auto func = [=]()
    {
        if(m_Geometries.count() == fileList.count())
        {
            QObject::disconnect(this, &Scene::signalGeometryChanged, nullptr, nullptr);
            qDebug() << objectName() << "All geometries loaded:" << m_Geometries.count();
            // TODO: do next
        }
    };
    QObject::connect(this, &Scene::signalGeometryChanged, func);

    for(QString f: fileList)
        loadGeometry(config->PathAssetsDir() + QDir::separator() + f);
}

void Scene::loadMaterial(const QString& path)
{
    auto material = new Material(this);
    auto func = [=]()
    {
        auto m = m_Materials.take(material->objectName());
        if(m)
        {
            m->deleteLater();
            Q_EMIT signalMaterialChanged(m->objectName());
            qDebug() << objectName() << ": Materials count" << m_Materials.count();
        }

        m_Materials.insert(material->objectName(), material);
        qDebug() << objectName() << ": Material loaded" << material->objectName();
        Q_EMIT signalMaterialChanged(material->objectName());
    };
    QObject::connect(material, &Material::signalReady, func);
    material->load(path);
}

void Scene::loadMaterials()
{
    QDir resdir(config->PathAssetsDir());
    if(!resdir.exists()) { qCritical() << "Path not exist:" << config->PathAssetsDir(); return; }

    auto fileList = resdir.entryList({MATERIAL_EXTENSION}, QDir::Files);
    if(fileList.count() <= 0) return;

    auto func = [=]()
    {
        if(m_Materials.count() == fileList.count())
        {
            QObject::disconnect(this, &Scene::signalMaterialChanged, nullptr, nullptr);
            qDebug() << objectName() << "All materials loaded:" << m_Materials.count();
            // TODO: do next
        }
    };
    QObject::connect(this, &Scene::signalMaterialChanged, func);

    for(QString f: fileList)
        loadMaterial(config->PathAssetsDir() + QDir::separator() + f);
}

float Scene::CameraFarPlane() const { return m_CameraFarPlane; }
float Scene::CellSize() const { return m_CellSize; }
QVector3D Scene::Size() const { return QVector3D(m_Width, m_Height, m_Depth); }
QVector3D Scene::RealSize() const { return m_CellSize * QVector3D(m_Width, m_Height, m_Depth); }
FrameRateCalculator *Scene::FRC() const { return m_FRC; }
SceneEntity *Scene::SelectedEntity() const { return m_SelectedEntity; }
QHash<QString, SceneObject* > Scene::Objects() const { return m_Objects; }
QHash<QString, Light* > Scene::Lights() const { return m_Lights; }
QHash<QString, Qt3DRender::QGeometryRenderer *> Scene::Geometries() const { return m_Geometries; }
QHash<QString, Qt3DRender::QMaterial* > Scene::Materials() const { return m_Materials; }
