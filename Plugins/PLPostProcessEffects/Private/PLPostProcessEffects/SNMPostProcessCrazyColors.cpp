/*********************************************************\
 *  File: SNMPostProcessCrazyColors.cpp                  *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Timing.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLCompositing/Shaders/PostProcessing/PostProcessManager.h>
#include "PLPostProcessEffects/SNMPostProcessCrazyColors.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPostProcessCrazyColors)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessCrazyColors::SNMPostProcessCrazyColors(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	ColorScaleX(this),
	ColorSpeedX(this),
	ColorOffsetX(this),
	ColorScaleY(this),
	ColorSpeedY(this),
	ColorOffsetY(this),
	Filename(this),
	SlotOnSceneNodeUpdate(this),
	m_fTimer(0.0f)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/CrazyColors.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessCrazyColors::~SNMPostProcessCrazyColors()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNodeModifier functions  ]
//[-------------------------------------------------------]
void SNMPostProcessCrazyColors::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(SlotOnSceneNodeUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(SlotOnSceneNodeUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNMPostProcessCrazyColors::OnSceneNodeUpdate()
{
	m_fTimer += Timing::GetInstance()->GetTimeDifference();
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessCrazyColors::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // Timer
		Parameter *pParameter = GetParameter("Timer");
		if (pParameter)
			pParameter->SetValue1f(m_fTimer);
	}

	{ // ColorScaleX
		Parameter *pParameter = GetParameter("ColorScaleX");
		if (pParameter)
			pParameter->SetValue3fv(ColorScaleX.Get());
	}

	{ // ColorSpeedX
		Parameter *pParameter = GetParameter("ColorSpeedX");
		if (pParameter)
			pParameter->SetValue3fv(ColorSpeedX.Get());
	}

	{ // ColorOffsetX
		Parameter *pParameter = GetParameter("ColorOffsetX");
		if (pParameter)
			pParameter->SetValue3fv(ColorOffsetX.Get());
	}

	{ // ColorScaleY
		Parameter *pParameter = GetParameter("ColorScaleY");
		if (pParameter)
			pParameter->SetValue3fv(ColorScaleY.Get());
	}

	{ // ColorSpeedY
		Parameter *pParameter = GetParameter("ColorSpeedY");
		if (pParameter)
			pParameter->SetValue3fv(ColorSpeedY.Get());
	}

	{ // ColorOffsetY
		Parameter *pParameter = GetParameter("ColorOffsetX");
		if (pParameter)
			pParameter->SetValue3fv(ColorOffsetX.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
