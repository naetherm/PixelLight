/*********************************************************\
 *  File: SNMPostProcessDepthOfField.cpp                 *
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
#include "PLCompositing/Shaders/PostProcessing/PostProcessManager.h"
#include "PLCompositing/SceneNodeModifiers/SNMPostProcessDepthOfField.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPostProcessDepthOfField)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessDepthOfField::SNMPostProcessDepthOfField(SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	NearBlurDepth(this),
	FocalPlaneDepth(this),
	FarBlurDepth(this),
	BlurrinessCutoff(this),
	BloomScale(this),
	Strength(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/DepthOfField.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessDepthOfField::~SNMPostProcessDepthOfField()
{
}


//[-------------------------------------------------------]
//[ Public virtual SNMPostProcess functions               ]
//[-------------------------------------------------------]
void SNMPostProcessDepthOfField::SetParameters()
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

	{ // NearBlurDepth
		Array<Parameter*> lstParameters;
		if (GetParameters("NearBlurDepth", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(NearBlurDepth);
		}
	}

	{ // FocalPlaneDepth
		Array<Parameter*> lstParameters;
		if (GetParameters("FocalPlaneDepth", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(FocalPlaneDepth);
		}
	}
	
	{ // FarBlurDepth
		Array<Parameter*> lstParameters;
		if (GetParameters("FarBlurDepth", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(FarBlurDepth);
		}
	}
	
	{ // BlurrinessCutoff
		Array<Parameter*> lstParameters;
		if (GetParameters("BlurrinessCutoff", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(BlurrinessCutoff);
		}
	}

	{ // BloomScale
		Array<Parameter*> lstParameters;
		if (GetParameters("BloomScale", lstParameters)) {
			Iterator<Parameter*> cIterator = lstParameters.GetIterator();
			while (cIterator.HasNext())
				cIterator.Next()->SetValue1f(BloomScale);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
