/*********************************************************\
 *  File: SNMPostProcessEdgeDetect.cpp                   *
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
#include "PLPostProcessEffects/SNMPostProcessEdgeDetect.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPostProcessEdgeDetect)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessEdgeDetect::SNMPostProcessEdgeDetect(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	LuminanceConvert(this),
	PixelKernel0(this),
	PixelKernel1(this),
	PixelKernel2(this),
	PixelKernel3(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/EdgeDetect.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessEdgeDetect::~SNMPostProcessEdgeDetect()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessEdgeDetect::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // LuminanceConvert
		Parameter *pParameter = GetParameter("LuminanceConvert");
		if (pParameter)
			pParameter->SetValue3fv(LuminanceConvert.Get());
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

	{ // PixelKernel[2]
		Parameter *pParameter = GetParameter("PixelKernel[2]");
		if (pParameter)
			pParameter->SetValue2fv(PixelKernel2.Get());
	}

	{ // PixelKernel[3]
		Parameter *pParameter = GetParameter("PixelKernel[3]");
		if (pParameter)
			pParameter->SetValue2fv(PixelKernel3.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
