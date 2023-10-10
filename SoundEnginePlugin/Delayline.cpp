#include "Delayline.h"
#include "Helpers.h"

#include <memory>

AKRESULT Delayline::Init(AkUInt32 inSampleRate, AkReal32 maxDelayTime)
{
    if (!circularBuffer)
    {
        circularBuffer = std::make_unique<CircularBuffer>(inSampleRate, maxDelayTime);
    }

    circularBuffer->Init();

    return AK_Success;
}

void Delayline::write(AkReal32 inValue)
{
    circularBuffer->write(inValue);
}

AkReal32 Delayline::read()
{
    const unsigned readHeadInt = static_cast<unsigned>(circularBuffer->getReadHead());

    const AkReal32 readHeadFraction = circularBuffer->getReadHead() - readHeadInt;
    const unsigned readHeadNext = static_cast<unsigned>(circularBuffer->getNextReadHead());

    return CS::lerp(circularBuffer->getValue(readHeadInt),
                 circularBuffer->getValue(readHeadNext), readHeadFraction);
}

void Delayline::updateReadHead(AkReal32 delayTime)
{
    circularBuffer->updateReadHead(delayTime);
}

void Delayline::updateWriteHead()
{
    circularBuffer->updateWriteHead();
}
