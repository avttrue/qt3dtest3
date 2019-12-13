#include "sceneview.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"
#include "sceneobject.h"

#include <QDebug>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QRenderSettings>

SceneView::SceneView(QScreen *screen):
    Qt3DExtras::Qt3DWindow(screen),
    m_Scene(nullptr)
{
    defaultFrameGraph()->setClearColor(QColor(COLOR_SCENE_BG));
    renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);
}

void SceneView::createScene(float cell, float width, float height, float depth, const QString& name)
{
    if(m_Scene) m_Scene->deleteLater();
    m_Scene = new Scene(this, cell, width, height, depth, name);
    m_Scene->slotShowBoxes(config->DrawSceneBoxes());

    Q_EMIT signalSceneChanged(m_Scene);
}

void SceneView::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "Button:" << e->key();
}

void SceneView::resizeEvent(QResizeEvent *e)
{
    if(!m_Scene) return;

    auto camera_aspect = static_cast<float>(e->size().width()) / e->size().height();
    camera()->lens()->setPerspectiveProjection(45.0f, camera_aspect, 0.1f, getScene()->CameraFarPlane());
}

Scene* SceneView::getScene() const { return m_Scene; }
