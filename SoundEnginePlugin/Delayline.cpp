#include "Delayline.h"
#include "Helpers.h"

#include <memory>

void Delayline::write(float inValue)
{
    circularBuffer->buffer[circularBuffer->writeHead] =
        inValue + mFeedback;
}

void Delayline::process(AkReal32 *pBuf, AkUInt16 pBufPos, float inFeedback, float dryWet)
{
    int readHead_x = (int)circularBuffer->readHead;
    int readHead_x1 = readHead_x + 1;
    if (readHead_x1 >= circularBuffer->length)
    {
        readHead_x1 - circularBuffer->length;
    }
    float readHeadFloat = circularBuffer->readHead - readHead_x;

    float delayedSample =
        CS::lerp(circularBuffer->buffer[readHead_x],
                 circularBuffer->buffer[readHead_x1], readHeadFloat);

    mFeedback = delayedSample * inFeedback;

    pBuf[pBufPos] =
        delayedSample * dryWet +
        pBuf[pBufPos] * (1.f - dryWet);
}

void Delayline::updateReadHead(float delayTime)
{
    circularBuffer->readHead = circularBuffer->writeHead - delayTime;
    if (circularBuffer->readHead < 0)
    {
        circularBuffer->readHead += circularBuffer->length;
    }
}

void Delayline::updateWriteHead()
{
    ++circularBuffer->writeHead;
    if (circularBuffer->writeHead >= circularBuffer->length)
    {
        circularBuffer->writeHead = 0;
    }
}
