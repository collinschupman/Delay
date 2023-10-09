#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(float inSampleRate, float maxDelayTime)
    : length(inSampleRate * maxDelayTime), buffer(std::make_unique<float[]>(length))
{
}

void CircularBuffer::Init()
{
    memset(buffer.get(), 0, sizeof(float) * length);
    readHead = 0.f;
    writeHead = 0;
}

void CircularBuffer::write(float inValue)
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

void CircularBuffer::updateReadHead(float delayTime)
{
    readHead = writeHead - delayTime;
    if (readHead < 0)
    {
        readHead += length;
    }
}

float CircularBuffer::getReadHead() const
{
    return readHead;
}

float CircularBuffer::getNextReadHead() const
{
    auto readHead_x1 = readHead + 1;

    if (readHead_x1 >= length)
    {
        readHead_x1 -= length;
    }
    return readHead_x1;
}

float CircularBuffer::getValue(unsigned position) const
{
    return buffer[position];
}
