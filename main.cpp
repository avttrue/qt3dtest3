#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QDir>

#include "mainwindow.h"
#include "properties.h"
#include "helpers.h"

bool prepare()
{
    // каталог логов
    if(!QDir().exists(config->PathAppLogDir()) && !QDir().mkpath(config->PathAppLogDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathAppLogDir();
        return false;
    }

    // копируются дефолтные текстуры
    if(!copyResources(DEF_TEXTURES, config->PathAssetsDir())) return false;

    // копируются дефолтные модели
    if(!copyResources(DEF_MODELS, config->PathAssetsDir())) return false;

    return  true;
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    application.setStyleSheet(getTextFromRes(":/res/qss/main.css"));

    // параметры
    config = new Config(application.applicationDirPath());

    if(!prepare())
    {
        qCritical() << "Fatal error";
        exit(0);
    }

    MainWindow window;

    window.show();
    return application.exec();
}
