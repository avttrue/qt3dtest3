QT       += core 3dlogic 3dextras 3dinput

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    config.cpp \
    core/3d/cameracontroller.cpp \
    main.cpp \
    core/3d/helpers.cpp \
    core/3d/scene.cpp \
    core/3d/sceneentity.cpp \
    core/3d/window3d.cpp \
    properties.cpp

HEADERS += \
    config.h \
    core/3d/cameracontroller.h \
    core/3d/helpers.h \
    core/3d/scene.h \
    core/3d/sceneentity.h \
    core/3d/window3d.h \
    properties.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    main.qml

RESOURCES += \
    resources.qrc
