#include "light.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"

#include <QPhongMaterial>

Light::Light(Scene *parent,
             Qt3DRender::QGeometryRenderer *geometry,
             Qt3DRender::QMaterial *material,
             Qt3DCore::QTransform *transform,
             Qt3DRender::QAbstractLight *light)
    :SceneEntity(parent, geometry, material, transform),
    m_Light(light)
{
    addComponent(light);
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
