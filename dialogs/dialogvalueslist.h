#ifndef DIALOGVALUESLIST_H
#define DIALOGVALUESLIST_H

#include <QDialog>
#include <QGridLayout>
#include <QToolBar>

struct DialogValue
{
    QVariant::Type type = QVariant::Invalid;
    QVariant value = QVariant();
    QVariant minValue = QVariant();
    QVariant maxValue = QVariant();
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
    void slotLoadContent(QMap<QString, DialogValue> *values);

protected:
    void addWidgetContent(QWidget* widget);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    QGridLayout* glContent;
    QToolBar* toolBar;
    QMap<QString, DialogValue>* m_Values;

private Q_SLOTS:
    void slotStringValueChanged(const QString& value);
    void slotStringListValueChanged(const QString& value);
    void slotBoolValueChanged(bool value);
    void slotIntValueChanged(int value);
};

#endif // DIALOGVALUESLIST_H
