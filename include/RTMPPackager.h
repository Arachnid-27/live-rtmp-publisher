#ifndef _RTMP_PACKAGER_H_
#define _RTMP_PACKAGER_H_

#include <librtmp/rtmp.h>

class RTMPPackager {
public:
    virtual RTMPPacket pack(char* buf, const char* data, int length) const = 0;

    virtual RTMPPacket metadata(char* buf, const char* data, int length) const = 0;

    virtual int getBodyLength(int length) const = 0;
};

#endif
