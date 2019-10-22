#ifndef SCENE_H
#define SCENE_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QAbstractLight>

class SceneEntity;
class CameraController;

class Scene : public Qt3DCore::QEntity
{
public:
    Scene(Qt3DExtras::Qt3DWindow* view, const QString &name = "");
    void addLight(Qt3DRender::QAbstractLight* light, Qt3DCore::QTransform* transform, const QString &name = "");
    bool delLight(const QString& name);
    SceneEntity* createEntity(Qt3DRender::QGeometryRenderer *geometry,
                              Qt3DRender::QMaterial *material,
                              const QString &name = "");
    void deleteEntity(const QString &name);
    QHash<QString, SceneEntity *> Entities() const;
    QHash<QString, Qt3DCore::QEntity *> Lights() const;

protected:
    SceneEntity* EntityByName(const QString& name);

private:
    CameraController* m_CameraController;
    Qt3DRender::QCamera* m_Camera;
    SceneEntity* m_SelectedEntity;
    QHash <QString, Qt3DCore::QEntity*> m_Lights;
    QHash <QString, SceneEntity*> m_Entities;

private Q_SLOTS:
    void slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name);
    void slotEntitySelected(SceneEntity* entity, bool selected);
};

#endif // SCENE_H
