#pragma once

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#define MAX_DELAY_TIME 2

#include <math.h>
#include <AK/SoundEngine/Common/AkNumeralTypes.h>

struct LFO
{
private:
    float mCurrentPhase = 0.f;

public:
    float get(float rate, float phaseOffset, AkUInt32 sampleRate)
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
};