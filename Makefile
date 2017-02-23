OBJ = main.o CameraSource.o MotionDetector.o H264Encoder.o H264Stream.o RTMPPublisher.o PacketQueue.o AACEncoder.o AACStream.o PCMSource.o H264RTMPPackager.o AACRTMPPackager.o YUY2Converter.o V4L2Source.o FilteredVideoSource.o

HDR = include
OPENCV = `pkg-config --cflags --libs opencv`

CC = g++
CFLAGS = -std=c++11 -O2 -pthread -I $(HDR) -lx264 -lrtmp -lasound -lfaac $(OPENCV)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) main
