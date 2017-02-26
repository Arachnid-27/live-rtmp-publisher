#ifndef _PACKET_QUEUE_H_
#define _PACKET_QUEUE_H_

#include <cstring>
#include <mutex>
#include <condition_variable>
#include <librtmp/rtmp.h>
#include "RTMPPackager.h"

// single consumer !!!

struct PacketNode {
    RTMPPacket packet;
    bool metadata;
};

class PacketQueue {
public:
    PacketQueue(int capacity = 100);

    ~PacketQueue();

    void push(const RTMPPacket& packet, bool metadata = false);

    RTMPPacket& front();

    bool pop();
private:
    PacketNode *mDataBuf;
    std::mutex mMutex;
    std::condition_variable mFull;
    std::condition_variable mEmpty;
    int mHead;
    int mTail;
    int mCapacity;
};

#endif
