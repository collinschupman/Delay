#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(AkReal32 inSampleRate, AkReal32 maxDelayTime)
    : length(inSampleRate * maxDelayTime), buffer(std::make_unique<float[]>(length))
{
}

void CircularBuffer::Init()
{
    memset(buffer.get(), 0, sizeof(AkReal32) * length);
    readHead = 0.f;
    writeHead = 0;
}

void CircularBuffer::write(AkReal32 inValue)
{
    buffer[writeHead] = inValue;
}

void CircularBuffer::updateWriteHead()
{
    ++writeHead;
    if (writeHead >= length)
    {
        writeHead = 0;
    }
}

void CircularBuffer::updateReadHead(AkReal32 delayTime)
{
    readHead = writeHead - delayTime;
    if (readHead < 0)
    {
        readHead += length;
    }
}

AkReal32 CircularBuffer::getReadHead() const
{
    return readHead;
}

AkReal32 CircularBuffer::getNextReadHead() const
{
    auto readHead_x1 = readHead + 1;

    if (readHead_x1 >= length)
    {
        readHead_x1 -= length;
    }
    return readHead_x1;
}

AkReal32 CircularBuffer::getValue(unsigned position) const
{
    return buffer[position];
}
