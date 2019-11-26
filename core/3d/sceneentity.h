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
                Qt3DRender::QMaterial *material,
                Qt3DCore::QTransform *transform);
    Qt3DCore::QEntity *SelectionBox() const;
    /*!
     * \brief applyGeometry - установить новую геометрию сущности.
     * geometry - новая геометрия.
     * diagonal - фактор масштаба по диагонали сущности, если 0, то неучитывается.
     */
    void applyGeometry(Qt3DRender::QGeometryRenderer* geometry, float diagonal = 0.0f);
    void applyGeometry(const QString& name);
    Qt3DRender::QGeometryRenderer* Geometry() const;
    void applyMaterial(Qt3DRender::QMaterial* material);
    void applyMaterial(const QString& name);
    Qt3DRender::QMaterial* Material() const;
    void applyPosition(const QVector3D& position);
    QVector3D Position();
    void Interactive(bool value);

Q_SIGNALS:
    void signalClicked(Qt3DRender::QPickEvent *event, SceneEntity* entity);

public Q_SLOTS:
    void slotClicked(Qt3DRender::QPickEvent *event);
    void slotSelect(bool value = true);

protected:
    void createSelectionBox();

private:
    Scene* m_Scene;
    Qt3DRender::QGeometryRenderer* m_Geometry;
    Qt3DRender::QMaterial* m_Material;
    Qt3DCore::QTransform *m_Transform;
    Qt3DRender::QObjectPicker* m_Picker;
    Qt3DCore::QEntity* m_SelectionBox;
};

#endif // SCENEENTITY_H
