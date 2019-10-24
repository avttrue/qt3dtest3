#include "mainwindow.h"
#include "properties.h"
#include "core/3d/view.h"
#include "core/3d/scene.h"
#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint |
                   Qt::FramelessWindowHint);

    setWindowIcon(QIcon(":/res/icons/star.svg"));
    setWindowTitle(QString("%1 %2").arg(APP_NAME, APP_VERS));

    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    resize(settings.value("MainWindow/Width", WINDOW_WIDTH).toInt(),
           settings.value("MainWindow/Height", WINDOW_HEIGHT).toInt());

    createGUI();
}

void MainWindow::createGUI()
{
    // View
    view = new View;

    auto  viewContainer = QWidget::createWindowContainer(view);
    setCentralWidget(viewContainer);

    // статусбар
    auto statusBar = new QStatusBar(this);

    labelSceneFPS = new QLabel(this);
    labelSceneFPS->setText("");
    labelSceneFPS->setAlignment(Qt::AlignVCenter);
    statusBar->addWidget(labelSceneFPS);

    labelSceneStat = new QLabel(this);
    labelSceneStat->setText("");
    labelSceneStat->setAlignment(Qt::AlignVCenter);
    statusBar->addWidget(labelSceneStat);

    setStatusBar(statusBar);

    QObject::connect(view, &View::signalSceneChanged, this, &MainWindow::slotViewSceneChanged);
    QObject::connect(view, &View::signalSceneChanged, this, &MainWindow::slotPrintSceneStat);

    viewContainer->setFocus();
}

void MainWindow::atAppClose()
{
    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    settings.setValue("MainWindow/Height",height());
    settings.setValue("MainWindow/Width",width());
}

void MainWindow::slotPrintSceneStat()
{
    labelSceneStat->setText(QString("<b>Lights:</b>%1 | <b>Entities:</b>%2").
                            arg(QString::number(view->getScene()->Lights().count()),
                                QString::number(view->getScene()->Entities().count())));
}

void MainWindow::slotViewSceneChanged(Scene *scene)
{
    QObject::connect(scene, &Scene::signalLightsCountChanged, this, &MainWindow::slotPrintSceneStat);
    QObject::connect(scene, &Scene::signalEntitiesCountChanged, this, &MainWindow::slotPrintSceneStat);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Внимание", QString("Закончить работу с '%1'?").arg(APP_NAME),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
    {
        event->ignore();
        return;
    }

    atAppClose();
    event->accept();
}
