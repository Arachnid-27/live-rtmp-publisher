#include "AACStream.h"

AACStream::AACStream(AudioSource* source, PacketQueue* queue, int bitrate): 
    mSource(source), mQueue(queue),
    mEncoder(source->getSampleRate(), source->getChannals(), bitrate) {}

void AACStream::buildMetadata(int size, char* data) {
    char *body = mDataBuf + RTMP_MAX_HEADER_SIZE;

    mMetadata.m_headerType = RTMP_PACKET_SIZE_LARGE;
    mMetadata.m_packetType = RTMP_PACKET_TYPE_AUDIO;
    mMetadata.m_nChannel = 0x04;
    mMetadata.m_hasAbsTimestamp = 0;
    mMetadata.m_nBodySize = size + 2;
    mMetadata.m_body = body;

    *(body++) = 0xAF;
    *(body++) = 0x00;
    memcpy(body, data, size);
}

void AACStream::run() {
    if (!mSource->isOpened()) {
        return;
    }

    char *frame;
    std::pair<int, char*> result;
    AACRTMPPackager packager;

    result = mEncoder.getMetadata();
    buildMetadata(result.first, result.second);

    mQueue->push(mMetadata);

    while ((frame = mSource->getNextFrames()) != NULL) {
        result = mEncoder.encode(mSource->getSampleNum(), frame);

        if (result.first != 0) {
            packager.setSize(result.first);
            packager.setData(result.second);
            mQueue->push(packager);
        }
    }
}
