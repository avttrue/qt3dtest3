#ifndef VIEW_H
#define VIEW_H

#include <QKeyEvent>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QLayer>

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

Q_SIGNALS:
    void signalSceneChanged(Scene* scene);

protected:
    void keyPressEvent(QKeyEvent *e);
    void resizeEvent(QResizeEvent *e);
    /*!
     * \brief applyBackToFrontSortPolicy - Устанавливает порядок отрисовки объектов относительно камеры;
     * используется для корректного отображения объектов с прозрачностью.
     */
    void applyBackToFrontSortPolicy();
    void applySceneCamera(); // устанавливает параметры и управление камеры для сцены

private:
    Scene* m_Scene;
    Qt3DRender::QCamera* m_Camera;
    CameraController* m_CameraController;
    Qt3DRender::QLayer *m_TransparentLayer;
    Qt3DRender::QLayer *m_OpaqueLayer;
    Qt3DRender::QLayer *m_InterfaceLayer;

    float m_CameraFarPlane;
};

#endif // VIEW_H
