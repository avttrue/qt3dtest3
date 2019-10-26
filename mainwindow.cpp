#include "controls.h"
#include "mainwindow.h"
#include "properties.h"
#include "widgettools.h"
#include "core/3d/sceneview.h"
#include "core/3d/scene.h"
#include "core/3d/sceneentity.h"
#include <core/3d/frameratecalculator.h>
#include <QMessageBox>
#include <QScrollArea>
#include <QSplitter>
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
    setWidgetToScreenCenter(this);
}

void MainWindow::createGUI()
{
    // View
    sceneView = new SceneView;
    auto  viewContainer = QWidget::createWindowContainer(sceneView);

    // Controls
    auto saControls = new QScrollArea();
    saControls->setAlignment(Qt::AlignTop);
    saControls->setWidgetResizable(true);
    auto wControls = new QWidget();
    saControls->setWidget(wControls);
    glControls = new QGridLayout();
    glControls->setMargin(1);
    glControls->setSpacing(1);
    wControls->setLayout(glControls);
    wControls->setFocusPolicy(Qt::NoFocus);
    glControls->setAlignment(Qt::AlignTop);

    // Splitter
    auto vertSplitter = new QSplitter(this);
    vertSplitter->setOrientation(Qt::Orientation::Horizontal);
    vertSplitter->setChildrenCollapsible(false);
    vertSplitter->addWidget(saControls);
    vertSplitter->addWidget(viewContainer);
    vertSplitter->setSizes(QList<int>({100, 400}));
    setCentralWidget(vertSplitter);

    // управление
    // новая сцена
    auto btnNewScene = new ControlButton(QIcon(":/res/icons/cube.svg"), tr("Новая сцена"), this);
    QObject::connect(btnNewScene, &QPushButton::clicked, [=]() { sceneView->createScene(); viewContainer->setFocus(); });
    addControlWidget(btnNewScene);

    // удалить объект
    btnDelEntity = new ControlButton(QIcon(":/res/icons/delete.svg"), tr("Удалить объект"), this);
    btnDelEntity->setDisabled(true);
    QObject::connect(btnDelEntity, &QPushButton::clicked, [=]()
                     {
                         Scene* s = sceneView->getScene();
                         if(!s || !s->SelectedEntity()) { btnDelEntity->setDisabled(true); return; }
                         s->delEntity(s->SelectedEntity());
                         btnDelEntity->setDisabled(true);
                         viewContainer->setFocus();
                     });
    addControlWidget(btnDelEntity);

    // тест
    auto btnTest = new ControlButton(tr("   тест"), this);
    QObject::connect(btnTest, &QPushButton::clicked, [=]() { sceneView->createSpheresTest(); viewContainer->setFocus(); });
    addControlWidget(btnTest);

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

    QObject::connect(sceneView, &SceneView::signalSceneChanged, this, &MainWindow::slotViewSceneChanged);
    QObject::connect(sceneView, &SceneView::signalSceneChanged, this, &MainWindow::slotWriteSceneStat);

    viewContainer->setFocus();
}

void MainWindow::addControlWidget(QWidget *widget)
{
    glControls->addWidget(widget, glControls->count(), 0, 1, 1, Qt::AlignTop);
}

void MainWindow::slotWriteSceneStat()
{
    labelSceneStat->setText(tr("<b>Свет:</b>%1 | <b>Сущности:</b>%2").
                            arg(QString::number(sceneView->getScene()->Lights().count()),
                                QString::number(sceneView->getScene()->Entities().count())));
}

void MainWindow::slotViewSceneChanged(Scene *scene)
{
    QObject::connect(scene, &Scene::signalLightsCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalEntitiesCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene->FRC(), &FrameRateCalculator::signalFramesPerSecondChanged, [=](auto value)
                     { labelSceneFPS->setText(tr("<b>К/С:</b>%1 | ").arg(QString::number(value, 'f', 1))); });

    QObject::connect(sceneView->getScene(), &Scene::signalEntitySelected, [=](SceneEntity* se)
                     { btnDelEntity->setEnabled(se); });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Внимание"), tr("Закончить работу с '%1'?").arg(APP_NAME),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
    {
        event->ignore();
        return;
    }

    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    settings.setValue("MainWindow/Height",height());
    settings.setValue("MainWindow/Width",width());

    event->accept();
}
