#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(float inSampleRate, float maxDelayTime)
{
    length = inSampleRate * maxDelayTime;
    buffer = new float[length];
}

CircularBuffer::~CircularBuffer()
{
    if (buffer)
    {
        delete[] buffer;
        buffer = nullptr;
    }
}

void CircularBuffer::reset()
{
    for (int j = 0; j < length; ++j)
    {
        buffer[j] = 0.0f;
    }
    readHead = 0.f;
    writeHead = 0;
}