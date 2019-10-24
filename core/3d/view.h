#ifndef VIEW_H
#define VIEW_H

#include <QKeyEvent>
#include <Qt3DExtras/Qt3DWindow>

class Scene;

class View : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT

public:
    View(QScreen *screen = nullptr);
    void createScene();
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
