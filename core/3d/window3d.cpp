#include "window3d.h"
#include "helpers.h"
#include "scene.h"
#include "sceneentity.h"

#include <QDebug>
#include <QRandomGenerator>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QRenderSettings>

Window3D::Window3D(QScreen *screen):
    Qt3DExtras::Qt3DWindow(screen),
    m_Scene(nullptr)
{
    m_Scene = new Scene(this);

    auto light = new Qt3DRender::QPointLight;
    m_Scene->addComponent(light);

    auto cam = camera();
    cam->lens()->setPerspectiveProjection(60.0f, static_cast<float>(width()) / height(), 0.1f, 1000.0f);
    cam->setPosition(QVector3D(0.0f, 0.0f, 500.0f));
    cam->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    auto camController = new Qt3DExtras::QFirstPersonCameraController(m_Scene);
    camController->setCamera(cam);

    renderSettings()->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
}

void Window3D::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Insert)
    {
        //if(m_Scene) m_Scene->deleteLater();
        //m_Scene = new Scene(this);
        createSpheresTest();
    }
}

void Window3D::createSpheresTest()
{
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

                auto sphere = m_Scene->createEntity(mesh, material);
                sphere->addComponent(transform);
            }
    m_Scene->setEnabled(true);
}
