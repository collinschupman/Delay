#pragma once

#include <AK/SoundEngine/Common/AKTypes.h>
#include <AK/SoundEngine/Common/AKCommonDefs.h>

#include <memory>

struct CircularBuffer
{
public:
    CircularBuffer(AkReal32 inSampleRate, AkReal32 maxDelayTime);
    void Init();
    void write(AkReal32 inValue);

    void updateWriteHead();

    void updateReadHead(AkReal32 delayTime);

    AkReal32 getReadHead() const;
    AkReal32 getNextReadHead() const;

    AkReal32 getValue(unsigned position) const;

private:
    std::size_t length = 0;
    AkReal32 readHead = 0.f;
    unsigned writeHead = 0;
    std::unique_ptr<AkReal32[]> buffer = nullptr;
};