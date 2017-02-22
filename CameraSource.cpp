#include "CameraSource.h"

CameraSource::CameraSource(int index, int width, int height): mWidth(width), mHeight(height), mCamera(cv::VideoCapture(index)) {
    if (mCamera.isOpened()) {
        mCamera.set(cv::CAP_PROP_FRAME_WIDTH, width);
        mCamera.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    }
}

CameraSource::~CameraSource() {
    if (mCamera.isOpened()) {
        mCamera.release();
    }
}

char* CameraSource::getNextFrame() {
    if (!mCamera.read(mFrame)) {
        return NULL;
    }
    //mDetector.filter(mFrame);
    cv::cvtColor(mFrame, mYuv, CV_RGB2YUV_I420);
    return reinterpret_cast<char*>(mYuv.data);
}
