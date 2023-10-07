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

AK::IAkPlugin *CreateDelayFX(AK::IAkPluginMemAlloc *in_pAllocator) {
  return AK_PLUGIN_NEW(in_pAllocator, DelayFX());
}

AK::IAkPluginParam *CreateDelayFXParams(AK::IAkPluginMemAlloc *in_pAllocator) {
  return AK_PLUGIN_NEW(in_pAllocator, DelayFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(DelayFX, AkPluginTypeEffect, DelayConfig::CompanyID,
                            DelayConfig::PluginID)

DelayFX::DelayFX()
    : m_pParams(nullptr), m_pAllocator(nullptr), m_pContext(nullptr),
      mCircularBufferLength(0), mCircularBuffers(nullptr), mSampleRate(0),
      mDelayTimeSmoothed(0) {}

DelayFX::~DelayFX() {
  mCircularBufferLength = 0;

  if (mCircularBuffers != nullptr) {
    for (int i = 0; i < 2; ++i) {
      delete[] mCircularBuffers[i].buffer;
    }
    delete[] mCircularBuffers;
    mCircularBuffers = nullptr;
  }
}

AKRESULT DelayFX::Init(AK::IAkPluginMemAlloc *in_pAllocator,
                       AK::IAkEffectPluginContext *in_pContext,
                       AK::IAkPluginParam *in_pParams,
                       AkAudioFormat &in_rFormat) {
  m_pParams = (DelayFXParams *)in_pParams;
  m_pAllocator = in_pAllocator;
  m_pContext = in_pContext;
  mSampleRate = in_rFormat.uSampleRate;

  mCircularBufferLength = mSampleRate * MAX_DELAY_TIME;
  mDelayTimeSamples = mSampleRate * m_pParams->RTPC.fDelayTime;
  feedback = 0.f;
  mDelayTimeSmoothed = m_pParams->RTPC.fDelayTime;

  // init if needed
  if (mCircularBuffers == nullptr) {
    mCircularBuffers = new CircularBuffer[2];

    for (int i = 0; i < 2; ++i) {
      mCircularBuffers[i].buffer = new float[mCircularBufferLength];
    }
  }

  // zero out
  for (int i = 0; i < 2; ++i) {
    for (AkUInt32 j = 0; j < mCircularBufferLength; ++j) {
      mCircularBuffers[i].buffer[j] = 0.0f;
      mCircularBuffers->readHead = 0;
      mCircularBuffers->writeHead = 0;
    }
  }

  return AK_Success;
}

AKRESULT DelayFX::Term(AK::IAkPluginMemAlloc *in_pAllocator) {
  AK_PLUGIN_DELETE(in_pAllocator, this);
  return AK_Success;
}

AKRESULT DelayFX::Reset() { return AK_Success; }

AKRESULT DelayFX::GetPluginInfo(AkPluginInfo &out_rPluginInfo) {
  out_rPluginInfo.eType = AkPluginTypeEffect;
  out_rPluginInfo.bIsInPlace = true;
  out_rPluginInfo.bCanProcessObjects = false;
  out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
  return AK_Success;
}

void DelayFX::Execute(AkAudioBuffer *io_pBuffer) {
  const AkUInt32 uNumChannels = io_pBuffer->NumChannels();

  AkUInt16 uFramesProcessed;

  // TD: need to make this one loop for smoothing to work correctly
  for (AkUInt32 channel = 0; channel < uNumChannels; ++channel) {
    AkReal32 *AK_RESTRICT pBuf =
        (AkReal32 * AK_RESTRICT) io_pBuffer->GetChannel(channel);
    CircularBuffer *circularBuffer = &mCircularBuffers[channel];

    uFramesProcessed = 0;
    while (uFramesProcessed < io_pBuffer->uValidFrames) {
      mDelayTimeSmoothed =
          mDelayTimeSmoothed -
          0.0001 * (mDelayTimeSmoothed - m_pParams->RTPC.fDelayTime);
      mDelayTimeSamples = mSampleRate * mDelayTimeSmoothed;

      // update the circular buffer
      circularBuffer->buffer[circularBuffer->writeHead] =
          pBuf[uFramesProcessed] + feedback;

      // update the read head
      circularBuffer->readHead = circularBuffer->writeHead - mDelayTimeSamples;
      if (circularBuffer->readHead < 0) {
        circularBuffer->readHead += mCircularBufferLength;
      }

      int readHead_x = (int)circularBuffer->readHead;
      int readHead_x1 = readHead_x + 1;
      if (readHead_x1 >= mCircularBufferLength) {
        readHead_x1 - mCircularBufferLength;
      }
      float readHeadFloat = circularBuffer->readHead - readHead_x;

      float delayedSample =
          lerp(circularBuffer->buffer[readHead_x],
               circularBuffer->buffer[readHead_x1], readHeadFloat);

      feedback = delayedSample * m_pParams->RTPC.fFeedback;

      pBuf[uFramesProcessed] =
          delayedSample * m_pParams->RTPC.fDryWet +
          pBuf[uFramesProcessed] * (1.f - m_pParams->RTPC.fDryWet);

      // update the write head
      ++circularBuffer->writeHead;
      if (circularBuffer->writeHead >= mCircularBufferLength) {
        circularBuffer->writeHead = 0;
      }

      // increment the current frame
      ++uFramesProcessed;
    }
  }
}

AKRESULT DelayFX::TimeSkip(AkUInt32 in_uFrames) { return AK_DataReady; }

float DelayFX::lerp(float sample_x, float sample_x1, float phase) {
  return (1 - phase) * sample_x + phase * sample_x1;
}
