#ifndef VIEW_H
#define VIEW_H

#include <QKeyEvent>
#include <Qt3DExtras/Qt3DWindow>

class Scene;

class SceneView : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    SceneView(QScreen *screen = nullptr);
    void createScene(float cell, float width, float height, float depth, const QString& name = "");
    Scene *getScene() const;
    void createSpheresTest();

Q_SIGNALS:
    void signalSceneChanged(Scene* scene);

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Scene* m_Scene;
};

#endif // VIEW_H
