#ifndef _AAC_RTMP_PACKAGER_H_
#define _AAC_RTMP_PACKAGER_H_

#include <cstring>
#include "RTMPPackager.h"

class AACRTMPPackager: public RTMPPackager {
public:
    virtual RTMPPacket pack(char* buf, const char* data, int length) const;

    virtual RTMPPacket metadata(char* buf, const char* data, int length) const;

    virtual int getBodyLength(int length) const { return length - 5 + RTMP_MAX_HEADER_SIZE; }
};

#endif
