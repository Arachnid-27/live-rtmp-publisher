#include "AACRTMPPackager.h"

RTMPPacket AACRTMPPackager::pack(char* buf) {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    packet.m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = mLength - 5;
    packet.m_body = body;

    *(body++) = 0xAF;
    *(body++) = 0x01;
    memcpy(body, mData + 7, mLength - 7);

    return packet;
}

RTMPPacket AACRTMPPackager::metadata(char* buf) {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet.m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = mLength + 2;
    packet.m_body = body;

    *(body++) = 0xAF;
    *(body++) = 0x00;
    memcpy(body, mData, mLength);

    return packet;
}
