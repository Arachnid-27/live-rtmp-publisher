#include <iostream>
#include <thread>
#include "RTMPPublisher.h"
#include "V4L2Source.h"
#include "H264Stream.h"
#include "AACStream.h"
#include "PCMSource.h"
#include "FilteredVideoSource.h"
#include "YUY2Converter.h"
#include "MotionDetector.h"

int main(int argc, char *argv[]) {
    PacketQueue queue;
    V4L2Source v4l2(320, 240);
    PCMSource pcm;

    YUY2Converter converter(YUY2_CVT_I420);
//    YUY2Converter converter(YUY2_CVT_RGB24);
//    MotionDetector detector;
    FilteredVideoSource videoSource(&v4l2);
    videoSource.addFilter(&converter);
//    videoSource.addFilter(&detector);

    RTMPPublisher publisher(&queue);
    char url[] = "rtmp://127.0.0.1/live/test";

    if (!videoSource.isOpened()) {
        std::cout << "open camera error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!publisher.connect(url)) {
        std::cout << "connect server error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    H264Stream videoStream(&videoSource, &queue, 16, 512);
    AACStream audioStream(&pcm, &queue);

    std::thread VideoEncodeThread(&H264Stream::run, &videoStream);
    std::thread AudioEncodeThread(&AACStream::run, &audioStream);

    publisher.run();

    return 0;
}
