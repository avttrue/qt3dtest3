#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QDir>

#include "mainwindow.h"
#include "properties.h"

bool prepare()
{
    // каталог логов
    if(!QDir().exists(config->PathAppLogDir()) && !QDir().mkpath(config->PathAppLogDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathAppLogDir();
        return false;
    }

    // каталог ресурсов
    if(!QDir().exists(config->PathAssetsDir()) && !QDir().mkpath(config->PathAssetsDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathAssetsDir();
        return false;
    }

    // копируются дефолтные ресурсы
    if(QDir().exists(config->PathAssetsDir()))
    {
        QDir resdir(DEF_ASSETS);
        QStringList filesList = resdir.entryList(QDir::Files);
        for(QString f: filesList)
        {
            QFile file(resdir.path() + "/" + f);
            QString newfilename = config->PathAssetsDir() + QDir::separator() + f;

            if(!QFile::exists(newfilename) && !file.copy(newfilename))
            {
                qCritical() << "Unable to write file" << newfilename << "from" << file.fileName();
                return false;
            }
            else
            {
                QFileDevice::Permissions p = QFile(newfilename).permissions();
                QFile::setPermissions(newfilename, p | QFileDevice::WriteOwner | QFileDevice::ReadOwner);
                qDebug() << "Resource" << newfilename << "ready";
            }
        }
    }
    return  true;
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));

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
