#include "config.h"
#include "properties.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <utility>

Config::Config(const QString& in_AppDirectory)
{
    pathAppDir = in_AppDirectory;
    pathAppConfig = pathAppDir + QDir::separator() + CFG_APP;
    pathAppLogDir = pathAppDir + QDir::separator() + LOG_DIR;
    pathAssetsDir = pathAppDir + QDir::separator() + ASSETS_DIR;

    qDebug() << "AppConfig:" << pathAppConfig;
    qDebug() << "AppLogDir:" << pathAppLogDir;
    qDebug() << "AssetsDir:" << pathAssetsDir;

    settings = new QSettings(pathAppConfig, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));

    load();
}

// записываем дефолты в конфиг-файл и формируем параметры
void Config::load()
{
    if(!settings->contains("MainWindow/Height"))
        settings->setValue("MainWindow/Height", WINDOW_HEIGHT);

    if(!settings->contains("MainWindow/Width"))
        settings->setValue("MainWindow/Width", WINDOW_WIDTH);

    if(!settings->contains("MainWindow/SplashTime"))
        settings->setValue("MainWindow/SplashTime", SPLASH_TIME);
    m_SplashTime = settings->value("MainWindow/SplashTime").toInt();

    if(!settings->contains("DateTimeFormat"))
        settings->setValue("DateTimeFormat", DT_FORMAT);
    m_DateTimeFormat = settings->value("DateTimeFormat").toString();

    if(!settings->contains("Keyboard/ButtonAcceleration"))
        settings->setValue("Keyboard/ButtonAcceleration", BUTTON_ACCELERATION);
    m_ButtonAcceleration = settings->value("Keyboard/ButtonAcceleration").toInt();

    if(!settings->contains("Camera/RotationSpeed"))
        settings->setValue("Camera/RotationSpeed", ROTATION_SPEED);
    m_RotationSpeed = settings->value("Camera/RotationSpeed").toInt();

    if(!settings->contains("Camera/MoveSpeed"))
        settings->setValue("Camera/MoveSpeed", MOVE_SPEED);
    m_MoveSpeed = settings->value("Camera/MoveSpeed").toInt();

    if(!settings->contains("Camera/RotationAcceleration"))
        settings->setValue("Camera/RotationAcceleration", ROTATION_ACCELERATION);
    m_RotationAcceleration = settings->value("Camera/RotationAcceleration").toInt();

    if(!settings->contains("Camera/MoveAcceleration"))
        settings->setValue("Camera/MoveAcceleration", MOVE_ACCELERATION);
    m_MoveAcceleration = settings->value("Camera/MoveAcceleration").toInt();
}

void Config::setDefaults()
{
    settings->setValue("MainWindow/SplashTime", SPLASH_TIME);
    m_SplashTime = SPLASH_TIME;

    settings->setValue("DateTimeFormat", DT_FORMAT);
    m_DateTimeFormat = DT_FORMAT;

    settings->setValue("Keyboard/ButtonAcceleration", BUTTON_ACCELERATION);
    m_ButtonAcceleration = settings->value("Keyboard/ButtonAcceleration").toInt();

    settings->setValue("Camera/RotationSpeed", ROTATION_SPEED);
    m_RotationSpeed = ROTATION_SPEED;

    settings->setValue("Camera/MoveSpeed", MOVE_SPEED);
    m_MoveSpeed = MOVE_SPEED;

    settings->setValue("Camera/RotationAcceleration", ROTATION_ACCELERATION);
    m_RotationAcceleration = ROTATION_ACCELERATION;

    settings->setValue("Camera/MoveAcceleration", MOVE_ACCELERATION);
    m_MoveAcceleration = MOVE_ACCELERATION;
}

void Config::setRotationSpeed(int RotationSpeed)
{
    m_RotationSpeed = RotationSpeed;
    settings->setValue("Camera/RotationSpeed", m_RotationSpeed);
    emit signalConfigChanged();
}

void Config::setMoveSpeed(int MoveSpeed)
{
    m_MoveSpeed = MoveSpeed;
    settings->setValue("Camera/MoveSpeed", m_MoveSpeed);
    emit signalConfigChanged();
}

void Config::setRotationAcceleration(int RotationAcceleration)
{
    m_RotationAcceleration = RotationAcceleration;
    settings->setValue("Camera/RotationAcceleration", m_RotationAcceleration);
    emit signalConfigChanged();
}

void Config::setMoveAcceleration(int MoveAcceleration)
{
    m_MoveAcceleration = MoveAcceleration;
    settings->setValue("Camera/MoveAcceleration", m_MoveAcceleration);
    emit signalConfigChanged();
}

QString Config::Caption(QString key)
{
    QString result = captions.value(key);
    return result.isEmpty() ? key : result;
}

void Config::setDateTimeFormat(QString DateTimeFormat)
{
    m_DateTimeFormat = std::move(DateTimeFormat);
    settings->setValue("DateTimeFormat", m_DateTimeFormat);
    emit signalConfigChanged();
}

void Config::setButtonAcceleration(int ButtonAcceleration)
{
    m_ButtonAcceleration = ButtonAcceleration;
    settings->setValue("Keyboard/ButtonAcceleration", m_ButtonAcceleration);
    emit signalConfigChanged();
}

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

