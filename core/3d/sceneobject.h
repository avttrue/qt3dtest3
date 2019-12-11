#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "sceneentity.h"

class SceneObject : public SceneEntity
{
    Q_OBJECT

public:
    SceneObject(Scene* parent,
                Qt3DRender::QGeometryRenderer* geometry,
                Qt3DRender::QMaterial* material,
                const QString &name = "");
};

#endif // SCENEOBJECT_H
