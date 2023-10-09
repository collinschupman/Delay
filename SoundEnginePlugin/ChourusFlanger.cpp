#include "ChorusFlanger.h"

#include <cassert>
#include <string>

AKRESULT ChorusFlanger::Init(AkUInt32 inSampleRate, float delayTime, float maxDelayTime)
{
    mSampleRate = inSampleRate;

    for (int i = 0; i < mOffsets.size(); i++)
    {
        mOffsets[i] = 0.f; // TD: memzero
    }

    for (Delayline &delayLine : mDelaylines)
    {
        delayLine.Init(inSampleRate, maxDelayTime);
    }

    return AK_Success;
}

void ChorusFlanger::Execute(AkAudioBuffer *io_pBuffer, AkReal32 pDepth, AkReal32 pRate, AkReal32 pPhaseOffset, AkReal32 pFeedback, AkReal32 pDryWet, AkUInt32 pType)
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

        mOffsets[1] = pPhaseOffset; // offset the right channel;

        for (AkUInt32 i = 0; i < io_pBuffer->NumChannels(); i++)
        {

            AkReal32 *AK_RESTRICT pBuf =
                (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(i);

            mDelaylines[i].write(pBuf[numFramesProcessed]);

            float delayTimeSamples = 0.f;
            if (pType == 0) // chorus
            {
                delayTimeSamples = CS::jmap(mLFOs[i].get(pRate, mOffsets[i], mSampleRate) * pDepth, kLFOMin, kLFOMax, kFiveMilliseconds, kThirtyMilliseconds) * mSampleRate;
            }
            else // flanger //TD: self document
            {

                delayTimeSamples = CS::jmap(mLFOs[i].get(pRate, mOffsets[i], mSampleRate) * pDepth, kLFOMin, kLFOMax, kOneMillisecond, kFiveMilliseconds) * mSampleRate;
            }

            mDelaylines[i].updateReadHead(delayTimeSamples);

            mDelaylines[i].process(pBuf, numFramesProcessed, pFeedback, pDryWet);

            mDelaylines[i].updateWriteHead();
        }

        ++numFramesProcessed;
    }
}
