/*********************************************************\
 *  File: SNMPostProcessToneMap.cpp                      *
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
#include "PLPostProcessEffects/SNMPostProcessToneMap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPostProcessToneMap, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(ExposureLevel,	float,				1.0f,								ReadWrite,	"Exposure level",						"")
	pl_attribute_metadata(GammaLevel,		float,				1.0f,								ReadWrite,	"Gamma level",							"")
	pl_attribute_metadata(DeFogLevel,		float,				0.0f,								ReadWrite,	"De-fog level",							"")
	pl_attribute_metadata(FogColor,			PLGraphics::Color3,	PLGraphics::Color3::White,			ReadWrite,	"Fog color",							"")
		// Overloaded SNMPostProcess attributes
	pl_attribute_metadata(Filename,			PLCore::String,		"Data/PostProcesses/ToneMap.pp",	ReadWrite,	"Filename of the post process to use",	"Type='PostProcess'")
pl_class_metadata_end(SNMPostProcessToneMap)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessToneMap::SNMPostProcessToneMap(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	ExposureLevel(this),
	GammaLevel(this),
	DeFogLevel(this),
	FogColor(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/ToneMap.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessToneMap::~SNMPostProcessToneMap()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessToneMap::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // ExposureLevel
		Parameter *pParameter = GetParameter("ExposureLevel");
		if (pParameter)
			pParameter->SetValue1f(ExposureLevel.Get());
	}

	{ // GammaLevel
		Parameter *pParameter = GetParameter("GammaLevel");
		if (pParameter)
			pParameter->SetValue1f(GammaLevel.Get());
	}

	{ // DeFogLevel
		Parameter *pParameter = GetParameter("DeFogLevel");
		if (pParameter)
			pParameter->SetValue1f(DeFogLevel.Get());
	}

	{ // FogColor
		Parameter *pParameter = GetParameter("FogColor");
		if (pParameter)
			pParameter->SetValue3fv(FogColor.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
