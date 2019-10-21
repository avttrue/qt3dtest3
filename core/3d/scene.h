#ifndef SCENE_H
#define SCENE_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QPickEvent>

class SceneEntity;

class Scene : public Qt3DCore::QEntity
{
public:
    Scene(Qt3DExtras::Qt3DWindow* view, const QString &name = "");
    SceneEntity* createEntity(Qt3DRender::QGeometryRenderer *geometry,
                              Qt3DRender::QMaterial *material,
                              const QString &name = "");
    void deleteEntity(const QString &name);

    QHash<QString, SceneEntity *> Entities() const;

private:
    QHash <QString, SceneEntity*> m_Entities;

private Q_SLOTS:
    void slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name);
};

#endif // SCENE_H
