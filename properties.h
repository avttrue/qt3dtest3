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
const QString APP_ARG_WRITE_LOGS = "-logtofile";
const QString LOG_DIR = "logs";
const QString LOG_FILE_PREFIX = "yyyyMMdd_hhmmss";
const QString ASSETS_DIR = "assets";
const QString DT_FORMAT = "hh:mm:ss.zzz";
const QString TEXT_CODEC = "UTF-8";
const QString DEF_TEXTURES = ":/textures";
const QString DEF_MODELS = ":/models";
const QString MATERIAL_EXTENSION = "*.material";
const QString GEOMETRY_EXTENSION = "*.obj";
const QString DEFAULT_TEXTURE = ":/models/default_texture.png";
const QString SCENE_COLOR_SELECT = "#1047C7";     // синий
const QString SCENE_COLOR_BOX = "#000000";  // чёрный
const QString SCENE_COLOR_BG = "#DCDCDC";   // светлосерый
const QString SCENE_COLOR_GRID = "#FFFAFA";// тёмнобелый
const QString RE_FIRST_LAST_SPACES = "^\\s+|\\s+$"; // регексп на наличие стартовых и финальных пробелов
const QString SCENE_EXCESS = "0.1";
const auto RENDERER_SORT_POLICY_TYPE = "BackToFront";
const auto DIALOG_SIZE = QSize(400, 400);
const auto SCENE_DRAW_BOXES = true;
const auto REWRITE_RESOURCES = false;
const auto RENDERER_CULL_FACE_MODE = false;
const auto FRAME_RATE_PERIOD = 500;
const auto WINDOW_HEIGHT = 700;
const auto WINDOW_WIDTH = 900;
const auto SPLASH_TIME = 1000;
const auto SCENE_CELL_SIZE = 10;
const auto SCENE_HEIGHT = 15;
const auto SCENE_WIDTH = 15;
const auto SCENE_DEPTH = 15;
const auto BUTTON_ACCELERATION = Qt::Key_Control;
const auto BUTTON_FORWARD = Qt::Key_W;
const auto BUTTON_BACKWADR = Qt::Key_S;
const auto BUTTON_LEFTWARD = Qt::Key_A;
const auto BUTTON_RIGHTWARD = Qt::Key_D;
const auto BUTTON_UPWARD = Qt::Key_Shift;
const auto BUTTON_DOWNWARD = Qt::Key_Space;
const auto CAMERA_MOVE_ACCELERATION = 10;
const auto CAMERA_ROTATION_ACCELERATION = 5;
const auto CAMERA_ROTATION_SPEED = 30;
const auto CAMERA_MOVE_SPEED = 50;
const auto CAMERA_FIELD_OF_VIEW = 45.0f;

#endif // PROPERTIES_H
