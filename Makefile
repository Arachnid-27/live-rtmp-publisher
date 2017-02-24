SOURCE = PCMSource.o V4L2Source.o CvVideoSource.o FilteredVideoSource.o
FILTER = YUY2Converter.o MotionDetector.o
ENCODE = AACEncoder.o H264Encoder.o
PACKET = AACRTMPPackager.o H264RTMPPackager.o
STREAM = AACStream.o H264Stream.o
OTHERS = RTMPPublisher.o PacketQueue.o

EXE = main
OBJS = main.o $(SOURCE) $(FILTER) $(ENCODE) $(PACKET) $(STREAM) $(OTHERS)

HDR = include
OPENCV = `pkg-config --cflags --libs opencv`

CC = g++
CFLAGS = -std=c++11 -O2 -pthread -I $(HDR) -lx264 -lrtmp -lasound -lfaac $(OPENCV)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(EXE) $(OBJS)
