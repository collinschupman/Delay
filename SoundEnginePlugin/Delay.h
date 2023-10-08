#pragma once

#include "Delayline.h"
#include "Helpers.h"

#include <AK/SoundEngine/Common/AKTypes.h>
#include <AK/SoundEngine/Common/AKCommonDefs.h>

#include <array>

struct Delay
{
private:
    std::array<Delayline, 2> mDelaylines;
    AkUInt32 mSampleRate = 0;
    float mDelayTimeSmoothed = 0.f;
    float mDelayTimeSamples = 0.f;

public:
    AKRESULT Init(AkUInt32 inSampleRate, float delayTime, float maxDelayTime);

    void Execute(AkAudioBuffer *io_pBuffer, AkReal32 pDelayTime, AkReal32 pFeedback, AkReal32 pDryWet);
};