#include "controls.h"
#include "mainwindow.h"
#include "properties.h"
#include "helperswidget.h"
#include "dialogs/dialogvalueslist.h"
#include "core/3d/sceneview.h"
#include "core/3d/scene.h"
#include "core/3d/sceneobject.h"
#include <core/3d/frameratecalculator.h>

#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>

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
    viewContainer = QWidget::createWindowContainer(sceneView);

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
    auto btnNewScene = new ControlButton(QIcon(":/res/icons/matrix.svg"), tr("Новая сцена"), this);
    QObject::connect(btnNewScene, &QPushButton::clicked, this, &MainWindow::createScene);
    addControlWidget(btnNewScene);

    // отображение контуров
    cbShowSceneBoxes = new QCheckBox(tr("Отображать сл.объекты"), this);
    cbShowSceneBoxes->setChecked(config->DrawSceneBoxes());
    cbShowSceneBoxes->setEnabled(false);
    QObject::connect(cbShowSceneBoxes, &QCheckBox::stateChanged, [=](int value){ config->setDrawSceneBoxes(value); });
    addControlWidget(cbShowSceneBoxes);

    // новый свет
    auto btnNewLight = new ControlButton(QIcon(":/res/icons/lamp.svg"), tr("Новый свет"), this);
    QObject::connect(btnNewLight, &QPushButton::clicked, this, &MainWindow::createPointLight);
    btnNewLight->setDisabled(true);
    addControlWidget(btnNewLight);

    // удалить объект
    btnDelEntity = new ControlButton(QIcon(":/res/icons/delete.svg"), tr("Удалить объект"), this);
    btnDelEntity->setDisabled(true);
    btnDelEntity->setShortcut(Qt::Key_Delete);
    btnDelEntity->setToolTip("Del");
    QObject::connect(btnDelEntity, &QPushButton::clicked, [=]()
                     {
                         Scene* s = sceneView->getScene();
                         if(!s || !s->SelectedEntity()) { btnDelEntity->setDisabled(true); return; }
                         s->delEntity(s->SelectedEntity());
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

    QObject::connect(sceneView, &SceneView::signalSceneChanged, [=](Scene* scene){ btnNewLight->setEnabled(scene); });
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
    cbShowSceneBoxes->setEnabled(true);

    QObject::connect(scene, &Scene::signalLightsCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalEntitiesCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene->FRC(), &FrameRateCalculator::signalFramesPerSecondChanged, [=](auto value)
                     { labelSceneFPS->setText(tr("<b>К/С:</b>%1 | ").arg(QString::number(value, 'f', 1))); });
    QObject::connect(scene, &Scene::signalSelectedEntityChanged, [=](SceneObject* se) { btnDelEntity->setEnabled(se); });
    QObject::connect(config, &Config::signalDrawSceneBoxes, scene, &Scene::slotShowBoxes);
}

void MainWindow::createScene()
{
    const QVector<QString> keys =
        {tr("1. Название"),
         tr("2. Сцена: размер клетки"),
         tr("3. Сцена: ширина (в клетках)"),
         tr("4. Сцена: высота (в клетках)"),
         tr("5. Сцена: глубина (в клетках)")
        };
    QMap<QString, QPair<QVariant::Type, QVariant>> map =
        {{keys.at(0), {QVariant::String, ""}},
         {keys.at(1), {QVariant::Int, SCENE_CELL_SIZE}},
         {keys.at(2), {QVariant::Int, SCENE_WIDTH}},
         {keys.at(3), {QVariant::Int, SCENE_HEIGHT}},
         {keys.at(4), {QVariant::Int, SCENE_DEPTH}}
        };
    auto dvl = new DialogValuesList(":/res/icons/matrix.svg", tr("Новая сцена"), true, &map, this);

    if(!dvl->exec()) return;

    sceneView->createScene(map.value(keys.at(1)).second.toInt(),
                           map.value(keys.at(2)).second.toInt(),
                           map.value(keys.at(3)).second.toInt(),
                           map.value(keys.at(4)).second.toInt(),
                           map.value(keys.at(0)).second.toString());
    viewContainer->setFocus();
}

void MainWindow::createPointLight()
{
    const QVector<QString> keys =
        {tr("1. Название"),
         tr("3. Положение: X (в клетках)"),
         tr("4. Положение: Y (в клетках)"),
         tr("5. Положение: Z (в клетках)"),
         tr("6. Интенсивность (N/100)"),
         tr("7. Цвет (#XXXXXX)"),
        };
    QMap<QString, QPair<QVariant::Type, QVariant>> map =
        {{keys.at(0), {QVariant::String, ""}},
         {keys.at(1), {QVariant::Int, sceneView->getScene()->Size().x() / 2}},
         {keys.at(2), {QVariant::Int, sceneView->getScene()->Size().y() / 2}},
         {keys.at(3), {QVariant::Int, sceneView->getScene()->Size().z() / 2}},
         {keys.at(4), {QVariant::Int, 50}},
         {keys.at(5), {QVariant::String, "#FFFFFF"}},
        };
    auto dvl = new DialogValuesList(":/res/icons/lamp.svg", tr("Новый свет"), true, &map, this);

    if(!dvl->exec()) return;

    auto color = QColor(map.value(keys.at(5)).second.toString());
    if(!color.isValid())
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Некорректно указан цвет: '%1'.").arg(map.value(keys.at(5)).second.toString()));
        return;
    }

    auto lightTransform = new Qt3DCore::QTransform;
    lightTransform->setTranslation(sceneView->getScene()->CellSize() *
                                   QVector3D(map.value(keys.at(1)).second.toInt(),
                                             map.value(keys.at(2)).second.toInt(),
                                             map.value(keys.at(3)).second.toInt()));
    auto light = new Qt3DRender::QPointLight;
    light->setIntensity(static_cast<float>(map.value(keys.at(4)).second.toInt()) / 100);
    light->setColor(color);
    sceneView->getScene()->addLight(lightTransform, light, map.value(keys.at(0)).second.toString());

    viewContainer->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Внимание"), tr("Закончить работу с '%1'?").arg(APP_NAME),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) { event->ignore(); return; }

    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    settings.setValue("MainWindow/Height",height());
    settings.setValue("MainWindow/Width",width());

    event->accept();
}
