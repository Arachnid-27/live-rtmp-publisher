#include "AACStream.h"
#include <iostream>

AACStream::AACStream(AudioSource& source, PacketQueue& queue, MemoryPool& pool, int bitrate): 
    mSource(source), mQueue(queue), mPool(pool),
    mEncoder(source.getSampleRate(), source.getChannals(), bitrate) {}

void AACStream::run() {
    if (!mSource.isOpened()) {
        std::cout << "audio device is not open" << std::endl;
        return;
    }

    mSource.setMaxSample(mEncoder.getMaxSample());

    char *buf;
    RTMPPacket packet;
    AACRTMPPackager packager;
    std::pair<int, char*> frame;
    std::pair<int, char*> result = mEncoder.getMetadata();

    buf = mPool.getChunk(256);
    packet = packager.metadata(buf, result.second, result.first);
    mQueue.push(packet);

    while ((frame = mSource.getNextFrames()).second != NULL) {
        result = mEncoder.encode(frame.first, frame.second);

        if (result.first != 0) {
            buf = mPool.getChunk(packager.getBodyLength(result.first));
            packet = packager.pack(buf, result.second, result.first);
            mQueue.push(packet);
        }
    }
}
