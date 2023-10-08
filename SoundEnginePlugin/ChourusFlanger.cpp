#include "ChorusFlanger.h"

#include <cassert>
#include <string>

AKRESULT ChorusFlanger::Init(AkUInt32 inSampleRate, float delayTime, float maxDelayTime)
{
    mSampleRate = inSampleRate;

    // mDelayTimeSamples = mSampleRate * delayTime;
    // mDelayTimeSmoothed = delayTime;

    for (int i = 0; i < mOffsets.size(); i++)
    {
        mOffsets[i] = 0.f;
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

    AkUInt16 numFramesProcessed = 0;
    while (numFramesProcessed < io_pBuffer->uValidFrames)
    {

        // offset the right channel;
        mOffsets[1] = pPhaseOffset;

        for (AkUInt32 i = 0; i < io_pBuffer->NumChannels(); i++)
        {
            Delayline &delayLine = mDelaylines[i];

            auto lfoOutMaped = CS::jmap(mLFOs[i].get(pRate, mOffsets[i], mSampleRate) * pDepth, -1.f, 1.f, 0.005f, 0.03f);

            const auto delayTimeSamples = mSampleRate * lfoOutMaped;

            AkReal32 *AK_RESTRICT pBuf =
                (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(i);

            delayLine.write(pBuf[numFramesProcessed]);

            delayLine.updateReadHead(delayTimeSamples);

            delayLine.process(pBuf, numFramesProcessed, pFeedback, pDryWet);

            delayLine.updateWriteHead();
        }

        ++numFramesProcessed;
    }
}
