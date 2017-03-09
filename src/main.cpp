#include <iostream>
#include <thread>
#include "RTMPPublisher.h"
#include "V4L2Source.h"
#include "H264Stream.h"
#include "AACStream.h"
#include "PCMSource.h"
#include "FilteredVideoSource.h"
#include "YUY2Converter.h"
#include "config.h"
#ifdef OPENCV_FOUND
#include "MotionDetector.h"
#endif

void showUsage() {
    std::cout << "usage: publisher [-w width] [-h height] [-f fps] [-b bitrate] [-m] url" << std::endl;
    std::cout << std::endl;
    std::cout << "-w the width of video" << std::endl;
    std::cout << "-h the height of video" << std::endl;
    std::cout << "-f the fps of video" << std::endl;
    std::cout << "-b the bitrate of video (kb/s)" << std::endl;
    std::cout << "-m enable motion detection (opencv required)" << std::endl;
#ifndef OPENCV_FOUND
    std::cout << "    (opencv not found!)" << std::endl;
#endif
    std::cout << std::endl;
    std::cout << "example: publisher -w 320 -h 240 rtmp://127.0.0.1/test/live" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int width = 320;
    int height = 240;
    int fps = 16;
    int bitrate = 128;
    char ch;
    bool enable = false;

    if (argc < 2) {
        showUsage();
    }

    while ((ch = getopt(argc, argv, "w:h:f:b:m")) != -1) {
        switch (ch) {
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'f':
                fps = atoi(optarg);
                break;
            case 'b':
                bitrate = atoi(optarg);
                break;
            case 'm':
                enable = true;
                break;
            default:
                std::cout << "unknown option " << ch << std::endl;
                showUsage();
        }
    }

    if (argc <= optind) {
        std::cout << "url not found" << std::endl;
        showUsage();
    }

    PacketQueue queue;
    MemoryPool pool;
    PCMSource pcm;
    V4L2Source v4l2(width, height);
    FilteredVideoSource videoSource(v4l2);

    if (enable) {
#ifdef OPENCV_FOUND
        videoSource.addFilter(new YUY2Converter(YUY2_CVT_RGB24));
        videoSource.addFilter(new MotionDetector());
#else
        videoSource.addFilter(new YUY2Converter(YUY2_CVT_I420));
#endif
    } else {
        videoSource.addFilter(new YUY2Converter(YUY2_CVT_I420));
    }

    RTMPPublisher publisher(queue, pool);

    if (!publisher.connect(argv[optind])) {
        std::cout << "connect server error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    H264Stream videoStream(videoSource, queue, pool, fps, bitrate);
    AACStream audioStream(pcm, queue, pool);

    std::thread videoEncodeThread(&H264Stream::run, &videoStream);
    std::thread audioEncodeThread(&AACStream::run, &audioStream);

    publisher.run();

    return 0;
}
