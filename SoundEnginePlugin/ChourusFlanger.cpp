#include "ChorusFlanger.h"

#include <cassert>

AKRESULT ChorusFlanger::Init(AkUInt32 inSampleRate, float delayTime, float maxDelayTime)
{
    mSampleRate = inSampleRate;

    mDelayTimeSamples = mSampleRate * delayTime;
    mDelayTimeSmoothed = delayTime;

    for (Delayline &delayLine : mDelaylines)
    {
        delayLine.Init(inSampleRate, maxDelayTime);
    }

    return AK_Success;
}

void ChorusFlanger::Execute(AkAudioBuffer *io_pBuffer, AkReal32 pDelayTime, AkReal32 pFeedback, AkReal32 pDryWet)
{
    assert(io_pBuffer->NumChannels() == mDelaylines.size());

    AkUInt16 numFramesProcessed = 0;
    while (numFramesProcessed < io_pBuffer->uValidFrames)
    {
        float lfoOutMaped = CS::jmap(mLFO.get(22, mSampleRate), -1.f, 1.f, 0.005f, 0.03f);

        mDelayTimeSmoothed = CS::smoothParameter(mDelayTimeSmoothed, lfoOutMaped);

        mDelayTimeSamples = mSampleRate * mDelayTimeSmoothed;

        for (int i = 0; i < io_pBuffer->NumChannels(); i++)
        {
            Delayline &delayLine = mDelaylines[i];

            AkReal32 *AK_RESTRICT pBuf =
                (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(i);

            delayLine.write(pBuf[numFramesProcessed]);

            delayLine.updateReadHead(mDelayTimeSamples);

            delayLine.process(pBuf, numFramesProcessed, pFeedback, pDryWet);

            delayLine.updateWriteHead();
        }

        ++numFramesProcessed;
    }
}
