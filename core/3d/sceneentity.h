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
    SceneEntity(Scene* parent);
    Qt3DCore::QEntity *SelectionBox() const;
    void applyGeometry(Qt3DRender::QGeometryRenderer* geometry);
    Qt3DRender::QGeometryRenderer* Geometry() const;
    void applyMaterial(Qt3DRender::QMaterial* material);
    Qt3DRender::QMaterial* Material() const;
    void applyPosition(const QVector3D& position);
    QVector3D Position();
    void Interactive(bool value);
    QVector3D Size() const;
    void applySize(const QVector3D &size);

Q_SIGNALS:

public Q_SLOTS:
    void slotClicked(Qt3DRender::QPickEvent *event);
    void slotSelect(bool value = true);

protected:
    Scene* m_Scene;
    Qt3DCore::QTransform *m_Transform;
    Qt3DRender::QGeometryRenderer* m_Geometry;
    Qt3DRender::QMaterial* m_Material;
    Qt3DRender::QObjectPicker* m_Picker;
    Qt3DCore::QEntity* m_SelectionBox;
    QVector3D m_Size;

    void createSelectionBox();
};

#endif // SCENEENTITY_H
