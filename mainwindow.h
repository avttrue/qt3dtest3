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

private:
    View* view;
    QLabel* labelSceneStat;
    QLabel* labelSceneFPS;


public Q_SLOTS:

private Q_SLOTS:
    void slotPrintSceneStat();
    void slotViewSceneChanged(Scene* scene);

};

#endif // MAINWINDOW_H
