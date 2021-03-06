#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QLayer>
#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QSortPolicy>

class Scene;
class CameraController;

class SceneView : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    SceneView(QScreen *screen = nullptr);
    void createScene(float cell, float width, float height, float depth, const QString& name = "");
    Scene *getScene() const;
    Qt3DRender::QLayer *TransparentLayer() const;
    Qt3DRender::QLayer *OpaqueLayer() const;
    Qt3DRender::QLayer *InterfaceLayer() const;

protected:
    void keyPressEvent(QKeyEvent *e);
    void resizeEvent(QResizeEvent *e);
    void applySceneCamera(); // устанавливает параметры и управление камерой для сцены
    void setCameraPerspectiveProjection(Qt3DRender::QCamera* camera, int width, int height);

private:
    Scene* m_Scene;
    Qt3DRender::QCamera* m_Camera;
    CameraController* m_CameraController;
    Qt3DRender::QLayer* m_TransparentLayer; // слой рендеринга прозрачных объектов
    Qt3DRender::QLayer* m_OpaqueLayer;      // слой рендеринга непрозрачных объектов
    Qt3DRender::QLayer* m_InterfaceLayer;   // слой рендеринга интерфейса
    Qt3DRender::QCullFace* m_CullFace;
    Qt3DRender::QSortPolicy* m_SortPolicy;

    float m_CameraFarPlane;

public Q_SLOTS:
    /*!
     * \brief slotCullFace - вкл-откл режим Cull Face
     */
    void slotCullFace(bool mode);
    /*!
     * \brief slotRenderSortPolicyType - Устанавливает порядок отрисовки объектов относительно камеры;
     * используется для корректного отображения объектов с прозрачностью.
     */
    void slotRenderSortPolicyType(const QString &value);

private Q_SLOTS:
    void slotSceneLoaded(); // когда сцена дала сигнал, что загружена

Q_SIGNALS:
    void signalSceneChanged(Scene* scene);

};

#endif // SCENEVIEW_H
