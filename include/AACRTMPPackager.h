#ifndef _AAC_RTMP_PACKAGER_H_
#define _AAC_RTMP_PACKAGER_H_

#include <cstring>
#include "RTMPPackager.h"

class AACRTMPPackager: public RTMPPackager {
public:
    AACRTMPPackager() {}

    AACRTMPPackager(int length, const char* data): RTMPPackager(length, data) {}

    virtual RTMPPacket pack(char* buf);

    virtual RTMPPacket metadata(char* buf);
};

#endif
