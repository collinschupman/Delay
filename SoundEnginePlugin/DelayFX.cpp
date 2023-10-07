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

#include "DelayFX.h"
#include "../DelayConfig.h"

#include <AK/AkWwiseSDKVersion.h>

#include <cassert>

AK::IAkPlugin *CreateDelayFX(AK::IAkPluginMemAlloc *in_pAllocator)
{
  return AK_PLUGIN_NEW(in_pAllocator, DelayFX());
}

AK::IAkPluginParam *CreateDelayFXParams(AK::IAkPluginMemAlloc *in_pAllocator)
{
  return AK_PLUGIN_NEW(in_pAllocator, DelayFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(DelayFX, AkPluginTypeEffect, DelayConfig::CompanyID,
                            DelayConfig::PluginID)

DelayFX::DelayFX()
    : m_pParams(nullptr), m_pAllocator(nullptr), m_pContext(nullptr),
      mSampleRate(0),
      mDelayTimeSmoothed(0) {}

DelayFX::~DelayFX()
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

AKRESULT DelayFX::Init(AK::IAkPluginMemAlloc *in_pAllocator,
                       AK::IAkEffectPluginContext *in_pContext,
                       AK::IAkPluginParam *in_pParams,
                       AkAudioFormat &in_rFormat)
{
  m_pParams = (DelayFXParams *)in_pParams;
  m_pAllocator = in_pAllocator;
  m_pContext = in_pContext;
  mSampleRate = in_rFormat.uSampleRate;

  mDelayTimeSamples = mSampleRate * m_pParams->RTPC.fDelayTime;
  mDelayTimeSmoothed = m_pParams->RTPC.fDelayTime;

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

AKRESULT DelayFX::Term(AK::IAkPluginMemAlloc *in_pAllocator)
{
  AK_PLUGIN_DELETE(in_pAllocator, this);
  return AK_Success;
}

AKRESULT DelayFX::Reset() { return AK_Success; }

AKRESULT DelayFX::GetPluginInfo(AkPluginInfo &out_rPluginInfo)
{
  out_rPluginInfo.eType = AkPluginTypeEffect;
  out_rPluginInfo.bIsInPlace = true;
  out_rPluginInfo.bCanProcessObjects = false;
  out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
  return AK_Success;
}

float DelayFX::_smoothParameter(float inParameterSmoothed, float inNewParameter)
{
  return inParameterSmoothed -
         0.0001 * (inParameterSmoothed - inNewParameter);
}

void DelayFX::Execute(AkAudioBuffer *io_pBuffer)
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

AKRESULT DelayFX::TimeSkip(AkUInt32 in_uFrames) { return AK_DataReady; }

float lerp(float sample_x, float sample_x1, float phase)
{
  return (1 - phase) * sample_x + phase * sample_x1;
}
