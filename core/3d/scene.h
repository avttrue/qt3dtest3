#ifndef SCENE_H
#define SCENE_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DLogic/QFrameAction>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QAbstractLight>

class SceneEntity;
class SceneObject;
class CameraController;
class FrameRateCalculator;
class Light;

class Scene : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Scene(Qt3DExtras::Qt3DWindow* window, float cell, float width, float height, float depth, const QString &name = "");
    void addLight(Qt3DCore::QTransform *transform,
                  Qt3DRender::QAbstractLight* light,
                  const QString &name = "");
    bool delLight(const QString& name);
    bool delLight(SceneEntity *entity);
    SceneObject* addObject(Qt3DRender::QGeometryRenderer *geometry,
                           Qt3DRender::QMaterial *material,
                           Qt3DCore::QTransform *transform,
                           const QString &name = "");
    SceneObject* addObject(const QString& geometry,
                           Qt3DRender::QMaterial *material,
                           Qt3DCore::QTransform *transform,
                           const QString &name = "");

    bool delObject(const QString &name);
    bool delObject(SceneEntity *entity);
    QHash<QString, SceneObject* > Objects() const;
    QHash<QString, Light* > Lights() const;
    QHash<QString, Qt3DRender::QGeometryRenderer *> Geometries() const;
    SceneEntity *SelectedEntity() const;
    FrameRateCalculator *FRC() const;
    float CellSize() const;
    QVector3D Size() const;
    QVector3D RealSize() const;
    float CameraFarPlane() const;

Q_SIGNALS:
    void signalSelectedEntityChanged(SceneEntity* entity);
    void signalObjectsCountChanged(int count);
    void signalLightsCountChanged(int count);
    void signalGeometriesCountChanged(int count);
    void signalGeometryLoaded(const QString& name);

public Q_SLOTS:
    void slotEntityClicked(Qt3DRender::QPickEvent *event, SceneEntity* entity);
    void slotFrameActionTriggered(float dt);
    void slotShowBoxes(bool value);
    void slotLoadGeometry(const QString& path);

protected:
    void SelectEntity(SceneEntity* entity);
    void loadGeometries();

private:
    Qt3DLogic::QFrameAction* m_FrameAction;
    CameraController* m_CameraController;
    Qt3DRender::QCamera* m_Camera;
    SceneEntity* m_SelectedEntity;
    FrameRateCalculator* m_FRC;
    Qt3DCore::QEntity* m_Box;
    QHash <QString, Light*> m_Lights;
    QHash <QString, SceneObject*> m_Objects;
    QHash <QString, Qt3DRender::QGeometryRenderer*> m_Geometries;
    Qt3DExtras::QSphereMesh* m_LightMesh;
    float m_CellSize;
    float m_Height;
    float m_Width;
    float m_Depth;
    float m_CameraFarPlane;
};

#endif // SCENE_H
