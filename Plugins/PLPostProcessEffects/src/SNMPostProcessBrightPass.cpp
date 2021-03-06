/*********************************************************\
 *  File: SNMPostProcessBrightPass.cpp                   *
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
#include "PLPostProcessEffects/SNMPostProcessBrightPass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPostProcessBrightPass, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(Luminance,	float,			0.08f,								ReadWrite,	"Luminance",							"")
	pl_attribute_metadata(MiddleGray,	float,			0.18f,								ReadWrite,	"Middle gray value",					"")
	pl_attribute_metadata(WhiteCutoff,	float,			0.8f,								ReadWrite,	"White cutoff",							"")
		// Overloaded SNMPostProcess attributes
	pl_attribute_metadata(Filename,		PLCore::String,	"Data/PostProcesses/BrightPass.pp",	ReadWrite,	"Filename of the post process to use",	"Type='PostProcess'")
pl_class_metadata_end(SNMPostProcessBrightPass)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessBrightPass::SNMPostProcessBrightPass(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	Luminance(this),
	MiddleGray(this),
	WhiteCutoff(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/BrightPass.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessBrightPass::~SNMPostProcessBrightPass()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessBrightPass::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	// Luminance
	Parameter *pParameter = GetParameter("Luminance", 0);
	if (pParameter)
		pParameter->SetValue1f(Luminance.Get());

	// MiddleGray
	pParameter = GetParameter("MiddleGray", 0);
	if (pParameter)
		pParameter->SetValue1f(MiddleGray.Get());

	// WhiteCutoff
	pParameter = GetParameter("WhiteCutoff", 0);
	if (pParameter)
		pParameter->SetValue1f(WhiteCutoff.Get());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
