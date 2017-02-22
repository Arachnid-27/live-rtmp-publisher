#include "MotionDetector.h"

void MotionDetector::filter(cv::Mat& frame) {
    cv::Mat fg;
    PointArray contours;

    mSubtractor->apply(frame, fg);

//    cv::erode(fg, fg, cv::Mat());
//    cv::dilate(fg, fg, cv::Mat());
    cv::findContours(fg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    drawRect(contours, frame, 1000);
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
