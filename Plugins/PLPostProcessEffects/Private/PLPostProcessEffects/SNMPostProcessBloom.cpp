/*********************************************************\
 *  File: SNMPostProcessBloom.cpp                        *
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
#include "PLPostProcessEffects/SNMPostProcessBloom.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPostProcessBloom)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessBloom::SNMPostProcessBloom(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	Luminance(this),
	MiddleGray(this),
	WhiteCutoff(this),
	BloomScale(this),
	Strength(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/Bloom.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessBloom::~SNMPostProcessBloom()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessBloom::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // Strength
		// Update scale down parameters
		Parameter *pParameter = GetParameter("ScaleX", 0);
		if (pParameter)
			pParameter->SetValue1f(1.0f/Strength.Get().x);
		pParameter = GetParameter( "ScaleY", 0);
		if (pParameter)
			pParameter->SetValue1f(1.0f/Strength.Get().y);

		// Update scale up parameters
		pParameter = GetParameter("ScaleX", 6);
		if (pParameter)
			pParameter->SetValue1f(Strength.Get().x);
		pParameter = GetParameter("ScaleY", 6);
		if (pParameter)
			pParameter->SetValue1f(Strength.Get().y);
	}

	{ // Luminance
		Array<Parameter*> lstParameters;
		if (GetParameters("Luminance", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(Luminance.Get());
		}
	}

	{ // MiddleGray
		Array<Parameter*> lstParameters;
		if (GetParameters("MiddleGray", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(MiddleGray.Get());
		}
	}

	{ // WhiteCutoff
		Array<Parameter*> lstParameters;
		if (GetParameters("WhiteCutoff", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(WhiteCutoff.Get());
		}
	}

	{ // BloomScale
		Array<Parameter*> lstParameters;
		if (GetParameters("BloomScale", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(BloomScale.Get());
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
