#include "MotionDetector.h"

char* MotionDetector::filter(const unsigned char* data, int width, int height) {
    if (mRgb.empty()) {
        mRgb.create(height, width, CV_8UC3);
    }

    cv::Mat fg;
    PointArray contours;

    memcpy(mRgb.data, data, height * width * 3);
    mSubtractor->apply(mRgb, fg);

//    cv::erode(fg, fg, cv::Mat());
//    cv::dilate(fg, fg, cv::Mat());
    cv::findContours(fg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    drawRect(contours, mRgb, 3000);
    cv::cvtColor(mRgb, mYuv, CV_RGB2YUV_I420);

    return reinterpret_cast<char*>(mYuv.data);
}

void MotionDetector::drawRect(const PointArray& contours, cv::Mat& frame, double threshold) {
    for (auto contour : contours) {
        double area = cv::contourArea(contour);
        if (area > threshold) {
            cv::Rect rect = cv::boundingRect(contour);
            cv::rectangle(frame, rect, mScalar);
        }
    }
}
