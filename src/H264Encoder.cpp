#include "H264Encoder.h"

H264Encoder::H264Encoder(int width, int height, int fps, int bitrate): mPts(0) {
    x264_param_t param;

    mLumaSize = width * height;
    mChromaSize = mLumaSize / 4;

    x264_param_default_preset(&param, "ultrafast", "zerolatency");
    x264_param_apply_profile(&param, "baseline");

    param.i_log_level = X264_LOG_NONE;
    param.i_csp = X264_CSP_I420;
    param.i_width = width;
    param.i_height = height;
    param.i_fps_den = 1;
    param.i_fps_num = fps;
    param.i_keyint_max = fps;

    param.rc.i_rc_method = X264_RC_ABR;
    param.rc.i_bitrate = bitrate;

    param.b_repeat_headers = 0; // add sps and pps manually
    param.b_annexb = 0; // for the convenience of packing

    x264_picture_alloc(&mPicture, param.i_csp, param.i_width, param.i_height);

    mHandle = x264_encoder_open(&param);
}

H264Encoder::~H264Encoder() {
    x264_encoder_close(mHandle);
    x264_picture_clean(&mPicture);
}

std::pair<int, char*> H264Encoder::encode(char* frame) {
    int temp, size;
    x264_picture_t out;

    memcpy(mPicture.img.plane[0], frame, mLumaSize);
    memcpy(mPicture.img.plane[1], frame + mLumaSize, mChromaSize);
    memcpy(mPicture.img.plane[2], frame + mLumaSize + mChromaSize, mChromaSize);

    mPicture.i_pts = mPts++;

    size = x264_encoder_encode(mHandle, &mNal, &temp, &mPicture, &out);

    return std::make_pair(size, reinterpret_cast<char*>(mNal->p_payload));
}

std::pair<int, char*> H264Encoder::getMetadata() {
    int temp, size;

    x264_encoder_headers(mHandle, &mNal, &temp);
    size = mNal[0].i_payload + mNal[1].i_payload;

    return std::make_pair(size, reinterpret_cast<char*>(mNal->p_payload));
}
