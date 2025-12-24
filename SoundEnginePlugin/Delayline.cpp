#include "Delayline.h"
#include "Helpers.h"

Delayline::Delayline(float sampleRate, float maxDelayTime)
    : circularBuffer(sampleRate, maxDelayTime)
{
    circularBuffer.Init();
}

void Delayline::write(float inValue)
{
    circularBuffer.write(inValue);
}

float Delayline::read() const
{
    const unsigned readHeadInt = static_cast<unsigned>(circularBuffer.getReadHead());
    const float readHeadFraction = circularBuffer.getReadHead() - static_cast<float>(readHeadInt);
    const unsigned readHeadNext = static_cast<unsigned>(circularBuffer.getNextReadHead());

    return CS::lerp(circularBuffer.getValue(readHeadInt), circularBuffer.getValue(readHeadNext),
                    readHeadFraction);
}

void Delayline::updateReadHead(float delayTime)
{
    circularBuffer.updateReadHead(delayTime);
}

void Delayline::updateWriteHead()
{
    circularBuffer.updateWriteHead();
}
