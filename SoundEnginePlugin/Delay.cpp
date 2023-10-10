#include "Delay.h"

#include <cassert>

AKRESULT Delay::Init(AkUInt32 inSampleRate, AkReal32 delayTime, AkReal32 maxDelayTime)
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

void Delay::Execute(AkAudioBuffer *io_pBuffer, AkReal32 pDelayTime, AkReal32 pFeedback, AkReal32 pDryWet)
{
    assert(io_pBuffer->NumChannels() == mDelaylines.size());

    AkUInt16 numFramesProcessed = 0;
    while (numFramesProcessed < io_pBuffer->uValidFrames)
    {
        mDelayTimeSmoothed = CS::smoothParameter(mDelayTimeSmoothed, pDelayTime, CS::kParamCoeff_Fine);
        mDelayTimeSamples = mSampleRate * mDelayTimeSmoothed;

        for (AkUInt32 i = 0; i < io_pBuffer->NumChannels(); i++)
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
