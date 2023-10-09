#include "Delayline.h"
#include "Helpers.h"

#include <memory>

AKRESULT Delayline::Init(AkUInt32 inSampleRate, float maxDelayTime)
{
    if (!circularBuffer)
    {
        circularBuffer = std::make_unique<CircularBuffer>(inSampleRate, maxDelayTime);
    }

    circularBuffer->Init();

    return AK_Success;
}

void Delayline::write(float inValue)
{
    circularBuffer->write(inValue + mFeedback);
}

void Delayline::process(AkReal32 *pBuf, AkUInt16 pBufPos, float inFeedback, float dryWet)
{
    const unsigned readHeadInt = static_cast<unsigned>(circularBuffer->getReadHead());

    const float readHeadFraction = circularBuffer->getReadHead() - readHeadInt;
    const unsigned readHeadNext = static_cast<unsigned>(circularBuffer->getNextReadHead());

    const float delayedSample =
        CS::lerp(circularBuffer->getValue(readHeadInt),
                 circularBuffer->getValue(readHeadNext), readHeadFraction);

    mFeedback = delayedSample * inFeedback;

    pBuf[pBufPos] =
        delayedSample * dryWet +
        pBuf[pBufPos] * (1.f - dryWet);
}

void Delayline::updateReadHead(float delayTime)
{
    circularBuffer->updateReadHead(delayTime);
}

void Delayline::updateWriteHead()
{
    circularBuffer->updateWriteHead();
}
