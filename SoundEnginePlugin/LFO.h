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
    float phase = 0.f;

public:
    float get(float rate, float phaseOffset, AkUInt32 sampleRate)
    {
        phase += phaseOffset;
        const float out = static_cast<float>(sin(2.0f * M_PI * phase));
        phase += rate / sampleRate; 
        if (phase >= 1.f)
        {
            phase -= 1.f;
        }
        return out;
    }
};