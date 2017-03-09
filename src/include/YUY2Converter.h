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
    YUY2Converter(PixelFormat format);

    virtual ~YUY2Converter();

    virtual char* filter(const unsigned char* data, int width, int height);
private:
    void cvtI420(const unsigned char* data, int width, int height);

    void cvtRGB24(const unsigned char* data, int width, int height);

    void createTable();

    unsigned char round(float value);

    int INDEX(int x, int y) const { return x * 256 + y; }
private:
    char *mDataBuf;
    char *mYV, *mYU, *mUV, *mYX;
    PixelFormat mFormat;
};

#endif
