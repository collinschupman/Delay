#include "ChorusFlanger.h"

#include <cassert>

AKRESULT ChorusFlanger::Init(AkUInt32 inSampleRate, AkReal32 delayTime, AkReal32 maxDelayTime)
{
    mSampleRate = inSampleRate;

    for (int i = 0; i < mOffsets.size(); i++)
    {
        mOffsets[i] = 0.f;
    }

    for (Delayline& delayLine : mDelaylines)
    {
        delayLine.Init(inSampleRate, maxDelayTime);
    }

    return AK_Success;
}

void ChorusFlanger::Execute(AkAudioBuffer* io_pBuffer, AkReal32 pDepth, AkReal32 pRate,
                            AkReal32 pPhaseOffset, AkReal32 pFeedback, AkReal32 pDryWet,
                            AkUInt32 pType)
{
    assert(io_pBuffer->NumChannels() == mDelaylines.size());

    static constexpr float kLFOMin = -1.f;
    static constexpr float kLFOMax = 1.f;
    static constexpr float kFiveMilliseconds = 0.005f;
    static constexpr float kOneMillisecond = 0.001f;
    static constexpr float kThirtyMilliseconds = 0.03f;

    AkUInt16 numFramesProcessed = 0;
    while (numFramesProcessed < io_pBuffer->uValidFrames)
    {
        pDepthSmooth = CS::smoothParameter(pDepthSmooth, pDepth, CS::kParamCoeff_Fine);
        pRateSmooth = CS::smoothParameter(pRateSmooth, pRate, CS::kParamCoeff_Fine);
        pPhaseOffsetSmooth =
            CS::smoothParameter(pPhaseOffsetSmooth, pPhaseOffset, CS::kParamCoeff_Fine);
        pFeedbackSmooth = CS::smoothParameter(pFeedbackSmooth, pFeedback, CS::kParamCoeff_Fine);
        pDryWetSmooth = CS::smoothParameter(pDryWetSmooth, pDryWet, CS::kParamCoeff_Fine);

        mOffsets[1] = pPhaseOffsetSmooth; // Offset the right channel

        for (AkUInt32 i = 0; i < io_pBuffer->NumChannels(); i++)
        {

            AkReal32* AK_RESTRICT pBuf = (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(i);

            mDelaylines[i].write(pBuf[numFramesProcessed] + mFeedback);

            float delayTimeSamples = 0.f;
            if (pType == 0) // Chorus: 5ms-30ms modulation range
            {
                delayTimeSamples =
                    CS::jmap(mLFOs[i].get(pRateSmooth, mOffsets[i], mSampleRate) * pDepthSmooth,
                             kLFOMin, kLFOMax, kFiveMilliseconds, kThirtyMilliseconds) *
                    mSampleRate;
            }
            else // Flanger: 1ms-5ms modulation range
            {
                delayTimeSamples =
                    CS::jmap(mLFOs[i].get(pRateSmooth, mOffsets[i], mSampleRate) * pDepthSmooth,
                             kLFOMin, kLFOMax, kOneMillisecond, kFiveMilliseconds) *
                    mSampleRate;
            }

            mDelaylines[i].updateReadHead(delayTimeSamples);

            const auto delayedSample = mDelaylines[i].read();
            mFeedback = delayedSample * pFeedback;

            pBuf[numFramesProcessed] =
                delayedSample * pDryWet + pBuf[numFramesProcessed] * (1.f - pDryWet);

            mDelaylines[i].updateWriteHead();
        }

        ++numFramesProcessed;
    }
}
