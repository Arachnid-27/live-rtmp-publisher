#ifndef _FRAME_FILTER_H_
#define _FRAME_FILTER_H_

class FrameFilter {
public:
    virtual void filter(char* output, const char* intput) = 0;
};

#endif
