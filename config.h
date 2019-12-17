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
    bool RewriteResources() const;
    void setRewriteResources(bool inRewriteResources);
    int SceneCellSize() const;
    void setSceneCellSize(int inSceneCellSize);
    int SceneHeight() const;
    void setSceneHeight(int inSceneHeight);
    int SceneWidth() const;
    void setSceneWidth(int inSceneWidth);
    int SceneDepth() const;
    void setSceneDepth(int inSceneDepth);
    bool SceneFrustumCulling() const;
    void setSceneFrustumCulling(bool inSceneFrustumCulling);
    QString SceneColorBG() const;
    void setSceneColorBG(const QString &inSceneColorBG);
    float SceneExcess() const;
    void setSceneExcess(float inSceneExcess);
    QString SceneColorSelect() const;
    void setSceneColorSelect(const QString &inSceneColorSelect);

protected:
    void load();

private:
    QSettings* m_Settings;
    QString m_PathAppConfig;            // путь до конфига приложения
    QString m_PathAppDir;               // путь до приложения
    QString m_PathAppLogDir;            // путь до логов приложения
    QString m_PathAssetsDir;            // путь до ресурсов
    QString m_DateTimeFormat;           // формат даты и времени
    QString m_SceneColorBG;             // цвет задника сцены
    QString m_SceneColorSelect;         // цвет рамки выделения
    bool m_RewriteResources;            // переписывать при копировании файлы ресурсов
    bool m_DrawSceneBoxes;              // отображать контуры сцены и служебных объектов
    bool m_SceneFrustumCulling;         // FrameGraph()->frustumCullingEnabled
    int m_SplashTime;                   // время отображения сплеш-заставки
    int m_ButtonAcceleration;           // кнопка ускорения перемещения
    int m_MoveAcceleration;             // ускорение линейного перемещения
    int m_RotationAcceleration;         // ускорение поворота
    int m_MoveSpeed;                    // скорость линейного перемещения
    int m_RotationSpeed;                // скорость поворота
    int m_SceneCellSize;                // размер ячейки сцены
    int m_SceneHeight;                  // размер сцены высота
    int m_SceneWidth;                   // размер сцены ширина
    int m_SceneDepth;                   // размер сцены глубина
    float m_SceneExcess;                // отступы при отображении служебных сущностей


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
