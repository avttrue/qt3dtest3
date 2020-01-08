#include "frameratecalculator.h"

#include <QDateTime>

FrameRateCalculator::FrameRateCalculator(int period, QObject *parent) :
    QObject(parent),
    m_FramesPerSecond(0.0),
    m_Period(abs(period))
{
    m_Time = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void FrameRateCalculator::calculate()
{
    m_CurrentFrameCount++;
    auto time = QDateTime::currentDateTime().toMSecsSinceEpoch();
    auto timedelta = time - m_Time;
    if(timedelta > 500)
    {

        auto fps = 1000 * static_cast<float>(m_CurrentFrameCount) / (timedelta);
        if(abs(m_FramesPerSecond - fps) > 0.09f)
        {
            m_FramesPerSecond = fps;
            Q_EMIT signalFramesPerSecondChanged(m_FramesPerSecond);
        }

        m_Time = time;
        m_CurrentFrameCount = 0;
    }
}

void FrameRateCalculator::setPeriod(int Period)
{
    if(Period != m_Period) m_Period = Period;
}

int FrameRateCalculator::Period() const { return m_Period; }
float FrameRateCalculator::FramesPerSecond() const { return m_FramesPerSecond; }
