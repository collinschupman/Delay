#pragma once

#include "Delayline.h"
#include "LFO.h"
#include "Helpers.h"

#include <AK/SoundEngine/Common/AKTypes.h>
#include <AK/SoundEngine/Common/AKCommonDefs.h>

#include <array>

struct ChorusFlanger
{
private:
    std::array<Delayline, 2> mDelaylines;
    AkUInt32 mSampleRate = 0;
    float mDelayTimeSmoothed = 0.f;
    float mDelayTimeSamples = 0.f;
    LFO mLFO;

public:
    AKRESULT Init(AkUInt32 inSampleRate, float delayTime, float maxDelayTime);

    void Execute(AkAudioBuffer *io_pBuffer, AkReal32 pDelayTime, AkReal32 pFeedback, AkReal32 pDryWet);
};