#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt3DRender/QPickEvent>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QMainWindow>

class SceneView;
class Scene;
class SceneEntity;
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

private:
    QWidget* viewContainer;
    SceneView* sceneView;
    QLabel* labelSceneStat;
    QLabel* labelSceneFPS;
    QGridLayout* glControls;
    ControlButton* btnDelEntity;
    QCheckBox* cbShowSceneBoxes;

public Q_SLOTS:
    void slotWriteSceneStat();
    void slotViewSceneChanged(Scene* scene);
    void slotCreateScene();
    void slotCreatePointLight();
    void slotDeleteSelectedEntity();
    void slotSceneEntityClicked(Qt3DRender::QPickEvent *event, SceneEntity *entity);
};

#endif // MAINWINDOW_H
