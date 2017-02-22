#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

class AudioSource {
public:
    virtual ~AudioSource() {}

    virtual int getSampleRate() const = 0;

    virtual int getChannals() const = 0;

    virtual bool isOpened() const = 0;

    virtual int getSampleNum() const = 0;

    virtual char* getNextFrames() = 0;
};

#endif
