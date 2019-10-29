#include "scene.h"
#include "sceneentity.h"
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
    m_Lights(QHash<QString, Light*>()),
    m_Entities(QHash<QString, SceneEntity*>()),
    m_CellSize(cell),
    m_Height(height),
    m_Width(width),
    m_Depth(depth)
{
    applyEntityName(this, "scene", name);
    window->setRootEntity(this);

    auto sizeDelta = QVector3D(0.1f, 0.1f, 0.1f);
    auto w = cell * width; auto h = cell * height; auto d = cell * depth;

    m_Box = createEntityBox(QVector3D(0.0, 0.0, 0.0) + sizeDelta, QVector3D(w, h, d) - sizeDelta, COLOR_SCENE_BOX, this);
    m_Box->setEnabled(config->DrawSceneBoxes());

    m_FRC = new FrameRateCalculator(FRAME_RATE_COUNT_CALC, this);

    auto camera_farplane = static_cast<float>(sqrt(pow(static_cast<double>(w), 2) + pow(static_cast<double>(h), 2)+ pow(static_cast<double>(d), 2)));
    m_Camera = window->camera();
    m_Camera->lens()->setPerspectiveProjection(45.0f, static_cast<float>(window->width()) / window->height(), 0.1f, camera_farplane);

    m_Camera->setPosition(QVector3D(w, h, d) - sizeDelta);
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

void Scene::addLight(Qt3DRender::QAbstractLight *light,
                     Qt3DCore::QTransform *transform,
                     const QString &name)
{
    auto l = new Light(this, light, transform);
    applyEntityName(l, "light", name);

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

SceneEntity* Scene::addEntity(Qt3DRender::QGeometryRenderer *geometry,
                              Qt3DRender::QMaterial *material,
                              const QString &name)
{

    auto entity = new SceneEntity(this, geometry, material);
    applyEntityName(entity, "entity", name);

    QObject::connect(entity, &SceneEntity::signalClicked, this, &Scene::slotEntityClicked, Qt::DirectConnection);
    QObject::connect(entity, &SceneEntity::signalSelected, this, &Scene::slotEntitySelected, Qt::DirectConnection);

    delEntity(entity->objectName());
    m_Entities.insert(entity->objectName(), entity);

    qDebug() << objectName() << ": Entity created, count =" << m_Entities.count();
    emit signalEntitiesCountChanged(m_Entities.count());
    return entity;
}

bool Scene::delEntity(const QString &name)
{
    auto entity = m_Entities.take(name);
    if(entity)
    {
        if(m_SelectedEntity == entity) m_SelectedEntity = nullptr;
        entity->deleteLater();
        emit Scene::signalEntitiesCountChanged(m_Entities.count());
        return true;
    }
    return false;
}

bool Scene::delEntity(SceneEntity *entity) { return delEntity(entity->objectName()); }

void Scene::slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name)
{
    auto e = EntityByName(name);
    if(!e) { qCritical() << __func__ << ": Entity not found"; return; }

    if(event->button() == Qt3DRender::QPickEvent::Buttons::LeftButton)
    {
        e->Select(!e->isSelected());
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
        cm->setXExtent(40);
        cm->setYExtent(40);
        cm->setZExtent(40);
        e->applyGeometry(cm);
    }
}

void Scene::slotEntitySelected(SceneEntity *entity, bool selected)
{
    if(! entity) { qCritical() << __func__ << ": Selected entity is empty"; return; }

    if(m_SelectedEntity && m_SelectedEntity->isSelected()) m_SelectedEntity->Select(false);

    m_SelectedEntity = nullptr;
    if(selected) m_SelectedEntity = entity;

    emit signalEntitySelected(m_SelectedEntity);
}

void Scene::slotFrameActionTriggered(float dt)
{
    Q_UNUSED(dt)
    m_FRC->calculate();
}

void Scene::slotShowBoxes(bool value)
{
    for(Light* l: m_Lights) l->slotShowBox(value);
    m_Box->setEnabled(value);
}

SceneEntity *Scene::EntityByName(const QString &name)
{
    auto e = Entities().value(name);
    if(!e) { qDebug() << ": Entity <" << name << "> not found"; return nullptr; }
    return  e;
}

float Scene::CellSize() const { return m_CellSize; }
QVector3D Scene::Size() const { return QVector3D(m_Width, m_Height, m_Depth); }
QVector3D Scene::RealSize() const { return m_CellSize * QVector3D(m_Width, m_Height, m_Depth); }
FrameRateCalculator *Scene::FRC() const { return m_FRC; }
SceneEntity *Scene::SelectedEntity() const { return m_SelectedEntity; }
QHash<QString, SceneEntity* > Scene::Entities() const { return m_Entities; }
QHash<QString, Light* > Scene::Lights() const { return m_Lights; }
