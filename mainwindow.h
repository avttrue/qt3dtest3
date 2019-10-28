#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>

class SceneView;
class Scene;
class ControlButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

Q_SIGNALS:

protected:
    void createGUI();
    void addControlWidget(QWidget* widget);
    void closeEvent(QCloseEvent* event);

    void slotWriteSceneStat();
    void slotViewSceneChanged(Scene* scene);
    void createScene();

private:
    QWidget* viewContainer;
    SceneView* sceneView;
    QLabel* labelSceneStat;
    QLabel* labelSceneFPS;
    QGridLayout* glControls;
    ControlButton* btnDelEntity;


public Q_SLOTS:


};

#endif // MAINWINDOW_H
