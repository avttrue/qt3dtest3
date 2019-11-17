#include "sceneview.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"
#include "sceneobject.h"

#include <QDebug>
#include <QRandomGenerator>

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

    emit signalSceneChanged(m_Scene);
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

void SceneView::test()
{
    if(!m_Scene) {qWarning() << "Scene is absent"; return; }

    m_Scene->setEnabled(false);
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            for(int k = 0; k < 10; k++)
            {
                auto mesh = new Qt3DExtras::QSphereMesh;
                mesh->setRadius(20.0);
                mesh->setSlices(32);
                mesh->setRings(32);

                auto transform = new Qt3DCore::QTransform;
                transform->setScale(m_Scene->CellSize());
                transform->setTranslation(QVector3D(i*40.0f, j*40.0f, k*40.0f));

                auto matname = m_Scene->Materials().keys().at(
                    QRandomGenerator::global()->bounded(0, m_Scene->Materials().keys().count()));
                m_Scene->addObject("cube", matname, transform);
            }
    m_Scene->setEnabled(true);
}

Scene* SceneView::getScene() const { return m_Scene; }
