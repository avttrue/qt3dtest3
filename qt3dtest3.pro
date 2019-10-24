QT       += core widgets gui 3dlogic 3dextras 3dinput

CONFIG += c++17

TARGET      = run
TEMPLATE    = app
VERSION     = 0.1

DEFINES     += APP_VERS=\\\"$${VERSION}\\\"
DEFINES     += APP_NAME=\\\"QT3DTest\\\"
#DEFINES     += GIT_VERS=\\\"$$system(git describe --always)\\\"
DEFINES     += GIT_VERS=\\\"-------\\\"
DEFINES     += BUILD_DATE='"\\\"$(shell date)\\\""'
DEFINES     += QT_DEPRECATED_WARNINGS

SOURCES += \
    core/3d/cameracontroller.cpp \
    core/3d/entity.cpp \
    core/3d/helpers.cpp \
    core/3d/scene.cpp \
    core/3d/view.cpp \
    main.cpp \
    mainwindow.cpp \
    properties.cpp \
    config.cpp

HEADERS += \
    core/3d/cameracontroller.h \
    core/3d/entity.h \
    core/3d/helpers.h \
    core/3d/scene.h \
    core/3d/view.h \
    mainwindow.h \
    properties.h \
    config.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
