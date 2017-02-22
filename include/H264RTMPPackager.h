#ifndef _H264_RTMP_Packager_H_
#define _H264_RTMP_Packager_H_

#include <cstring>
#include "RTMPPackager.h"

class H264RTMPPackager: public RTMPPackager {
public:
    H264RTMPPackager() {}
    
    H264RTMPPackager(int size, const char* data): RTMPPackager(size, data) {}

    virtual RTMPPacket pack(char* buf);
};

#endif
