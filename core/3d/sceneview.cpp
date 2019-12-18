#include "sceneview.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"
#include "sceneobject.h"
#include "cameracontroller.h"

#include <QDebug>
#include <cmath>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QSortPolicy>

SceneView::SceneView(QScreen *screen):
    Qt3DExtras::Qt3DWindow(screen),
    m_Scene(nullptr),
    m_Camera(nullptr),
    m_CameraController(nullptr)
{
    m_Camera = camera();
    defaultFrameGraph()->setClearColor(QColor(config->SceneColorBG()));
    defaultFrameGraph()->setFrustumCullingEnabled(config->SceneFrustumCulling());

    renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);
}

void SceneView::createScene(float cell, float width, float height, float depth, const QString& name)
{
    if(m_Scene) m_Scene->deleteLater();

    m_Scene = new Scene(this, cell, width, height, depth, name);
    applyBackToFrontSortPolicy();
    m_Scene->setEnabled(false);
    setRootEntity(m_Scene);

    auto func = [=]()
    {
        applySceneCamera();
        m_Scene->setEnabled(true);
        m_Scene->slotShowBoxes(config->DrawSceneBoxes());
        Q_EMIT signalSceneChanged(m_Scene);
    };
    QObject::connect(m_Scene, &Scene::signalLoaded, func);
    m_Scene->load();
}

void SceneView::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "Button:" << e->key();
}

void SceneView::resizeEvent(QResizeEvent *e)
{
    if(!m_Scene) return;

    auto camera_aspect = static_cast<float>(e->size().width()) / e->size().height();
    camera()->lens()->setPerspectiveProjection(45.0f, camera_aspect, 0.1f, m_CameraFarPlane);
}

void SceneView::applySceneCamera()
{
    if(!m_Scene) return;

    auto w = m_Scene->CellSize()* m_Scene->Width();
    auto h = m_Scene->CellSize()* m_Scene->Height();
    auto d = m_Scene->CellSize()* m_Scene->Depth();

    m_CameraFarPlane = static_cast<float>(sqrt(pow(static_cast<double>(w), 2) +
                                               pow(static_cast<double>(h), 2) +
                                               pow(static_cast<double>(d), 2)));
    auto camera_aspect = static_cast<float>(width()) / height();
    m_Camera->lens()->setPerspectiveProjection(45.0f, camera_aspect, 0.1f, m_CameraFarPlane);

    m_Camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    m_Camera->setPosition(QVector3D(w, h, d) -
                          QVector3D(config->SceneExcess(), config->SceneExcess(), config->SceneExcess()));
    m_Camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    m_CameraController = new CameraController(m_Scene);
    m_CameraController->setCamera(m_Camera);
}

void SceneView::applyBackToFrontSortPolicy()
{
    if(!m_Scene) return;

    auto sortPolicy = new Qt3DRender::QSortPolicy(m_Scene);
    QVector<Qt3DRender::QSortPolicy::SortType> sortTypes;
    sortTypes << Qt3DRender::QSortPolicy::BackToFront;
    sortPolicy->setSortTypes(sortTypes);
    auto framegraph = activeFrameGraph();
    framegraph->setParent(sortPolicy); // вот в этом месте может падать
    setActiveFrameGraph(framegraph);
}

Scene* SceneView::getScene() const { return m_Scene; }
