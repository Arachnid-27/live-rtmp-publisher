#ifndef _VIDEO_SOURCE_H_
#define _VIDEO_SOURCE_H_

class VideoSource {
public:
    VideoSource(int width, int height): mWidth(width), mHeight(height) {}

    virtual ~VideoSource() {}

    int getWidth() { return mWidth; }

    int getHeight() { return mHeight; }

    virtual char* getNextFrame() = 0;
private:
    int mWidth;
    int mHeight;
};

#endif
