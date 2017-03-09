#ifndef _MOTION_DETECTOR_H_
#define _MOTION_DETECTOR_H_

#include "VideoFrameFilter.h"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

class MotionDetector: public VideoFrameFilter {
public:
    using PointArray = std::vector<std::vector<cv::Point>>;

    MotionDetector(int history = 500, double threshold = 32): 
        mSubtractor(cv::createBackgroundSubtractorMOG2(history, threshold, false)), 
        mScalar(cv::Scalar(0, 255, 0)) {}

    virtual ~MotionDetector() {}

    virtual char* filter(const unsigned char* data, int width, int height);
private:
    void drawRect(const PointArray& contours, cv::Mat& frame, double threshold);
private:
    cv::Ptr<cv::BackgroundSubtractor> mSubtractor;
    cv::Scalar mScalar;
    cv::Mat mYuv;
    cv::Mat mRgb;
};

#endif
