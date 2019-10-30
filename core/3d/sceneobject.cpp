#include "sceneobject.h"
#include "properties.h"

#include <Qt3DRender/QObjectPicker>

SceneObject::SceneObject(Scene* parent,
                         Qt3DRender::QGeometryRenderer *geometry,
                         Qt3DRender::QMaterial *material, Qt3DCore::QTransform *transform) :
    SceneEntity(parent, geometry, material, transform)
{

}
