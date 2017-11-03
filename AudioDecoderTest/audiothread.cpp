#include "audiothread.h"

AudioThread::AudioThread(QObject *parent) : QThread(parent)
{
    frame = av_frame_alloc();
}

AudioThread::~AudioThread()
{
    if (frame) {
        av_frame_free(&frame);
        frame = 0;
    }
}

bool AudioThread::isPaused() const
{
    //if d.next_pause is true, the thread will pause soon, may happens before you can handle the result
    return paused || next_pause;
}

bool AudioThread::tryPause(unsigned long timeout)
{
    if (!isPaused())
        return false;
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    return cond.wait(&mutex, timeout);
    qDebug("paused thread waked up!!!");
    return true;
}

bool AudioThread::processNextTask()
{
    if (tasks.isEmpty())
        return true;
    QRunnable *task = tasks.take();
    task->run();
    if (task->autoDelete()) {
        delete task;
    }
    return true;
}

bool AudioThread::decode(const Packet &packet)
{
    decoded.clear();
    int got_frame_ptr = 0;
    int ret = 0;
    if (packet.isEOF()) {
        AVPacket eofpkt;
        av_init_packet(&eofpkt);
        eofpkt.data = NULL;
        eofpkt.size = 0;
        ret = avcodec_decode_audio4(codec_ctx, frame, &got_frame_ptr, &eofpkt);
    } else {
    // const AVPacket*: ffmpeg >= 1.0. no libav
        ret = avcodec_decode_audio4(codec_ctx, frame, &got_frame_ptr, (AVPacket*)packet.asAVPacket());
    }
    undecoded_size = qMin(packet.data.size() - ret, packet.data.size());
    if (ret == AVERROR(EAGAIN)) {
        return false;
    }
    if (ret < 0) {
        qWarning("[AudioDecoder] %s", av_err2str(ret));
        return false;
    }
    if (!got_frame_ptr) {
        qWarning("[AudioDecoder] got_frame_ptr=false. decoded: %d, un: %d %s", ret, undecoded_size, av_err2str(ret));
        return !packet.isEOF();
    }

    return true;
}

void AudioThread::run()
{
    resetState();
    Packet pkt;
    while (!stop) {
        processNextTask();
        if (render_pts0 < 0) { // no pause when seeking
            if (tryPause()) { //DO NOT continue, or stepForward() will fail
                if (stop)
                    break; //the queue is empty and may block. should setBlocking(false) wake up cond empty?
            } else {
                if (isPaused())
                    continue;
            }
        }
        if (seek_requested) {
            seek_requested = false;
            qDebug("request seek audio thread");
            pkt = Packet(); // last decode failed and pkt is valid, reset pkt to force take the next packet if seek is requested
            msleep(1);
        }

        if (!pkt.isValid()) {
            // can't seek back if eof packet is read
            //qDebug("eof pkt: %d valid: %d, aqueue size: %d, abuffer: %d %.3f %d, fake_duration: %lld", pkt.isEOF(), pkt.isValid(), d.packets.size(), d.packets.bufferValue(), d.packets.bufferMax(), d.packets.isFull(), fake_duration);
            // If seek requested but last decode failed
            if (!pkt.isEOF() && !packets.isEmpty()) {
                pkt = packets.take(); //wait to dequeue
            }
            if (pkt.isEOF()) {
                qDebug("audio thread gets an eof packet. pkt.pts: %.3f, d.render_pts0:%.3f", pkt.pts, render_pts0);
            }
            if (!pkt.isValid()) {
                if (pkt.pts >= 0) { // check seek first
                    qDebug("Invalid packet! flush audio codec context!!!!!!!! audio queue size=%d", packets.size());
                    QMutexLocker locker(&mutex);
                    Q_UNUSED(locker);
                    if (codec_ctx) //maybe set to null in setDecoder()
                        avcodec_flush_buffers(codec_ctx);
                    render_pts0 = pkt.pts;
                    qDebug("audio seek: %.3f", render_pts0);
                    pkt = Packet(); //mark invalid to take next
                    continue;
                }
            }
        }
        if (!pkt.isValid() && !pkt.isEOF()) // decode it will cause crash
            continue;
        qreal dts = pkt.dts; //FIXME: pts and dts
        // no key frame for audio. so if pts reaches, try decode and skip render if got frame pts does not reach
        bool skip_render = pkt.pts >= 0 && pkt.pts < render_pts0; // if audio stream is too short, seeking will fail and d.render_pts0 keeps >0
        // audio has no key frame, skip rendering equals to skip decoding
        if (skip_render) {
            pkt = Packet(); //mark invalid to take next
            continue;
        }

        QMutexLocker locker(&mutex);
        Q_UNUSED(locker);

        if (stop) {
            qDebug("audio thread stop before decode()");
            break;
        }

        if (!decode(pkt)) {
            qWarning("Decode audio failed. undecoded: %d", undecoded_size);
            if (pkt.isEOF()) {
                qDebug("audio decode eof done");
                Q_EMIT eofDecoded();
                if (render_pts0 >= 0) {
                    qDebug("audio seek done at eof pts: %.3f", pkt.pts);
                    render_pts0 = -1;
                    Q_EMIT seekFinished(qint64(pkt.pts*1000.0)); //TODO: pts
                }
                if (!pkt.position)
                    break;
            }
            pkt = Packet();
            continue;
        }
        // reduce here to ensure to decode the rest data in the next loop
        if (!pkt.isEOF())
            pkt.skip(pkt.data.size() - dec->undecodedSize());

        AudioFrame frame(dec->frame());
        if (!frame)
            continue; //pkt data is updated after decode, no reset here
        if (frame.timestamp() <= 0)
            frame.setTimestamp(pkt.pts); // pkt.pts is wrong. >= real timestamp
        if (d.render_pts0 >= 0.0) { // seeking
            d.clock->updateValue(frame.timestamp());
            if (frame.timestamp() < d.render_pts0) {
                qDebug("skip audio rendering: %f-%f", frame.timestamp(), d.render_pts0);
                continue; //pkt data is updated after decode, no reset here
            }
            qDebug("audio seek finished @%.3f. id: %d", frame.timestamp(), sync_id);
            d.render_pts0 = -1.0;
            d.clock->syncEndOnce(sync_id);
            Q_EMIT seekFinished(qint64(frame.timestamp()*1000.0));
            if (has_ao) {
                ao->clear();
            }
        }
        if (has_ao) {
            applyFilters(frame);
            frame.setAudioResampler(dec->resampler()); //!!!
            // FIXME: resample ONCE is required for audio frames from ffmpeg
            //if (ao->audioFormat() != frame.format()) {
            frame = frame.to(ao->audioFormat());
            //}
        }
        QByteArray decoded(frame.data());

        int decodedSize = decoded.size();
        int decodedPos = 0;
        qreal delay = 0;
        const qreal byte_rate = frame.format().bytesPerSecond();
        qreal pts = frame.timestamp();
        //qDebug("frame samples: %d @%.3f+%lld", frame.samplesPerChannel()*frame.channelCount(), frame.timestamp(), frame.duration()/1000LL);
        while (decodedSize > 0) {
            if (d.stop) {
                qDebug("audio thread stop after decode()");
                break;
            }
            const int chunk = qMin(decodedSize, has_ao ? ao->bufferSize() : 512*frame.format().bytesPerFrame());//int(max_len*byte_rate));
            //AudioFormat.bytesForDuration
            const qreal chunk_delay = (qreal)chunk/(qreal)byte_rate;
            if (has_ao && ao->isOpen()) {
                QByteArray decodedChunk = QByteArray::fromRawData(decoded.constData() + decodedPos, chunk);
                //qDebug("ao.timestamp: %.3f, pts: %.3f, pktpts: %.3f", ao->timestamp(), pts, pkt.pts);
                ao->play(decodedChunk, pts);
                if (!is_external_clock && ao->timestamp() > 0) {//TODO: clear ao buffer
                    // const qreal da = qAbs(pts - ao->timestamp());
                    // if (da > 1.0) { // what if frame duration is long?
                    // }
                    // TODO: check seek_requested(atomic bool)
                    d.clock->updateValue(ao->timestamp());
                }
            } else {
                d.clock->updateDelay(delay += chunk_delay);
                /*
             * why need this even if we add delay? and usleep sounds weird
             * the advantage is if no audio device, the play speed is ok too
             * So is portaudio blocking the thread when playing?
             */
                //TODO: avoid acummulative error. External clock?
                msleep((unsigned long)(chunk_delay * 1000.0));
            }
            decodedPos += chunk;
            decodedSize -= chunk;
            pts += chunk_delay;
            pkt.pts += chunk_delay; // packet not fully decoded, use new pts in the next decoding
            pkt.dts += chunk_delay;
        }
        if (has_ao)
            emit frameDelivered();
        d.last_pts = d.clock->value(); //not pkt.pts! the delay is updated!
    }
}

void AudioThread::resetState()
{
    pause(false);
    tasks.clear();
    render_pts0 = -1;
    stop = false;
    packets.setBlocking(true);
    packets.clear();
}

void AudioThread::pause(bool p)
{
    if (paused == p)
        return;
    paused = p;
    if (!paused) {
        qDebug("wake up paused thread");
        next_pause = false;
        cond.wakeAll();
    }
}
