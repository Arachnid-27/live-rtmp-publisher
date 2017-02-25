#include "YUY2Converter.h"

YUY2Converter::~YUY2Converter() {
    if (mDataBuf) {
        delete[] mDataBuf;
    }
}

char* YUY2Converter::filter(const unsigned char* data, int width, int height) {
    switch(mFormat) {
        case YUY2_CVT_I420:
            cvtI420(data, width, height);
            break;
        case YUY2_CVT_RGB24:
            cvtRGB24(data, width, height);
            break;
    }

    return mDataBuf;
}

void YUY2Converter::cvtI420(const unsigned char* data, int width, int height) {
    if (!mDataBuf) {
        mDataBuf = new char[width * height * 3 / 2];
    }

    const int s1 = width * height * 2;
    const int s2 = width * 2;

    char *pY = mDataBuf;
    char *pU = pY + width * height;
    char *pV = pU + (width * height / 4); 
    bool uFlags = true;

    for (int i = 0; i < s1; ++i) {
        if (i % 2 == 0) {
            *(pY++) = data[i];
        } else if ((i / s2) % 2 == 0) {
            if (uFlags) {
                *(pU++) = data[i];
            } else {
                *(pV++) = data[i];
            }
            uFlags = !uFlags;
        }
    }
}

void YUY2Converter::cvtRGB24(const unsigned char* data, int width, int height) {
    if (!mDataBuf) {
        mDataBuf = new char[width * height * 3];
    }

    const int s1 = width * height * 2;
    char *p = mDataBuf;
    int y1, y2, u, v;

    for (int i = 0; i < s1; i += 4) {
        y1 = *(data++);
        u = *(data++);
        y2 = *(data++);
        v = *(data++);

        *(p++) = 1.164 * (y1 - 16) + 1.596 * (v - 128);
        *(p++) = 1.164 * (y1 - 16) - 0.391 * (u - 128) - 0.813 * (v - 128);
        *(p++) = 1.164 * (y1 - 16) + 2.018 * (u - 128);
        *(p++) = 1.164 * (y2 - 16) + 1.596 * (v - 128);
        *(p++) = 1.164 * (y2 - 16) - 0.391 * (u - 128) - 0.813 * (v - 128);
        *(p++) = 1.164 * (y2 - 16) + 2.018 * (u - 128);
    }
}
