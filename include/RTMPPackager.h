#ifndef _RTMP_PACKAGER_H_
#define _RTMP_PACKAGER_H_

#include <librtmp/rtmp.h>

class RTMPPackager {
public:
    RTMPPackager() {}

    RTMPPackager(int length, const char* data): mLength(length), mData(data) {}

    void setData(int length, const char* data) {
        mLength = length;
        mData = data;
    }

    virtual int getBodyLength() const = 0;

    virtual RTMPPacket pack(char* buf) = 0;

    virtual RTMPPacket metadata(char* buf) = 0;
protected:
    int mLength;
    const char *mData;
};

#endif
