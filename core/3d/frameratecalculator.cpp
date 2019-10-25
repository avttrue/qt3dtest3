#include "frameratecalculator.h"

#include <QDateTime>

FrameRateCalculator::FrameRateCalculator(int frameCount, QObject *parent) :
    QObject(parent),
    m_FramesPerSecond(0.0),
    m_CurrentFrameCount(0)
{
    m_Time = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(frameCount <= 0) m_FrameCount = 1;
    else m_FrameCount = frameCount;
}

void FrameRateCalculator::calculate()
{
    m_CurrentFrameCount++;
    if(m_CurrentFrameCount >= m_FrameCount)
    {
        auto time = QDateTime::currentDateTime().toMSecsSinceEpoch();
        auto fps = 1000 * static_cast<float>(m_CurrentFrameCount) / (time - m_Time);

        if(abs(m_FramesPerSecond - fps) > 0.09f)
        {
            m_FramesPerSecond = fps;
            emit signalFramesPerSecondChanged(m_FramesPerSecond);
        }

        m_Time = time;
        m_CurrentFrameCount = 0;
    }
}

void FrameRateCalculator::setFrameCount(int fc)
{
    if (m_FrameCount == fc) return;

    m_FrameCount = fc;
    emit signalFrameCountChanged(m_FrameCount);
}

float FrameRateCalculator::FramesPerSecond() const { return m_FramesPerSecond; }
int FrameRateCalculator::FrameCount() const { return m_FrameCount; }
