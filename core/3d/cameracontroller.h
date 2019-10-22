#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <Qt3DCore/QEntity>

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
    CameraController(Qt3DCore::QNode* parent = nullptr);

    void setCamera(Qt3DRender::QCamera *Camera);

protected:


protected slots:
    void activeChanged(bool isActiv);
    void valueChanged(float value);
    void frameActionTriggered(float dt);

private:
    Qt3DInput::QMouseDevice* m_MouseDevice;
    Qt3DInput::QKeyboardDevice* m_KeyboardDevice;
    Qt3DInput::QLogicalDevice* m_LogicalDevice;

    Qt3DInput::QAction* m_RollZRightKeyAction;
    Qt3DInput::QAction* m_ButtonAccelerationAction;
    Qt3DInput::QAction* m_LeftButtonAction;

    Qt3DInput::QActionInput* m_LeftButtonInput;
    Qt3DInput::QActionInput* m_ButtonAccelerationInput;
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

    bool m_LeftButtonPressed;
    bool m_ButtonAccelerationPressed;

};

#endif // CAMERACONTROLLER_H
