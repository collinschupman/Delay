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

#ifndef DelayFXParams_H
#define DelayFXParams_H

#include <AK/Plugin/PluginServices/AkFXParameterChangeHandler.h>
#include <AK/SoundEngine/Common/IAkPlugin.h>

static const AkPluginParamID PARAM_DRYWET_ID = 0;
static const AkPluginParamID PARAM_FEEDBACK_ID = 1;
static const AkPluginParamID PARAM_DELAY_TIME_ID = 2;
static const AkPluginParamID PARAM_DEPTH_ID = 3;
static const AkPluginParamID PARAM_RATE_ID = 4;
static const AkPluginParamID PARAM_PHASE_OFFSET_ID = 5;

static const AkUInt32 NUM_PARAMS = 6;

struct DelayRTPCParams
{
  AkReal32 fDryWet;
  AkReal32 fFeedback;
  AkReal32 fDelayTime;
  AkReal32 fDepth;
  AkReal32 fRate;
  AkReal32 fPhaseOffset;
};

struct DelayNonRTPCParams
{
};

struct DelayFXParams : public AK::IAkPluginParam
{
  DelayFXParams();
  DelayFXParams(const DelayFXParams &in_rParams);

  ~DelayFXParams();

  IAkPluginParam *Clone(AK::IAkPluginMemAlloc *in_pAllocator) override;

  AKRESULT Init(AK::IAkPluginMemAlloc *in_pAllocator,
                const void *in_pParamsBlock, AkUInt32 in_ulBlockSize) override;

  AKRESULT Term(AK::IAkPluginMemAlloc *in_pAllocator) override;

  AKRESULT SetParamsBlock(const void *in_pParamsBlock,
                          AkUInt32 in_ulBlockSize) override;

  AKRESULT SetParam(AkPluginParamID in_paramID, const void *in_pValue,
                    AkUInt32 in_ulParamSize) override;

  AK::AkFXParameterChangeHandler<NUM_PARAMS> m_paramChangeHandler;

  DelayRTPCParams RTPC;
  DelayNonRTPCParams NonRTPC;
};

#endif // DelayFXParams_H
