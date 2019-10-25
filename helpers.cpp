#include "helpers.h"

#include <QDateTime>
#include <QFile>
#include <QtMath>
#include <sys/utsname.h>
#include <QDebug>

QString getTextFromRes(const QString& path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QString text(file.readAll());
        return text;
    }
    qCritical() << "Resurce not found:" << path;
    return "";
}

QString getSystemInfo()
{
    QString result;

    struct utsname buf{};
    if (uname(&buf) == 0)
    {
        result.append(buf.sysname).append(" ").
                append(buf.version).append(" ").
                append(buf.release).append(" ").
                append(buf.machine);
        return result;
    }
    return "unknown";
}

QString readableByteCount(qint64 bytes, bool si)
{
    int unit = si ? 1000 : 1024;
    if (bytes < unit) return QString("%1B").arg(QString::number(bytes));

    int exp = static_cast<int>(qLn(bytes) / qLn(unit));
    QString pre = si ? "kMGTPE" : "KMGTPE";

    return QString("%1%2%3B").
            arg(bytes / qPow(unit, exp), 0, 'f', 1, '0').
            arg(pre[exp - 1]).
            arg(si ? "" : "i");
}

bool textToFile(const QString& text, const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical() << "File not open for writing:" << path;
        return false;
    }

    QTextStream stream(&file);
    stream << text;
    file.close();

    if (stream.status() != QTextStream::Ok)
    {
        qCritical() << "Error at file saving:" << path;
        return false;
    }
    return true;
}

QString fileToText(const QString& path)
{
    QString text;
    QFile file(path);

    if (!file.exists())
    {
        qCritical() << "File not found:" << path;
        return nullptr;
    }

    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qCritical() << "File not open for reading:" << path;
        return nullptr;
    }

    QTextStream stream(&file);
    text = stream.readAll();
    file.close();
    return text;
}

