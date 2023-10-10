#pragma once

#include "CircularBuffer.h"
#include "Helpers.h"

#include <AK/SoundEngine/Common/AkNumeralTypes.h>
#include <AK/SoundEngine/Common/AKCommonDefs.h>

#include <memory>

struct Delayline
{
private:
    AkReal32 mFeedback = 0.f;
    std::unique_ptr<CircularBuffer> circularBuffer = nullptr;

public:
    AKRESULT Init(AkUInt32 inSampleRate, AkReal32 maxDelayTime);

    void write(AkReal32 inValue);

    void process(AkReal32 *pBuf, AkUInt16 pBufPos, AkReal32 inFeedback, AkReal32 dryWet);

    void updateReadHead(AkReal32 delayTime);

    void updateWriteHead();
};