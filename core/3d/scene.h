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

class SceneObject;
class CameraController;
class FrameRateCalculator;
class Light;

class Scene : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Scene(Qt3DExtras::Qt3DWindow* window, float cell, float width, float height, float depth, const QString &name = "");
    void addLight(Qt3DCore::QTransform *transform, Qt3DRender::QAbstractLight* light, const QString &name = "");
    bool delLight(const QString& name);
    SceneObject* addEntity(Qt3DRender::QGeometryRenderer *geometry,
                           Qt3DRender::QMaterial *material,
                           Qt3DCore::QTransform *transform,
                           const QString &name = "");
    bool delEntity(const QString &name);
    bool delEntity(SceneObject* entity);
    QHash<QString, SceneObject* > Entities() const;
    QHash<QString, Light* > Lights() const;
    SceneObject *SelectedEntity() const;
    FrameRateCalculator *FRC() const;

    float CellSize() const;
    QVector3D Size() const;
    QVector3D RealSize() const;

Q_SIGNALS:
    void signalSelectedEntityChanged(SceneObject* entity);
    void signalEntitiesCountChanged(int count);
    void signalLightsCountChanged(int count);

public Q_SLOTS:
    void slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name);

    void slotFrameActionTriggered(float dt);
    void slotShowBoxes(bool value);

protected:
    SceneObject* EntityByName(const QString& name);
    void SelectEntity(SceneObject* entity);

private:
    Qt3DLogic::QFrameAction* m_FrameAction;
    CameraController* m_CameraController;
    Qt3DRender::QCamera* m_Camera;
    SceneObject* m_SelectedEntity;
    FrameRateCalculator* m_FRC;
    Qt3DCore::QEntity* m_Box;
    QHash <QString, Light*> m_Lights;
    QHash <QString, SceneObject*> m_Objects;
    float m_CellSize;
    float m_Height;
    float m_Width;
    float m_Depth;
};

#endif // SCENE_H
