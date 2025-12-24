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

AK::IAkPlugin* CreateDelayFX(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, DelayFX());
}

AK::IAkPluginParam* CreateDelayFXParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, DelayFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(DelayFX, AkPluginTypeEffect, DelayConfig::CompanyID,
                            DelayConfig::PluginID)

DelayFX::DelayFX() : m_pParams(nullptr), m_pAllocator(nullptr), m_pContext(nullptr) {}

DelayFX::~DelayFX() {}

AKRESULT DelayFX::Init(AK::IAkPluginMemAlloc* in_pAllocator,
                       AK::IAkEffectPluginContext* in_pContext, AK::IAkPluginParam* in_pParams,
                       AkAudioFormat& in_rFormat)
{
    m_pParams = (DelayFXParams*) in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;
    mSampleRate = in_rFormat.uSampleRate;

    mDelayModule.Init(mSampleRate, m_pParams->RTPC.fDelayTimeLeft, MAX_DELAY_TIME);

    return AK_Success;
}

AKRESULT DelayFX::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT DelayFX::Reset()
{
    return AK_Success;
}

AKRESULT DelayFX::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeEffect;
    out_rPluginInfo.bIsInPlace = true;
    out_rPluginInfo.bCanProcessObjects = false;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

void DelayFX::Execute(AkAudioBuffer* io_pBuffer)
{
    std::array<Delay::InDelayParams, 2> params;
    params[0] = {m_pParams->RTPC.fDelayTimeLeft, m_pParams->RTPC.fFeedbackLeft,
                 m_pParams->RTPC.fDryWetLeft};
    params[1] = {m_pParams->RTPC.fDelayTimeRight, m_pParams->RTPC.fFeedbackRight,
                 m_pParams->RTPC.fDryWetRight};
    mDelayModule.Execute(io_pBuffer, params, m_pParams->NonRTPC.uDelayMode);
}

AKRESULT DelayFX::TimeSkip(AkUInt32 in_uFrames)
{
    return AK_DataReady;
}