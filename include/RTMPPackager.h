#ifndef _RTMP_PACKAGER_H_
#define _RTMP_PACKAGER_H_

#include <librtmp/rtmp.h>

class RTMPPackager {
public:
    RTMPPackager() {}

    RTMPPackager(int size, const char* data): mSize(size), mData(data) {}

    void setSize(int size) { mSize = size; };

    void setData(const char* data) { mData = data; }

    virtual RTMPPacket pack(char* buf) = 0;
protected:
    int mSize;
    const char *mData;
};

#endif
