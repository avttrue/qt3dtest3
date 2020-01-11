#include "sceneobject.h"
#include "helpers3d.h"

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
