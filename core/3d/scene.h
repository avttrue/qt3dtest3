#ifndef SCENE_H
#define SCENE_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QAbstractLight>

class Entity;
class CameraController;

class Scene : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Scene(Qt3DExtras::Qt3DWindow* window, const QString &name = "");
    void addLight(Qt3DRender::QAbstractLight* light, Qt3DCore::QTransform* transform, const QString &name = "");
    bool delLight(const QString& name);
    Entity* addEntity(Qt3DRender::QGeometryRenderer *geometry,
                           Qt3DRender::QMaterial *material,
                           const QString &name = "");
    bool delEntity(const QString &name);
    QHash<QString, Entity *> Entities() const;
    QHash<QString, Qt3DCore::QEntity *> Lights() const;
    Entity *SelectedEntity() const;

Q_SIGNALS:
    void signalEntitySelected(Entity* entity);
    void signalEntitiesCountChanged(int count);
    void signalLightsCountChanged(int count);

public Q_SLOTS:
    void slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name);
    void slotEntitySelected(Entity* entity, bool selected);

protected:
    Entity* EntityByName(const QString& name);

private:
    CameraController* m_CameraController;
    Qt3DRender::QCamera* m_Camera;
    Entity* m_SelectedEntity;
    QHash <QString, Qt3DCore::QEntity*> m_Lights;
    QHash <QString, Entity*> m_Entities;

};

#endif // SCENE_H
