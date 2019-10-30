#include "scene.h"
#include "sceneobject.h"
#include "light.h"
#include "helpers3d.h"
#include "cameracontroller.h"
#include "frameratecalculator.h"
#include "properties.h"

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <cmath>

Scene::Scene(Qt3DExtras::Qt3DWindow *window, float cell, float width, float height, float depth, const QString &name):
    Qt3DCore::QEntity(nullptr),
    m_SelectedEntity(nullptr),
    m_CellSize(cell),
    m_Height(height),
    m_Width(width),
    m_Depth(depth)
{
    applyEntityName(this, "scene", name);
    window->setRootEntity(this);

    auto w = cell * width; auto h = cell * height; auto d = cell * depth;

    m_Box = createEntityBox(QVector3D(0.0, 0.0, 0.0) + BOX_EXCESS, QVector3D(w, h, d) - BOX_EXCESS, COLOR_SCENE_BOX, this);
    m_Box->setEnabled(config->DrawSceneBoxes());

    m_FRC = new FrameRateCalculator(FRAME_RATE_COUNT_CALC, this);

    auto camera_farplane = static_cast<float>(sqrt(pow(static_cast<double>(w), 2) + pow(static_cast<double>(h), 2)+ pow(static_cast<double>(d), 2)));
    m_Camera = window->camera();
    m_Camera->lens()->setPerspectiveProjection(45.0f, static_cast<float>(window->width()) / window->height(), 0.1f, camera_farplane);

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
    m_FrameAction = new Qt3DLogic::QFrameAction(this);
    QObject::connect(m_FrameAction, &Qt3DLogic::QFrameAction::triggered, this, &Scene::slotFrameActionTriggered);

    QObject::connect(this, &QObject::destroyed, [=](QObject* o){ qDebug() << o->objectName() << ": destroyed"; });
    qDebug() << objectName() << ": Scene created";
}

void Scene::addLight(Qt3DCore::QTransform *transform,
                     Qt3DRender::QAbstractLight *light,
                     const QString &name)
{
    auto mesh = new Qt3DExtras::QSphereMesh;
    mesh->setRadius(m_CellSize / 2);
    mesh->setSlices(20);
    mesh->setRings(20);

    auto material = new Qt3DExtras::QPhongMaterial;
    material->setAmbient(light->color());
    material->setShininess(light->intensity());

    auto l = new Light(this,mesh, material, transform, light);
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
        l->deleteLater();
        qDebug() << objectName() << ": Lights count" << m_Lights.count();
        emit signalLightsCountChanged(m_Lights.count());
        return true;
    }
    return false;
}

SceneObject* Scene::addEntity(Qt3DRender::QGeometryRenderer *geometry,
                              Qt3DRender::QMaterial *material,
                              Qt3DCore::QTransform *transform,
                              const QString &name)
{

    auto entity = new SceneObject(this, geometry, material, transform);
    applyEntityName(entity, "object", name);

    delEntity(entity->objectName());
    m_Objects.insert(entity->objectName(), entity);

    QObject::connect(entity, &SceneObject::signalClicked, this, &Scene::slotEntityClicked, Qt::DirectConnection);

    qDebug() << objectName() << ": Entity created, count =" << m_Objects.count();
    emit signalEntitiesCountChanged(m_Objects.count());
    return entity;
}

bool Scene::delEntity(const QString &name)
{
    auto entity = m_Objects.take(name);
    if(entity)
    {
        if(m_SelectedEntity == entity) m_SelectedEntity = nullptr;
        entity->deleteLater();
        emit signalSelectedEntityChanged(m_SelectedEntity);
        emit signalEntitiesCountChanged(m_Objects.count());
        return true;
    }
    return false;
}

void Scene::slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name)
{
    auto e = EntityByName(name);
    if(!e) { qCritical() << objectName() << "(" << __func__ << "): Entity is empty"; return; }

    if(event->button() == Qt3DRender::QPickEvent::Buttons::LeftButton)
    {
        SelectEntity(e);
    }
    else if(event->button() == Qt3DRender::QPickEvent::Buttons::MiddleButton)
    {
        qDebug() << "Mouse button: MiddleButton";
    }
    else if(event->button() == Qt3DRender::QPickEvent::Buttons::RightButton)
    {
        qDebug() << "Mouse button: RightButton";
        // test
        auto cm = new Qt3DExtras::QCuboidMesh;
        cm->setXExtent(20);
        cm->setYExtent(20);
        cm->setZExtent(40);
        e->applyGeometry(cm);
    }
}

void Scene::SelectEntity(SceneObject *entity)
{
    if(!entity) { qCritical() << objectName() << "(" << __func__ << "): Entity is empty"; return; }

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

SceneObject *Scene::EntityByName(const QString &name)
{
    auto e = Entities().value(name);
    if(!e) { qDebug() << ": Entity <" << name << "> not found"; return nullptr; }
    return  e;
}

bool Scene::delEntity(SceneObject *entity) { return delEntity(entity->objectName()); }
float Scene::CellSize() const { return m_CellSize; }
QVector3D Scene::Size() const { return QVector3D(m_Width, m_Height, m_Depth); }
QVector3D Scene::RealSize() const { return m_CellSize * QVector3D(m_Width, m_Height, m_Depth); }
FrameRateCalculator *Scene::FRC() const { return m_FRC; }
SceneObject *Scene::SelectedEntity() const { return m_SelectedEntity; }
QHash<QString, SceneObject* > Scene::Entities() const { return m_Objects; }
QHash<QString, Light* > Scene::Lights() const { return m_Lights; }
