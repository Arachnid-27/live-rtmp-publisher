#ifndef _VIDEO_SOURCE_H_
#define _VIDEO_SOURCE_H_

class VideoSource {
public:
    virtual ~VideoSource() {}

    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;

    virtual bool isOpened() const = 0;

    virtual char* getNextFrame() = 0;
};

#endif
