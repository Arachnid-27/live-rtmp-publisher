#ifndef _AAC_STREAM_H_
#define _AAC_STREAM_H_

#include <unistd.h>
#include "AACEncoder.h"
#include "AACRTMPPackager.h"
#include "AudioSource.h"
#include "PacketQueue.h"
#include "MemoryPool.h"

class AACStream {
public:
    AACStream(AudioSource& source, PacketQueue& queue, MemoryPool& pool, int bitrate = 48000);

    void run();
private:
    AudioSource &mSource;
    PacketQueue &mQueue;
    MemoryPool &mPool;
    AACEncoder mEncoder;
};

#endif
