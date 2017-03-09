#include "YUY2Converter.h"

YUY2Converter::YUY2Converter(PixelFormat format): mDataBuf(NULL), mFormat(format) {
    if (format == YUY2_CVT_RGB24) {
        createTable();
    }
}

YUY2Converter::~YUY2Converter() {
    if (mFormat == YUY2_CVT_RGB24) {
        delete[] mYV;
        delete[] mYU;
        delete[] mUV;
        delete[] mYX;
    }
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

        *(p++) = mYV[INDEX(y1, v)];
        *(p++) = mYX[INDEX(y1, mUV[INDEX(u, v)])];
        *(p++) = mYU[INDEX(y1, u)];
        *(p++) = mYV[INDEX(y2, v)];
        *(p++) = mYX[INDEX(y2, mUV[INDEX(u, v)])];
        *(p++) = mYU[INDEX(y2, u)];
    }
}

void YUY2Converter::createTable() {
    mYV = new char[65536];
    mYU = new char[65536];
    mUV = new char[65536];
    mYX = new char[65536];

    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            mYV[INDEX(i, j)] = round(1.164 * i + 1.596 * j - 222.912);
            mYU[INDEX(i, j)] = round(1.164 * i + 2.018 * j - 276.928);
            mUV[INDEX(i, j)] = round(0.391 * i + 0.813 * j - 28.416);
            mYX[INDEX(i, j)] = round(1.164 * i - j + 109.376);
        }
    }
}

unsigned char YUY2Converter::round(float value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}
