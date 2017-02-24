#include "CvVideoSource.h"

CvVideoSource::CvVideoSource(int index, int width, int height): mWidth(width), mHeight(height), mVideo(cv::VideoCapture(index)) {
    if (mVideo.isOpened()) {
        mVideo.set(cv::CAP_PROP_FRAME_WIDTH, width);
        mVideo.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    }
}

CvVideoSource::~CvVideoSource() {
    if (mVideo.isOpened()) {
        mVideo.release();
    }
}

char* CvVideoSource::getNextFrame() {
    if (!mVideo.read(mFrame)) {
        return NULL;
    }
    return reinterpret_cast<char*>(mFrame.data);
}
