#ifndef DIALOGVALUESLIST_H
#define DIALOGVALUESLIST_H

#include <QDialog>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QToolBar>

/*!
 * \brief DialogValueMode - режим отображения значений
 */
enum DialogValueMode
{
    Default = 0,    // поумолчанию, для StringList - перечисление через запятую
    OneFromList,    // для StringList - один из списка
    ManyFromList    // для StringList - несколько из списка
};

/*!
 * \brief struct DialogValue - передаваемая для редактирования структура;
 * type: тип результата;
 * value: результат и значение поумолчанию
 * minValue: минимальное значение, если доступно;
 * maxValue: максимальнеа значение, для StringList вида OneFromList, ManyFromList - все варианты выбора;
 * mode: способ отображения значений
 */
struct DialogValue
{
    QVariant::Type type = QVariant::Invalid;
    QVariant value = QVariant();
    QVariant minValue = QVariant();
    QVariant maxValue = QVariant();
    DialogValueMode mode = DialogValueMode::Default;
};

class DialogValuesList : public QDialog
{
public:
    DialogValuesList(const QString &icon,
                     const QString &caption,
                     bool modal,
                     QMap<QString, DialogValue>* values,
                     QWidget* parent = nullptr);
    void addToolbarButton(QAction* action);

protected:
    void addWidgetContent(QWidget* widget);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    QGridLayout* glContent;
    QToolBar* toolBar;
    QMap<QString, DialogValue>* m_Values;

public Q_SLOTS:
    void slotLoadContent(QMap<QString, DialogValue> *values);

private Q_SLOTS:    
    void slotStringValueChanged(const QString& value);
    void slotStringListValueChanged();
    void slotOneOfStringListValueChanged(const QString& value);
    void slotManyOfStringListValueChanged();
    void slotBoolValueChanged(bool value);
    void slotIntValueChanged(int value);
};

#endif // DIALOGVALUESLIST_H
