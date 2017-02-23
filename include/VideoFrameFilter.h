#ifndef _FRAME_FILTER_FILTER_H_
#define _FRAME_FILTER_FILTER_H_

class VideoFrameFilter {
public:
    virtual ~VideoFrameFilter() {}

    virtual char* filter(const unsigned char* data, int width, int height) = 0;
};

#endif
