#include <iostream>
#include <thread>
#include "RTMPPublisher.h"
#include "CameraSource.h"
#include "H264Stream.h"
#include "AACStream.h"
#include "PCMSource.h"

int main(int argc, char *argv[]) {
    CameraSource videoSource(0, 320, 240);
    PCMSource audioSource;
    PacketQueue queue;
    H264Stream videoStream(&videoSource, &queue, 16, 512);
    AACStream audioStream(&audioSource, &queue);
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

    std::thread VideoEncodeThread(&H264Stream::run, &videoStream);
    std::thread AudioEncodeThread(&AACStream::run, &audioStream);

    publisher.run();

    return 0;
}
