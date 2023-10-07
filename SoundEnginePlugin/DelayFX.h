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

#define MAX_DELAY_TIME 2

float lerp(float sample_x, float sample_x1, float phase);

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

  float _smoothParameter(float inParameterSmoothed, float inNewParameter);

  float mDelayTimeSmoothed;

  float mDelayTimeSamples;

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

  std::array<DelayFX::Delayline, 2> mDelaylines;
};

#endif // DelayFX_H
