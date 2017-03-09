#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

#include <utility>

class AudioSource {
public:
    virtual ~AudioSource() {}

    virtual void setMaxSample(int sample) = 0;

    virtual int getSampleRate() const = 0;

    virtual int getChannals() const = 0;

    virtual bool isOpened() const = 0;

    // return the number of sample and data
    virtual std::pair<int, char*> getNextFrames() = 0;
};

#endif
