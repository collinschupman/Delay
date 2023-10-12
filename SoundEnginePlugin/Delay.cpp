#include "Delay.h"

#include <cassert>

AKRESULT Delay::Init(AkUInt32 inSampleRate, AkReal32 delayTime, AkReal32 maxDelayTime)
{
    mSampleRate = inSampleRate;

    // mDelayTimeSamples = mSampleRate * delayTime;
    // mDelayTimeSmoothed = delayTime;

    for (LocalDelayParams &param : mParams)
    {
        param.mDelayTimeSamples = mSampleRate * delayTime;
        param.mDelayTimeSmoothed = delayTime;
    }

    for (Delayline &delayLine : mDelaylines)
    {
        delayLine.Init(inSampleRate, maxDelayTime);
    }

    return AK_Success;
}

void Delay::Execute(AkAudioBuffer *io_pBuffer, std::array<InDelayParams,2>& inParams)
{
    assert(io_pBuffer->NumChannels() == mDelaylines.size());

    AkUInt16 numFramesProcessed = 0;
    while (numFramesProcessed < io_pBuffer->uValidFrames)
    {

        for (AkUInt32 i = 0; i < io_pBuffer->NumChannels(); i++)
        {
            mParams[i].mDelayTimeSmoothed = CS::smoothParameter(mParams[i].mDelayTimeSmoothed, inParams[i].pDelayTime, CS::kParamCoeff_Fine);
            mParams[i].mDelayTimeSamples = mSampleRate * mParams[i].mDelayTimeSmoothed;

            AkReal32 *AK_RESTRICT pBuf =
                (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(i);

            mDelaylines[i].write(pBuf[numFramesProcessed] + mParams[i].mFeedback); // store the incoming sample for delay d(n)

            mDelaylines[i].updateReadHead(mParams[i].mDelayTimeSamples); // update the read head

            const auto delayedSample = mDelaylines[i].read(); // get the delayed sample
            mParams[i].mFeedback = delayedSample * inParams[i].pFeedback;        // update the feedback

            pBuf[numFramesProcessed] =
                delayedSample * inParams[i].pDryWet +
                pBuf[numFramesProcessed] * (1.f - inParams[i].pDryWet); // output

            mDelaylines[i].updateWriteHead(); // update the write head
        }

        ++numFramesProcessed;
    }
}