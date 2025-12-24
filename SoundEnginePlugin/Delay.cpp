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

void Delay::Execute(AkAudioBuffer *io_pBuffer, std::array<InDelayParams, 2> &inParams, AkUInt32 uDelayMode)
{
    assert(io_pBuffer->NumChannels() == mDelaylines.size());

    AkUInt16 numFramesProcessed = 0;
    while (numFramesProcessed < io_pBuffer->uValidFrames)
    {
        mParams[0].mDelayTimeSmoothed = CS::smoothParameter(mParams[0].mDelayTimeSmoothed, inParams[0].pDelayTime, CS::kParamCoeff_Fine);
        mParams[1].mDelayTimeSmoothed = CS::smoothParameter(mParams[1].mDelayTimeSmoothed, inParams[1].pDelayTime, CS::kParamCoeff_Fine);

        mParams[0].mDelayTimeSamples = mSampleRate * mParams[0].mDelayTimeSmoothed;
        mParams[1].mDelayTimeSamples = mSampleRate * mParams[1].mDelayTimeSmoothed;

        AkReal32 *AK_RESTRICT pBufLeft =
            (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(0);

        AkReal32 *AK_RESTRICT pBufRight =
            (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(1);

        const auto leftOut = pBufLeft[numFramesProcessed] + mParams[0].mFeedback;
        const auto rightOut = pBufRight[numFramesProcessed] + mParams[1].mFeedback;

        if (uDelayMode == 0) // normal
        {
            mDelaylines[0].write(leftOut);
            mDelaylines[1].write(rightOut);
        }
        else // ping pong
        {
            mDelaylines[0].write(rightOut);
            mDelaylines[1].write(leftOut);
        }

        mDelaylines[0].updateReadHead(mParams[0].mDelayTimeSamples);
        mDelaylines[1].updateReadHead(mParams[1].mDelayTimeSamples);

        const auto delayedSampleLeft = mDelaylines[0].read();
        const auto delayedSampleRight = mDelaylines[1].read();

        mParams[0].mFeedback = delayedSampleLeft * inParams[0].pFeedback;
        mParams[1].mFeedback = delayedSampleRight * inParams[1].pFeedback;

        pBufLeft[numFramesProcessed] =
            delayedSampleLeft * inParams[0].pDryWet +
            pBufLeft[numFramesProcessed] * (1.f - inParams[0].pDryWet);

        pBufRight[numFramesProcessed] =
            delayedSampleRight * inParams[1].pDryWet +
            pBufRight[numFramesProcessed] * (1.f - inParams[1].pDryWet);

        mDelaylines[0].updateWriteHead();
        mDelaylines[1].updateWriteHead();

        ++numFramesProcessed;
    }
}