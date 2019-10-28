#ifndef DIALOGVALUESLIST_H
#define DIALOGVALUESLIST_H

#include <QDialog>
#include <QGridLayout>
#include <QToolBar>

/*!
 * \brief DialogValuesList - диалог установки значений списка параметров values вида QMap<QString, QPair<QVariant::Type, QVariant>>
 */
class DialogValuesList : public QDialog
{
public:
    DialogValuesList(const QString &icon,
                     const QString &caption,
                     bool modal,
                     QMap<QString, QPair<QVariant::Type, QVariant>>* values,
                     QWidget* parent = nullptr);
    void addToolbarButton(QAction* action);
    void slotLoadContent(QMap<QString, QPair<QVariant::Type, QVariant>>* values);

protected:
    void addWidgetContent(QWidget* widget);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    QGridLayout* glContent;
    QToolBar* toolBar;
    QMap<QString, QPair<QVariant::Type, QVariant>>* m_Values;

private Q_SLOTS:
    void slotStringValueChanged(const QString& value);
    void slotStringListValueChanged(const QString& value);
    void slotBoolValueChanged(bool value);
    void slotIntValueChanged(int value);
};

#endif // DIALOGVALUESLIST_H
