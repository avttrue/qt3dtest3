#include "sceneview.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"
#include "sceneobject.h"

#include <QDebug>
#include <QRandomGenerator>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QRenderSettings>

SceneView::SceneView(QScreen *screen):
    Qt3DExtras::Qt3DWindow(screen),
    m_Scene(nullptr)
{
    renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);
}

void SceneView::createScene(float cell, float width, float height, float depth, const QString& name)
{
    if(m_Scene) m_Scene->deleteLater();
    m_Scene = new Scene(this, cell, width, height, depth, name);
    emit signalSceneChanged(m_Scene);
}

void SceneView::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "Button:" << e->key();
}

void SceneView::createSpheresTest()
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
                transform->setTranslation(QVector3D(i*40.0f, j*40.0f, k*40.0f));

                auto material = new Qt3DExtras::QPhongMaterial;
                material->setAmbient(QColor(
                    QRandomGenerator::global()->bounded(0, 256),
                    QRandomGenerator::global()->bounded(0, 256),
                    QRandomGenerator::global()->bounded(0, 256)));

                m_Scene->addObject(mesh, material, transform);
            }
    m_Scene->setEnabled(true);
}

Scene* SceneView::getScene() const { return m_Scene; }
