/******************************************************************************
    QtAV:  Media play library based on Qt and FFmpeg
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>
    
*   This file is part of QtAV

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


#ifndef QTAV_RAWAUDIOBUFFER_H
#define QTAV_RAWAUDIOBUFFER_H

#include <QReadWriteLock>
#include <QScopedPointer>
#include <QWaitCondition>
#include <QByteArray>

class RawAudioBuffer
{
public:
    RawAudioBuffer();
    virtual ~RawAudioBuffer() {}

    /*!
     * \brief setThreshold
     * do nothing if min >= capacity()
     * \param min
     */
    void setThreshold(int min); //wake up and enqueue

    /*! \brief put
     *  put t into the queue. Will block if blockFull is set to true (and optionally can specify a wait timeout in ms).
     *  \param t the item to copy into the queue
     *  \return true if item was successfully placed in the queue and the queue was not full.
     *          false is returned if the queue is (still) full. The item is still placed into a full queue!
     *          Note that even a 'full' queue will accept new items and t WILL be placed in the queue regardless of return value.
     */
    bool put(const QByteArray& t);
    /*! \brief take
     *   Dequeue 1 item from queue, optionally blocking.
     * \param wait_timeout_ms this parameter is used if blockEmpty == true (the default).
     *         If the queue is empty, optionally wait for the queue to not be empty a maximum of wait_timeout_ms milliseconds.
     *         ULONG_MAX means wait indefinitely.
     * \param isValid a pointer to a bool (optional).  If isValid is set to true after a call, the returned item is valid. False means the queue was empty or the timeout expired.
     * \return the item taken.  It may not be valid if the queue was empty and timeout expired. Check optional isValid flag to determine if that is the case.
     */
    QByteArray take(int len, unsigned long wait_timeout_ms = ULONG_MAX, bool *isValid = 0);
    void setBlocking(bool block); //will wake if false. called when no more data can enqueue
    void blockEmpty(bool block);
//TODO:setMinBlock,MaxBlock
    inline void clear();
    // TODO: checkEmpty, Enough, Full?
    inline bool isEmpty() const;
    inline bool isEnough() const; //size > thres
    inline int size() const;
    inline int threshold() const;

    class StateChangeCallback
    {
    public:
        virtual ~StateChangeCallback(){}
        virtual void call() = 0;
    };

protected:
    /*!
     * \brief checkFull
     * Check whether the queue is full. Default implemention is compare queue size to capacity.
     * Full is now a more generic notion. You can implement it as checking queued bytes etc.
     * \return true if queue is full
     */
    bool checkEnough() const;
    bool checkEnoughForReadSpecificData(int len, int &actual_len);

    void onPut(const QByteArray&) {}
    void onTake(const QByteArray&) {}

    bool block_enough;
    int thres, cur_pos, total_size;
    QByteArray data_container;
private:
    mutable QReadWriteLock lock; //locker in const func
    QReadWriteLock block_change_lock;
    QWaitCondition cond_empty;
};

/* cap - thres = 24, about 1s
 * if fps is large, then larger capacity and threshold is preferred
 */
RawAudioBuffer::RawAudioBuffer()
    :block_enough(true),thres(32),cur_pos(0),total_size(0)
{
}

void RawAudioBuffer::setThreshold(int min)
{
    //qDebug("queue threshold==>>%d", min);
    QWriteLocker locker(&lock);
    Q_UNUSED(locker);

    thres = min;
}

bool RawAudioBuffer::put(const QByteArray &t)
{
    bool ret = true;
    QWriteLocker locker(&lock);
    Q_UNUSED(locker);
    data_container.append(t);
    onPut(t); // emit bufferProgressChanged here if buffering
    if (checkEnough()) {
        cond_empty.wakeOne(); //emit buffering finished here
        //qDebug("queue is enough: %d/%d~%d", queue.size(), thres, cap);
    } else {
        //qDebug("buffering: %d/%d~%d", queue.size(), thres, cap);
    }
    return ret;
}

QByteArray RawAudioBuffer::take(int len, unsigned long timeout_ms, bool *isValid)
{
    if (isValid) *isValid = false;
    QWriteLocker locker(&lock);
    Q_UNUSED(locker);
    int actual_len = -1;
    if (checkEnoughForReadSpecificData(len, actual_len)) {//TODO:always block?
        //qDebug("queue empty!!");
        if (block_enough)
            cond_empty.wait(&lock,timeout_ms); //block when empty only
    }
    if (checkEnoughForReadSpecificData(len, actual_len)) {
        //qWarning("Queue is still empty");
        return QByteArray();
    }

    QByteArray data = data_container.mid(cur_pos, actual_len);
    cur_pos += actual_len;
    if (isValid) *isValid = true;
    onTake(data); // emit start buffering here if empty
    return data;
}

void RawAudioBuffer::setBlocking(bool block)
{
    QWriteLocker locker(&lock);
    Q_UNUSED(locker);
    block_enough = block;
    if (!block) {
        cond_empty.wakeAll(); //empty still wait. setBlock=>setCapacity(-1)
    }
}

void RawAudioBuffer::blockEmpty(bool block)
{
    if (!block) {
        cond_empty.wakeAll();
    }
    QWriteLocker locker(&block_change_lock);
    Q_UNUSED(locker);
    block_enough = block;
}

void RawAudioBuffer::clear()
{
    QWriteLocker locker(&lock);
    Q_UNUSED(locker);
    //cond_empty.wakeAll();
    data_container.clear();
    //TODO: assert not empty
    onTake(QByteArray());
}

bool RawAudioBuffer::isEmpty() const
{
    QReadLocker locker(&lock);
    Q_UNUSED(locker);
    return data_container.isEmpty();
}

bool RawAudioBuffer::isEnough() const
{
    QReadLocker locker(&lock);
    Q_UNUSED(locker);
    return data_container.size() >= thres;
}

int RawAudioBuffer::size() const
{
    QReadLocker locker(&lock);
    Q_UNUSED(locker);
    return data_container.size();
}

int RawAudioBuffer::threshold() const
{
    QReadLocker locker(&lock);
    Q_UNUSED(locker);
    return thres;
}

bool RawAudioBuffer::checkEnough() const
{
    return data_container.size()-cur_pos >= thres || data_container.size() == total_size;
}

bool RawAudioBuffer::checkEnoughForReadSpecificData(int len, int &actual_len)
{
    if (data_container.size()-cur_pos >= len) {
        actual_len = len;
        return true;
    } else if (data_container.size() == total_size) {
        actual_len = total_size-cur_pos;
        return true;
    } else {
        actual_len = 0;
        return false;
    }
}
#endif
