#include "sceneobject.h"
#include "properties.h"
#include "helpers3d.h"

#include <Qt3DRender/QObjectPicker>

SceneObject::SceneObject(Scene* parent,
                         Qt3DRender::QGeometryRenderer *geometry,
                         Qt3DRender::QMaterial *material,
                         const QString &name) :
    SceneEntity(parent)
{
    applyEntityName(this, "object", name);
    applyGeometry(geometry);
    applyMaterial(material);
}
