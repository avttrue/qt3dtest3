#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <QKeyEvent>
#include <Qt3DExtras/Qt3DWindow>

class Scene;

class Window3D : public Qt3DExtras::Qt3DWindow
{
public:
    Window3D(QScreen *screen = nullptr);
    void createScene();

protected:
    void keyPressEvent(QKeyEvent *e);
    void createSpheresTest();

private:
    Scene* m_Scene;
};

#endif // WINDOW3D_H
