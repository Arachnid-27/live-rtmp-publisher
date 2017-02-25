#include "PCMSource.h"

PCMSource::PCMSource(const char *name, int sampleRate, int channals, int period): 
    mSampleRate(sampleRate), mChannals(channals), mDataBuf(NULL), mPeriod(period) {
        int direct;
        snd_pcm_hw_params_t *param;

        mIsOpened = (snd_pcm_open(&mHandle, name, SND_PCM_STREAM_CAPTURE, 0) == 0);

        if (mIsOpened) {
            snd_pcm_hw_params_alloca(&param);
            snd_pcm_hw_params_any(mHandle, param);
            snd_pcm_hw_params_set_access(mHandle, param, SND_PCM_ACCESS_RW_INTERLEAVED);
            snd_pcm_hw_params_set_format(mHandle, param, SND_PCM_FORMAT_S16_LE);
            snd_pcm_hw_params_set_channels(mHandle, param, channals);
            snd_pcm_hw_params_set_rate_near(mHandle, param, &mSampleRate, &direct);
            snd_pcm_hw_params_set_period_size_near(mHandle, param, &mPeriod, &direct);
            mIsOpened = (snd_pcm_hw_params(mHandle, param) == 0);

            if (!mIsOpened) {
                snd_pcm_drain(mHandle);
                snd_pcm_close(mHandle);
            }
        }
    }

PCMSource::~PCMSource() {
    if (mIsOpened) {
        snd_pcm_drain(mHandle);
        snd_pcm_close(mHandle);
        delete[] mDataBuf;
    }
}

void PCMSource::setMaxSample(int sample) {
    int period = mPeriod;
    mPeriod = sample / mChannals; 

    if (period < sample / mChannals) {
        int direct;
        snd_pcm_hw_params_t *param;
        
        snd_pcm_hw_params_alloca(&param);
        snd_pcm_hw_params_current(mHandle, param);
        snd_pcm_hw_params_set_period_size_near(mHandle, param, &mPeriod, &direct);
    }
}

std::pair<int, char*> PCMSource::getNextFrames() {
    if (!mIsOpened) {
        return std::make_pair<int, char*>(0, NULL);
    }

    if (!mDataBuf) {
        mDataBuf = new char[mPeriod * mChannals * 2];
    }

    int ret;
    for (int i = 0; i != 5; ++i) {
        ret = snd_pcm_readi(mHandle, mDataBuf, mPeriod);
        if (ret == -EPIPE) {
            // overrun, retry!
            snd_pcm_prepare(mHandle);
        } else if (ret < 0) {
            break;
        } else {
            // maybe read incompletely
            return std::make_pair(ret * mChannals, mDataBuf);
        }
    }
    return std::make_pair<int, char*>(0, NULL);
}
