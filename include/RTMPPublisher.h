#ifndef _RTMP_PUBLISHER_H_
#define _RTMP_PUBLISHER_H_

#include <librtmp/rtmp.h>
#include "PacketQueue.h"

class RTMPPublisher {
public:
    RTMPPublisher(PacketQueue* queue): mQueue(queue), mRTMP(RTMP_Alloc()) {
        RTMP_Init(mRTMP);
    }

    ~RTMPPublisher();

    bool connect(char* url); 

    void run();
private:
    PacketQueue *mQueue;
    RTMP *mRTMP;
};

#endif
