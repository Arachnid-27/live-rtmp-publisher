#ifndef _V4L2_SOURCE_H_
#define _V4L2_SOURCE_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <cstring>
#include <iostream>
#include "VideoSource.h"

struct __QBuf {
    char *data;
    size_t length;
};

class V4L2Source: public VideoSource {
public:
    V4L2Source(int width, int height, const char* name = "/dev/video0", 
            char a = 'Y', char b = 'U', char c = 'Y', char d = 'V');

    virtual ~V4L2Source();

    virtual int getWidth() const { return mWidth; }

    virtual int getHeight() const { return mHeight; }

    virtual bool isOpened() const { return mDeviceFd != -1; }

    virtual char* getNextFrame();
private:
    bool checkCapability();
    bool checkFormat(int width, int height, char a, char b, char c, char d);
    bool initBuffer();
    void munmapBuffer(int end);
private:
    int mWidth;
    int mHeight;
    int mDeviceFd;
    int mBufCount;
    bool mIsCaptrue;
    __QBuf mQueueBuf[4];
    v4l2_buffer mDataBuf;
};

#endif
