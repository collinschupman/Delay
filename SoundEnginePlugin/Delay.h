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

    // AkReal32 mDelayTimeSmoothed = 0.f;
    // AkReal32 mDelayTimeSamples = 0.f;
    // AkReal32 mFeedback = 0.f;

    struct LocalDelayParams
    {
        AkReal32 mDelayTimeSmoothed = 0.f;
        AkReal32 mDelayTimeSamples = 0.f;
        AkReal32 mFeedback = 0.f;
    };

    std::array<LocalDelayParams, 2> mParams;

public:
    AKRESULT Init(AkUInt32 inSampleRate, AkReal32 delayTime, AkReal32 maxDelayTime);

    struct InDelayParams
    {
        AkReal32 pDelayTime;
        AkReal32 pFeedback;
        AkReal32 pDryWet;
    };

    void Execute(AkAudioBuffer *io_pBuffer, std::array<InDelayParams,2>& inParams, AkUInt32 uDelayMode);
};