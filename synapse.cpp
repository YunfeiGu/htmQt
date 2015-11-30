#include "synapse.h"
#include "htmGlobal.h"
#include <QTime>
#include <QDebug>


// 用于从文件载入时
Synapse::Synapse(int nId, float perm, QObject *parent) :
    QObject(parent)
{
    m_nId   = nId;
    m_fPerm = perm;
    m_nActiveTimes = 0;
}

Synapse::Synapse(int nId, QObject *parent) :
    QObject(parent)
{
    m_nId = nId;
    m_nActiveTimes = 0;

    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec() + time.second()*1000);
    float delta = (qrand()%200- 100)*0.01;

    qDebug() << "random delta: " << delta << "\n";

    m_fPerm = HtmGlobal::m_fPermConnected*(1+delta);
}


// 此处是 receive valid input时作出的反应
// 当然只有 input 是 valid， input 才会发出信号来激活此 slot
void Synapse::onRecv()
{
    if (m_fPerm >= HtmGlobal::m_fPermConnected)
    {
        m_bIsActivated = true;

        // inc activeTimes
        m_nActiveTimes++;

        emit activated();
    }
    else
    {
        m_bIsActivated = false;
    }
}

bool Synapse::isConnected()
{
    return (m_fPerm >= HtmGlobal::m_fPermConnected);
}

// activated 是指收到valid input，同时synapse 处于 connected状态
bool Synapse::isActivated()
{
    return m_bIsActivated;
}

int Synapse::activeTimes()
{
    return m_nActiveTimes;
}

void Synapse::incPerm()
{
    m_fPerm += m_fPermTmpDelta;
    m_fPermTmpDelta = 0.f;

    if (m_fPerm > 1.0)
    {
        m_fPerm = 1.0;
    }
}

void Synapse::decPerm()
{
    m_fPerm -= m_fPermTmpDelta;
    m_fPermTmpDelta = 0.f;

    if (m_fPerm < 0.0)
    {
        m_fPerm = 0.0;
    }
}

void Synapse::incPermTemporal()
{
    m_fPermTmpDelta += HtmGlobal::m_fDeltaPermInc;
}

void Synapse::decPermTemporal()
{
    m_fPermTmpDelta += HtmGlobal::m_fDeltaPermDec;
}
