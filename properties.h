/*
 * параметры приложения
 */
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "config.h"

#include <QSize>
#include <QString>

extern Config* config; // интерфейс над QSettings

// дефолтные параметры
const QString CFG_APP = "config.cfg";
const QString LOG_DIR = "logs";
const QString ASSETS_DIR = "assets";
const QString DT_FORMAT = "hh:mm:ss.zzz";
const QString TEXT_CODEC = "UTF-8";
const QString DEF_ASSETS = ":/assets";
const QString COLOR_SELECT = "#1047c7";     // синий
const QString COLOR_SCENE_BOX = "#000000";  // чёрный
const auto DIALOG_SIZE = QSize(400, 400);

const auto FRAME_RATE_COUNT_CALC = 50;
const auto WINDOW_HEIGHT = 700;
const auto WINDOW_WIDTH = 900;
const auto SPLASH_TIME = 1000;
const auto SCENE_CELL_SIZE = 10;
const auto SCENE_HEIGHT = 100;
const auto SCENE_WIDTH = 100;
const auto SCENE_DEPTH = 100;
const auto SELECTION_BOX_EXCESS = 0.1f;

const auto BUTTON_ACCELERATION = Qt::Key_Control;
const auto MOVE_ACCELERATION = 10;
const auto ROTATION_ACCELERATION = 5;
const auto ROTATION_SPEED = 20;
const auto MOVE_SPEED = 50;

#endif // PROPERTIES_H
