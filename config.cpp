#include "config.h"
#include "properties.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <utility>

Config::Config(const QString& in_AppDirectory):
    m_Settings(nullptr)
{
    pathAppDir = in_AppDirectory;
    pathAppConfig = pathAppDir + QDir::separator() + CFG_APP;
    pathAppLogDir = pathAppDir + QDir::separator() + LOG_DIR;
    pathAssetsDir = pathAppDir + QDir::separator() + ASSETS_DIR;

    qDebug() << "AppConfig:" << pathAppConfig;
    qDebug() << "AppLogDir:" << pathAppLogDir;
    qDebug() << "AssetsDir:" << pathAssetsDir;

    m_Settings = new QSettings(pathAppConfig, QSettings::IniFormat);
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

    if(!m_Settings->contains("Camera/RotationSpeed"))
        m_Settings->setValue("Camera/RotationSpeed", ROTATION_SPEED);
    m_RotationSpeed = m_Settings->value("Camera/RotationSpeed").toInt();

    if(!m_Settings->contains("Camera/MoveSpeed"))
        m_Settings->setValue("Camera/MoveSpeed", MOVE_SPEED);
    m_MoveSpeed = m_Settings->value("Camera/MoveSpeed").toInt();

    if(!m_Settings->contains("Camera/RotationAcceleration"))
        m_Settings->setValue("Camera/RotationAcceleration", ROTATION_ACCELERATION);
    m_RotationAcceleration = m_Settings->value("Camera/RotationAcceleration").toInt();

    if(!m_Settings->contains("Camera/MoveAcceleration"))
        m_Settings->setValue("Camera/MoveAcceleration", MOVE_ACCELERATION);
    m_MoveAcceleration = m_Settings->value("Camera/MoveAcceleration").toInt();

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

    m_Settings->setValue("Camera/RotationSpeed", ROTATION_SPEED);
    m_RotationSpeed = ROTATION_SPEED;

    m_Settings->setValue("Camera/MoveSpeed", MOVE_SPEED);
    m_MoveSpeed = MOVE_SPEED;

    m_Settings->setValue("Camera/RotationAcceleration", ROTATION_ACCELERATION);
    m_RotationAcceleration = ROTATION_ACCELERATION;

    m_Settings->setValue("Camera/MoveAcceleration", MOVE_ACCELERATION);
    m_MoveAcceleration = MOVE_ACCELERATION;

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
}

void Config::setSceneDepth(int inSceneDepth)
{
    if(m_SceneDepth == inSceneDepth) return;
    m_SceneDepth = inSceneDepth;
    m_Settings->setValue("Scene/Depth", m_SceneDepth);
    emit signalConfigChanged();
}

void Config::setSceneWidth(int inSceneWidth)
{
    if(m_SceneWidth == inSceneWidth) return;
    m_SceneWidth = inSceneWidth;
    m_Settings->setValue("Scene/Width", m_SceneWidth);
    emit signalConfigChanged();
}

void Config::setSceneHeight(int inSceneHeight)
{
    if(m_SceneHeight == inSceneHeight) return;
    m_SceneHeight = inSceneHeight;
    m_Settings->setValue("Scene/Height", m_SceneHeight);
    emit signalConfigChanged();
}

void Config::setSceneCellSize(int inSceneCellSize)
{
    if(m_SceneCellSize == inSceneCellSize) return;
    m_SceneCellSize = inSceneCellSize;
    m_Settings->setValue("Scene/CellSize", m_SceneCellSize);
    emit signalConfigChanged();
}

void Config::setRewriteResources(bool inRewriteResources)
{
    if (m_RewriteResources == inRewriteResources) return;
    m_RewriteResources = inRewriteResources;
    m_Settings->setValue("Resources/RewriteResources", m_RewriteResources);
    emit signalConfigChanged();
}

void Config::setRotationSpeed(int inRotationSpeed)
{
    if (m_RotationSpeed == inRotationSpeed) return;
    m_RotationSpeed = inRotationSpeed;
    m_Settings->setValue("Camera/RotationSpeed", m_RotationSpeed);
    emit signalConfigChanged();
}

void Config::setMoveSpeed(int inMoveSpeed)
{
    if (m_MoveSpeed == inMoveSpeed) return;
    m_MoveSpeed = inMoveSpeed;
    m_Settings->setValue("Camera/MoveSpeed", m_MoveSpeed);
    emit signalConfigChanged();
}

void Config::setRotationAcceleration(int inRotationAcceleration)
{
    if (m_RotationAcceleration == inRotationAcceleration) return;
    m_RotationAcceleration = inRotationAcceleration;
    m_Settings->setValue("Camera/RotationAcceleration", m_RotationAcceleration);
    emit signalConfigChanged();
}

void Config::setMoveAcceleration(int inMoveAcceleration)
{
    if (m_MoveAcceleration == inMoveAcceleration) return;
    m_MoveAcceleration = inMoveAcceleration;
    m_Settings->setValue("Camera/MoveAcceleration", m_MoveAcceleration);
    emit signalConfigChanged();
}

QString Config::Caption(QString key)
{
    QString result = captions.value(key);
    return result.isEmpty() ? key : result;
}

void Config::setDateTimeFormat(QString inDateTimeFormat)
{
    if (m_DateTimeFormat == inDateTimeFormat) return;
    m_DateTimeFormat = std::move(inDateTimeFormat);
    m_Settings->setValue("DateTimeFormat", m_DateTimeFormat);
    emit signalConfigChanged();
}

void Config::setButtonAcceleration(int inButtonAcceleration)
{
    if (m_ButtonAcceleration == inButtonAcceleration) return;
    m_ButtonAcceleration = inButtonAcceleration;
    m_Settings->setValue("Keyboard/ButtonAcceleration", m_ButtonAcceleration);
    emit signalConfigChanged();
}

void Config::setDrawSceneBoxes(bool inDrawSceneBoxes)
{
    if (m_DrawSceneBoxes == inDrawSceneBoxes) return;
    m_DrawSceneBoxes = inDrawSceneBoxes;
    m_Settings->setValue("Scene/DrawBoxes", m_DrawSceneBoxes);
    emit signalDrawSceneBoxes(m_DrawSceneBoxes);
}

int Config::SceneDepth() const { return m_SceneDepth; }
int Config::SceneWidth() const { return m_SceneWidth; }
int Config::SceneHeight() const { return m_SceneHeight; }
int Config::SceneCellSize() const { return m_SceneCellSize; }
bool Config::RewriteResources() const { return m_RewriteResources; }
bool Config::DrawSceneBoxes() const { return m_DrawSceneBoxes; }
int Config::SplashTime() const { return m_SplashTime; }
int Config::ButtonAcceleration() const { return m_ButtonAcceleration; }
int Config::MoveAcceleration() const { return m_MoveAcceleration; }
int Config::RotationAcceleration() const { return m_RotationAcceleration; }
int Config::MoveSpeed() const { return m_MoveSpeed; }
int Config::RotationSpeed() const { return m_RotationSpeed; }
QString Config::DateTimeFormat() const { return m_DateTimeFormat; }
QString Config::PathApp() const { return pathAppDir; }
QString Config::PathAppLogDir() const { return pathAppLogDir; }
QString Config::PathAppConfig() const { return pathAppConfig; }
QString Config::PathAssetsDir() const { return pathAssetsDir; }

