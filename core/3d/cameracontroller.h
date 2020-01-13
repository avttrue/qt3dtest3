#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QKeyboardDevice>
#include <Qt3DInput/QLogicalDevice>
#include <Qt3DInput/QAction>
#include <Qt3DInput/QActionInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DRender/QCamera>
#include <Qt3DLogic/QFrameAction>

class CameraController: public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    CameraController( Qt3DCore::QNode* parent, Qt3DRender::QCamera* camera);

protected:


protected Q_SLOTS:
    void activeChanged(bool isActiv);
    void valueChanged(float value);
    void frameActionTriggered(float dt);

private:
    Qt3DInput::QMouseDevice* m_MouseDevice;
    Qt3DInput::QKeyboardDevice* m_KeyboardDevice;
    Qt3DInput::QLogicalDevice* m_LogicalDevice;

    Qt3DInput::QAction* m_RollZRightKeyAction;
    Qt3DInput::QAction* m_ButtonAccelerationAction;
    Qt3DInput::QAction* m_ButtonForwardAction;
    Qt3DInput::QAction* m_ButtonBackwardAction;
    Qt3DInput::QAction* m_ButtonLeftwardAction;
    Qt3DInput::QAction* m_ButtonRightwardAction;
    Qt3DInput::QAction* m_ButtonUpwardAction;
    Qt3DInput::QAction* m_ButtonDownwardAction;

    Qt3DInput::QAction* m_MouseLeftButtonAction;

    Qt3DInput::QActionInput* m_MouseLeftButtonInput;
    Qt3DInput::QActionInput* m_ButtonAccelerationInput;
    Qt3DInput::QActionInput* m_ButtonForwardInput;
    Qt3DInput::QActionInput* m_ButtonBackwardInput;
    Qt3DInput::QActionInput* m_ButtonLeftwardInput;
    Qt3DInput::QActionInput* m_ButtonRightwardInput;
    Qt3DInput::QActionInput* m_ButtonUpwardInput;
    Qt3DInput::QActionInput* m_ButtonDownwardInput;
    Qt3DLogic::QFrameAction* m_FrameAction;

    Qt3DInput::QAxis* m_XAxis;
    Qt3DInput::QAxis* m_YAxis;
    Qt3DInput::QAxis *m_ZAxis;

    Qt3DInput::QAnalogAxisInput* m_MouseXInput;
    Qt3DInput::QAnalogAxisInput* m_MouseYInput;    
    Qt3DInput::QAnalogAxisInput *m_MouseZInput;

    Qt3DRender::QCamera* m_Camera;    

    float m_dx;
    float m_dy;
    float m_dz;

    bool m_MouseLeftButtonPressed;
    bool m_ButtonAccelerationPressed;   
};

#endif // CAMERACONTROLLER_H
