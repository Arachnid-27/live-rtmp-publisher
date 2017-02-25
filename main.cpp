#include <iostream>
#include <thread>
#include "RTMPPublisher.h"
#include "V4L2Source.h"
#include "H264Stream.h"
#include "AACStream.h"
#include "PCMSource.h"
#include "FilteredVideoSource.h"
#include "YUY2Converter.h"
// #include "MotionDetector.h"

void showUsage() {
    std::cout << "usage: publisher [-w width] [-h height] [-f fps] [-b bitrate] url" << std::endl;
    std::cout << "example: publisher -w 320 -h 240 rtmp://127.0.0.1/test/live" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int width = 320;
    int height = 240;
    int fps = 16;
    int bitrate = 128;
    char ch;

    if (argc < 2) {
        showUsage();
    }

    while ((ch = getopt(argc, argv, "w:h:f:b:")) != -1) {
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
    V4L2Source v4l2(width, height);
    PCMSource pcm;

    YUY2Converter converter(YUY2_CVT_I420);
//    YUY2Converter converter(YUY2_CVT_RGB24);
//    MotionDetector detector;
    FilteredVideoSource videoSource(v4l2);
    videoSource.addFilter(&converter);
//    videoSource.addFilter(&detector);

    RTMPPublisher publisher(&queue);

    if (!publisher.connect(argv[optind])) {
        std::cout << "connect server error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    H264Stream videoStream(videoSource, queue, fps, bitrate);
    AACStream audioStream(pcm, queue);

    std::thread VideoEncodeThread(&H264Stream::run, &videoStream);
    std::thread AudioEncodeThread(&AACStream::run, &audioStream);

    publisher.run();

    return 0;
}
