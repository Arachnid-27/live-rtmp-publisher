#include "AACRTMPPackager.h"

RTMPPacket AACRTMPPackager::pack(char* buf, const char* data, int length) const {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    packet.m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = length - 5;
    packet.m_body = body;

    *(body++) = 0xAF;
    *(body++) = 0x01;
    memcpy(body, data + 7, length - 7);

    return packet;
}

RTMPPacket AACRTMPPackager::metadata(char* buf, const char* data, int length) const {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet.m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = length + 2;
    packet.m_body = body;

    *(body++) = 0xAF;
    *(body++) = 0x00;
    memcpy(body, data, length);

    return packet;
}
