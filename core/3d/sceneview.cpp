#include "sceneview.h"
#include "properties.h"
#include "guientity.h"
#include "helpers3d.h"
#include "scene.h"
#include "sceneobject.h"
#include "cameracontroller.h"

#include <cmath>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QSortPolicy>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QLayerFilter>
#include <Qt3DRender/QRenderStateSet>
//#include <Qt3DRender/QDepthTest>

SceneView::SceneView(QScreen *screen):
    Qt3DExtras::Qt3DWindow(screen),
    m_Scene(nullptr),
    m_Camera(nullptr),
    m_CameraController(nullptr)
{
    auto renderSurfaceSelector = new Qt3DRender::QRenderSurfaceSelector;
    renderSurfaceSelector->setSurface(this);

    auto viewport = new Qt3DRender::QViewport(renderSurfaceSelector);
    auto cameraSelector = new Qt3DRender::QCameraSelector(viewport);

    auto clearBuffers = new Qt3DRender::QClearBuffers(cameraSelector);
    clearBuffers->setBuffers(Qt3DRender::QClearBuffers::AllBuffers);
    clearBuffers->setClearColor(config->SceneColorBG());

    auto renderStateSet = new Qt3DRender::QRenderStateSet(clearBuffers);
    m_CullFace = new Qt3DRender::QCullFace(renderStateSet);
    slotCullFace(config->RendererCullFaceMode());
    renderStateSet->addRenderState(m_CullFace);

    //    auto depthTest = new Qt3DRender::QDepthTest;
    //    depthTest->setDepthFunction(Qt3DRender::QDepthTest::Less);
    //    renderStateSet->addRenderState(depthTest);

    m_Camera = new Qt3DRender::QCamera(cameraSelector);

    cameraSelector->setCamera(m_Camera);

    // именно в таком порядке: m_OpaqueLayer, m_TransparentLayer, последний - m_InterfaceLayer
    m_OpaqueLayer = new Qt3DRender::QLayer;
    auto opaqueFilter = new Qt3DRender::QLayerFilter(m_Camera);
    m_OpaqueLayer->setObjectName("OpaqueLayer");
    m_OpaqueLayer->setRecursive(true);
    opaqueFilter->addLayer(m_OpaqueLayer);

    m_TransparentLayer = new Qt3DRender::QLayer;
    auto transparentFilter = new Qt3DRender::QLayerFilter(m_Camera);
    m_TransparentLayer->setObjectName("TransparentLayer");
    m_TransparentLayer->setRecursive(true);
    transparentFilter->addLayer(m_TransparentLayer);

    m_InterfaceLayer = new Qt3DRender::QLayer;
    auto interfaceFilter = new Qt3DRender::QLayerFilter(m_Camera);
    m_InterfaceLayer->setObjectName("InterfaceLayer");
    m_TransparentLayer->setRecursive(true);
    interfaceFilter->addLayer(m_InterfaceLayer);

    setActiveFrameGraph(renderSurfaceSelector);

    renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);
}

void SceneView::createScene(float cell, float width, float height, float depth, const QString& name)
{
    if(m_Scene) m_Scene->deleteLater();

    m_Scene = new Scene(this, cell, width, height, depth, name);
    m_Scene->setEnabled(false);
    slotRenderSortPolicyType(config->RendererSortPolicyType());
    setRootEntity(m_Scene);

    auto func = [=]()
    {
        QObject::disconnect(m_Scene, &Scene::signalLoaded, nullptr, nullptr);
        applySceneCamera();
        m_Scene->slotShowBoxes(config->DrawSceneBoxes());
        m_Scene->setEnabled(true);
        Q_EMIT signalSceneChanged(m_Scene);
    };
    QObject::connect(m_Scene, &Scene::signalLoaded, func);
    m_Scene->load();
}

void SceneView::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "Button:" << e->key();
    if(e->key() == Qt::Key_T)
    {

    }
}

void SceneView::resizeEvent(QResizeEvent *e)
{
    if(!m_Scene) return;

    auto camera_aspect = static_cast<float>(e->size().width()) / e->size().height();
    camera()->lens()->setPerspectiveProjection(45.0f, camera_aspect, 0.1f, m_CameraFarPlane);
}

void SceneView::applySceneCamera()
{
    if(!m_Scene) { qCritical() << __func__  << "Scene is empty"; return; }

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

void SceneView::slotRenderSortPolicyType(const QString& value)
{
    if(!m_Scene) { qCritical() << __func__  << "Scene is empty"; return; }

    auto vector = StringListToRenderSortPolicyType(value.split(',', QString::SkipEmptyParts));

    m_SortPolicy = new Qt3DRender::QSortPolicy(m_Scene);
    activeFrameGraph()->setParent(m_SortPolicy);

    m_SortPolicy->setSortTypes(vector);
}

void SceneView::slotCullFace(bool mode)
{
    m_CullFace->setMode(mode ? Qt3DRender::QCullFace::Back : Qt3DRender::QCullFace::NoCulling);
}

Scene* SceneView::getScene() const { return m_Scene; }
Qt3DRender::QLayer *SceneView::TransparentLayer() const { return m_TransparentLayer; }
Qt3DRender::QLayer *SceneView::OpaqueLayer() const { return m_OpaqueLayer; }
Qt3DRender::QLayer *SceneView::InterfaceLayer() const { return m_InterfaceLayer; }
