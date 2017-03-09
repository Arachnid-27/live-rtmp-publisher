#ifndef _AAC_ENCODER_H_
#define _AAC_ENCODER_H_

#include <cstring>
#include <utility>
#include "faac.h"

class AACEncoder {
public:
    AACEncoder(int sampleRate, int channals, int bitrate);

    ~AACEncoder();

    int getMaxSample() const { return mMaxSample; }

    std::pair<int, char*> getMetadata();
    
    std::pair<int, char*> encode(int sample, char* data);
private:
    faacEncHandle mEncoder;
    unsigned long mMaxSample;
    unsigned long mBufLength;
    char *mDataBuf;
};

#endif
