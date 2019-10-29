#ifndef SCENE_H
#define SCENE_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DLogic/QFrameAction>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QAbstractLight>

class SceneEntity;
class CameraController;
class FrameRateCalculator;
class Light;

class Scene : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Scene(Qt3DExtras::Qt3DWindow* window, float cell, float width, float height, float depth, const QString &name = "");
    void addLight(Qt3DRender::QAbstractLight* light, Qt3DCore::QTransform* transform, const QString &name = "");
    bool delLight(const QString& name);
    SceneEntity* addEntity(Qt3DRender::QGeometryRenderer *geometry,
                           Qt3DRender::QMaterial *material,
                           const QString &name = "");
    bool delEntity(const QString &name);
    bool delEntity(SceneEntity* entity);
    QHash<QString, SceneEntity* > Entities() const;
    QHash<QString, Light* > Lights() const;
    SceneEntity *SelectedEntity() const;
    FrameRateCalculator *FRC() const;

    float CellSize() const;
    QVector3D Size() const;
    QVector3D RealSize() const;

Q_SIGNALS:
    void signalEntitySelected(SceneEntity* entity);
    void signalEntitiesCountChanged(int count);
    void signalLightsCountChanged(int count);

public Q_SLOTS:
    void slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name);
    void slotEntitySelected(SceneEntity* entity, bool selected);
    void slotFrameActionTriggered(float dt);
    void slotShowBoxes(bool value);

protected:
    SceneEntity* EntityByName(const QString& name);

private:
    Qt3DLogic::QFrameAction* m_FrameAction;
    CameraController* m_CameraController;
    Qt3DRender::QCamera* m_Camera;
    SceneEntity* m_SelectedEntity;
    FrameRateCalculator* m_FRC;
    Qt3DCore::QEntity* m_Box;
    QHash <QString, Light*> m_Lights;
    QHash <QString, SceneEntity*> m_Entities;
    float m_CellSize;
    float m_Height;
    float m_Width;
    float m_Depth;
};

#endif // SCENE_H
