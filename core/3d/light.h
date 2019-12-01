#ifndef LIGHT_H
#define LIGHT_H

#include "sceneentity.h"

#include <Qt3DRender/QAbstractLight>

class Light : public SceneEntity
{
    Q_OBJECT

public:
    Light(Scene* parent,
          Qt3DRender::QGeometryRenderer* geometry,
          Qt3DRender::QMaterial* material,
          Qt3DCore::QTransform *transform,
          Qt3DRender::QAbstractLight *light);
    void applyLight(Qt3DRender::QAbstractLight* light);
    Qt3DRender::QAbstractLight* getLight() const;

Q_SIGNALS:

public Q_SLOT:
        void slotShowGeometry(bool value);

private:
    Qt3DRender::QAbstractLight *m_Light;
};

#endif // LIGHT_H
