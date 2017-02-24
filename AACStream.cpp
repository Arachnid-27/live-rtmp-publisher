#include "AACStream.h"

AACStream::AACStream(AudioSource& source, PacketQueue& queue, int bitrate): 
    mSource(source), mQueue(queue),
    mEncoder(source.getSampleRate(), source.getChannals(), bitrate) {}

void AACStream::run() {
    if (!mSource.isOpened()) {
        return;
    }

    std::pair<int, char*> frame;
    std::pair<int, char*> result = mEncoder.getMetadata();
    AACRTMPPackager packager(result.first, result.second);

    mMetadata = packager.metadata(mDataBuf);
    mQueue.push(mMetadata);

    while ((frame = mSource.getNextFrames()).second != NULL) {
        result = mEncoder.encode(frame.first, frame.second);

        if (result.first != 0) {
            packager.setData(result.first, result.second);
            mQueue.push(packager);
        }
    }
}
