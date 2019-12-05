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
class SceneObject;
class CameraController;
class FrameRateCalculator;
class Light;

class Scene : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Scene(Qt3DExtras::Qt3DWindow* window,
          float cell,
          float width, float height, float depth,
          const QString &name = "");
    Light *addLight(Qt3DRender::QAbstractLight* light,
                    Qt3DCore::QTransform *transform = nullptr,
                    const QString &name = "");
    bool delLight(const QString& name);
    bool delLight(SceneEntity *entity);
    SceneObject* addObject(const QString& geometry,
                           const QString &material,
                           Qt3DCore::QTransform *transform = nullptr,
                           const QString &name = "");

    bool delObject(const QString &name);
    bool delObject(SceneEntity *entity);
    QHash<QString, SceneObject* > Objects() const;
    QHash<QString, Light* > Lights() const;
    QHash<QString, Qt3DRender::QGeometryRenderer *> Geometries() const;
    QHash<QString, Qt3DRender::QMaterial* > Materials() const;
    SceneEntity *SelectedEntity() const;
    FrameRateCalculator *FRC() const;
    float CellSize() const;
    QVector3D Size() const;
    QVector3D RealSize() const;
    float CameraFarPlane() const;
    void SelectEntity(SceneEntity* entity);
    QString EntityGeometry(SceneEntity* entity) const;
    QString EntityMaterial(SceneEntity* entity) const;
    void setEntityPosition(SceneEntity* entity, const QVector3D& position);
    QVector3D EntityPosition(SceneEntity* entity) const;
    void setEntitySize(SceneEntity* entity, const QVector3D& size);
    QVector3D EntitySize(SceneEntity* entity) const;

Q_SIGNALS:
    void signalSelectedEntityChanged(SceneEntity* entity);
    void signalObjectsCountChanged(int count);
    void signalLightsCountChanged(int count);
    void signalGeometriesCountChanged(int count);
    void signalGeometryLoaded(const QString& name);
    void signalMaterialsCountChanged(int count);
    void signalMaterialLoaded(const QString& name);
    void signalEntityClicked(Qt3DRender::QPickEvent *event, SceneEntity *entity);

public Q_SLOTS:
    void slotFrameActionTriggered(float dt);
    void slotShowBoxes(bool value);

protected:
    void loadGeometries();
    void loadMaterials();
    void loadGeometry(const QString& path);
    void loadMaterial(const QString& path);
    SceneObject* addObject(Qt3DRender::QGeometryRenderer *geometry,
                           Qt3DRender::QMaterial *material,
                           Qt3DCore::QTransform *transform,
                           const QString &name = "");

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
    QHash <QString, Qt3DRender::QMaterial*> m_Materials;
    float m_CellSize;
    float m_Height;
    float m_Width;
    float m_Depth;
    float m_CameraFarPlane;
};

#endif // SCENE_H
