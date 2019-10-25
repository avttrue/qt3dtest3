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
    setStyleSheet(getTextFromRes(":/res/qss/button.qss"));
    setFocusPolicy(Qt::NoFocus);
}

WidgetSpacer::WidgetSpacer(QWidget *parent): QWidget(parent)
{ setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); }
