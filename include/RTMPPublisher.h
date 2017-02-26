#ifndef _RTMP_PUBLISHER_H_
#define _RTMP_PUBLISHER_H_

#include <librtmp/rtmp.h>
#include "PacketQueue.h"
#include "MemoryPool.h"

class RTMPPublisher {
public:
    RTMPPublisher(PacketQueue& queue, MemoryPool& pool);

    ~RTMPPublisher();

    bool connect(char* url); 

    void run();
private:
    PacketQueue &mQueue;
    MemoryPool &mPool;
    RTMP *mRTMP;
};

#endif
