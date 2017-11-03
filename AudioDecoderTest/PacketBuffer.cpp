/******************************************************************************
    QtAV:  Multimedia framework based on Qt and FFmpeg
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV (from 2015)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#include "PacketBuffer.h"
#include <QtCore/QDateTime>

static const int kAvgSize = 16;
PacketBuffer::PacketBuffer()
    : m_buffering(true) // in buffering state at the beginning
    , m_max(1.5)
    , m_buffer(0)
    , m_value0(0)
    , m_value1(0)
    , m_history(kAvgSize)
{
}

PacketBuffer::~PacketBuffer()
{
}

void PacketBuffer::setBufferValue(qint64 value)
{
    m_buffer = value;
}

qint64 PacketBuffer::bufferValue() const
{
    return m_buffer;
}

void PacketBuffer::setBufferMax(qreal max)
{
    if (max < 1.0) {
        qWarning("max (%f) must >= 1.0", max);
        return;
    }
    m_max = max;
}

qreal PacketBuffer::bufferMax() const
{
    return m_max;
}

qint64 PacketBuffer::buffered() const
{
    Q_ASSERT(m_value1 >= m_value0);
    return m_value1 - m_value0;
}

bool PacketBuffer::isBuffering() const
{
    return m_buffering;
}

qreal PacketBuffer::bufferProgress() const
{
    const qreal p = qreal(buffered())/qreal(bufferValue());
    return qMax<qreal>(qMin<qreal>(p, 1.0), 0.0);
}

qreal PacketBuffer::bufferSpeed() const
{
    return calc_speed(false);
}

qreal PacketBuffer::bufferSpeedInBytes() const
{
    return calc_speed(true);
}

bool PacketBuffer::checkEnough() const
{
    return buffered() >= bufferValue();
}

bool PacketBuffer::checkFull() const
{
    return buffered() >= qint64(qreal(bufferValue())*bufferMax());
}

void PacketBuffer::onPut(const Packet &p)
{
    m_value1++;

    if (!m_buffering)
        return;
    if (checkEnough()) {
        m_buffering = false;
    }
    if (!m_buffering) { //buffering=>buffered
        m_history = ring<BufferInfo>(kAvgSize);
        return;
    }
    BufferInfo bi;
    bi.bytes = p.data.size();
    if (!m_history.empty())
        bi.bytes += m_history.back().bytes;
    bi.v = m_value1;
    bi.t = QDateTime::currentMSecsSinceEpoch();
    m_history.push_back(bi);
}

void PacketBuffer::onTake(const Packet &p)
{
    if (checkEmpty()) {
        m_buffering = true;
    }
    if (queue.isEmpty()) {
        m_value0 = 0;
        m_value1 = 0;
        return;
    }

    m_value1--;
}

qreal PacketBuffer::calc_speed(bool use_bytes) const
{
    if (m_history.empty())
        return 0;
    const qreal dt = (double)QDateTime::currentMSecsSinceEpoch()/1000.0 - m_history.front().t/1000.0;
    // dt should be always > 0 because history stores absolute time
    if (qFuzzyIsNull(dt))
        return 0;
    const qint64 delta = use_bytes ? m_history.back().bytes - m_history.front().bytes : m_history.back().v - m_history.front().v;
    if (delta < 0) {
        qWarning("PacketBuffer internal error. delta(bytes %d): %lld", use_bytes, delta);
        return 0;
    }
    return (qreal)delta/dt;
}

