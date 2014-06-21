/*********************************************************\
 *  File: SNMPostProcessBloomToneMap.cpp                 *
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
#include <PLRenderer/Material/Parameter.h>
#include <PLCompositing/Shaders/PostProcessing/PostProcessManager.h>
#include "PLPostProcessEffects/SNMPostProcessBloomToneMap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPostProcessBloomToneMap)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessBloomToneMap::SNMPostProcessBloomToneMap(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	BloomScale(this),
	Strength(this),
	ExposureLevel(this),
	GammaLevel(this),
	DeFogLevel(this),
	FogColor(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/BloomToneMap.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessBloomToneMap::~SNMPostProcessBloomToneMap()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessBloomToneMap::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // Strength
		// Update scale down parameters
		Parameter *pParameter = GetParameter("ScaleX", 0);
		if (pParameter)
			pParameter->SetValue1f(1.0f/Strength.Get().x);
		pParameter = GetParameter("ScaleY", 0);
		if (pParameter)
			pParameter->SetValue1f(1.0f/Strength.Get().y);

		// Update scale up parameters
		pParameter = GetParameter("ScaleX", 3);
		if (pParameter)
			pParameter->SetValue1f(Strength.Get().x);
		pParameter = GetParameter("ScaleY", 3);
		if (pParameter)
			pParameter->SetValue1f(Strength.Get().y);
	}

	{ // BloomScale
		Array<Parameter*> lstParameters;
		if (GetParameters("BloomScale", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(BloomScale.Get());
		}
	}

	{ // ExposureLevel
		Array<Parameter*> lstParameters;
		if (GetParameters("ExposureLevel", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(ExposureLevel.Get());
		}
	}

	{ // GammaLevel
		Array<Parameter*> lstParameters;
		if (GetParameters("GammaLevel", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(GammaLevel.Get());
		}
	}

	{ // DeFogLevel
		Array<Parameter*> lstParameters;
		if (GetParameters("DeFogLevel", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(DeFogLevel.Get());
		}
	}

	{ // FogColor
		Array<Parameter*> lstParameters;
		if (GetParameters("FogColor", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue3fv(FogColor.Get());
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
