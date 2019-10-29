#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QSettings>

/*!
 * \brief Config - настройки программы с загрузкой из файла конфига и сохранением в файл.
 * Интерфейс над QSettings.
 */
class Config : public QObject
{
    Q_OBJECT
    // в Q_PROPERTY пишутся только те параметры, которые может изменять пользователь
    Q_PROPERTY(QString DateTimeFormat READ DateTimeFormat WRITE setDateTimeFormat FINAL)
    Q_PROPERTY(bool DrawSceneBoxes READ DrawSceneBoxes WRITE setDrawSceneBoxes NOTIFY signalDrawSceneBoxes FINAL)
    Q_PROPERTY(int ButtonAcceleration READ ButtonAcceleration WRITE setButtonAcceleration FINAL)

public:
    explicit Config(const QString& in_AppDirectory);
    void setDefaults();
    QString Caption(QString key); // получить название параметра в читабельном виде

    // Q_PROPERTY
    QString DateTimeFormat() const;
    void setDateTimeFormat(QString inDateTimeFormat);
    int ButtonAcceleration() const;
    void setButtonAcceleration(int inButtonAcceleration);
    bool DrawSceneBoxes() const;
    void setDrawSceneBoxes(bool inDrawSceneBoxes);

    //
    int SplashTime() const;
    QString PathApp() const;
    QString PathAppConfig() const;
    QString PathAssetsDir() const;
    QString PathAppLogDir() const;   
    int MoveAcceleration() const;
    void setMoveAcceleration(int inMoveAcceleration);
    int RotationAcceleration() const;
    void setRotationAcceleration(int inRotationAcceleration);
    int MoveSpeed() const;
    void setMoveSpeed(int inMoveSpeed);
    int RotationSpeed() const;
    void setRotationSpeed(int inRotationSpeed);

protected:
    void load();

private:
    QSettings* m_Settings;
    QString m_DateTimeFormat;           // формат даты и времени
    QString pathAppConfig;              // путь до конфига приложения
    QString pathAppDir;                 // путь до приложения
    QString pathAppLogDir;              // путь до логов приложения
    QString pathAssetsDir;              // путь до ресурсов

    bool m_DrawSceneBoxes;              // отображать контуры сцены и служебных объектов
    int m_SplashTime;                   // время отображения сплеш-заставки
    int m_ButtonAcceleration;           // кнопка ускорения перемещения
    int m_MoveAcceleration;             // ускорение линейного перемещения
    int m_RotationAcceleration;         // ускорение поворота
    int m_MoveSpeed;                    // скорость линейного перемещения
    int m_RotationSpeed;                // скорость поворота 


    const QHash<QString, QString> captions =
        {
            {"DateTimeFormat", tr("формат времени")},
            {"ButtonAcceleration", tr("кнопка ускорения камеры")},
            {"DrawSceneBoxes", tr("отображать контуры сцены и сл. объектов")}
        };



signals:
    void signalConfigChanged();         // сигнал изменения параметров
    void signalDrawSceneBoxes(bool DrawSceneBoxes);
};

#endif // CONFIG_H
