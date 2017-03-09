#ifndef _CV_VIDEO_SOURCE_H_
#define _CV_VIDEO_SOURCE_H_

#include <opencv2/opencv.hpp>
#include "VideoSource.h"

class CvVideoSource: public VideoSource {
public:
    CvVideoSource(int index, int width, int height);

    virtual ~CvVideoSource();

    virtual int getWidth() const { return mWidth; }

    virtual int getHeight() const { return mHeight; }

    virtual bool isOpened() const { return mVideo.isOpened(); }

    virtual char* getNextFrame();
protected:
    int mWidth;
    int mHeight;
    cv::VideoCapture mVideo;
    cv::Mat mFrame;
};

#endif
