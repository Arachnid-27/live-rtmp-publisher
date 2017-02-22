#include "AACRTMPPackager.h"

RTMPPacket AACRTMPPackager::pack(char* buf) {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    packet.m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = mSize - 5;
    packet.m_body = body;

    *(body++) = 0xAF;
    *(body++) = 0x01;
    memcpy(body, mData + 7, mSize - 7);

    return packet;
}
