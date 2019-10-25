#ifndef HELPERS_H
#define HELPERS_H

#include <QString>

/*!
 * \brief getTextFromRes - получить текстовый файл из ресурсов приложения.
 */
QString getTextFromRes(const QString& path);

/*!
 * \brief getSystemInfo - получить информацию об ОС.
 */
QString getSystemInfo();

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
 * \brief numDigits - получить количество знаков в числе.
 */
template <class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1;
    while (number)
    {
        number /= 10;
        digits++;
    }
    return digits;
}

#endif // HELPERS_H
