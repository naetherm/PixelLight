/*********************************************************\
 *  File: SNMPostProcessEmbossed.cpp                     *
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
#include "PLPostProcessEffects/SNMPostProcessEmbossed.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPostProcessEmbossed)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessEmbossed::SNMPostProcessEmbossed(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	StartColor(this),
	ColorScale(this),
	PixelKernel0(this),
	PixelKernel1(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/Embossed.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessEmbossed::~SNMPostProcessEmbossed()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessEmbossed::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // StartColor
		Parameter *pParameter = GetParameter("StartColor");
		if (pParameter)
			pParameter->SetValue3fv(StartColor.Get());
	}

	{ // ColorScale
		Parameter *pParameter = GetParameter("ColorScale");
		if (pParameter)
			pParameter->SetValue2fv(ColorScale.Get());
	}

	{ // PixelKernel[0]
		Parameter *pParameter = GetParameter("PixelKernel[0]");
		if (pParameter)
			pParameter->SetValue2fv(PixelKernel0.Get());
	}

	{ // PixelKernel[1]
		Parameter *pParameter = GetParameter("PixelKernel[1]");
		if (pParameter)
			pParameter->SetValue2fv(PixelKernel1.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
