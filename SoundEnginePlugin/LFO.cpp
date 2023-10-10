#include "LFO.h"

AkReal32 LFO::get(AkReal32 rate, AkReal32 phaseOffset, AkUInt32 sampleRate)
{
    AkReal32 offsetPhase = mCurrentPhase + phaseOffset;
    if (offsetPhase >= 1.f)
    {
        offsetPhase -= 1.f;
    }
    const AkReal32 out = static_cast<AkReal32>(sin(2.f * M_PI * offsetPhase));
    mCurrentPhase += rate / sampleRate;
    if (mCurrentPhase >= 1.f)
    {
        mCurrentPhase -= 1.f;
    }
    return out;
}