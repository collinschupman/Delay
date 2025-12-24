#include "LFO.h"

float LFO::get(float rate, float phaseOffset, uint32_t sampleRate)
{
    // Apply phase offset and wrap to [0, 1)
    float offsetPhase = mCurrentPhase + phaseOffset;
    if (offsetPhase >= 1.f)
    {
        offsetPhase -= 1.f;
    }
    
    // Generate sine wave
    const float out = std::sin(2.f * M_PI * offsetPhase);
    
    // Advance phase and wrap
    mCurrentPhase += rate / static_cast<float>(sampleRate);
    if (mCurrentPhase >= 1.f)
    {
        mCurrentPhase -= 1.f;
    }
    
    return out;
}