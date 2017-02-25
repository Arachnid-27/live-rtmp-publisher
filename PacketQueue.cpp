#include "PacketQueue.h"

PacketQueue::PacketQueue(int capacity): mCapacity(capacity) {
    mDataBuf = new PacketNode[capacity];
    memset(mDataBuf, 0, capacity * sizeof(PacketNode));
    mHead = mTail = 0;
}

PacketQueue::~PacketQueue() {
    delete[] mDataBuf;
}

void PacketQueue::push(RTMPPackager& packager) {
    std::unique_lock<std::mutex> lock(mMutex);

    while ((mTail + 1) % mCapacity == mHead) {
        mFull.wait(lock);
    }

    mDataBuf[mTail].buf = mPool.getChunk(packager.getBodyLength() + RTMP_MAX_HEADER_SIZE);
    mDataBuf[mTail].packet = packager.pack(mDataBuf[mTail].buf);

    if (++mTail == mCapacity) {
        mTail = 0;
    }
    mEmpty.notify_all();

    lock.unlock();
}

void PacketQueue::push(const RTMPPacket& packet) {
    std::unique_lock<std::mutex> lock(mMutex);

    while ((mTail + 1) % mCapacity == mHead) {
        mFull.wait(lock);
    }

    mDataBuf[mTail].buf = NULL;
    mDataBuf[mTail].packet = packet;

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

void PacketQueue::pop() {
    std::lock_guard<std::mutex> lock(mMutex);

    if (mHead != mTail) {
        if (mDataBuf[mHead].buf) {
            mPool.putChunk(mDataBuf[mHead].packet.m_nBodySize + RTMP_MAX_HEADER_SIZE, mDataBuf[mHead].buf);
            mDataBuf[mHead].buf = NULL;
        }
        if (++mHead == mCapacity) {
            mHead = 0;
        }
        mFull.notify_all();
    }
}
