#pragma once

#include "CircularBuffer.h"
#include "Helpers.h"

#include <cstdint>
#include <stdexcept>

struct Delayline
{
public:
    Delayline(float sampleRate, float maxDelayTime);

    void write(float inValue);
    float read() const;
    void updateReadHead(float delayTime);
    void updateWriteHead();

private:
    CircularBuffer circularBuffer;
};