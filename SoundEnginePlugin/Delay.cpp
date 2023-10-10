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
            AkReal32 *AK_RESTRICT pBuf =
                (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(i);

            mDelaylines[i].write(pBuf[numFramesProcessed] + mFeedback); // store the incoming sample for delay d(n)

            mDelaylines[i].updateReadHead(mDelayTimeSamples); // update the read head

            const auto delayedSample = mDelaylines[i].read(); // get the delayed sample
            mFeedback = delayedSample * pFeedback; // update the feedback

            pBuf[numFramesProcessed] =
                delayedSample * pDryWet +
                pBuf[numFramesProcessed] * (1.f - pDryWet); //output

            mDelaylines[i].updateWriteHead(); // update the write head
        }

        ++numFramesProcessed;
    }
}
