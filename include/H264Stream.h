#ifndef _H264_STREAM_H_
#define _H264_STREAM_H_

#include <unistd.h>
#include <chrono>
#include "H264Encoder.h"
#include "H264RTMPPackager.h"
#include "VideoSource.h"
#include "PacketQueue.h"

#define msleep(x) usleep((x) * 1000)

class H264Stream {
public:
    H264Stream(VideoSource* source, PacketQueue* queue, int fps, int bitrate);

    void run();
private:
    void buildMetadata(x264_nal_t* sps, x264_nal_t* pps);
private:
    std::chrono::milliseconds mInterval;
    VideoSource *mSource;
    PacketQueue *mQueue;
    H264Encoder mEncoder;
    RTMPPacket mMetadata;
    char mDataBuf[256];
};

#endif
