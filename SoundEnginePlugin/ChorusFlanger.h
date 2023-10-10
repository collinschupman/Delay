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
    std::array<LFO, 2> mLFOs;
    std::array<AkReal32, 2> mOffsets;
    AkReal32 pDepthSmooth;
    AkReal32 pRateSmooth;
    AkReal32 pPhaseOffsetSmooth;
    AkReal32 pFeedbackSmooth;
    AkReal32 pDryWetSmooth;

public:
    AKRESULT Init(AkUInt32 inSampleRate, AkReal32 delayTime, AkReal32 maxDelayTime);

    void Execute(AkAudioBuffer *io_pBuffer, AkReal32 pDepth, AkReal32 pRate, AkReal32 pPhaseOffset, AkReal32 pFeedback, AkReal32 pDryWet, AkUInt32 pType);
};