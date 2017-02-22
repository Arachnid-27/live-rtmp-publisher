#include "RTMPPublisher.h"

RTMPPublisher::~RTMPPublisher() {
    if (mRTMP) {
        if (RTMP_IsConnected(mRTMP)) {
            RTMP_Close(mRTMP);
        }
        RTMP_Free(mRTMP);
    }
}

bool RTMPPublisher::connect(char *url) {
    if (!RTMP_SetupURL(mRTMP, url)) {
        return false;
    }

    RTMP_EnableWrite(mRTMP);

    if (!RTMP_Connect(mRTMP, NULL)) {
        return false;
    }

    if (!RTMP_ConnectStream(mRTMP, 0)) {
        RTMP_Close(mRTMP);
        return false;
    }

    return true;
}

void RTMPPublisher::run() {
    while (true) {
        RTMPPacket &packet = mQueue->front();
        packet.m_nInfoField2 = mRTMP->m_stream_id;
        packet.m_nTimeStamp = RTMP_GetTime();

        if (!RTMP_IsConnected(mRTMP)) {
            return;
        }

        if (!RTMP_SendPacket(mRTMP, &packet, 1)) {
            return;
        }

        mQueue->pop();
    }
}
