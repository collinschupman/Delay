#pragma once

#include "CircularBuffer.h"
#include "Helpers.h"

#include <AK/SoundEngine/Common/AkNumeralTypes.h>
#include <AK/SoundEngine/Common/AKCommonDefs.h>

#include <memory>

struct Delayline
{
private:
    float mFeedback = 0.f;
    std::unique_ptr<CircularBuffer> circularBuffer = nullptr;

public:
    AKRESULT Init(AkUInt32 inSampleRate, float maxDelayTime);

    void write(float inValue);

    void process(AkReal32 *pBuf, AkUInt16 pBufPos, float inFeedback, float dryWet);

    void updateReadHead(float delayTime);

    void updateWriteHead();
};