#include "controls.h"
#include "helpers.h"

#include <QCoreApplication>
#include <QVBoxLayout>

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

LoadingPage::LoadingPage(const QString &caption, QWidget *parent):
QWidget(parent)
{
    m_LabelCaption = new QLabel(caption, this);

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(0);
    vblForm->setSpacing(0);
    setLayout(vblForm);

    vblForm->addWidget(m_LabelCaption);
}

void LoadingPage::slotStep(const QString &text)
{
    m_LabelCaption->setText(text);

    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}
