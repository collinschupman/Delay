#include "CircularBuffer.h"
#include <cstring>
#include <string>
#include <stdexcept>

CircularBuffer::CircularBuffer(float inSampleRate, float maxDelayTime)
    : length(static_cast<std::size_t>(inSampleRate * maxDelayTime)),
      lengthFloat(static_cast<float>(length))
{
    if (length > MAX_BUFFER_SIZE)
    {
        throw std::runtime_error(
            "CircularBuffer: Requested size (" + std::to_string(length) + 
            " samples) exceeds maximum (" + std::to_string(MAX_BUFFER_SIZE) + 
            " samples). Reduce sample rate or max delay time."
        );
    }
}

void CircularBuffer::Init()
{
    // Buffer is already zero-initialized in declaration
    // Just reset the indices
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
    readHead = static_cast<float>(writeHead) - delayTime;
    if (readHead < 0.f)
    {
        readHead += lengthFloat;
    }
}

float CircularBuffer::getReadHead() const
{
    return readHead;
}

float CircularBuffer::getNextReadHead() const
{
    float readHead_x1 = readHead + 1.f;
    return (readHead_x1 >= lengthFloat) ? (readHead_x1 - lengthFloat) : readHead_x1;
}

float CircularBuffer::getValue(unsigned position) const
{
    return buffer[position];
}
