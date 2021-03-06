#include "scene.h"
#include "sceneobject.h"
#include "guientity.h"
#include "light.h"
#include "material.h"
#include "helpers3d.h"
#include "frameratecalculator.h"
#include "properties.h"

#include <QDir>
#include <cmath>

#include <Qt3DRender/QMesh>

Scene::Scene(SceneView *view,
             float cell,
             float width, float height, float depth,
             const QString &name):
    Qt3DCore::QEntity(nullptr),
    m_View(view),
    m_FrameAction(nullptr),
    m_SelectedEntity(nullptr),
    m_FRC(nullptr),
    m_CellSize(abs(ceilf(cell))),
    m_Height(abs(ceilf(height))),
    m_Width(abs(ceilf(width))),
    m_Depth(abs(ceilf(depth))),
    m_LoadingSteps(0)
{
    applyEntityName(this, "scene", name);

    //    auto m_SkyBox = new Qt3DExtras::QSkyboxEntity(this);
    //    m_SkyBox->setGammaCorrectEnabled(true);
    //    m_SkyBox->setBaseName(config->PathAssetsDir() + QDir::separator() + QStringLiteral("default_sky"));
    //    m_SkyBox->setExtension(QStringLiteral(".png"));
    //    auto skytrfm = new Qt3DCore::QTransform();
    //    skytrfm->setTranslation(QVector3D( 0.0f, 0.0f, 0.0f));
    //    skytrfm->setScale3D(QVector3D(SCENE_WIDTH, SCENE_HEIGHT, SCENE_DEPTH));
    //    m_SkyBox->addComponent(skytrfm);

    m_Box = createEntityBox(this,
                            QVector3D(0.0, 0.0, 0.0) + QVector3D(config->SceneExcess(),
                                                                 config->SceneExcess(),
                                                                 config->SceneExcess()),
                            RealSize() - QVector3D(config->SceneExcess(),
                                                   config->SceneExcess(),
                                                   config->SceneExcess()),
                            config->SceneColorBox());
    m_Box->addComponent(m_View->OpaqueLayer());
    m_Box->setObjectName("SceneBox");

    auto grid = createEntityBottomGrid(m_Box,
                                       QVector3D(0.0, 0.0, 0.0),
                                       QVector3D(RealSize().x(), 0.0, RealSize().z()), m_CellSize,
                                       config->SceneColorGrid());

    grid->setObjectName("SceneGrid");
    grid->addComponent(m_View->OpaqueLayer());

    m_EntityBox = new EntityBox(this, config->SceneExcess(), config->SceneColorSelect());
    m_EntityBox->addComponentToDeep(m_View->InterfaceLayer());
    m_EntityBox->setEnabled(false);

    // test
    m_InterfaceText1 = new EntityText(this, 10, "");
    m_InterfaceText1->setEnabled(false);
    m_InterfaceText1->Transform()->setTranslation(m_CellSize * QVector3D( 0.0f, m_Height  - 5,  0.0f));
    m_InterfaceText1->addComponentToDeep(m_View->InterfaceLayer());
    //

    QObject::connect(this, &QObject::destroyed, [=](QObject* o){ qDebug() << o->objectName() << ": destroyed"; });
    qInfo() << objectName() << ": Scene created";
}

void Scene::slotLoaded()
{
    m_LoadingSteps++;
    if(m_LoadingSteps >= LOADING_STEPS)
    {
        m_FRC = new FrameRateCalculator(FRAME_RATE_PERIOD, this);
        m_FrameAction = new Qt3DLogic::QFrameAction(this);

        QObject::connect(config, &Config::signalRendererSortPolicyType, m_View, &SceneView::slotRenderSortPolicyType);
        QObject::connect(config, &Config::signalRendererCullFaceMode, m_View, &SceneView::slotCullFace);
        QObject::connect(m_FrameAction, &Qt3DLogic::QFrameAction::triggered, this, &Scene::slotFrameActionTriggered);
        QObject::connect(config, &Config::signalDrawSceneBoxes, this, &Scene::slotShowBoxes);

        qInfo() << objectName() << ": Resources loaded";
        Q_EMIT signalLoaded();
    }
}

void Scene::load()
{
    QObject::connect(this, &Scene::signalMaterialsLoaded, this, &Scene::slotLoaded);
    QObject::connect(this, &Scene::signalGeometriesLoaded, this, &Scene::slotLoaded);

    loadMaterials({MATERIAL_EXTENSION});
    loadGeometries({GEOMETRY_EXTENSION});
}

void Scene::loadGeometry(const QString &path)
{
    QFileInfo fi(path);
    if(!fi.exists()){ qCritical() << objectName() << "(" << __func__ << "): Wrong path:" << path;  return; }

    auto name = fi.baseName();
    auto geometry = m_Geometries.take(name);
    if(geometry) geometry->deleteLater();
    auto mesh = new Qt3DRender::QMesh(this);
    mesh->setObjectName(name);

    auto conn = std::make_shared<QMetaObject::Connection>();
    auto func = [=](Qt3DRender::QMesh::Status s)
    {
        if(s == Qt3DRender::QMesh::Ready)
        {
            m_Geometries.insert(name, mesh);
            qInfo() << objectName() << ": Geometry loaded" << name;
            Q_EMIT signalGeometryLoaded(name);
            qDebug() << "Qt3DRender::QMesh::statusChanged disconnection:" << QObject::disconnect(*conn);
        }
        else if(s == Qt3DRender::QMesh::Error)
        { 
            qCritical() << objectName() << ": Error at geometry loading" << name;
            Q_EMIT signalGeometryLoaded(name);
            qDebug() << "Qt3DRender::QMesh::statusChanged disconnection:" << QObject::disconnect(*conn);
        }
        else
        { qDebug() << objectName() << ": Geometry" << name << "loading status:" << s; }
    };
    *conn = QObject::connect(mesh, &Qt3DRender::QMesh::statusChanged, func);
    QObject::connect(mesh, &QObject::destroyed, [=](QObject* o){ qDebug() << objectName() << ": Geometry" << o->objectName() << "destroyed"; });
    mesh->setSource(QUrl::fromLocalFile(path));
}

void Scene::loadGeometries(const QStringList &filters)
{
    QDir resdir(config->PathAssetsDir());
    if(!resdir.exists())
    {
        qCritical() << "Path not exist:" << config->PathAssetsDir();
        Q_EMIT signalGeometriesLoaded(0);
        return;
    }

    auto fileList = resdir.entryList(filters, QDir::Files);
    if(fileList.count() <= 0)
    {
        Q_EMIT signalGeometriesLoaded(0);
        return;
    }

    auto conn = std::make_shared<QMetaObject::Connection>();
    auto func = [=]()
    {
        if(m_Geometries.count() == fileList.count())
        {
            qInfo() << objectName() << "All geometries loaded:" << m_Geometries.count();
            Q_EMIT signalGeometriesLoaded(m_Geometries.count());
            qDebug() << "Scene::signalGeometryLoaded disconnection:" << QObject::disconnect(*conn);
        }
    };
    *conn = QObject::connect(this, &Scene::signalGeometryLoaded, func);

    for(QString f: fileList)
        loadGeometry(config->PathAssetsDir() + QDir::separator() + f);
}

void Scene::slotMaterialReady(Material* material)
{
    auto m = m_Materials.take(material->objectName());
    if(m) m->deleteLater();
    m_Materials.insert(material->objectName(), material);

    qInfo() << objectName() << ": Material loaded" << material->objectName();
    Q_EMIT signalMaterialLoaded(material->objectName());
    QObject::disconnect(material, &Material::signalReady, this, &Scene::slotMaterialReady);
}

void Scene::loadMaterial(const QString& path)
{
    auto material = new Material(this);
    QObject::connect(material, &Material::signalReady, this, &Scene::slotMaterialReady);
    material->load(path);
}

void Scene::loadMaterials(const QStringList& filters)
{
    QDir resdir(config->PathAssetsDir());
    if(!resdir.exists())
    {
        qCritical() << "Path not exist:" << config->PathAssetsDir();
        Q_EMIT signalMaterialsLoaded(0);
        return;
    }

    auto fileList = resdir.entryList(filters, QDir::Files);
    if(fileList.count() <= 0)
    {
        Q_EMIT signalMaterialsLoaded(0);
        return;
    }

    auto conn = std::make_shared<QMetaObject::Connection>();
    auto func = [=]()
    {
        if(m_Materials.count() == fileList.count())
        {
            qInfo() << objectName() << "All materials loaded:" << m_Materials.count();
            Q_EMIT signalMaterialsLoaded(m_Materials.count());
            qDebug() << "Scene::signalMaterialLoaded disconnection:" << QObject::disconnect(*conn);
        }
    };
    *conn = QObject::connect(this, &Scene::signalMaterialLoaded, func);

    for(QString f: fileList)
        loadMaterial(config->PathAssetsDir() + QDir::separator() + f);
}

Light* Scene::addLight(Qt3DRender::QAbstractLight *light,
                       const QString &name)
{
    auto l = new Light(this, light);
    applyEntityName(l, "light", name);
    l->addComponent(m_View->OpaqueLayer());
    l->slotShowGeometry(config->DrawSceneBoxes());

    delLight(l->objectName());
    m_Lights.insert(l->objectName(), l);

    qInfo() << l->objectName() << ": Light added, count" << m_Lights.count();
    Q_EMIT signalLightChanged(name);

    return l;
}

bool Scene::delLight(const QString &name)
{
    auto light = m_Lights.take(name);
    if(light)
    {
        deleteEntity(light);
        if(m_SelectedEntity == light) m_SelectedEntity = nullptr;
        m_EntityBox->setEnabled(false);
        Q_EMIT signalSelectedEntity(m_SelectedEntity);
        Q_EMIT signalLightChanged(name);
        qInfo() << objectName() << ": Lights count" << m_Lights.count();
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

    applyEntityRenderLayer(entity);
    delObject(entity->objectName());
    m_Objects.insert(entity->objectName(), entity);

    qInfo() << objectName() << ": Entity created, count" << m_Objects.count();
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
        deleteEntity(entity);
        if(m_SelectedEntity == entity) m_SelectedEntity = nullptr;
        m_EntityBox->setEnabled(false);
        Q_EMIT signalSelectedEntity(m_SelectedEntity);
        Q_EMIT signalObjectChanged(name);
        qInfo() << objectName() << ": Objects count" << m_Objects.count();
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

    if(m_SelectedEntity == entity)
    {
        m_EntityBox->setEnabled(false);
        m_SelectedEntity = nullptr;
    }
    else
    {
        m_EntityBox->applyToEntity(entity);
        m_SelectedEntity = entity;
    }

    // test
    if(m_SelectedEntity)
    {
        m_InterfaceText1->setText(m_SelectedEntity->objectName());
        m_InterfaceText1->setEnabled(true);
    }
    else
    {
        m_InterfaceText1->setText(" ");
        m_InterfaceText1->setEnabled(false);
    }

    Q_EMIT signalSelectedEntity(m_SelectedEntity);
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
    if(m_SelectedEntity == entity) m_EntityBox->applyToEntity(entity);
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
    if(m_SelectedEntity == entity) m_EntityBox->applyToEntity(entity);
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
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return; }

    auto gr = Geometries().value(name);
    if(!gr){ qCritical() << objectName() << "(" << __func__ << "): Wrong geometry name:" << name;  return; }

    // события maxExtentChanged и minExtentChanged не всегда вызываются, на всякий случай сбрасываем соединения
    qDebug() << "Qt3DRender::QGeometry::maxExtentChanged preventive disconnection:"
            << gr->disconnect(gr->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr);
    qDebug() << "Qt3DRender::QGeometry::minExtentChanged preventive disconnection:"
            << gr->disconnect(gr->geometry(), &Qt3DRender::QGeometry::minExtentChanged, nullptr, nullptr);

    auto conn1 = std::make_shared<QMetaObject::Connection>();
    auto conn2 = std::make_shared<QMetaObject::Connection>();
    auto func1 = [=]()
    {
        if(m_SelectedEntity == entity) m_EntityBox->applyToEntity(entity);
        qDebug() << "Qt3DRender::QGeometry::maxExtentChanged disconnection:" << QObject::disconnect(*conn1);
    };
    auto func2 = [=]()
    {
        if(m_SelectedEntity == entity) m_EntityBox->applyToEntity(entity);
        qDebug() << "Qt3DRender::QGeometry::minExtentChanged disconnection:" << QObject::disconnect(*conn2);
    };
    *conn1 = QObject::connect(gr->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, func1);
    *conn2 = QObject::connect(gr->geometry(), &Qt3DRender::QGeometry::minExtentChanged, func2);

    entity->applyGeometry(gr);
}

void Scene::setEntityMaterial(SceneEntity *entity, const QString &name)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return; }

    auto m = Materials().value(name);
    if(!m) { qCritical() << objectName() << "(" << __func__ << "): Wrong material name" << name;  return; }

    entity->applyMaterial(m);
    applyEntityRenderLayer(entity);
}

void Scene::applyEntityRenderLayer(SceneEntity *entity)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity"; return; }

    auto material = entity->Material();
    if(!material) { qCritical() << objectName() << "(" << __func__ << "): Wrong entity material";  return; }

    auto property = material->property("Transparent");
    if(!property.isValid()) { qCritical() << objectName() << "(" << __func__ << "): 'Transparent' property is absent";  return; }

    auto layer = property.toBool() ? m_View->TransparentLayer() : m_View->OpaqueLayer();

    QVector<Qt3DCore::QComponent*> vc;
    for(Qt3DCore::QComponent* c: entity->components())
        if(qobject_cast<Qt3DRender::QLayer*>(c)) vc.append(c);

    entity->addComponent(layer);
    for(Qt3DCore::QComponent* c: vc)
    {
        if(c == layer) continue; // тот же самый
        entity->removeComponent(c);
    }
    qInfo() << entity->objectName() << ": RenderLayer applied" << layer->objectName();
}

void Scene::slotFrameActionTriggered(float dt)
{
    Q_UNUSED(dt)
    m_FRC->calculate();
}

void Scene::slotShowBoxes(bool value)
{
    for(Light* l: m_Lights) l->slotShowGeometry(value);

    m_Box->setEnabled(value);
}

float Scene::CellSize() const { return m_CellSize; }
QVector3D Scene::Size() const { return QVector3D(m_Width, m_Height, m_Depth); }
QVector3D Scene::RealSize() const { return m_CellSize * QVector3D(m_Width, m_Height, m_Depth); }
FrameRateCalculator *Scene::FRC() const { return m_FRC; }
SceneEntity *Scene::SelectedEntity() const { return m_SelectedEntity; }
QHash<QString, SceneObject* > Scene::Objects() const { return m_Objects; }
QHash<QString, Light* > Scene::Lights() const { return m_Lights; }
QHash<QString, Qt3DRender::QGeometryRenderer *> Scene::Geometries() const { return m_Geometries; }
QHash<QString, Qt3DRender::QMaterial* > Scene::Materials() const { return m_Materials; }
float Scene::Depth() const { return m_Depth; }
float Scene::Width() const { return m_Width; }
float Scene::Height() const { return m_Height; }
SceneView *Scene::View() const { return m_View; }
