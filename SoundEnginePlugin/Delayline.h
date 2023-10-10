#pragma once

#include "CircularBuffer.h"
#include "Helpers.h"

#include <AK/SoundEngine/Common/AkNumeralTypes.h>
#include <AK/SoundEngine/Common/AKCommonDefs.h>

#include <memory>

struct Delayline
{
private:
    std::unique_ptr<CircularBuffer> circularBuffer = nullptr;

public:
    AKRESULT Init(AkUInt32 inSampleRate, AkReal32 maxDelayTime);

    void write(AkReal32 inValue);

    AkReal32 read();

    void updateReadHead(AkReal32 delayTime);

    void updateWriteHead();
};