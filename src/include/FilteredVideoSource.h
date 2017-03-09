#ifndef _FILTERED_VIDEO_SOURCE_H_
#define _FILTERED_VIDEO_SOURCE_H_

#include <vector>
#include <cstddef>
#include "VideoSource.h"
#include "VideoFrameFilter.h"

class FilteredVideoSource {
public:
    FilteredVideoSource(VideoSource& source): mSource(source) {}

    void addFilter(VideoFrameFilter* filter) { mFilters.push_back(filter); }

    char* getNextFrame();

    int getWidth() { return mSource.getWidth(); }

    int getHeight() { return mSource.getHeight(); }

    bool isOpened() { return mSource.isOpened(); }
private:
    VideoSource &mSource;
    std::vector<VideoFrameFilter*> mFilters;
};

#endif
