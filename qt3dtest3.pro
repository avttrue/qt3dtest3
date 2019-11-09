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
    controls.cpp \
    core/3d/cameracontroller.cpp \
    core/3d/frameratecalculator.cpp \
    core/3d/helpers3d.cpp \
    core/3d/light.cpp \
    core/3d/scene.cpp \
    core/3d/sceneentity.cpp \
    core/3d/sceneobject.cpp \
    core/3d/sceneview.cpp \
    dialogs/dialogvalueslist.cpp \
    helpers.cpp \
    helperswidget.cpp \
    main.cpp \
    mainwindow.cpp \
    properties.cpp \
    config.cpp

HEADERS += \
    controls.h \
    core/3d/cameracontroller.h \
    core/3d/frameratecalculator.h \
    core/3d/helpers3d.h \
    core/3d/light.h \
    core/3d/scene.h \
    core/3d/sceneentity.h \
    core/3d/sceneobject.h \
    core/3d/sceneview.h \
    dialogs/dialogvalueslist.h \
    helpers.h \
    helperswidget.h \
    mainwindow.h \
    properties.h \
    config.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    interface.qrc \
    models.qrc \
    textures.qrc
