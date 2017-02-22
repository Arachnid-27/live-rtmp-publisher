#include "PacketQueue.h"

PacketQueue::PacketQueue(int capacity, int size): mCapacity(capacity) {
    mDataBuf = new PacketNode[capacity];
    for (int i = 0; i != capacity; ++i) {
        mDataBuf[i].buf = new char[size];
    }
    mHead = mTail = 0;
}

PacketQueue::~PacketQueue() {
    for (int i = 0; i != mCapacity; ++i) {
        delete[] mDataBuf[i].buf;
    }
    delete[] mDataBuf;
}

void PacketQueue::push(RTMPPackager& packager) {
    std::unique_lock<std::mutex> lock(mMutex);

    while ((mTail + 1) % mCapacity == mHead) {
        mFull.wait(lock);
    }

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
        if (++mHead == mCapacity) {
            mHead = 0;
        }
        mFull.notify_all();
    }
}
