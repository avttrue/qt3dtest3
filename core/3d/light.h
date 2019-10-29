#ifndef LIGHT_H
#define LIGHT_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DRender/QAbstractLight>

class Scene;

class Light : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Light(Scene* parent,
          Qt3DRender::QAbstractLight *light,
          Qt3DCore::QTransform *transform);

public Q_SLOT:
        void slotShowBox(bool value);

private:
    Scene* m_Scene;
    Qt3DCore::QEntity* m_Box;
    Qt3DExtras::QSphereMesh* m_Mesh;
};

#endif // LIGHT_H
