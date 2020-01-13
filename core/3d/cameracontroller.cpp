#include "cameracontroller.h"
#include "properties.h"

CameraController::CameraController(Qt3DCore::QNode *parent, Qt3DRender::QCamera* camera):
    Qt3DCore::QEntity(parent),
    m_Camera(camera),
    m_dx(0.0f),
    m_dy(0.0f),
    m_dz(0.0f),
    m_MouseLeftButtonPressed(false),
    m_ButtonAccelerationPressed(false)
{
    m_FrameAction = new Qt3DLogic::QFrameAction(this);

    // Keyboard
    m_KeyboardDevice = new Qt3DInput::QKeyboardDevice(this);
    // KeyboardButtonAcceleration
    m_ButtonAccelerationInput = new Qt3DInput::QActionInput(this);
    m_ButtonAccelerationInput->setButtons(QVector<int>() << config->ButtonAcceleration());
    m_ButtonAccelerationInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonAccelerationAction = new Qt3DInput::QAction(this);
    m_ButtonAccelerationAction->addInput(m_ButtonAccelerationInput);
    m_ButtonAccelerationAction->setObjectName("ButtonAcceleration");
    // KeyboardButtonForward
    m_ButtonForwardInput = new Qt3DInput::QActionInput(this);
    m_ButtonForwardInput->setButtons(QVector<int>() << config->ButtonForward());
    m_ButtonForwardInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonForwardAction = new Qt3DInput::QAction(this);
    m_ButtonForwardAction->addInput(m_ButtonForwardInput);
    m_ButtonForwardAction->setObjectName("ButtonForward");
    // KeyboardButtonBackward
    m_ButtonBackwardInput = new Qt3DInput::QActionInput(this);
    m_ButtonBackwardInput->setButtons(QVector<int>() << config->ButtonBackward());
    m_ButtonBackwardInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonBackwardAction = new Qt3DInput::QAction(this);
    m_ButtonBackwardAction->addInput(m_ButtonBackwardInput);
    m_ButtonBackwardAction->setObjectName("ButtonBackward");
    // KeyboardButtonLeftward
    m_ButtonLeftwardInput = new Qt3DInput::QActionInput(this);
    m_ButtonLeftwardInput->setButtons(QVector<int>() << config->ButtonLeftward());
    m_ButtonLeftwardInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonLeftwardAction = new Qt3DInput::QAction(this);
    m_ButtonLeftwardAction->addInput(m_ButtonLeftwardInput);
    m_ButtonLeftwardAction->setObjectName("ButtonLeftward");
    // KeyboardButtonRightward
    m_ButtonRightwardInput = new Qt3DInput::QActionInput(this);
    m_ButtonRightwardInput->setButtons(QVector<int>() << config->ButtonRightward());
    m_ButtonRightwardInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonRightwardAction = new Qt3DInput::QAction(this);
    m_ButtonRightwardAction->addInput(m_ButtonRightwardInput);
    m_ButtonRightwardAction->setObjectName("ButtonRightward");
    // KeyboardButtonUpward
    m_ButtonUpwardInput = new Qt3DInput::QActionInput(this);
    m_ButtonUpwardInput->setButtons(QVector<int>() << config->ButtonUpward());
    m_ButtonUpwardInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonUpwardAction = new Qt3DInput::QAction(this);
    m_ButtonUpwardAction->addInput(m_ButtonUpwardInput);
    m_ButtonUpwardAction->setObjectName("ButtonUpward");
    // KeyboardButtonDownward
    m_ButtonDownwardInput = new Qt3DInput::QActionInput(this);
    m_ButtonDownwardInput->setButtons(QVector<int>() << config->ButtonDownward());
    m_ButtonDownwardInput->setSourceDevice(m_KeyboardDevice);
    m_ButtonDownwardAction = new Qt3DInput::QAction(this);
    m_ButtonDownwardAction->addInput(m_ButtonDownwardInput);
    m_ButtonDownwardAction->setObjectName("ButtonDownward");

    /// Mouse
    m_MouseDevice = new Qt3DInput::QMouseDevice(this);
    // MouseLeftButton
    m_MouseLeftButtonInput = new Qt3DInput::QActionInput(this);
    m_MouseLeftButtonInput->setButtons(QVector<int>() << Qt::LeftButton);
    m_MouseLeftButtonInput->setSourceDevice(m_MouseDevice);
    m_MouseLeftButtonAction = new Qt3DInput::QAction(this);
    m_MouseLeftButtonAction->addInput(m_MouseLeftButtonInput);
    m_MouseLeftButtonAction->setObjectName("MouseLeftButton");

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
    m_LogicalDevice->addAction(m_MouseLeftButtonAction);
    m_LogicalDevice->addAction(m_ButtonAccelerationAction);
    m_LogicalDevice->addAction(m_ButtonForwardAction);
    m_LogicalDevice->addAction(m_ButtonBackwardAction);
    m_LogicalDevice->addAction(m_ButtonLeftwardAction);
    m_LogicalDevice->addAction(m_ButtonRightwardAction);
    m_LogicalDevice->addAction(m_ButtonUpwardAction);
    m_LogicalDevice->addAction(m_ButtonDownwardAction);
    m_LogicalDevice->addAxis(m_XAxis);
    m_LogicalDevice->addAxis(m_YAxis);
    m_LogicalDevice->addAxis(m_ZAxis);

    QObject::connect(m_MouseLeftButtonAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::activeChanged);
    QObject::connect(m_ButtonAccelerationAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::activeChanged);
    QObject::connect(m_ButtonForwardAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::valueChanged);
    QObject::connect(m_ButtonBackwardAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::valueChanged);
    QObject::connect(m_ButtonLeftwardAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::valueChanged);
    QObject::connect(m_ButtonRightwardAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::valueChanged);
    QObject::connect(m_ButtonUpwardAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::valueChanged);
    QObject::connect(m_ButtonDownwardAction, &Qt3DInput::QAction::activeChanged, this, &CameraController::valueChanged);

    QObject::connect(m_XAxis, &Qt3DInput::QAxis::valueChanged, this, &CameraController::valueChanged);
    QObject::connect(m_YAxis, &Qt3DInput::QAxis::valueChanged, this, &CameraController::valueChanged);
    QObject::connect(m_ZAxis, &Qt3DInput::QAxis::valueChanged, this, &CameraController::valueChanged);

    QObject::connect(m_FrameAction, &Qt3DLogic::QFrameAction::triggered, this, &CameraController::frameActionTriggered);
}

void CameraController::activeChanged(bool isActiv)
{
    if(sender()->objectName() == "MouseLeftButton") m_MouseLeftButtonPressed = isActiv;
    if(sender()->objectName() == "ButtonAcceleration") m_ButtonAccelerationPressed = isActiv;
}

void CameraController::valueChanged(float value)
{
    if(sender()->objectName() == "XAxis") m_dx = value;
    else if(sender()->objectName() == "YAxis") m_dy = value;
    else if(sender()->objectName() == "ZAxis") m_dz = value;
    else if(sender()->objectName() == "ButtonForward") m_dz = value;
    else if(sender()->objectName() == "ButtonBackward") m_dz = -value;
    else if(sender()->objectName() == "ButtonLeftward") m_dx = -value;
    else if(sender()->objectName() == "ButtonRightward") m_dx = value;
    else if(sender()->objectName() == "ButtonUpward") m_dy = value;
    else if(sender()->objectName() == "ButtonDownward") m_dy = -value;
}

void CameraController::frameActionTriggered(float dt)
{
    if(!m_Camera) return;

    auto translate =  QVector3D(0.0f, 0.0f, 0.0f);

    float la = (m_ButtonAccelerationPressed ? config->CameraRotationAcceleration() : 1) * config->CameraRotationSpeed();
    float ma = (m_ButtonAccelerationPressed ? config->CameraMoveAcceleration() : 1) * config->CameraMoveSpeed();

    if(m_MouseLeftButtonPressed)
    {
        m_Camera->pan(-m_dx * la * dt, QVector3D(0.0, 1.0, 0.0));
        m_Camera->tilt(-m_dy * la * dt);
    }
    else
    {
       translate += QVector3D(ma * m_dx * dt, ma * m_dy * dt, 0.0f);
    }

    translate += QVector3D(0.0f, 0.0f, ma * m_dz * dt);

    m_Camera->translate(translate);

}
