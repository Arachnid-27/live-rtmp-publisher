#include "H264RTMPPackager.h"

RTMPPacket H264RTMPPackager::pack(char* buf) {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet.m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = mLength + 5;
    packet.m_body = body;

    *(body++) = (mData[4] & 0x1f) == 0x05 ? 0x17 : 0x27;
    *(body++) = 0x01;
    *(body++) = 0x00;
    *(body++) = 0x00;
    *(body++) = 0x00;
    memcpy(body, mData, mLength);

    return packet;
}

RTMPPacket H264RTMPPackager::metadata(char* buf) {
    char *body = buf + RTMP_MAX_HEADER_SIZE;

    RTMPPacket packet;
    packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    packet.m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet.m_nChannel = 0x04;
    packet.m_hasAbsTimestamp = 0;
    packet.m_nBodySize = 8 + mLength;
    packet.m_body = body;

    *(body++) = 0x17; // 1-keyframe, 7-AVC
    *(body++) = 0x00;
    *(body++) = 0x00;
    *(body++) = 0x00;
    *(body++) = 0x00;

    // AVCDecoderConfigurationRecord

    *(body++) = 0x01; // configurationVersion
    *(body++) = mData[5]; // AVCProfileIndication
    *(body++) = mData[6]; // profile_compatibility
    *(body++) = mData[7]; // AVCLevelIndication
    *(body++) = 0xff; // 111111(reserved) + lengthSizeMinusOne

    int len = (mData[2] << 8) | mData[3];

    *(body++) = 0xe1; // 111(reserved) + numOfSequenceParameterSets
    memcpy(body, mData + 2, len + 2);

    body += (len + 2);
    *(body++) = 0x01; // numOfPictureParameterSets
    memcpy(body, mData + len + 6, mLength - len - 6);

    return packet;
}
