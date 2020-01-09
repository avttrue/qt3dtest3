#include "controls.h"
#include "helpers.h"


ControlButton::ControlButton(QWidget *parent):
    QPushButton(parent)
{ setup(); }

ControlButton::ControlButton(const QString &text, QWidget *parent):
    QPushButton(text, parent)
{ setup(); }

ControlButton::ControlButton(const QIcon &icon, const QString &text, QWidget *parent):
    QPushButton(icon, text, parent)
{ setup(); }

void ControlButton::setup()
{
    setStyleSheet(getTextFromRes(":/res/qss/button.css"));
    setFocusPolicy(Qt::NoFocus);
}

WidgetSpacer::WidgetSpacer(QWidget *parent): QWidget(parent)
{ setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); }

SeparatorH::SeparatorH(QWidget *parent):
    QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::HLine);
    setMidLineWidth(2);
    setLineWidth(1);
    setFocusPolicy(Qt::NoFocus);
}
