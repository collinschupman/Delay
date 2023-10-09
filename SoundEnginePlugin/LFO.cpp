#include "LFO.h"

float LFO::get(float rate, float phaseOffset, AkUInt32 sampleRate)
{
    float offsetPhase = mCurrentPhase + phaseOffset;
    if (offsetPhase >= 1.f)
    {
        offsetPhase -= 1.f;
    }
    const float out = static_cast<float>(sin(2.0f * M_PI * offsetPhase));
    mCurrentPhase += rate / sampleRate;
    if (mCurrentPhase >= 1.f)
    {
        mCurrentPhase -= 1.f;
    }
    return out;
}