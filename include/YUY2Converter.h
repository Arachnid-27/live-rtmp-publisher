#ifndef _YUY2_CONVERTER_H_
#define _YUY2_CONVERTER_H_

#include <cstddef>
#include "VideoFrameFilter.h"

enum PixelFormat {
    YUY2_CVT_I420 = 0,
    YUY2_CVT_RGB24
};

class YUY2Converter: public VideoFrameFilter {
public:
    YUY2Converter(PixelFormat format): mDataBuf(NULL), mFormat(format) {}

    virtual ~YUY2Converter();

    virtual char* filter(const unsigned char* data, int width, int height);
private:
    void cvtI420(const unsigned char* data, int width, int height);

    void cvtRGB24(const unsigned char* data, int width, int height);
private:
    char *mDataBuf;
    PixelFormat mFormat;
};

#endif
