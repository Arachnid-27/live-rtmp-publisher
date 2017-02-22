#ifndef _CAMERA_SOURCE_H_
#define _CAMERA_SOURCE_H_

#include <opencv2/opencv.hpp>
#include "MotionDetector.h"
#include "VideoSource.h"

class CameraSource: public VideoSource {
public:
    CameraSource(int index, int width, int height);

    ~CameraSource();

    virtual int getWidth() const { return mWidth; }

    virtual int getHeight() const { return mHeight; }

    virtual bool isOpened() const { return mCamera.isOpened(); }

    virtual char* getNextFrame();
protected:
    int mWidth;
    int mHeight;
    cv::VideoCapture mCamera;
    cv::Mat mFrame;
    cv::Mat mYuv;
    MotionDetector mDetector;
};

#endif
