#include "H264Stream.h"
#include <iostream>

H264Stream::H264Stream(FilteredVideoSource& source, PacketQueue& queue, MemoryPool& pool, int fps, int bitrate): 
    mInterval(1000 / fps), mSource(source), mQueue(queue), mPool(pool),
    mEncoder(source.getWidth(), source.getHeight(), fps, bitrate) {}

void H264Stream::run() {
    if (!mSource.isOpened()) {
        std::cout << "video device is not open" << std::endl;
        return;
    }

    char *buf;
    RTMPPacket packet;
    H264RTMPPackager packager;
    std::chrono::milliseconds duration;
    char *frame;
    auto result = mEncoder.getMetadata();

    mMetadata = packager.metadata(mDataBuf, result.second, result.first);
    mSource.getNextFrame(); // warm-up

    auto last = std::chrono::system_clock::now();

    while ((frame = mSource.getNextFrame()) != NULL) {
        result = mEncoder.encode(frame);
    
        if (H264RTMPPackager::isKeyFrame(result.second)) {
            mQueue.push(mMetadata, true);
        }
        buf = mPool.getChunk(packager.getBodyLength(result.first));
        packet = packager.pack(buf, result.second, result.first);
        mQueue.push(packet);

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last);

        if (duration < mInterval) {
            msleep((mInterval - duration).count());
        }
        last = std::chrono::system_clock::now();
    }
}
