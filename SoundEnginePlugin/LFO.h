#pragma once

#include <cmath>
#include <cstdint>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct LFO
{
public:
    constexpr LFO() noexcept = default;

    float get(float rate, float phaseOffset, uint32_t sampleRate);
    void reset() noexcept
    {
        mCurrentPhase = 0.f;
    }

private:
    float mCurrentPhase = 0.f;
};