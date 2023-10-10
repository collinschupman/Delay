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
    AkReal32 mCurrentPhase = 0.f;

public:
    AkReal32 get(AkReal32 rate, AkReal32 phaseOffset, AkUInt32 sampleRate);
};