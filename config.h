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
    QString SceneColorBG() const;
    void setSceneColorBG(const QString &inSceneColorBG);
    float SceneExcess() const;
    void setSceneExcess(float inSceneExcess);
    QString SceneColorSelect() const;
    void setSceneColorSelect(const QString &inSceneColorSelect);
    QString SceneColorBox() const;
    void setSceneColorBox(const QString &inSceneColorBox);
    QString SceneColorGrid() const;
    void setSceneColorGrid(const QString &inSceneColorGrid);
    QString RendererSortPolicyType() const;
    void setRendererSortPolicyType(const QString& inRendererSortPolicyType);
    bool RendererCullFaceMode() const;
    void setRendererCullFaceMode(bool inRendererCullFaceMode);
    int ButtonForward() const;
    void setButtonForward(int inButtonForward);
    int ButtonBackward() const;
    void setButtonBackward(int inButtonBackward);
    int ButtonLeftward() const;
    void setButtonLeftward(int inButtonLeftward);
    int ButtonRightward() const;
    void setButtonRightward(int inButtonRightward);
    int ButtonUpward() const;
    void setButtonUpward(int inButtonUpward);
    int ButtonDownward() const;
    void setButtonDownward(int inButtonDownward);

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
    QString m_SceneColorBox;            // цвет границ сцены
    QString m_SceneColorGrid;           // цвет координатной сетки
    QString m_RendererSortPolicyType;   // пересортировка объектов относительно камеры
    int m_SplashTime;                   // время отображения сплеш-заставки
    int m_ButtonAcceleration;           // кнопка ускорения перемещения
    int m_ButtonForward;                // кнопка перемещения вперёд
    int m_ButtonBackward;               // кнопка перемещения назад
    int m_ButtonLeftward;               // кнопка перемещения влево
    int m_ButtonRightward;              // кнопка перемещения вправо
    int m_ButtonUpward;                 // кнопка перемещения вверх
    int m_ButtonDownward;               // кнопка перемещения вниз
    int m_MoveAcceleration;             // ускорение линейного перемещения
    int m_RotationAcceleration;         // ускорение поворота
    int m_MoveSpeed;                    // скорость линейного перемещения
    int m_RotationSpeed;                // скорость поворота
    int m_SceneCellSize;                // размер ячейки сцены
    int m_SceneHeight;                  // размер сцены высота
    int m_SceneWidth;                   // размер сцены ширина
    int m_SceneDepth;                   // размер сцены глубина
    float m_SceneExcess;                // отступы при отображении служебных сущностей
    bool m_RewriteResources;            // переписывать при копировании файлы ресурсов
    bool m_DrawSceneBoxes;              // отображать контуры сцены и служебных объектов
    bool m_RendererCullFaceMode;        // включает Face Culling

Q_SIGNALS:
    void signalConfigChanged();         // сигнал изменения параметров
    void signalDrawSceneBoxes(bool value); // сигнал изменения параметров отрисовки контуров сцены и служебных объектов
    void signalRendererCullFaceMode(bool value); // сигнал изменения параметра Face Culling
    void signalRendererSortPolicyType(const QString& value ); // сигнал изменения параметра RendererSortPolicy
};

#endif // CONFIG_H
