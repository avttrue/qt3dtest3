#include <QGuiApplication>

#include "core/3d/window3d.h"

int main(int argc, char *argv[])
{
    QGuiApplication application(argc, argv);
    Window3D window;
    window.show();
    return application.exec();
}
