/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

#ifndef DelayFX_H
#define DelayFX_H

#include "DelayFXParams.h"
#include <memory>
#include <array>
#include <cassert>

#define MAX_DELAY_TIME 2

// helpers
float lerp(float sample_x, float sample_x1, float phase);
float _smoothParameter(float inParameterSmoothed, float inNewParameter);

class DelayFX : public AK::IAkInPlaceEffectPlugin
{
public:
  DelayFX();
  ~DelayFX();

  AKRESULT Init(AK::IAkPluginMemAlloc *in_pAllocator,
                AK::IAkEffectPluginContext *in_pContext,
                AK::IAkPluginParam *in_pParams,
                AkAudioFormat &in_rFormat) override;

  AKRESULT Term(AK::IAkPluginMemAlloc *in_pAllocator) override;

  AKRESULT Reset() override;

  AKRESULT GetPluginInfo(AkPluginInfo &out_rPluginInfo) override;

  void Execute(AkAudioBuffer *io_pBuffer) override;

  AKRESULT TimeSkip(AkUInt32 in_uFrames) override;

private:
  DelayFXParams *m_pParams;
  AK::IAkPluginMemAlloc *m_pAllocator;
  AK::IAkEffectPluginContext *m_pContext;

  AkUInt32 mSampleRate;

  struct CircularBuffer
  {
    int length = 0;
    float readHead = 0.f;
    int writeHead = 0;
    float *buffer = nullptr;
  };

  struct Delayline
  {
  private:
    float mFeedback = 0.f;

  public:
    std::unique_ptr<CircularBuffer> circularBuffer = nullptr;
    void write(float inValue)
    {
      circularBuffer->buffer[circularBuffer->writeHead] =
          inValue + mFeedback;
    }

    void process(AkReal32 *AK_RESTRICT pBuf, AkUInt16 pBufPos, float inFeedback, float dryWet)
    {
      int readHead_x = (int)circularBuffer->readHead;
      int readHead_x1 = readHead_x + 1;
      if (readHead_x1 >= circularBuffer->length)
      {
        readHead_x1 - circularBuffer->length;
      }
      float readHeadFloat = circularBuffer->readHead - readHead_x;

      float delayedSample =
          lerp(circularBuffer->buffer[readHead_x],
               circularBuffer->buffer[readHead_x1], readHeadFloat);

      mFeedback = delayedSample * inFeedback;

      pBuf[pBufPos] =
          delayedSample * dryWet +
          pBuf[pBufPos] * (1.f - dryWet);
    }

    void updateReadHead(float delayTime)
    {
      circularBuffer->readHead = circularBuffer->writeHead - delayTime;
      if (circularBuffer->readHead < 0)
      {
        circularBuffer->readHead += circularBuffer->length;
      }
    }

    void updateWriteHead()
    {
      ++circularBuffer->writeHead;
      if (circularBuffer->writeHead >= circularBuffer->length)
      {
        circularBuffer->writeHead = 0;
      }
    }
  };

  struct Delay
  {
  private:
    std::array<DelayFX::Delayline, 2> mDelaylines;
    AkUInt32 mSampleRate = 0;
    float mDelayTimeSmoothed = 0.f;
    float mDelayTimeSamples = 0.f;

  public:
    ~Delay()
    {
      for (Delayline &delayLine : mDelaylines)
      {
        if (delayLine.circularBuffer)
        {
          delete[] delayLine.circularBuffer->buffer;
          delayLine.circularBuffer->buffer = nullptr;
        }
      }
    }

    AKRESULT Init(AkUInt32 inSampleRate, float delayTime)
    {
      mSampleRate = inSampleRate;

      mDelayTimeSamples = mSampleRate * delayTime;
      mDelayTimeSmoothed = delayTime;

      for (Delayline &delayLine : mDelaylines)
      {

        if (!delayLine.circularBuffer)
        {
          delayLine.circularBuffer = std::make_unique<CircularBuffer>();
          delayLine.circularBuffer->length = mSampleRate * MAX_DELAY_TIME;
          delayLine.circularBuffer->buffer = new float[delayLine.circularBuffer->length];
        }

        for (AkUInt32 j = 0; j < delayLine.circularBuffer->length; ++j)
        {
          delayLine.circularBuffer->buffer[j] = 0.0f;
        }
        delayLine.circularBuffer->readHead = 0;
        delayLine.circularBuffer->writeHead = 0;
      }

      return AK_Success;
    }

    void Execute(AkAudioBuffer *io_pBuffer, DelayFXParams *m_pParams)
    {
      assert(io_pBuffer->NumChannels() == mDelaylines.size());

      AkUInt16 numFramesProcessed = 0;
      while (numFramesProcessed < io_pBuffer->uValidFrames)
      {
        mDelayTimeSmoothed = _smoothParameter(mDelayTimeSmoothed, m_pParams->RTPC.fDelayTime);
        mDelayTimeSamples = mSampleRate * mDelayTimeSmoothed;

        for (int i = 0; i < io_pBuffer->NumChannels(); i++)
        {
          Delayline &delayLine = mDelaylines[i];

          AkReal32 *AK_RESTRICT pBuf =
              (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(i);

          delayLine.write(pBuf[numFramesProcessed]);

          delayLine.updateReadHead(mDelayTimeSamples);

          delayLine.process(pBuf, numFramesProcessed, m_pParams->RTPC.fFeedback, m_pParams->RTPC.fDryWet);

          delayLine.updateWriteHead();
        }

        ++numFramesProcessed;
      }
    }
  };

  Delay mDelayModule;
};

#endif // DelayFX_H
