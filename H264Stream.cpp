#include "H264Stream.h"

H264Stream::H264Stream(FilteredVideoSource* source, PacketQueue* queue, int fps, int bitrate): 
    mInterval(1000 / fps), mSource(source), mQueue(queue), 
    mEncoder(source->getWidth(), source->getHeight(), fps, bitrate) {
        x264_nal_t *result = mEncoder.getMetadata();
        x264_nal_t *sps = result;
        x264_nal_t *pps = result + 1;

        buildMetadata(sps, pps);
    }

void H264Stream::buildMetadata(x264_nal_t* sps, x264_nal_t* pps) {
    char *body = mDataBuf + RTMP_MAX_HEADER_SIZE;

    mMetadata.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    mMetadata.m_packetType = RTMP_PACKET_TYPE_VIDEO;
    mMetadata.m_nChannel = 0x04;
    mMetadata.m_hasAbsTimestamp = 0;
    mMetadata.m_nBodySize = 8 + sps->i_payload + pps->i_payload;
    mMetadata.m_body = body;

    *(body++) = 0x17; // 1-keyframe, 7-AVC
    *(body++) = 0x00;
    *(body++) = 0x00;
    *(body++) = 0x00;
    *(body++) = 0x00;

    // AVCDecoderConfigurationRecord

    *(body++) = 0x01; // configurationVersion
    *(body++) = sps->p_payload[5]; // AVCProfileIndication
    *(body++) = sps->p_payload[6]; // profile_compatibility
    *(body++) = sps->p_payload[7]; // AVCLevelIndication
    *(body++) = 0xff; // 111111(reserved) + lengthSizeMinusOne

    *(body++) = 0xe1; // 111(reserved) + numOfSequenceParameterSets
    memcpy(body, sps->p_payload + 2, sps->i_payload - 2);

    body += (sps->i_payload - 2);

    *(body++) = 0x01; // numOfPictureParameterSets
    memcpy(body, pps->p_payload + 2, pps->i_payload - 2);
}

void H264Stream::run() {
    std::chrono::milliseconds duration;
    auto frame = mSource->getNextFrame();
    auto result = mEncoder.encode(frame);
    H264RTMPPackager packager(result.first, result.second);

    mQueue->push(mMetadata);
    mQueue->push(packager);

    auto last = std::chrono::system_clock::now();

    while ((frame = mSource->getNextFrame()) != NULL) {
        result = mEncoder.encode(frame);
        packager.setSize(result.first);
        packager.setData(result.second);

        if ((result.second[4] & 0x1f) == 0x05) { // key frame
            mQueue->push(mMetadata);
        }
        mQueue->push(packager);

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last);

        if (duration < mInterval) {
            msleep((mInterval - duration).count());
        }
        last = std::chrono::system_clock::now();
    }
}
