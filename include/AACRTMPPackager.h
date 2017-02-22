#ifndef _AAC_RTMP_PACKAGER_H_
#define _AAC_RTMP_PACKAGER_H_

#include <cstring>
#include "RTMPPackager.h"

class AACRTMPPackager: public RTMPPackager {
public:
    AACRTMPPackager() {}

    AACRTMPPackager(int size, const char* data): RTMPPackager(size, data) {}

    virtual RTMPPacket pack(char* buf);
};

#endif
