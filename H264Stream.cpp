#include "H264Stream.h"

H264Stream::H264Stream(FilteredVideoSource& source, PacketQueue& queue, int fps, int bitrate): 
    mInterval(1000 / fps), mSource(source), mQueue(queue), 
    mEncoder(source.getWidth(), source.getHeight(), fps, bitrate) {}

void H264Stream::run() {
    if (!mSource.isOpened()) {
        return;
    }

    std::chrono::milliseconds duration;
    char *frame;
    auto result = mEncoder.getMetadata();
    H264RTMPPackager packager(result.first, result.second);

    mMetadata = packager.metadata(mDataBuf);
    mSource.getNextFrame(); // warm-up

    auto last = std::chrono::system_clock::now();

    while ((frame = mSource.getNextFrame()) != NULL) {
        result = mEncoder.encode(frame);
        packager.setData(result.first, result.second);

        if ((result.second[4] & 0x1f) == 0x05) { // key frame
            mQueue.push(mMetadata);
        }
        mQueue.push(packager);

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last);

        if (duration < mInterval) {
            msleep((mInterval - duration).count());
        }
        last = std::chrono::system_clock::now();
    }
}
