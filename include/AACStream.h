#ifndef _AAC_STREAM_H_
#define _AAC_STREAM_H_

#include <unistd.h>
#include "AACEncoder.h"
#include "AACRTMPPackager.h"
#include "AudioSource.h"
#include "PacketQueue.h"

class AACStream {
public:
    AACStream(AudioSource* source, PacketQueue* queue, int bitrate = 48000);

    void run();
private:
    void buildMetadata(int size, char* data);
private:
    AudioSource *mSource;
    PacketQueue *mQueue;
    AACEncoder mEncoder;
    RTMPPacket mMetadata;
    char mDataBuf[32];
};

#endif
