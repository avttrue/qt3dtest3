#include "light.h"
#include "properties.h"
#include "helpers3d.h"
#include "scene.h"

Light::Light(Scene *parent,
             Qt3DRender::QGeometryRenderer *geometry,
             Qt3DRender::QMaterial *material, Qt3DCore::QTransform *transform,
             Qt3DRender::QAbstractLight *light)
    :SceneEntity(parent, geometry, material, transform),
    m_Light(light)
{
    addComponent(light);
}

void Light::slotShowGeometry(bool value)
{
    Geometry()->setEnabled(value);
    Interactive(value);
}
