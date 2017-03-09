#include "V4L2Source.h"

V4L2Source::V4L2Source(int width, int height, const char* name, char a, char b, char c, char d) {
    mIsCaptrue = false;
    if ((mDeviceFd = open(name, O_RDWR)) == -1) {
        std::cout << "can not open " << name << std::endl;
    } else if (!checkCapability() || !checkFormat(width, height, a, b, c, d) || !initBuffer()) {
        close(mDeviceFd);
    }
}

V4L2Source::~V4L2Source() {
    if (mDeviceFd != -1) {
        close(mDeviceFd);
        munmapBuffer(mBufCount);
    }
}

char* V4L2Source::getNextFrame() {
    if (!isOpened()) {
        std::cout << "device is not open" << std::endl;
        return NULL;
    }

    if (!mIsCaptrue) {
        v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl(mDeviceFd, VIDIOC_STREAMON, &type) == -1) {
            std::cout << "capture error" << std::endl;
            return NULL;
        }

        memset(&mDataBuf, 0, sizeof(mDataBuf));
        mDataBuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        mDataBuf.memory = V4L2_MEMORY_MMAP;

        mIsCaptrue = true;
    } else if (ioctl(mDeviceFd, VIDIOC_QBUF, &mDataBuf) == -1) {
        std::cout << "set buffer error" << std::endl;
        return NULL;
    }

    while (ioctl(mDeviceFd, VIDIOC_DQBUF, &mDataBuf) == -1) {
        if (errno == EIO) continue;

        std::cout << "get buffer error" << std::endl;
        return NULL;
    }

    return mQueueBuf[mDataBuf.index].data;
}

bool V4L2Source::checkCapability() {
    v4l2_capability cap;

    if (ioctl(mDeviceFd, VIDIOC_QUERYCAP, &cap) == -1) {
        std::cout << "query capability error" << std::endl;
        return false;
    }
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        std::cout << "unsupport capture" << std::endl;
        return false;
    }

    return true;
}

bool V4L2Source::checkFormat(int width, int height, char a, char b, char c, char d) {
    v4l2_format fmt;

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = v4l2_fourcc(a, b, c, d);
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(mDeviceFd, VIDIOC_S_FMT, &fmt) == -1) {
        std::cout << "set format error" << std::endl;
        return false;
    }
    if (fmt.fmt.pix.pixelformat != v4l2_fourcc(a, b, c, d)) {
        std::cout << "unsupport format (" << a << b << c << d << ")" << std::endl;
        return false;
    }

    mWidth = fmt.fmt.pix.width;
    mHeight = fmt.fmt.pix.height;

    return true;
}

bool V4L2Source::initBuffer() {
    v4l2_requestbuffers reqbuf;
    v4l2_buffer buf;

    reqbuf.count = 4;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;

    if (ioctl(mDeviceFd, VIDIOC_REQBUFS, &reqbuf) == -1) {
        std::cout << "request buffers error" << std::endl;
        return false;
    }
    if (reqbuf.count == 0) {
        std::cout << "the number of buffer is 0" << std::endl;
        return 0;
    }

    mBufCount = reqbuf.count;

    for (int i = 0; i < mBufCount; ++i) {
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (ioctl(mDeviceFd, VIDIOC_QUERYBUF, &buf) == -1) {
            std::cout << "query buffer error" << std::endl;
            return false;
        }

        mQueueBuf[i].length = buf.length;
        mQueueBuf[i].data = static_cast<char*>(mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, mDeviceFd, buf.m.offset));

        if (mQueueBuf[i].data == MAP_FAILED) {
            std::cout << "mmap buffer error" << std::endl;
            munmapBuffer(i);
            return false;
        }

        if (ioctl(mDeviceFd, VIDIOC_QBUF, &buf) == -1) {
            std::cout << "set buffer error" << std::endl;
            munmapBuffer(i);
            return false;
        }
    }

    return true;
}

inline void V4L2Source::munmapBuffer(int end) {
    for (int i = 0; i != end; ++i) {
        munmap(mQueueBuf[i].data, mQueueBuf[i].length);
    }
}
