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
    SceneView* view;
    QLabel* labelSceneStat;
    QLabel* labelSceneFPS;
    QGridLayout* glControls;
    ControlButton* btnEditEntity;
    QCheckBox* cbShowSceneBoxes;
    QAction* actionDelObject;

public Q_SLOTS:
    void slotWriteSceneStat();
    void slotSceneChanged(Scene* scene);
    void slotCreateScene();
    void slotCreatePointLight();
    void slotCreateObject();
    void slotDeleteSelectedEntity();
    void slotEditSelectedEntity();
    void slotTest();
    void slotSceneEntityClicked(Qt3DRender::QPickEvent *event, SceneEntity *entity);
};

#endif // MAINWINDOW_H
