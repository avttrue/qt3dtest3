#ifndef VIEW_H
#define VIEW_H

#include <QKeyEvent>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QLayer>
#include <Qt3DRender/QCullFace>

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
    Qt3DRender::QLayer* m_TransparentLayer; // слой рендеринга прозрачных объектов
    Qt3DRender::QLayer* m_OpaqueLayer;      // слой рендеринга непрозрачных объектов
    Qt3DRender::QLayer* m_InterfaceLayer;   // слой рендеринга интерфейса
    Qt3DRender::QCullFace* m_CullFace;

    float m_CameraFarPlane;

public Q_SLOTS:
    void slotCullFace(bool mode);

Q_SIGNALS:
    void signalSceneChanged(Scene* scene);

};

#endif // VIEW_H
