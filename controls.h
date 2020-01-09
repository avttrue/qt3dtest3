#ifndef CONTROLS_H
#define CONTROLS_H

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

#endif // CONTROLS_H
