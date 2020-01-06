#ifndef HELPERS_H
#define HELPERS_H

#include <QMetaProperty>
#include <QString>

/*!
 * \brief getTextFromRes - получить текстовый файл из ресурсов приложения.
 */
QString getTextFromRes(const QString& path);

/*!
 * \brief getSystemInfo - получить информацию об ОС.
 */
extern QString getSystemInfo();

/*!
 * \brief humanReadableByteCount
 * \param bytes - количество байт для пересчёта
 * \param si - SI или BINARY формат
 * \return количество байт в виде строки
 */
QString readableByteCount(qint64 bytes, bool si = false);

/*!
 * \brief textToFile - записать текст в файл.
 */
bool textToFile(const QString& text, const QString& path);

/*!
 * \brief fileToText - считать файл как текст.
 */
QString fileToText(const QString& path);

/*!
 * \brief copyResources - скопировать файлы из outPath (qrc) в inPath
 */
bool copyResources(const QString& outPath, const QString& inPath, bool rewrite = false);


#endif // HELPERS_H
