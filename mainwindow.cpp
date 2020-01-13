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
#include "core/3d/helpers3d.h"

#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>

#include <QRandomGenerator> // test
#include <QMessageBox>
#include <QScrollArea>
#include <QSplitter>
#include <QStatusBar>
#include <QCloseEvent>

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
    view = new SceneView;
    viewContainer = QWidget::createWindowContainer(view);
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

    // настройки
    btnOptions = new ControlButton(QIcon(":/res/icons/setup.svg"), tr("Options"), this);
    QObject::connect(btnOptions, &QPushButton::clicked, this, &MainWindow::slotOptions);
    addControlWidget(btnOptions);

    addControlWidget(new SeparatorH(this));

    // новая сцена
    auto btnNewScene = new ControlButton(QIcon(":/res/icons/matrix.svg"), tr("New scene"), this);
    QObject::connect(btnNewScene, &QPushButton::clicked, this, &MainWindow::slotCreateScene);
    addControlWidget(btnNewScene);

    // новый объект
    auto btnNewObject = new ControlButton(QIcon(":/res/icons/cube.svg"), tr("Add object"), this);
    QObject::connect(btnNewObject, &QPushButton::clicked, this, &MainWindow::slotCreateObject);
    btnNewObject->setDisabled(true);
    addControlWidget(btnNewObject);

    // новый свет
    auto btnNewLight = new ControlButton(QIcon(":/res/icons/lamp.svg"), tr("Add light"), this);
    QObject::connect(btnNewLight, &QPushButton::clicked, this, &MainWindow::slotCreatePointLight);
    btnNewLight->setDisabled(true);
    addControlWidget(btnNewLight);

    // удалить объект
    actionDelObject = new QAction(QIcon(":/res/icons/delete.svg"), tr("Delete"), this);
    actionDelObject->setToolTip(actionDelObject->text());
    actionDelObject->setDisabled(true);
    actionDelObject->setShortcut(Qt::Key_Delete);
    QObject::connect(actionDelObject, &QAction::triggered, this, &MainWindow::slotDeleteSelectedEntity);
    addAction(actionDelObject);

    // редактировать объект
    btnEditEntity = new ControlButton(QIcon(":/res/icons/edit.svg"), tr("Edit selection"), this);
    btnEditEntity->setDisabled(true);
    btnEditEntity->setShortcut(Qt::CTRL + Qt::Key_E);
    QObject::connect(btnEditEntity, &QPushButton::clicked, this, &MainWindow::slotEditSelectedEntity);
    addControlWidget(btnEditEntity);

    // тест
    auto btnTest = new ControlButton(tr("  test 1000"), this);
    QObject::connect(btnTest, &QPushButton::clicked, this, &MainWindow::slotTest);
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

    QObject::connect(view, &SceneView::signalSceneChanged, [=](Scene* scene){ btnNewObject->setEnabled(scene); });
    QObject::connect(view, &SceneView::signalSceneChanged, [=](Scene* scene){ btnNewLight->setEnabled(scene); });
    QObject::connect(view, &SceneView::signalSceneChanged, this, &MainWindow::slotSceneChanged);
    QObject::connect(view, &SceneView::signalSceneChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(config, &Config::signalDrawSceneBoxes, view, &SceneView::slotCullFace);
}

void MainWindow::addControlWidget(QWidget *widget)
{
    glControls->addWidget(widget, glControls->count(), 0, 1, 1, Qt::AlignTop);
}

void MainWindow::slotWriteSceneStat()
{
    labelSceneStat->setText(tr("<b>Lights:</b>%1 | <b>Entities:</b>%2 | <b>Geometries:</b>%3 | <b>Materials:</b>%4").
                            arg(QString::number(view->getScene()->Lights().count()),
                                QString::number(view->getScene()->Objects().count()),
                                QString::number(view->getScene()->Geometries().count()),
                                QString::number(view->getScene()->Materials().count())));
}

void MainWindow::slotSceneChanged(Scene *scene)
{
    QObject::connect(scene, &Scene::signalLightChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalObjectChanged, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalGeometriesLoaded, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalMaterialsLoaded, this, &MainWindow::slotWriteSceneStat);
    QObject::connect(scene, &Scene::signalEntityClicked, this, &MainWindow::slotSceneEntityClicked);

    QObject::connect(scene->FRC(), &FrameRateCalculator::signalFramesPerSecond, [=](auto value)
                     { labelSceneFPS->setText(tr("<b>FPS:</b>%1 | ").arg(QString::number(value, 'f', 1))); });
    QObject::connect(scene, &Scene::signalSelectedEntity, [=](SceneEntity* se) { btnEditEntity->setEnabled(se); });
    QObject::connect(scene, &Scene::signalSelectedEntity, [=](SceneEntity* se) { actionDelObject->setEnabled(se); });
}

void MainWindow::slotCreateScene()
{
    const QVector<QString> keys =
        {tr("1. Name (m.b. empty):"),
         tr("2. Scene: cell size"),
         tr("3. Scene: width (cells)"),
         tr("4. Scene: height (cells)"),
         tr("5. Scene: depth (cells)")
        };
    QMap<QString, DialogValue> map =
        {{keys.at(0), {QVariant::String, ""}},
         {keys.at(1), {QVariant::Int, config->SceneCellSize(), 1, 1000}},
         {keys.at(2), {QVariant::Int, config->SceneWidth(), 10, std::numeric_limits<int>::max()}},
         {keys.at(3), {QVariant::Int, config->SceneHeight(), 10, std::numeric_limits<int>::max()}},
         {keys.at(4), {QVariant::Int, config->SceneDepth(), 10, std::numeric_limits<int>::max()}}
        };
    auto dvl = new DialogValuesList(":/res/icons/matrix.svg", tr("New scene"), true, &map, this);

    if(!dvl->exec()) return;

    viewContainer->setFocus();

    view->createScene(map.value(keys.at(1)).value.toInt(),
                      map.value(keys.at(2)).value.toInt(),
                      map.value(keys.at(3)).value.toInt(),
                      map.value(keys.at(4)).value.toInt(),
                      map.value(keys.at(0)).value.toString());

    config->setSceneCellSize(map.value(keys.at(1)).value.toInt());
    config->setSceneWidth(map.value(keys.at(2)).value.toInt());
    config->setSceneHeight(map.value(keys.at(3)).value.toInt());
    config->setSceneDepth(map.value(keys.at(4)).value.toInt());

    btnEditEntity->setDisabled(true);
}

void MainWindow::slotCreatePointLight()
{
    auto s = view->getScene();

    const QVector<QString> keys =
        {tr("1. Name (m.b. empty):"),
         tr("2. Position: X (cells)"),
         tr("3. Position: Y (cells)"),
         tr("4. Position: Z (cells)"),
         tr("5. Intensity (N/100):"),
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

    if(!dvl->exec()) return;

    auto color = QColor(map.value(keys.at(5)).value.toString());
    if(!color.isValid())
    {
        QMessageBox::critical(this, tr("Error"), tr("Wrong color: '%1'.").arg(map.value(keys.at(5)).value.toString()));
        return;
    }

    auto light = new Qt3DRender::QPointLight;
    light->setIntensity(static_cast<float>(map.value(keys.at(4)).value.toInt()) / 100);
    light->setColor(color);
    auto e = s->addLight(light, map.value(keys.at(0)).value.toString());
    s->setEntityPosition(e, QVector3D(map.value(keys.at(1)).value.toInt(),
                                      map.value(keys.at(2)).value.toInt(),
                                      map.value(keys.at(3)).value.toInt()));
    viewContainer->setFocus();
}

void MainWindow::slotCreateObject()
{
    auto s = view->getScene();

    const QVector<QString> keys =
        {tr("1. Name (m.b. empty):"),
         tr("2. Position: X (cells)"),
         tr("3. Position: Y (cells)"),
         tr("4. Position: Z (cells)"),
         tr("5. Size: X (cells)"),
         tr("6. Size: Y (cells)"),
         tr("7. Size: Z (cells)"),
         tr("8. Material:"),
         tr("9. Geometry:")
        };
    QMap<QString, DialogValue>
        map =
            {{keys.at(0), {QVariant::String, ""}},
             {keys.at(1), {QVariant::Int, 0, 0, s->Size().x() - 1}},
             {keys.at(2), {QVariant::Int, 0, 0, s->Size().y() - 1}},
             {keys.at(3), {QVariant::Int, 0, 0, s->Size().z() - 1}},
             {keys.at(4), {QVariant::Int, 1, 1, s->Size().x()}},
             {keys.at(5), {QVariant::Int, 1, 1, s->Size().y()}},
             {keys.at(6), {QVariant::Int, 1, 1, s->Size().z()}},
             {keys.at(7), {QVariant::StringList,
                           s->Materials().keys().isEmpty() ? "" : s->Materials().keys().first(),
                           "", QStringList(s->Materials().keys()), DialogValueMode::OneFromList}},             
             {keys.at(8), {QVariant::StringList,
                           s->Geometries().keys().isEmpty() ? "" : s->Geometries().keys().first(),
                           "", QStringList(s->Geometries().keys()), DialogValueMode::OneFromList}}
            };
    auto dvl = new DialogValuesList(":/res/icons/cube.svg", tr("New object"), true, &map, this);

    if(!dvl->exec()) return;

    auto e = s->addObject(map.value(keys.at(8)).value.toString(),
                          map.value(keys.at(7)).value.toString(),
                          map.value(keys.at(0)).value.toString());

    s->setEntitySize(e, QVector3D(map.value(keys.at(4)).value.toInt(),
                                  map.value(keys.at(5)).value.toInt(),
                                  map.value(keys.at(6)).value.toInt()));

    s->setEntityPosition(e, QVector3D(map.value(keys.at(1)).value.toInt(),
                                      map.value(keys.at(2)).value.toInt(),
                                      map.value(keys.at(3)).value.toInt()));

    viewContainer->setFocus();
}

void MainWindow::slotDeleteSelectedEntity()
{
    auto s = view->getScene();
    auto e = view->getScene()->SelectedEntity();

    if(!s || !e) { actionDelObject->setDisabled(true); return; }

    if(qobject_cast<SceneObject*>(e)) { if(!s->delObject(e)) qCritical() << __func__ << ": SceneObject not deleted"; return; }
    if(qobject_cast<Light*>(e)) { if(!s->delLight(e)) qCritical() << __func__ << ": Light not deleted"; return; }

    viewContainer->setFocus();
}

void MainWindow::slotEditSelectedEntity()
{
    auto s = view->getScene();
    auto e = view->getScene()->SelectedEntity();
    auto e_pos = s->EntityPosition(e);

    if(!s || !e) { btnEditEntity->setDisabled(true); return; }

    QVector<QString> keys = {
        tr("1. Name:"),
        tr("2. Position: X (cells)"),
        tr("3. Position: Y (cells)"),
        tr("4. Position: Z (cells)")
    };
    QMap<QString, DialogValue> map = {
        {keys.at(0), {QVariant::String, e->objectName(), "", ',', DialogValueMode::Disabled}},
        {keys.at(1), {QVariant::Int, e_pos.x(), 0, s->Size().x() - 1}},
        {keys.at(2), {QVariant::Int, e_pos.y(), 0, s->Size().y() - 1}},
        {keys.at(3), {QVariant::Int, e_pos.z(), 0, s->Size().z() - 1}}
    };

    auto le = qobject_cast<Light*>(e);
    auto so = qobject_cast<SceneObject*>(e);

    if(le)
    {
        keys.append(tr("5. Intensity (N/100):"));
        keys.append(tr("6. Color (#XXXXXX):"));

        map.insert(keys.at(4), {QVariant::Int, le->getLight()->intensity() * 100, 1, 100});
        map.insert(keys.at(5), {QVariant::String, le->getLight()->color().name(QColor::HexRgb)});
    }    
    else if(so)
    {
        keys.append(tr("5. Size: X (cells)"));
        keys.append(tr("6. Size: Y (cells)"));
        keys.append(tr("7. Size: Z (cells)"));
        keys.append(tr("8. Material:"));
        keys.append(tr("9. Geometry:"));

        map.insert(keys.at(4), {QVariant::Int, s->EntitySize(e).x(), 1, s->Size().x()});
        map.insert(keys.at(5), {QVariant::Int, s->EntitySize(e).y(), 1, s->Size().y()});
        map.insert(keys.at(6), {QVariant::Int, s->EntitySize(e).z(), 1, s->Size().z()});
        map.insert(keys.at(7), {QVariant::StringList, s->EntityMaterial(e),
                                "", QStringList(s->Materials().keys()), DialogValueMode::OneFromList});
        map.insert(keys.at(8), {QVariant::StringList, s->EntityGeometry(e),
                                "", QStringList(s->Geometries().keys()), DialogValueMode::OneFromList});
    }
    else
    {
        qCritical() << e->objectName() << "(" << __func__ << "): Unknown object type";
    }

    auto dvl = new DialogValuesList(":/res/icons/edit.svg", tr("Edit object"), true, &map, this);
    dvl->addToolbarButton(actionDelObject);
    QObject::connect(s, &Scene::signalSelectedEntity, dvl, &QDialog::reject);

    if(!dvl->exec()) return;

    if(le)
    {
        auto color = QColor(map.value(keys.at(5)).value.toString());
        if(!color.isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Wrong color: '%1'.").arg(map.value(keys.at(5)).value.toString()));
            color = le->getLight()->color();
        }

        auto light = new Qt3DRender::QPointLight;
        light->setIntensity(static_cast<float>(map.value(keys.at(4)).value.toInt()) / 100);
        light->setColor(color);
        le->applyLight(light);
    }
    else if(so)
    {
        s->setEntitySize(e, QVector3D(map.value(keys.at(4)).value.toInt(),
                                      map.value(keys.at(5)).value.toInt(),
                                      map.value(keys.at(6)).value.toInt()));
        s->setEntityMaterial(e, map.value(keys.at(7)).value.toString());
        s->setEntityGeometry(e, map.value(keys.at(8)).value.toString());
    }

    s->setEntityPosition(e, QVector3D(map.value(keys.at(1)).value.toInt(),
                                      map.value(keys.at(2)).value.toInt(),
                                      map.value(keys.at(3)).value.toInt()));

    viewContainer->setFocus();
}

void MainWindow::slotOptions()
{
    QMap<QString, int> mapSortPolicy = RenderSortPolicyTypeToMap();

    const QVector<QString> keys =
        {tr("Show grid, lights, etc."),
         tr("Render: Face Culling"),
         tr("Overwrite resources at start"),
         tr("Render:\nSort Policy"),
         tr("camera move speed"),
         tr("camera move speed acceleration"),
         tr("camera rotation speed"),
         tr("camera rotation speed acceleration")
        };
    QMap<QString, DialogValue> map =
        {{keys.at(0), {QVariant::Bool, config->DrawSceneBoxes()}},
         {keys.at(1), {QVariant::Bool, config->RendererCullFaceMode()}},
         {keys.at(2), {QVariant::Bool, config->RewriteResources()}},
         {keys.at(3), {QVariant::StringList,
                       config->RendererSortPolicyType().split(',', QString::SkipEmptyParts),
                       "", QStringList(mapSortPolicy.keys()), DialogValueMode::ManyFromList}},
         {keys.at(4), {QVariant::Int, config->CameraMoveSpeed(), 1, 100}},
         {keys.at(5), {QVariant::Int, config->CameraMoveAcceleration(), 1, 100}},
         {keys.at(6), {QVariant::Int, config->CameraRotationSpeed(), 1, 100}},
         {keys.at(7), {QVariant::Int, config->CameraRotationAcceleration(), 1, 100}}
        };
    auto dvl = new DialogValuesList(":/res/icons/setup.svg", tr("Options"), true, &map, this);

    if(!dvl->exec()) return;

    config->setDrawSceneBoxes(map.value(keys.at(0)).value.toBool());
    config->setRendererCullFaceMode(map.value(keys.at(1)).value.toBool());
    config->setRewriteResources(map.value(keys.at(2)).value.toBool());
    config->setRendererSortPolicyType(map.value(keys.at(3)).value.toStringList().join(','));
    config->setCameraMoveSpeed(map.value(keys.at(4)).value.toInt());
    config->setCameraMoveAcceleration(map.value(keys.at(5)).value.toInt());
    config->setCameraRotationSpeed(map.value(keys.at(6)).value.toInt());
    config->setCameraRotationAcceleration(map.value(keys.at(7)).value.toInt());
}

void MainWindow::slotTest()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirm"), tr("Cretare 1000 objects?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)  return;

    auto s = view->getScene();
    if(!s) {qWarning() << "Scene is absent"; return; }

    s->setEnabled(false);
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            for(int k = 0; k < 10; k++)
            {
                auto matname = s->Materials().keys().at(
                    QRandomGenerator::global()->bounded(0, s->Materials().keys().count()));
                auto e = s->addObject("cube", matname,
                                      QString("object-%1").arg(QString::number(100 * i + 10 * j + k)));
                s->setEntitySize(e, QVector3D(1.0, 1.0, 1.0));
                s->setEntityPosition(e, 1 * QVector3D(i, j, k));
            }
    s->setEnabled(true);

    viewContainer->setFocus();
}

void MainWindow::slotSceneEntityClicked(Qt3DRender::QPickEvent *event, SceneEntity *entity)
{
    if(!entity) { qCritical() << __func__ << ": Entity is empty"; return; }
    if(!event) { qCritical() << __func__ << ": Event is empty"; return; }

    auto s = view->getScene();

    if(event->button() == Qt3DRender::QPickEvent::Buttons::LeftButton)
    {
        qDebug() << "Mouse button: LeftButton";
        s->SelectEntity(entity);
    }
    else if(event->button() == Qt3DRender::QPickEvent::Buttons::MiddleButton)
    {
        qDebug() << "Mouse button: MiddleButton";
    }
    else if(event->button() == Qt3DRender::QPickEvent::Buttons::RightButton)
    {
        qDebug() << "Mouse button: RightButton";
    }
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
