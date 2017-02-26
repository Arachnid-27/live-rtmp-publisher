#include "AACStream.h"

AACStream::AACStream(AudioSource& source, PacketQueue& queue, MemoryPool& pool, int bitrate): 
    mSource(source), mQueue(queue), mPool(pool),
    mEncoder(source.getSampleRate(), source.getChannals(), bitrate) {}

void AACStream::run() {
    if (!mSource.isOpened()) {
        return;
    }

    mSource.setMaxSample(mEncoder.getMaxSample());

    AACRTMPPackager packager;
    std::pair<int, char*> frame;
    std::pair<int, char*> result = mEncoder.getMetadata();

    mMetadata = packager.metadata(mDataBuf, result.second, result.first);
    mQueue.push(mMetadata, true);

    while ((frame = mSource.getNextFrames()).second != NULL) {
        result = mEncoder.encode(frame.first, frame.second);

        if (result.first != 0) {
            mQueue.push(packager.pack(mPool.getChunk(packager.getBodyLength(result.first)), result.second, result.first));
        }
    }
}
