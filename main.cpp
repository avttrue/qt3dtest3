#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QSurfaceFormat>

#include "mainwindow.h"
#include "properties.h"
#include "helpers.h"

// подготовка обработчика консоли
static QScopedPointer<QFile> m_logFile;
void consoleOut(QtMsgType type, const QMessageLogContext &context, const QString &msg);

bool prepare()
{
    // копируются дефолтные текстуры
    if(!copyResources(DEF_TEXTURES, config->PathAssetsDir(), config->RewriteResources())) return false;

    // копируются дефолтные модели
    if(!copyResources(DEF_MODELS, config->PathAssetsDir(), config->RewriteResources())) return false;

    return  true;
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // OpenGL surface format
    QSurfaceFormat format;
    format.setDepthBufferSize(32);
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);

    auto arglist = application.arguments();

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    config = new Config(application.applicationDirPath());

    if(arglist.contains(APP_ARG_WRITE_LOGS))
    {
        qInfo() << "Logging to file is enabled";
        // каталог логов
        if(!QDir().exists(config->PathAppLogDir()) && !QDir().mkpath(config->PathAppLogDir()))
        {
            qCritical() << "Directory not exist and cannot be created:" << config->PathAppLogDir();
        }
        else
        {
            auto filenameprefix = QDateTime::currentDateTime().toString(LOG_FILE_PREFIX);
            auto logfile = config->PathAppLogDir() + QDir::separator() + filenameprefix + ".log";

            m_logFile.reset(new QFile(logfile));
            m_logFile.data()->open(QFile::Append | QFile::Text);

            qInstallMessageHandler(consoleOut);
        }
    }
    else qInfo() << "Logging to file is disabled, use" << APP_ARG_WRITE_LOGS << "key for enabling";

    qInfo() << "Git build:" << GIT_VERS;

    application.setStyleSheet(getTextFromRes(":/res/qss/main.css"));

    if(!prepare())
    {
        qCritical() << "Fatal error";
        exit(0);
    }

    MainWindow window;

    window.show();
    return application.exec();
}

// обработчик консоли
void consoleOut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream log_out(m_logFile.data());
    log_out.setCodec(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));

    log_out << QDateTime::currentDateTime().toString(config->DateTimeFormat().append("\t"));

    switch (type)
    {
    case QtDebugMsg:
    {
        log_out << "DBG:\t";
        log_out << context.file << " : " << context.line << "\t" << msg << endl;
        break;
    }
    case QtInfoMsg:
    {
        log_out << "INF:\t";
        log_out << context.file << " : " << context.line << "\t" << msg << endl;
        break;
    }
    case QtWarningMsg:
    {
        log_out << "WRN:\t";
        log_out << context.file << " : " << context.line << "\t" << msg << endl;
        break;
    }
    case QtCriticalMsg:
    {
        log_out << "CRT:\t";
        log_out << context.file << " : " << context.line << "\t" << msg << endl;
        break;
    }
    case QtFatalMsg:
    {
        log_out << "FTL:\t";
        log_out << context.file << " : " << context.line << "\t" << msg << endl;
        break;
    }}

    log_out.flush();
}
