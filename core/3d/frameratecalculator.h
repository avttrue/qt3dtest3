#ifndef FRAMERATECALCULATOR_H
#define FRAMERATECALCULATOR_H

#include <QObject>

class FrameRateCalculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int Period READ Period WRITE setPeriod)

private:
    long long m_Time;
    float m_FramesPerSecond;
    int m_CurrentFrameCount;    
    int m_Period;

public:
    explicit FrameRateCalculator(int period, QObject *parent = nullptr);
    float FramesPerSecond() const;
    void calculate();
    int Period() const;
    void setPeriod(int Period);

Q_SIGNALS:
    void signalFramesPerSecondChanged(float FramesPerSecond);

};

#endif // FRAMERATECALCULATOR_H
