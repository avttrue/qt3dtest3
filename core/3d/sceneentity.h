#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QObjectPicker>

class Scene;

class SceneEntity : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    SceneEntity(Scene* parent,
                Qt3DRender::QGeometryRenderer* geometry,
                Qt3DRender::QMaterial* material,
                Qt3DCore::QTransform *transform);
    Qt3DCore::QEntity *SelectionBox() const;
    void applyGeometry(Qt3DRender::QGeometryRenderer* geometry);
    void Interactive(bool value);



Q_SIGNALS:
    void signalClicked(Qt3DRender::QPickEvent *event, const QString &name);

public Q_SLOTS:
    void slotClicked(Qt3DRender::QPickEvent *event);
    void slotSelect(bool value = true);

protected:
    void createSelectionBox();
    Qt3DRender::QGeometryRenderer *Geometry() const;

private:
    Scene* m_Scene;
    Qt3DRender::QGeometryRenderer* m_Geometry;
    Qt3DRender::QMaterial* m_Material;
    Qt3DRender::QObjectPicker* m_Picker;
    Qt3DCore::QEntity* m_SelectionBox;
};

#endif // SCENEENTITY_H
