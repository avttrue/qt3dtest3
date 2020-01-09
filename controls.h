#ifndef CONTROLS_H
#define CONTROLS_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class ControlButton : public QPushButton
{
public:
    explicit ControlButton(QWidget *parent = nullptr);
    explicit ControlButton(const QString &text, QWidget *parent = nullptr);
    ControlButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
private:
    void setup();
};

class WidgetSpacer: public QWidget
{

public:
    explicit WidgetSpacer(QWidget* parent = nullptr);
};

class SeparatorH:  public QFrame
{
public:
    explicit SeparatorH(QWidget* parent = nullptr);
};

class LoadingPage: public QWidget
{
public:
    explicit LoadingPage(const QString &caption, QWidget* parent = nullptr);

private:
    QLabel* m_LabelCaption;

    public Q_SLOTS:
        void slotStep(const QString &text);
};

#endif // CONTROLS_H
