#ifndef _PCM_SOURCE_
#define _PCM_SOURCE_

#include "AudioSource.h"
#include <alsa/asoundlib.h>

class PCMSource: public AudioSource {
public:
    PCMSource(const char* name = "default", int sampleRate = 44100, int channals = 2, int period = 1024);

    virtual ~PCMSource();

    virtual void setMaxSample(int sample);

    virtual int getSampleRate() const { return mSampleRate; }

    virtual int getChannals() const { return mChannals; }

    virtual bool isOpened() const { return mIsOpened; }

    virtual std::pair<int, char*> getNextFrames();
private:
    unsigned int mSampleRate;
    int mChannals;
    bool mIsOpened;
    char *mDataBuf;
    snd_pcm_t *mHandle;
    snd_pcm_uframes_t mPeriod;
};

#endif
