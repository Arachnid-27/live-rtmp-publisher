#include "PacketQueue.h"

PacketQueue::PacketQueue(int capacity): mCapacity(capacity) {
    mDataBuf = new PacketNode[capacity];
    memset(mDataBuf, 0, capacity * sizeof(PacketNode));
    mHead = mTail = 0;
}

PacketQueue::~PacketQueue() {
    delete[] mDataBuf;
}

void PacketQueue::push(const RTMPPacket& packet, bool metadata) {
    std::unique_lock<std::mutex> lock(mMutex);

    while ((mTail + 1) % mCapacity == mHead) {
        mFull.wait(lock);
    }

    mDataBuf[mTail].packet = packet;
    mDataBuf[mTail].metadata = metadata;

    if (++mTail == mCapacity) {
        mTail = 0;
    }
    mEmpty.notify_all();

    lock.unlock();
}

RTMPPacket& PacketQueue::front() {
    std::unique_lock<std::mutex> lock(mMutex);

    while (mHead == mTail) {
        mEmpty.wait(lock);
    }

    lock.unlock();

    return mDataBuf[mHead].packet;
}

bool PacketQueue::pop() {
    bool metadata = mDataBuf[mHead].metadata;
    std::lock_guard<std::mutex> lock(mMutex);

    if (++mHead == mCapacity) {
        mHead = 0;
    }
    mFull.notify_all();

    return metadata;
}
