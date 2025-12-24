#pragma once

#include <cstddef>
#include <array>
#include <stdexcept>

struct CircularBuffer
{
public:
    // Maximum buffer size: 4 seconds at 48kHz (192,000 samples)
    static constexpr std::size_t MAX_BUFFER_SIZE = 192000;

    CircularBuffer(float inSampleRate, float maxDelayTime);
    void Init();
    void write(float inValue);

    void updateWriteHead();

    void updateReadHead(float delayTime);

    float getReadHead() const;
    float getNextReadHead() const;

    float getValue(unsigned position) const;

private:
    std::size_t length = 0;
    float lengthFloat = 0.f;  // Cached for faster calculations
    float readHead = 0.f;
    unsigned writeHead = 0;
    alignas(16) float buffer[MAX_BUFFER_SIZE] = {0};  // Static allocation, aligned for SIMD
};