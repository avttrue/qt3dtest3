#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>

class Scene;

class SceneEntity : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    SceneEntity(Scene* parent,
                Qt3DRender::QGeometryRenderer* geometry,
                Qt3DRender::QMaterial* material);
    Qt3DCore::QEntity *Box() const;
    void applyGeometry(Qt3DRender::QGeometryRenderer* geometry);
    void Select(bool value);
    bool isSelected() const;

Q_SIGNALS:
    void signalClicked(Qt3DRender::QPickEvent *event, const QString &name);
    void signalSelected(SceneEntity* entity, bool selected);

public Q_SLOTS:
    void slotClicked(Qt3DRender::QPickEvent *event);

protected:
    void createBox();

private:
    Scene* m_Scene;
    Qt3DRender::QGeometryRenderer* m_Geometry;
    Qt3DRender::QMaterial* m_Material;
    Qt3DCore::QEntity* m_Box;
    bool m_Selected;

};

#endif // SCENEENTITY_H
