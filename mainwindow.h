#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

class View;
class Scene;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

Q_SIGNALS:

protected:
    void createGUI();
    void atAppClose();

    void closeEvent(QCloseEvent* event);

    void slotPrintSceneStat();
    void slotViewSceneChanged(Scene* scene);

private:
    View* view;
    QLabel* labelSceneStat;
    QLabel* labelSceneFPS;


public Q_SLOTS:


};

#endif // MAINWINDOW_H
