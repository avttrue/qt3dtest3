#include "light.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"

#include <Qt3DExtras/QPhongMaterial>

Light::Light(Scene *parent, Qt3DRender::QAbstractLight *light,
             Qt3DCore::QTransform *transform)
    :Qt3DCore::QEntity(parent),
    m_Scene(parent)
{


    m_Mesh = new Qt3DExtras::QSphereMesh;
    m_Mesh->setRadius(m_Scene->CellSize() / 2);
    m_Mesh->setSlices(20);
    m_Mesh->setRings(20);
    m_Mesh->setEnabled(config->DrawSceneBoxes());

    auto material = new Qt3DExtras::QPhongMaterial;
    material->setAmbient(light->color());
    material->setShininess(light->intensity());
    addComponent(material);
    addComponent(m_Mesh);
    addComponent(light);
    addComponent(transform);

    auto g = m_Mesh->geometry();
    QObject::connect(g, &Qt3DRender::QGeometry::maxExtentChanged, [=](){
        m_Box = createEntityBox(g->minExtent(), g->maxExtent(), COLOR_SCENE_BOX, this);
        m_Box->setEnabled(config->DrawSceneBoxes());
        QObject::disconnect(g, &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr); });

    QObject::connect(this, &QObject::destroyed, [=](QObject* o){ qDebug() << o->objectName() << ": destroyed"; });
}

void Light::slotShowBox(bool value)
{
    if (m_Box) m_Box->setEnabled(value);
    if (m_Mesh) m_Mesh->setEnabled(value);
}
