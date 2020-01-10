#include "cameracontroller.h"
#include "properties.h"


CameraController::CameraController(Qt3DCore::QNode *parent):
    Qt3DCore::QEntity(parent),
    m_Camera(nullptr),
    m_dx(0.0f),
    m_dy(0.0f),
    m_dz(0.0f),
    m_LeftButtonPressed(false),
    m_ButtonAccelerationPressed(false)
{
    m_FrameAction = new Qt3DLogic::QFrameAction(this);

    /// Mouse
    m_MouseDevice = new Qt3DInput::QMouseDevice(this);
    // MouseLeftButton
    m_LeftButtonInput = new Qt3DInput::QActionInput(this);
    m_LeftButtonInput->setButtons(QVector<int>() << Qt::LeftButton);
    m_LeftButtonInput->setSourceDevice(m_MouseDevice);
    m_LeftButtonAction = new Qt3DInput::QAction(this);
    m_LeftButtonAction->addInput(m_LeftButtonInput);
    m_LeftButtonAction->setObjectName("LeftButton");

    // Keyboard
    m_KeyboardDevice = new Qt3DInput::QKeyboardDevice(this);
    // KeyboardButtonAcceleration
    m_ButtonAccelerationInput = new Qt3DInput::QActionInput(this);
    m_ButtonAccelerationInput->setButtons(QVector<int>() << config->ButtonAcceleration());
    m_ButtonAccelerationInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonAccelerationAction = new Qt3DInput::QAction(this);
    m_ButtonAccelerationAction->addInput(m_ButtonAccelerationInput);
    m_ButtonAccelerationAction->setObjectName("ButtonAcceleration");

    // MouseX
    m_MouseXInput = new Qt3DInput::QAnalogAxisInput(this);
    m_MouseXInput->setAxis(Qt3DInput::QMouseDevice::X);
    m_MouseXInput->setSourceDevice(m_MouseDevice);
    m_XAxis = new Qt3DInput::QAxis(this);
    m_XAxis->setObjectName("XAxis");
    m_XAxis->addInput(m_MouseXInput);

    // MouseY
    m_MouseYInput = new Qt3DInput::QAnalogAxisInput(this);
    m_MouseYInput->setAxis(Qt3DInput::QMouseDevice::Y);
    m_MouseYInput->setSourceDevice(m_MouseDevice);
    m_YAxis = new Qt3DInput::QAxis(this);
    m_YAxis->setObjectName("YAxis");
    m_YAxis->addInput(m_MouseYInput);

    // MouseZ (Wheel Y)
    m_MouseZInput = new Qt3DInput::QAnalogAxisInput(this);
    m_MouseZInput->setAxis(Qt3DInput::QMouseDevice::WheelY);
    m_MouseZInput->setSourceDevice(m_MouseDevice);
    m_ZAxis = new Qt3DInput::QAxis(this);
    m_ZAxis->setObjectName("ZAxis");
    m_ZAxis->addInput(m_MouseZInput);

    /// LogicalDevice
    m_LogicalDevice = new Qt3DInput::QLogicalDevice(this);
    m_LogicalDevice->addAction(m_LeftButtonAction);
    m_LogicalDevice->addAction(m_ButtonAccelerationAction);
    m_LogicalDevice->addAxis(m_XAxis);
    m_LogicalDevice->addAxis(m_YAxis);
    m_LogicalDevice->addAxis(m_ZAxis);

    QObject::connect(m_LeftButtonAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::activeChanged);
    QObject::connect(m_ButtonAccelerationAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::activeChanged);
    QObject::connect(m_XAxis, &Qt3DInput::QAxis::valueChanged, this, &CameraController::valueChanged);
    QObject::connect(m_YAxis, &Qt3DInput::QAxis::valueChanged, this, &CameraController::valueChanged);
    QObject::connect(m_ZAxis, &Qt3DInput::QAxis::valueChanged, this, &CameraController::valueChanged);
    QObject::connect(m_FrameAction, &Qt3DLogic::QFrameAction::triggered, this, &CameraController::frameActionTriggered);
}

void CameraController::activeChanged(bool isActiv)
{
    if(sender()->objectName() == "LeftButton") m_LeftButtonPressed = isActiv;
    if(sender()->objectName() == "ButtonAcceleration") m_ButtonAccelerationPressed = isActiv;
}

void CameraController::valueChanged(float value)
{
    if(sender()->objectName() == "XAxis") m_dx = value;
    else if(sender()->objectName() == "YAxis") m_dy = value;
    else if(sender()->objectName() == "ZAxis") m_dz = value;
}

void CameraController::frameActionTriggered(float dt)
{
    if(!m_Camera) return;

    float la = (m_ButtonAccelerationPressed ? config->RotationAcceleration() : 1) * config->RotationSpeed();
    float ma = (m_ButtonAccelerationPressed ? config->MoveAcceleration() : 1) * config->MoveSpeed();

    if(m_LeftButtonPressed)
    {
        m_Camera->pan(-m_dx * la * dt, QVector3D(0.0, 1.0, 0.0));
        m_Camera->tilt(-m_dy * la * dt);
    }

    m_Camera->translate(QVector3D(0.0f, 0.0f, ma * m_dz * dt));
}

void CameraController::setCamera(Qt3DRender::QCamera *Camera) { m_Camera = Camera; }
