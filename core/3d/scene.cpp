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
    m_LightMesh(new Qt3DExtras::QSphereMesh(this)),
    m_CellSize(abs(cell)),
    m_Height(abs(height)),
    m_Width(abs(width)),
    m_Depth(abs(depth))
{
    applyEntityName(this, "scene", name);
    window->setRootEntity(this);

    auto w = cell * width; auto h = cell * height; auto d = cell * depth;

    m_FRC = new FrameRateCalculator(FRAME_RATE_COUNT_CALC, this);

    m_LightMesh->setRadius(m_CellSize / 2);
    m_LightMesh->setSlices(64);
    m_LightMesh->setRings(64);

    m_Camera = window->camera();
    m_CameraFarPlane = static_cast<float>(sqrt(pow(static_cast<double>(w), 2) +
                                               pow(static_cast<double>(h), 2)+
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
    qDebug() << objectName() << ": Scene created";
}

void Scene::addLight(Qt3DCore::QTransform *transform,
                     Qt3DRender::QAbstractLight *light,
                     const QString &name)
{
    auto material = new Qt3DExtras::QPhongMaterial;
    material->setAmbient(light->color());
    material->setDiffuse(light->color());
    material->setSpecular(light->color());
    material->setShininess(light->intensity() * 100);

    auto l = new Light(this, m_LightMesh, material, transform, light);
    applyEntityName(l, "light", name);
    l->slotShowGeometry(config->DrawSceneBoxes());

    delLight(l->objectName());
    m_Lights.insert(l->objectName(), l);

    qDebug() << l->objectName() << ": Light added, count" << m_Lights.count();
    emit signalLightsCountChanged(m_Lights.count());
}

bool Scene::delLight(const QString &name)
{
    auto l = m_Lights.take(name);
    if(l)
    {
        if(m_SelectedEntity == l) m_SelectedEntity = nullptr;
        l->deleteLater();
        emit signalSelectedEntityChanged(m_SelectedEntity);
        emit signalLightsCountChanged(m_Lights.count());
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
                              Qt3DCore::QTransform *transform,
                              const QString &name)
{
    auto entity = new SceneObject(this, geometry, material, transform);
    applyEntityName(entity, "object", name);

    delObject(entity->objectName());
    m_Objects.insert(entity->objectName(), entity);

    qDebug() << objectName() << ": Entity created, count" << m_Objects.count();
    emit signalObjectsCountChanged(m_Objects.count());

    return entity;
}

SceneObject *Scene::addObject(const QString &geometry,
                              const QString &material,
                              Qt3DCore::QTransform *transform,
                              const QString &name)
{
    auto gr = m_Geometries.value(geometry);
    if(!gr) { qCritical() << objectName() << "Geonetry not exists:" << geometry; return nullptr; }

    auto mat = m_Materials.value(material);
    if(!mat) { qCritical() << objectName() << "Material not exists:" << material; return nullptr; }

    return addObject(gr, mat, transform, name);
}

bool Scene::delObject(const QString &name)
{
    auto entity = m_Objects.take(name);
    if(entity)
    {
        if(m_SelectedEntity == entity) m_SelectedEntity = nullptr;
        entity->deleteLater();
        emit signalSelectedEntityChanged(m_SelectedEntity);
        emit signalObjectsCountChanged(m_Objects.count());
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
    emit signalSelectedEntityChanged(m_SelectedEntity);
}

QString Scene::EntityGeometry(SceneEntity *entity) const
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity" << config->PathAssetsDir(); return ""; }
    return m_Geometries.key(entity->Geometry());
}

QString Scene::EntityMaterial(SceneEntity *entity) const
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity" << config->PathAssetsDir(); return ""; }
    return m_Materials.key(entity->Material());
}

void Scene::setEntityCellPosition(SceneEntity *entity, const QVector3D &position)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity" << config->PathAssetsDir(); return; }
    entity->applyPosition(m_CellSize * position);
}

QVector3D Scene::EntityCellPosition(SceneEntity *entity)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity" << config->PathAssetsDir(); return QVector3D(-1.0, -1.0, -1.0); }
    auto v = entity->Position() / m_CellSize;
    return QVector3D(abs(v.x()), abs(v.x()), abs(v.z()));
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
        emit signalGeometriesCountChanged(m_Geometries.count());
        qDebug() << objectName() << ": Geometries count" << m_Geometries.count();
    }

    Qt3DRender::QMesh* mesh = new Qt3DRender::QMesh(this);
    mesh->setObjectName(name);
    QObject::connect(mesh, &QObject::destroyed, [=](QObject* o){ qDebug() << objectName() << ": Geometry" << o->objectName() << "destroyed"; });
    /* auto func = [=](Qt3DRender::QMesh::Status s)
    {
        if(s == Qt3DRender::QMesh::Status::Ready)
        {
            QObject::disconnect(mesh, &Qt3DRender::QMesh::statusChanged, nullptr, nullptr);
            m_Geometries.insert(name, mesh);
            qDebug() << objectName() << ": Geometry loaded" << name << "count" << m_Geometries.count();
            emit signalGeometryLoaded(name);
            emit signalGeometriesCountChanged(m_Geometries.count());
        }
        else if(s == Qt3DRender::QMesh::Status::Error)
        {
            QObject::disconnect(mesh, &Qt3DRender::QMesh::statusChanged, nullptr, nullptr);
            qDebug() << objectName() << "Error at geometry loading" << name;
        }
        else
        {
            qDebug() << objectName() << "Geometry" << name << "loading status:" << s;
        }
    };
    QObject::connect(mesh, &Qt3DRender::QMesh::statusChanged, func); */
    mesh->setSource(QUrl::fromLocalFile(path));
    m_Geometries.insert(name, mesh);
    qDebug() << objectName() << ": Geometry loaded" << name << "count" << m_Geometries.count();
    emit signalGeometryLoaded(name);
    emit signalGeometriesCountChanged(m_Geometries.count());
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
            QObject::disconnect(this, &Scene::signalGeometryLoaded, nullptr, nullptr);
            qDebug() << objectName() << "All geometries loaded";
            // TODO: do next
        }
    };
    QObject::connect(this, &Scene::signalGeometryLoaded, this, func, Qt::DirectConnection);

    for(QString f: fileList)
        loadGeometry(config->PathAssetsDir() + QDir::separator() + f);
}

void Scene::loadMaterial(const QString& path)
{
    auto material = new Material(this);
    material->loadCFG(path);

    auto m = m_Materials.take(material->objectName());
    if(m)
    {
        m->deleteLater();
        emit signalMaterialsCountChanged(m_Materials.count());
        qDebug() << objectName() << ": Materials count" << m_Materials.count();
    }

    m_Materials.insert(material->objectName(), material);
    qDebug() << objectName() << ": Material loaded" << material->objectName() << "count" << m_Materials.count();
    emit signalMaterialLoaded(material->objectName());
    emit signalMaterialsCountChanged(m_Materials.count());
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
            QObject::disconnect(this, &Scene::signalMaterialLoaded, nullptr, nullptr);
            qDebug() << objectName() << "All materials loaded";
            // TODO: do next
        }
    };
    QObject::connect(this, &Scene::signalMaterialLoaded, this, func, Qt::DirectConnection);

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
