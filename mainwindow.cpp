#include "controls.h"
#include "mainwindow.h"
#include "properties.h"
#include "helperswidget.h"
#include "dialogs/dialogvalueslist.h"
#include "core/3d/scene.h"
#include "core/3d/sceneentity.h"
#include "core/3d/sceneview.h"
#include "core/3d/sceneobject.h"
#include "core/3d/light.h"
#include "core/3d/frameratecalculator.h"

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
    viewContainer->setFocusPolicy(Qt::StrongFocus);

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
    auto btnNewScene = new ControlButton(QIcon(":/res/icons/matrix.svg"), tr("New scene"), this);
    QObject::connect(btnNewScene, &QPushButton::clicked, this, &MainWindow::slotCreateScene);
    addControlWidget(btnNewScene);

    // отображение контуров
    cbShowSceneBoxes = new QCheckBox(tr("Show grid, lights, etc."), this);
    cbShowSceneBoxes->setChecked(config->DrawSceneBoxes());
    cbShowSceneBoxes->setEnabled(false);
    QObject::connect(cbShowSceneBoxes, &QCheckBox::stateChanged, [=](int value){ config->setDrawSceneBoxes(value); });
    addControlWidget(cbShowSceneBoxes);

    // новый свет
    auto btnNewLight = new ControlButton(QIcon(":/res/icons/lamp.svg"), tr("Add light"), this);
    QObject::connect(btnNewLight, &QPushButton::clicked, this, &MainWindow::slotCreatePointLight);
    btnNewLight->setDisabled(true);
    addControlWidget(btnNewLight);

    // удалить объект
    btnDelEntity = new ControlButton(QIcon(":/res/icons/delete.svg"), tr("delete"), this);
    btnDelEntity->setDisabled(true);
    btnDelEntity->setShortcut(Qt::Key_Delete);
    btnDelEntity->setToolTip("Del");
    QObject::connect(btnDelEntity, &QPushButton::clicked, this, &MainWindow::slotDeleteSelectedEntity);
    addControlWidget(btnDelEntity);

    // тест
    auto btnTest = new ControlButton(tr("   test entities"), this);
    QObject::connect(btnTest, &QPushButton::clicked, [=]() { sceneView->test(); viewContainer->setFocus(); });
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
}

void MainWindow::addControlWidget(QWidget *widget)
{
    glControls->addWidget(widget, glControls->count(), 0, 1, 1, Qt::AlignTop);
}

void MainWindow::slotWriteSceneStat()
{
    labelSceneStat->setText(tr("<b>Lights:</b>%1 | <b>Entities:</b>%2 | <b>Geometries:</b>%3 | <b>Materials:</b>%4").
                            arg(QString::number(sceneView->getScene()->Lights().count()),
                                QString::number(sceneView->getScene()->Objects().count()),
                                QString::number(sceneView->getScene()->Geometries().count()),
                                QString::number(sceneView->getScene()->Materials().count())));
}

void MainWindow::slotViewSceneChanged(Scene *scene)
{
    cbShowSceneBoxes->setEnabled(true);

    QObject::connect(scene, &Scene::signalLightsCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalObjectsCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalGeometriesCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalMaterialsCountChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene->FRC(), &FrameRateCalculator::signalFramesPerSecondChanged, [=](auto value)
                     { labelSceneFPS->setText(tr("<b>FPS:</b>%1 | ").arg(QString::number(value, 'f', 1))); });
    QObject::connect(scene, &Scene::signalSelectedEntityChanged, [=](SceneEntity* se) { btnDelEntity->setEnabled(se); });
    QObject::connect(config, &Config::signalDrawSceneBoxes, scene, &Scene::slotShowBoxes);
}

void MainWindow::slotCreateScene()
{
    const QVector<QString> keys =
        {tr("1. Name (m.b. empty):"),
         tr("2. Scene: cell size"),
         tr("3. Scene: width (in cells)"),
         tr("4. Scene: height (in cells)"),
         tr("5. Scene: depth (in cells)")
        };
    QMap<QString, DialogValue> map =
        {{keys.at(0), {QVariant::String, ""}},
         {keys.at(1), {QVariant::Int, SCENE_CELL_SIZE, 1, 1000}},
         {keys.at(2), {QVariant::Int, SCENE_WIDTH, 10, std::numeric_limits<int>::max()}},
         {keys.at(3), {QVariant::Int, SCENE_HEIGHT, 10, std::numeric_limits<int>::max()}},
         {keys.at(4), {QVariant::Int, SCENE_DEPTH, 10, std::numeric_limits<int>::max()}}
        };
    auto dvl = new DialogValuesList(":/res/icons/matrix.svg", tr("New scene"), true, &map, this);

    if(!dvl->exec()) return;

    sceneView->createScene(map.value(keys.at(1)).value.toInt(),
                           map.value(keys.at(2)).value.toInt(),
                           map.value(keys.at(3)).value.toInt(),
                           map.value(keys.at(4)).value.toInt(),
                           map.value(keys.at(0)).value.toString());
    viewContainer->setFocus();
}

void MainWindow::slotCreatePointLight()
{
    auto s = sceneView->getScene();

    const QVector<QString> keys =
        {tr("1. Name (m.b. empty):"),
         tr("2. Position: X (in cells)"),
         tr("3. Position: Y (in cells)"),
         tr("4. Position: Z (in cells)"),
         tr("5. Intensity (N/100)"),
         tr("6. Color (#XXXXXX):")
        };
    QMap<QString, DialogValue> map =
        {{keys.at(0), {QVariant::String, ""}},
         {keys.at(1), {QVariant::Int, s->Size().x() / 2, 0, s->Size().x() - 1}},
         {keys.at(2), {QVariant::Int, s->Size().y() / 2, 0, s->Size().y() - 1}},
         {keys.at(3), {QVariant::Int, s->Size().z() / 2, 0, s->Size().z() - 1}},
         {keys.at(4), {QVariant::Int, 10, 1, 100}},
         {keys.at(5), {QVariant::String, "#FFFFFF"}}
        };
    auto dvl = new DialogValuesList(":/res/icons/lamp.svg", tr("New light"), true, &map, this);

    if(!dvl->exec()) return;  qDebug() << map.last().value;

    auto color = QColor(map.value(keys.at(5)).value.toString());
    if(!color.isValid())
    {
        QMessageBox::critical(this, tr("Error"), tr("Wrong color: '%1'.").arg(map.value(keys.at(5)).value.toString()));
        return;
    }

    auto lightTransform = new Qt3DCore::QTransform;
    lightTransform->setTranslation(s->CellSize() *
                                   QVector3D(map.value(keys.at(1)).value.toInt(),
                                             map.value(keys.at(2)).value.toInt(),
                                             map.value(keys.at(3)).value.toInt()));
    auto light = new Qt3DRender::QPointLight;
    light->setIntensity(static_cast<float>(map.value(keys.at(4)).value.toInt()) / 100);
    light->setColor(color);
    s->addLight(lightTransform, light, map.value(keys.at(0)).value.toString());
    viewContainer->setFocus();
}

void MainWindow::slotDeleteSelectedEntity()
{
    auto s = sceneView->getScene();
    auto e = sceneView->getScene()->SelectedEntity();

    if(!s || !e) { btnDelEntity->setDisabled(true); return; }

    if(qobject_cast<SceneObject*>(e)) { s->delObject(e); return; }
    if(qobject_cast<Light*>(e)) { s->delLight(e); return; }

    viewContainer->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirm"), tr("Close application '%1'?").arg(APP_NAME),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) { event->ignore(); return; }

    QSettings settings(config->PathAppConfig(), QSettings::IniFormat);
    settings.setValue("MainWindow/Height",height());
    settings.setValue("MainWindow/Width",width());

    event->accept();
}
