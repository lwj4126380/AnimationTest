#include "RawAudioBuffer.h"
#include <QDebug>

/* cap - thres = 24, about 1s
 * if fps is large, then larger capacity and threshold is preferred
 */
RawAudioBuffer::RawAudioBuffer(int total_size, int thres_t)
    :block_enough(true),thres(thres_t),cur_pos(0),total_size(total_size)
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
        cond_enough.wakeOne(); //emit buffering finished here
        //qDebug("queue is enough: %d/%d~%d", queue.size(), thres, cap);
    } else {
        //qDebug("buffering: %d/%d~%d", queue.size(), thres, cap);
    }
    return ret;
}

QByteArray RawAudioBuffer::take(int len, bool *isValid, unsigned long timeout_ms)
{
    if (isValid) *isValid = false;
    QWriteLocker locker(&lock);
    Q_UNUSED(locker);
    int actual_len = -1;
    if (!checkEnoughForReadSpecificData(len, actual_len)) {//TODO:always block?
        //qDebug("queue empty!!");
        if (block_enough)
            cond_enough.wait(&lock,timeout_ms); //block when empty only
    }
    if (!checkEnoughForReadSpecificData(len, actual_len)) {
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
        cond_enough.wakeAll(); //empty still wait. setBlock=>setCapacity(-1)
    }
}

void RawAudioBuffer::blockEmpty(bool block)
{
    if (!block) {
        cond_enough.wakeAll();
    }
    QWriteLocker locker(&block_change_lock);
    Q_UNUSED(locker);
    block_enough = block;
}

void RawAudioBuffer::clear()
{
    QWriteLocker locker(&lock);
    Q_UNUSED(locker);
    cond_enough.wakeAll();
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
