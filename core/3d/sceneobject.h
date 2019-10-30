#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>

class Scene;

class SceneObject : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    SceneObject(Scene* parent,
                Qt3DRender::QGeometryRenderer* geometry,
                Qt3DRender::QMaterial* material);
    Qt3DCore::QEntity *Box() const;
    void applyGeometry(Qt3DRender::QGeometryRenderer* geometry);

Q_SIGNALS:
    void signalClicked(Qt3DRender::QPickEvent *event, const QString &name);

public Q_SLOTS:
    void slotClicked(Qt3DRender::QPickEvent *event);
    void slotSelect(bool value = true);

protected:
    void createSelectionBox();

private:
    Scene* m_Scene;
    Qt3DRender::QGeometryRenderer* m_Geometry;
    Qt3DRender::QMaterial* m_Material;
    Qt3DCore::QEntity* m_SelectionBox;
};

#endif // SCENEOBJECT_H
