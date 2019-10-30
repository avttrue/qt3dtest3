#include "dialogvalueslist.h"
#include "controls.h"
#include "properties.h"

#include <QCheckBox>
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QSpinBox>

DialogValuesList::DialogValuesList(const QString& icon,
                                   const QString& caption,
                                   bool modal,
                                   QMap<QString, DialogValue> *values,
                                   QWidget* parent) :
    QDialog(parent)
{
    m_Values = values;
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(caption);
    setWindowIcon(QIcon(icon));
    setModal(modal);

    auto vblForm = new QVBoxLayout();
    vblForm->setAlignment(Qt::AlignAbsolute);
    vblForm->setMargin(0);
    vblForm->setSpacing(0);
    setLayout(vblForm);

    auto saContent = new QScrollArea();
    saContent->setAlignment(Qt::AlignTop);
    saContent->setWidgetResizable(true);

    auto wContent = new QWidget();
    saContent->setWidget(wContent);

    glContent = new QGridLayout();
    wContent->setLayout(glContent);
    glContent->setAlignment(Qt::AlignTop);

    toolBar = new QToolBar();
    toolBar->setMovable(false);

    toolBar->addWidget(new WidgetSpacer());

    auto actionAccept = new QAction(QIcon(":/res/icons/yes.svg"), tr("Принять"));
    QObject::connect(actionAccept, &QAction::triggered, [=](){ accept(); });
    toolBar->addAction(actionAccept);

    auto actionCancel = new QAction(QIcon(":/res/icons/no.svg"), tr("Закрыть"));
    QObject::connect(actionCancel, &QAction::triggered, [=](){ reject(); });
    toolBar->addAction(actionCancel);

    vblForm->addWidget(saContent);
    vblForm->addWidget(toolBar);

    slotLoadContent(values);

    resize(DIALOG_SIZE);
}

void DialogValuesList::addWidgetContent(QWidget *widget)
{
    glContent->addWidget(widget, glContent->count(), 0, 1, 1, Qt::AlignTop);
}

void DialogValuesList::addToolbarButton(QAction *action)
{
    toolBar->addAction(action);
}

void DialogValuesList::slotLoadContent(QMap<QString, DialogValue>* values)
{
    QLayoutItem *child;
    while ((child = glContent->takeAt(0)) != nullptr) delete child->widget();

    if(!values) { qCritical() << __func__ << "Values is empty"; return; }

    for (auto s: values->keys())
    {
        QVariant::Type t = values->value(s).type;
        QVariant v = values->value(s).value;
        QVariant minv = values->value(s).minValue;
        QVariant maxv = values->value(s).maxValue;
        if(t == QVariant::String)
        {
            auto widget = new QWidget();
            auto bl = new QHBoxLayout();
            bl->setMargin(0);
            auto label = new QLabel(widget);
            label->setText(QString("<b>%1:</b>").arg(s));
            bl->addWidget(label, 1);
            auto le = new QLineEdit(v.toString(), widget);
            le->setProperty("ValueName", s);
            connect(le, &QLineEdit::textEdited, this, &DialogValuesList::slotStringValueChanged);
            bl->addWidget(le, 1);
            widget->setLayout(bl);
            addWidgetContent(widget);
        }
        else if(t == QVariant::Bool)
        {
            auto cbox = new QCheckBox(s);
            cbox->setChecked(v.toBool());
            cbox->setProperty("ValueName", s);
            connect(cbox, &QCheckBox::stateChanged, this, &DialogValuesList::slotBoolValueChanged);
            addWidgetContent(cbox);
        }
        else if(t == QVariant::Int || t == QVariant::Double)
        {
            auto spinbox = new QSpinBox();
            spinbox->setPrefix(QString("%1: ").arg(s));
            spinbox->setRange(minv.toInt(), maxv.toInt());
            spinbox->setSingleStep(1);
            spinbox->setValue(v.toInt());
            spinbox->installEventFilter(this);
            spinbox->setProperty("ValueName", s);
            void (QSpinBox::*Sender)(int) = &QSpinBox::valueChanged;
            void (DialogValuesList::*Receiver)(int) = &DialogValuesList::slotIntValueChanged;
            connect(spinbox, Sender, this, Receiver);
            addWidgetContent(spinbox);
        }
        else if(t == QVariant::StringList) // TODO: переделать в формат списка
        {
            auto widget = new QWidget();
            auto bl = new QHBoxLayout();
            bl->setMargin(0);
            auto label = new QLabel();
            label->setText(QString("<b>%1:</b>").arg(s));
            bl->addWidget(label, 1);
            auto le = new QLineEdit(v.toStringList().join(","), widget);
            le->setProperty("ValueName", s);
            connect(le, &QLineEdit::textEdited, this, &DialogValuesList::slotStringListValueChanged);
            bl->addWidget(le, 1);
            widget->setLayout(bl);
            addWidgetContent(widget);
        }
        else
        { qCritical() << __func__ << "Value" << s << ": unsupported type" << t; }
    }
}

bool DialogValuesList::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    if(event->type() == QEvent::Wheel) { return true; }
    return false;
}

void DialogValuesList::slotStringValueChanged(const QString &value)
{
    auto ledit = qobject_cast<QLineEdit*>(sender());
    if(ledit == nullptr) { qCritical() << __func__ << "Signal sender not found."; return; }
    auto key = ledit->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = value;
    m_Values->insert(key, dv);
}

void DialogValuesList::slotStringListValueChanged(const QString &value)
{
    auto ledit = qobject_cast<QLineEdit*>(sender());
    if(ledit == nullptr) { qCritical() << __func__ << "Signal sender not found."; return; }
    auto key = ledit->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = value.split(",");
    m_Values->insert(key, dv);
}

void DialogValuesList::slotBoolValueChanged(bool value)
{
    auto cbox = qobject_cast<QCheckBox*>(sender());
    if(cbox == nullptr) { qCritical() << __func__ << "Signal sender not found."; return; }
    auto key = cbox->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = value;
    m_Values->insert(key, dv);
}

void DialogValuesList::slotIntValueChanged(int value)
{
    auto spinbox = qobject_cast<QSpinBox*>(sender());
    if(spinbox == nullptr) { qCritical() << __func__ << "Signal sender not found."; return; }
    auto key = spinbox->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = value;
    m_Values->insert(key, dv);
}
