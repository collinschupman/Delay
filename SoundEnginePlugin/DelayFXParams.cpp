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

#include "DelayFXParams.h"

#include <AK/Tools/Common/AkBankReadHelpers.h>

DelayFXParams::DelayFXParams() {}

DelayFXParams::~DelayFXParams() {}

DelayFXParams::DelayFXParams(const DelayFXParams &in_rParams)
{
  RTPC = in_rParams.RTPC;
  NonRTPC = in_rParams.NonRTPC;
  m_paramChangeHandler.SetAllParamChanges();
}

AK::IAkPluginParam *DelayFXParams::Clone(AK::IAkPluginMemAlloc *in_pAllocator)
{
  return AK_PLUGIN_NEW(in_pAllocator, DelayFXParams(*this));
}

AKRESULT DelayFXParams::Init(AK::IAkPluginMemAlloc *in_pAllocator,
                             const void *in_pParamsBlock,
                             AkUInt32 in_ulBlockSize)
{
  if (in_ulBlockSize == 0)
  {
    RTPC.fDryWetLeft = 0.5f;
    RTPC.fDryWetRight = 0.5f;

    RTPC.fFeedbackLeft = 0.5f;
    RTPC.fFeedbackRight = 0.5f;

    RTPC.fDelayTimeLeft = 2.f;
    RTPC.fDelayTimeRight = 2.f;

    // RTPC.fDepth = 0.5f;
    // RTPC.fRate = 10.0f;
    // RTPC.fPhaseOffset = 0.f;

    NonRTPC.uDelayMode = 0;

    m_paramChangeHandler.SetAllParamChanges();
    return AK_Success;
  }

  return SetParamsBlock(in_pParamsBlock, in_ulBlockSize);
}

AKRESULT DelayFXParams::Term(AK::IAkPluginMemAlloc *in_pAllocator)
{
  AK_PLUGIN_DELETE(in_pAllocator, this);
  return AK_Success;
}

AKRESULT DelayFXParams::SetParamsBlock(const void *in_pParamsBlock,
                                       AkUInt32 in_ulBlockSize)
{
  AKRESULT eResult = AK_Success;
  AkUInt8 *pParamsBlock = (AkUInt8 *)in_pParamsBlock;

  // Read bank data here
  RTPC.fDryWetLeft = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
  RTPC.fDryWetRight = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

  RTPC.fFeedbackLeft = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
  RTPC.fFeedbackRight = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

  RTPC.fDelayTimeLeft = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
  RTPC.fDelayTimeRight = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

  // RTPC.fDepth = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
  // RTPC.fRate = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
  // RTPC.fPhaseOffset = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

  NonRTPC.uDelayMode = READBANKDATA(AkUInt32, pParamsBlock, in_ulBlockSize);

  CHECKBANKDATASIZE(in_ulBlockSize, eResult);
  m_paramChangeHandler.SetAllParamChanges();

  return eResult;
}

AKRESULT DelayFXParams::SetParam(AkPluginParamID in_paramID,
                                 const void *in_pValue,
                                 AkUInt32 in_ulParamSize)
{
  AKRESULT eResult = AK_Success;

  // Handle parameter change here
  switch (in_paramID)
  {
  case PARAM_DRYWET_LEFT_ID:
    RTPC.fDryWetLeft = *((AkReal32 *)in_pValue);
    m_paramChangeHandler.SetParamChange(PARAM_DRYWET_LEFT_ID);
    break;
  case PARAM_DRYWET_RIGHT_ID:
    RTPC.fDryWetRight = *((AkReal32 *)in_pValue);
    m_paramChangeHandler.SetParamChange(PARAM_DRYWET_RIGHT_ID);
    break;
  case PARAM_FEEDBACK_LEFT_ID:
    RTPC.fFeedbackLeft = *((AkReal32 *)in_pValue);
    m_paramChangeHandler.SetParamChange(PARAM_FEEDBACK_LEFT_ID);
    break;
  case PARAM_FEEDBACK_RIGHT_ID:
    RTPC.fFeedbackRight = *((AkReal32 *)in_pValue);
    m_paramChangeHandler.SetParamChange(PARAM_FEEDBACK_RIGHT_ID);
    break;
  case PARAM_DELAY_TIME_LEFT_ID:
    RTPC.fDelayTimeLeft = *((AkReal32 *)in_pValue);
    m_paramChangeHandler.SetParamChange(PARAM_DELAY_TIME_LEFT_ID);
    break;
  case PARAM_DELAY_TIME_RIGHT_ID:
    RTPC.fDelayTimeRight = *((AkReal32 *)in_pValue);
    m_paramChangeHandler.SetParamChange(PARAM_DELAY_TIME_RIGHT_ID);
    break;

  // case PARAM_DEPTH_ID:
  //   RTPC.fDepth = *((AkReal32 *)in_pValue);
  //   m_paramChangeHandler.SetParamChange(PARAM_DEPTH_ID);
  //   break;
  // case PARAM_RATE_ID:
  //   RTPC.fRate = *((AkReal32 *)in_pValue);
  //   m_paramChangeHandler.SetParamChange(PARAM_RATE_ID);
  //   break;
  // case PARAM_PHASE_OFFSET_ID:
  //   RTPC.fPhaseOffset = *((AkReal32 *)in_pValue);
  //   m_paramChangeHandler.SetParamChange(PARAM_PHASE_OFFSET_ID);
  //   break;
  case PARAM_DELAY_MODE_ID:
    NonRTPC.uDelayMode = *((AkUInt32 *)in_pValue);
    m_paramChangeHandler.SetParamChange(PARAM_DELAY_MODE_ID);
    break;
  default:
    eResult = AK_InvalidParameter;
    break;
  }

  return eResult;
}
