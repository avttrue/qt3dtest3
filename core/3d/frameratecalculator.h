#ifndef FRAMERATECALCULATOR_H
#define FRAMERATECALCULATOR_H

#include <QObject>

class FrameRateCalculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int FrameCount READ FrameCount WRITE setFrameCount NOTIFY signalFrameCountChanged)

private:
    long long m_Time;
    float m_FramesPerSecond;
    int m_FrameCount;
    int m_CurrentFrameCount;

public:
    explicit FrameRateCalculator(int frameCount, QObject *parent = nullptr);
    float FramesPerSecond() const;
    int FrameCount() const;
    void calculate();

signals:
    void signalFramesPerSecondChanged(float FramesPerSecond);
    void signalFrameCountChanged(int FrameCount);

public slots:
    void setFrameCount(int fc);
};

#endif // FRAMERATECALCULATOR_H
