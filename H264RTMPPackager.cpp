#include "H264RTMPPackager.h"

RTMPPacket H264RTMPPackager::pack(char* buf) {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet.m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = mSize + 5;
    packet.m_body = body;

    *(body++) = (mData[4] & 0x1f) == 0x05 ? 0x17 : 0x27;
    *(body++) = 0x01;
    *(body++) = 0x00;
    *(body++) = 0x00;
    *(body++) = 0x00;
    memcpy(body, mData, mSize);

    return packet;
}
