#include "controls.h"

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
    setFlat(true);
    setStyleSheet("text-align: left; border: 1px outset grey; padding: 2px; border-radius: 7px;");
    setFocusPolicy(Qt::NoFocus);
}

WidgetSpacer::WidgetSpacer(QWidget *parent): QWidget(parent)
{ setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); }
