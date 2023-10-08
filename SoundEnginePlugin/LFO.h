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
    float get(float lfoRate, AkUInt32 sampleRate)
    {
        float lfoOut = sin(2 * M_PI * phase);
        phase += lfoRate * sampleRate;
        if (phase >= 1.f)
        {
            phase -= 1.f;
        }
        return sin(2 * M_PI * phase);
    }
};