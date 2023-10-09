#pragma once

#include <memory>

struct CircularBuffer
{
public:
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
    float readHead = 0.f;
    unsigned writeHead = 0;
    std::unique_ptr<float[]> buffer = nullptr;
};