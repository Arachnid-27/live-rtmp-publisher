#ifndef _H264_RTMP_Packager_H_
#define _H264_RTMP_Packager_H_

#include <cstring>
#include "RTMPPackager.h"

class H264RTMPPackager: public RTMPPackager {
public:
    H264RTMPPackager() {}
    
    H264RTMPPackager(int length, const char* data): RTMPPackager(length, data) {}

    virtual int getBodyLength() const { return mLength + 5; }

    virtual RTMPPacket pack(char* buf);

    virtual RTMPPacket metadata(char* buf);
};

#endif
