#include "config.h"
#include "properties.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <utility>

Config::Config(const QString& in_AppDirectory):
    m_Settings(nullptr)
{
    m_PathAppDir = in_AppDirectory;
    m_PathAppConfig = m_PathAppDir + QDir::separator() + CFG_APP;
    m_PathAppLogDir = m_PathAppDir + QDir::separator() + LOG_DIR;
    m_PathAssetsDir = m_PathAppDir + QDir::separator() + ASSETS_DIR;

    qInfo() << "AppConfig:" << m_PathAppConfig;
    qInfo() << "AppLogDir:" << m_PathAppLogDir;
    qInfo() << "AssetsDir:" << m_PathAssetsDir;

    m_Settings = new QSettings(m_PathAppConfig, QSettings::IniFormat);
    m_Settings->setIniCodec(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));

    load();
}

// записываем дефолты в конфиг-файл и формируем параметры
void Config::load()
{
    if(!m_Settings->contains("MainWindow/Height"))
        m_Settings->setValue("MainWindow/Height", WINDOW_HEIGHT);

    if(!m_Settings->contains("MainWindow/Width"))
        m_Settings->setValue("MainWindow/Width", WINDOW_WIDTH);

    if(!m_Settings->contains("MainWindow/SplashTime"))
        m_Settings->setValue("MainWindow/SplashTime", SPLASH_TIME);
    m_SplashTime = m_Settings->value("MainWindow/SplashTime").toInt();

    if(!m_Settings->contains("DateTimeFormat"))
        m_Settings->setValue("DateTimeFormat", DT_FORMAT);
    m_DateTimeFormat = m_Settings->value("DateTimeFormat").toString();

    if(!m_Settings->contains("Resources/RewriteResources"))
        m_Settings->setValue("Resources/RewriteResources", REWRITE_RESOURCES);
    m_RewriteResources = m_Settings->value("Resources/RewriteResources").toBool();

    if(!m_Settings->contains("Keyboard/ButtonAcceleration"))
        m_Settings->setValue("Keyboard/ButtonAcceleration", BUTTON_ACCELERATION);
    m_ButtonAcceleration = m_Settings->value("Keyboard/ButtonAcceleration").toInt();

    if(!m_Settings->contains("Keyboard/ButtonForward"))
        m_Settings->setValue("Keyboard/ButtonForward", BUTTON_FORWARD);
    m_ButtonForward = m_Settings->value("Keyboard/ButtonForward").toInt();

    if(!m_Settings->contains("Keyboard/ButtonBackward"))
        m_Settings->setValue("Keyboard/ButtonBackward", BUTTON_BACKWADR);
    m_ButtonBackward = m_Settings->value("Keyboard/ButtonBackward").toInt();

    if(!m_Settings->contains("Keyboard/ButtonLeftward"))
        m_Settings->setValue("Keyboard/ButtonLeftward", BUTTON_LEFTWARD);
    m_ButtonLeftward = m_Settings->value("Keyboard/ButtonLeftward").toInt();

    if(!m_Settings->contains("Keyboard/ButtonRightward"))
        m_Settings->setValue("Keyboard/ButtonRightward", BUTTON_RIGHTWARD);
    m_ButtonRightward = m_Settings->value("Keyboard/ButtonRightward").toInt();

    if(!m_Settings->contains("Keyboard/ButtonUpward"))
        m_Settings->setValue("Keyboard/ButtonUpward", BUTTON_UPWARD);
    m_ButtonUpward = m_Settings->value("Keyboard/ButtonUpward").toInt();

    if(!m_Settings->contains("Keyboard/ButtonDownward"))
        m_Settings->setValue("Keyboard/ButtonDownward", BUTTON_DOWNWARD);
    m_ButtonDownward = m_Settings->value("Keyboard/ButtonDownward").toInt();

    if(!m_Settings->contains("Camera/RotationSpeed"))
        m_Settings->setValue("Camera/RotationSpeed", CAMERA_ROTATION_SPEED);
    m_CameraRotationSpeed = m_Settings->value("Camera/RotationSpeed").toInt();

    if(!m_Settings->contains("Camera/MoveSpeed"))
        m_Settings->setValue("Camera/MoveSpeed", CAMERA_MOVE_SPEED);
    m_CameraMoveSpeed = m_Settings->value("Camera/MoveSpeed").toInt();

    if(!m_Settings->contains("Camera/RotationAcceleration"))
        m_Settings->setValue("Camera/RotationAcceleration", CAMERA_ROTATION_ACCELERATION);
    m_CameraRotationAcceleration = m_Settings->value("Camera/RotationAcceleration").toInt();

    if(!m_Settings->contains("Camera/MoveAcceleration"))
        m_Settings->setValue("Camera/MoveAcceleration", CAMERA_MOVE_ACCELERATION);
    m_CameraMoveAcceleration = m_Settings->value("Camera/MoveAcceleration").toInt();

    if(!m_Settings->contains("Scene/DrawBoxes"))
        m_Settings->setValue("Scene/DrawBoxes", SCENE_DRAW_BOXES);
    m_DrawSceneBoxes = m_Settings->value("Scene/DrawBoxes").toBool();

    if(!m_Settings->contains("Scene/Depth"))
    m_Settings->setValue("Scene/Depth", SCENE_DEPTH);
    m_SceneDepth = m_Settings->value("Scene/Depth").toInt();

    if(!m_Settings->contains("Scene/Width"))
    m_Settings->setValue("Scene/Width", SCENE_WIDTH);
    m_SceneWidth = m_Settings->value("Scene/Width").toInt();

    if(!m_Settings->contains("Scene/Height"))
    m_Settings->setValue("Scene/Height", SCENE_HEIGHT);
    m_SceneHeight = m_Settings->value("Scene/Height").toInt();

    if(!m_Settings->contains("Scene/CellSize"))
    m_Settings->setValue("Scene/CellSize", SCENE_CELL_SIZE);
    m_SceneCellSize = m_Settings->value("Scene/CellSize").toInt();

    if(!m_Settings->contains("Scene/ColorBG"))
        m_Settings->setValue("Scene/ColorBG", SCENE_COLOR_BG);
    m_SceneColorBG = m_Settings->value("Scene/ColorBG").toString();

    if(!m_Settings->contains("Scene/ColorSelect"))
        m_Settings->setValue("Scene/ColorSelect", SCENE_COLOR_SELECT);
    m_SceneColorSelect = m_Settings->value("Scene/ColorSelect").toString();

    if(!m_Settings->contains("Scene/ColorBox"))
        m_Settings->setValue("Scene/ColorBox", SCENE_COLOR_BOX);
    m_SceneColorBox = m_Settings->value("Scene/ColorBox").toString();

    if(!m_Settings->contains("Scene/ColorGrid"))
        m_Settings->setValue("Scene/ColorGrid", SCENE_COLOR_GRID);
    m_SceneColorGrid = m_Settings->value("Scene/ColorGrid").toString();

    if(!m_Settings->contains("Scene/Excess"))
        m_Settings->setValue("Scene/Excess", SCENE_EXCESS);
    m_SceneExcess = m_Settings->value("Scene/Excess").toFloat();

    if(!m_Settings->contains("Renderer/SortPolicyType"))
        m_Settings->setValue("Renderer/SortPolicyType", RENDERER_SORT_POLICY_TYPE);
    m_RendererSortPolicyType = m_Settings->value("Renderer/SortPolicyType").toString();

    if(!m_Settings->contains("Renderer/CullFaceMode"))
        m_Settings->setValue("Renderer/CullFaceMode", RENDERER_CULL_FACE_MODE);
    m_RendererCullFaceMode = m_Settings->value("Renderer/CullFaceMode").toBool();
}

void Config::setDefaults()
{
    m_Settings->setValue("MainWindow/SplashTime", SPLASH_TIME);
    m_SplashTime = SPLASH_TIME;

    m_Settings->setValue("DateTimeFormat", DT_FORMAT);
    m_DateTimeFormat = DT_FORMAT;

    m_Settings->setValue("Resources/RewriteResources", REWRITE_RESOURCES);
    m_RewriteResources = m_Settings->value("Resources/RewriteResources").toBool();

    m_Settings->setValue("Keyboard/ButtonAcceleration", BUTTON_ACCELERATION);
    m_ButtonAcceleration = m_Settings->value("Keyboard/ButtonAcceleration").toInt();

    m_Settings->setValue("Keyboard/ButtonForward", BUTTON_FORWARD);
    m_ButtonForward = m_Settings->value("Keyboard/ButtonForward").toInt();

    m_Settings->setValue("Keyboard/ButtonBackward", BUTTON_BACKWADR);
    m_ButtonBackward = m_Settings->value("Keyboard/ButtonBackward").toInt();

    m_Settings->setValue("Keyboard/ButtonLeftward", BUTTON_LEFTWARD);
    m_ButtonLeftward = m_Settings->value("Keyboard/ButtonLeftward").toInt();

    m_Settings->setValue("Keyboard/ButtonRightward", BUTTON_RIGHTWARD);
    m_ButtonRightward = m_Settings->value("Keyboard/ButtonRightward").toInt();

    m_Settings->setValue("Keyboard/ButtonUpward", BUTTON_UPWARD);
    m_ButtonUpward = m_Settings->value("Keyboard/ButtonUpward").toInt();

    m_Settings->setValue("Keyboard/ButtonDownward", BUTTON_DOWNWARD);
    m_ButtonDownward = m_Settings->value("Keyboard/ButtonDownward").toInt();

    m_Settings->setValue("Camera/RotationSpeed", CAMERA_ROTATION_SPEED);
    m_CameraRotationSpeed = CAMERA_ROTATION_SPEED;

    m_Settings->setValue("Camera/MoveSpeed", CAMERA_MOVE_SPEED);
    m_CameraMoveSpeed = CAMERA_MOVE_SPEED;

    m_Settings->setValue("Camera/RotationAcceleration", CAMERA_ROTATION_ACCELERATION);
    m_CameraRotationAcceleration = CAMERA_ROTATION_ACCELERATION;

    m_Settings->setValue("Camera/MoveAcceleration", CAMERA_MOVE_ACCELERATION);
    m_CameraMoveAcceleration = CAMERA_MOVE_ACCELERATION;

    m_Settings->setValue("Scene/DrawBoxes", SCENE_DRAW_BOXES);
    m_DrawSceneBoxes = SCENE_DRAW_BOXES;

    m_Settings->setValue("Scene/Depth", SCENE_DEPTH);
    m_SceneDepth = SCENE_DEPTH;

    m_Settings->setValue("Scene/Width", SCENE_WIDTH);
    m_SceneWidth = SCENE_WIDTH;

    m_Settings->setValue("Scene/Height", SCENE_HEIGHT);
    m_SceneHeight = SCENE_HEIGHT;

    m_Settings->setValue("Scene/CellSize", SCENE_CELL_SIZE);
    m_SceneCellSize = SCENE_CELL_SIZE;

    m_Settings->setValue("Scene/ColorBG", SCENE_COLOR_BG);
    m_SceneColorBG = SCENE_COLOR_BG;

    m_Settings->setValue("Scene/Excess", QString(SCENE_EXCESS).toFloat());
    m_SceneExcess = QString(SCENE_EXCESS).toFloat();

    m_Settings->setValue("Scene/ColorSelect", SCENE_COLOR_SELECT);
    m_SceneColorSelect = SCENE_COLOR_SELECT;

    m_Settings->setValue("Scene/ColorBox", SCENE_COLOR_BOX);
    m_SceneColorBox = SCENE_COLOR_BOX;

    m_Settings->setValue("Scene/ColorGrid", SCENE_COLOR_GRID);
    m_SceneColorGrid = SCENE_COLOR_GRID;

    m_Settings->setValue("Renderer/SortPolicyType", RENDERER_SORT_POLICY_TYPE);
    m_RendererSortPolicyType = RENDERER_SORT_POLICY_TYPE;

    m_Settings->setValue("Renderer/CullFaceMode", RENDERER_CULL_FACE_MODE);
    m_RendererCullFaceMode = RENDERER_CULL_FACE_MODE;

    Q_EMIT signalConfigChanged();
    Q_EMIT signalDrawSceneBoxes(m_DrawSceneBoxes);
    Q_EMIT signalRendererCullFaceMode(m_RendererCullFaceMode);
    Q_EMIT signalRendererSortPolicyType(m_RendererSortPolicyType);
}

void Config::setRendererCullFaceMode(bool inRendererCullFaceMode)
{
    if(m_RendererCullFaceMode == inRendererCullFaceMode) return;
    m_RendererCullFaceMode = inRendererCullFaceMode;
    m_Settings->setValue("Renderer/CullFaceMode", m_RendererCullFaceMode);
    Q_EMIT signalConfigChanged();
    Q_EMIT signalRendererCullFaceMode(m_RendererCullFaceMode);
}


void Config::setRendererSortPolicyType(const QString &inRendererSortPolicyType)
{
    if(m_RendererSortPolicyType == inRendererSortPolicyType) return;
    m_RendererSortPolicyType = inRendererSortPolicyType;
    m_Settings->setValue("Renderer/SortPolicyType", m_RendererSortPolicyType);
    Q_EMIT signalConfigChanged();
    Q_EMIT signalRendererSortPolicyType(m_RendererSortPolicyType);
}

void Config::setSceneColorGrid(const QString &inSceneColorGrid)
{
    if(m_SceneColorGrid == inSceneColorGrid) return;
    m_SceneColorGrid = inSceneColorGrid;
    m_Settings->setValue("Scene/ColorGrid", m_SceneColorGrid);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneColorBox(const QString &inSceneColorBox)
{
    if(m_SceneColorBox == inSceneColorBox) return;
    m_SceneColorBox = inSceneColorBox;
    m_Settings->setValue("Scene/ColorBox", m_SceneColorBox);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneColorSelect(const QString &inSceneColorSelect)
{
    if(m_SceneColorSelect == inSceneColorSelect) return;
    m_SceneColorSelect = inSceneColorSelect;
    m_Settings->setValue("Scene/ColorSelect", m_SceneColorSelect);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneExcess(float inSceneExcess)
{
    if(abs(m_SceneExcess - inSceneExcess) < 0.0001f) return;
    m_SceneExcess = inSceneExcess;
    m_Settings->setValue("Scene/ColorBG", m_SceneExcess);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneColorBG(const QString &inSceneColorBG)
{
    if(m_SceneColorBG == inSceneColorBG) return;
    m_SceneColorBG = inSceneColorBG;
    m_Settings->setValue("Scene/ColorBG", m_SceneColorBG);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneDepth(int inSceneDepth)
{
    if(m_SceneDepth == inSceneDepth) return;
    m_SceneDepth = inSceneDepth;
    m_Settings->setValue("Scene/Depth", m_SceneDepth);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneWidth(int inSceneWidth)
{
    if(m_SceneWidth == inSceneWidth) return;
    m_SceneWidth = inSceneWidth;
    m_Settings->setValue("Scene/Width", m_SceneWidth);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneHeight(int inSceneHeight)
{
    if(m_SceneHeight == inSceneHeight) return;
    m_SceneHeight = inSceneHeight;
    m_Settings->setValue("Scene/Height", m_SceneHeight);
    Q_EMIT signalConfigChanged();
}

void Config::setSceneCellSize(int inSceneCellSize)
{
    if(m_SceneCellSize == inSceneCellSize) return;
    m_SceneCellSize = inSceneCellSize;
    m_Settings->setValue("Scene/CellSize", m_SceneCellSize);
    Q_EMIT signalConfigChanged();
}

void Config::setRewriteResources(bool inRewriteResources)
{
    if (m_RewriteResources == inRewriteResources) return;
    m_RewriteResources = inRewriteResources;
    m_Settings->setValue("Resources/RewriteResources", m_RewriteResources);
    Q_EMIT signalConfigChanged();
}

void Config::setCameraRotationSpeed(int inRotationSpeed)
{
    if (m_CameraRotationSpeed == inRotationSpeed) return;
    m_CameraRotationSpeed = inRotationSpeed;
    m_Settings->setValue("Camera/RotationSpeed", m_CameraRotationSpeed);
    Q_EMIT signalConfigChanged();
}

void Config::setCameraMoveSpeed(int inMoveSpeed)
{
    if (m_CameraMoveSpeed == inMoveSpeed) return;
    m_CameraMoveSpeed = inMoveSpeed;
    m_Settings->setValue("Camera/MoveSpeed", m_CameraMoveSpeed);
    Q_EMIT signalConfigChanged();
}

void Config::setCameraRotationAcceleration(int inRotationAcceleration)
{
    if (m_CameraRotationAcceleration == inRotationAcceleration) return;
    m_CameraRotationAcceleration = inRotationAcceleration;
    m_Settings->setValue("Camera/RotationAcceleration", m_CameraRotationAcceleration);
    Q_EMIT signalConfigChanged();
}

void Config::setCameraMoveAcceleration(int inMoveAcceleration)
{
    if (m_CameraMoveAcceleration == inMoveAcceleration) return;
    m_CameraMoveAcceleration = inMoveAcceleration;
    m_Settings->setValue("Camera/MoveAcceleration", m_CameraMoveAcceleration);
    Q_EMIT signalConfigChanged();
}

void Config::setDateTimeFormat(QString inDateTimeFormat)
{
    if (m_DateTimeFormat == inDateTimeFormat) return;
    m_DateTimeFormat = std::move(inDateTimeFormat);
    m_Settings->setValue("DateTimeFormat", m_DateTimeFormat);
    Q_EMIT signalConfigChanged();
}

void Config::setButtonAcceleration(int inButtonAcceleration)
{
    if (m_ButtonAcceleration == inButtonAcceleration) return;
    m_ButtonAcceleration = inButtonAcceleration;
    m_Settings->setValue("Keyboard/ButtonAcceleration", m_ButtonAcceleration);
    Q_EMIT signalConfigChanged();
}

void Config::setButtonRightward(int inButtonRightward)
{
    if (m_ButtonRightward == inButtonRightward) return;
    m_ButtonRightward = inButtonRightward;
    m_Settings->setValue("Keyboard/ButtonRightward", m_ButtonRightward);
    Q_EMIT signalConfigChanged();
}

void Config::setButtonLeftward(int inButtonLeftward)
{
    if (m_ButtonLeftward == inButtonLeftward) return;
    m_ButtonLeftward = inButtonLeftward;
    m_Settings->setValue("Keyboard/ButtonLeftward", m_ButtonLeftward);
    Q_EMIT signalConfigChanged();
}

void Config::setButtonBackward(int inButtonBackward)
{
    if (m_ButtonBackward == inButtonBackward) return;
    m_ButtonBackward = inButtonBackward;
    m_Settings->setValue("Keyboard/ButtonBackward", m_ButtonBackward);
    Q_EMIT signalConfigChanged();
}

void Config::setButtonForward(int inButtonForward)
{
    if (m_ButtonForward == inButtonForward) return;
    m_ButtonForward = inButtonForward;
    m_Settings->setValue("Keyboard/ButtonForward", m_ButtonForward);
    Q_EMIT signalConfigChanged();
}

void Config::setButtonDownward(int inButtonDownward)
{
    if (m_ButtonDownward == inButtonDownward) return;
    m_ButtonDownward = inButtonDownward;
    m_Settings->setValue("Keyboard/ButtonDownward", m_ButtonDownward);
    Q_EMIT signalConfigChanged();
}

void Config::setButtonUpward(int inButtonUpward)
{
    if (m_ButtonUpward == inButtonUpward) return;
    m_ButtonUpward = inButtonUpward;
    m_Settings->setValue("Keyboard/ButtonUpward", m_ButtonUpward);
    Q_EMIT signalConfigChanged();
}

void Config::setDrawSceneBoxes(bool inDrawSceneBoxes)
{
    if (m_DrawSceneBoxes == inDrawSceneBoxes) return;
    m_DrawSceneBoxes = inDrawSceneBoxes;
    m_Settings->setValue("Scene/DrawBoxes", m_DrawSceneBoxes);
    Q_EMIT signalDrawSceneBoxes(m_DrawSceneBoxes);
}

float Config::SceneExcess() const { return m_SceneExcess; }
int Config::SceneDepth() const { return m_SceneDepth; }
int Config::SceneWidth() const { return m_SceneWidth; }
int Config::SceneHeight() const { return m_SceneHeight; }
int Config::SceneCellSize() const { return m_SceneCellSize; }
bool Config::RendererCullFaceMode() const { return m_RendererCullFaceMode; }
bool Config::RewriteResources() const { return m_RewriteResources; }
bool Config::DrawSceneBoxes() const { return m_DrawSceneBoxes; }
QString Config::RendererSortPolicyType() const { return m_RendererSortPolicyType; }
int Config::SplashTime() const { return m_SplashTime; }
int Config::ButtonAcceleration() const { return m_ButtonAcceleration; }
int Config::ButtonRightward() const { return m_ButtonRightward; }
int Config::ButtonLeftward() const { return m_ButtonLeftward; }
int Config::ButtonBackward() const { return m_ButtonBackward; }
int Config::ButtonForward() const { return m_ButtonForward; }
int Config::ButtonDownward() const { return m_ButtonDownward; }
int Config::ButtonUpward() const { return m_ButtonUpward; }
int Config::CameraMoveAcceleration() const { return m_CameraMoveAcceleration; }
int Config::CameraRotationAcceleration() const { return m_CameraRotationAcceleration; }
int Config::CameraMoveSpeed() const { return m_CameraMoveSpeed; }
int Config::CameraRotationSpeed() const { return m_CameraRotationSpeed; }
QString Config::DateTimeFormat() const { return m_DateTimeFormat; }
QString Config::PathApp() const { return m_PathAppDir; }
QString Config::PathAppLogDir() const { return m_PathAppLogDir; }
QString Config::PathAppConfig() const { return m_PathAppConfig; }
QString Config::PathAssetsDir() const { return m_PathAssetsDir; }
QString Config::SceneColorBG() const { return m_SceneColorBG; }
QString Config::SceneColorSelect() const { return m_SceneColorSelect; }
QString Config::SceneColorBox() const { return m_SceneColorBox; }
QString Config::SceneColorGrid() const { return m_SceneColorGrid; }
