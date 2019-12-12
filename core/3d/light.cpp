#include "light.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"

#include <QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>

Light::Light(Scene *parent, Qt3DRender::QAbstractLight *light)
    :SceneEntity(parent),
    m_Light(light)
{
    addComponent(light);
    m_Light = light;

    auto material = new Qt3DExtras::QPhongMaterial;
    material->setAmbient(light->color());
    material->setDiffuse(light->color());
    material->setSpecular(light->color());
    material->setShininess(light->intensity() * 100);
    addComponent(material);
    m_Material = material;

    auto mesh = new Qt3DExtras::QSphereMesh(this);
    mesh->setRadius(parent->CellSize() / 2);
    mesh->setSlices(32);
    mesh->setRings(32);
    addComponent(mesh);
    m_Geometry = mesh;

    m_Size = QVector3D(1, 1, 1) * parent->CellSize() / 2;
}

void Light::applyLight(Qt3DRender::QAbstractLight *light)
{
    if(!light) { qCritical() << objectName() << "(" << __func__ << "): Wrong light";  return; }

    applyEntityLight(this, light);
    m_Light = light;

    auto material = new Qt3DExtras::QPhongMaterial;
    material->setAmbient(light->color());
    material->setDiffuse(light->color());
    material->setSpecular(light->color());
    material->setShininess(light->intensity() * 100);
    applyMaterial(material);
}

void Light::slotShowGeometry(bool value)
{
    Geometry()->setEnabled(value);
    Interactive(value);
}

Qt3DRender::QAbstractLight *Light::getLight() const { return m_Light; }
