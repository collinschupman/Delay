#pragma once

struct CircularBuffer
{
    CircularBuffer(float inSampleRate, float maxDelayTime);

    ~CircularBuffer();

    void reset();

    int length = 0;
    float readHead = 0.f;
    int writeHead = 0;
    float *buffer = nullptr;
};