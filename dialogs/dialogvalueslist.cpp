#include "dialogvalueslist.h"
#include "controls.h"
#include "properties.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
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

    for (auto key: values->keys())
    {
        QVariant::Type t = values->value(key).type;
        QVariant v = values->value(key).value;
        QVariant minv = values->value(key).minValue;
        QVariant maxv = values->value(key).maxValue;
        if(t == QVariant::String)
        {
            auto widget = new QWidget();
            auto bl = new QHBoxLayout();
            bl->setMargin(0);
            bl->setSpacing(1);
            auto label = new QLabel(widget);
            label->setText(QString(key));
            bl->addWidget(label, 0);
            auto le = new QLineEdit(v.toString(), widget);
            le->setProperty("ValueName", key);
            QObject::connect(le, &QLineEdit::textEdited, this, &DialogValuesList::slotStringValueChanged);
            bl->addWidget(le, 1);
            widget->setLayout(bl);
            addWidgetContent(widget);
        }
        else if(t == QVariant::Bool)
        {
            auto cbox = new QCheckBox(key);
            cbox->setChecked(v.toBool());
            cbox->setProperty("ValueName", key);
            QObject::connect(cbox, &QCheckBox::stateChanged, this, &DialogValuesList::slotBoolValueChanged);
            addWidgetContent(cbox);
        }
        else if(t == QVariant::Int || t == QVariant::Double)
        {
            auto spinbox = new QSpinBox();
            spinbox->setPrefix(QString("%1: ").arg(key));
            spinbox->setRange(minv.toInt(), maxv.toInt());
            spinbox->setSingleStep(1);
            spinbox->setValue(v.toInt());
            spinbox->installEventFilter(this);
            spinbox->setProperty("ValueName", key);
            void (QSpinBox::*Sender)(int) = &QSpinBox::valueChanged;
            void (DialogValuesList::*Receiver)(int) = &DialogValuesList::slotIntValueChanged;
            QObject::connect(spinbox, Sender, this, Receiver);
            addWidgetContent(spinbox);
        }
        else if(t == QVariant::StringList)
        {
            auto widget = new QWidget();
            auto label = new QLabel();
            label->setText(QString(key));
            auto bl = new QHBoxLayout();
            bl->setMargin(0);
            bl->setSpacing(2);
            bl->addWidget(label, 0);
            if(values->value(key).mode == DialogValueMode::Default)
            { 
                auto le = new QLineEdit(v.toStringList().join(","), widget);
                le->setProperty("ValueName", key);
                le->setToolTip("Enter");
                // значение присваивается по нажатию Enter
                QObject::connect(le, &QLineEdit::returnPressed, this, &DialogValuesList::slotStringListValueChanged);
                bl->addWidget(le, 1);
                widget->setLayout(bl);
            }
            else if(values->value(key).mode == DialogValueMode::OneFromList)
            {
                auto cb = new QComboBox(widget);
                cb->addItems(maxv.toStringList());
                cb->setProperty("ValueName", key);
                auto index = maxv.toStringList().indexOf(v.toString());
                if(index != -1) cb->setCurrentIndex(index);
                void (QComboBox::*Sender)(const QString&) = &QComboBox::currentIndexChanged;
                void (DialogValuesList::*Receiver)(const QString&) = &DialogValuesList::slotOneOfStringListValueChanged;
                QObject::connect(cb, Sender, this, Receiver);                
                bl->addWidget(cb, 1);
                widget->setLayout(bl);
            }
            else if(values->value(key).mode == DialogValueMode::ManyFromList)
            {
                auto lv = new QListView(widget);
                lv->setSelectionMode(QAbstractItemView::NoSelection);
                lv->setEditTriggers(QAbstractItemView::NoEditTriggers);
                lv->setSelectionRectVisible(false);                
                QFontMetrics fm(lv->font());
                lv->setFixedHeight(fm.height() * 4);
                auto model = new QStandardItemModel(lv);
                model->setProperty("ValueName", key);
                for(QString s: maxv.toStringList())
                {
                    auto i = new QStandardItem(s);
                    i->setCheckable(true);
                    i->setCheckState(v.toStringList().contains(s) ? Qt::Checked : Qt::Unchecked);
                    model->appendRow(i);
                }

                lv->setModel(model);
                connect(model, &QStandardItemModel::itemChanged, this, &DialogValuesList::slotManyOfStringListValueChanged);
                bl->addWidget(lv, 1);
                widget->setLayout(bl);
            }
            addWidgetContent(widget);
        }
        else
        { qCritical() << __func__ << ": Value" << key << "Unsupported type" << t; }
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
    if(!ledit) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = ledit->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = value;
    m_Values->insert(key, dv);
}

void DialogValuesList::slotStringListValueChanged()
{
    auto ledit = qobject_cast<QLineEdit*>(sender());
    if(!ledit) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = ledit->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = ledit->text().split(",", QString::SkipEmptyParts).replaceInStrings(QRegExp(RE_FIRST_LAST_SPACES), "");
    m_Values->insert(key, dv);
}

void DialogValuesList::slotOneOfStringListValueChanged(const QString &value)
{
    auto cb = qobject_cast<QComboBox*>(sender());
    if(!cb) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = cb->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = QStringList(value);
    m_Values->insert(key, dv);
}

void DialogValuesList::slotManyOfStringListValueChanged()
{
    auto sim = qobject_cast<QStandardItemModel*>(sender());
    if(!sim) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = sim->property("ValueName").toString();
    auto sl = QStringList();
    for(int i = 0; i < sim->rowCount(); i++)
        if(sim->item(i)->checkState() == Qt::Checked) sl.append(sim->item(i)->text());
    DialogValue dv = m_Values->value(key);
    dv.value = sl;
    m_Values->insert(key, dv);
}

void DialogValuesList::slotBoolValueChanged(bool value)
{
    auto cbox = qobject_cast<QCheckBox*>(sender());
    if(!cbox) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = cbox->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = value;
    m_Values->insert(key, dv);
}

void DialogValuesList::slotIntValueChanged(int value)
{
    auto spinbox = qobject_cast<QSpinBox*>(sender());
    if(!spinbox) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = spinbox->property("ValueName").toString();
    DialogValue dv = m_Values->value(key);
    dv.value = value;
    m_Values->insert(key, dv);
}
