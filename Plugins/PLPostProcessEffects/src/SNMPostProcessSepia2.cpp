/*********************************************************\
 *  File: SNMPostProcessSepia2.cpp                       *
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
#include "PLPostProcessEffects/SNMPostProcessSepia2.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPostProcessSepia2, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(TintColor0,	PLGraphics::Color4,	PLGraphics::Color4(0.299f, 0.587f, 0.184f,  0.31f),	ReadWrite,	"Tint color 0 (red component)",			"")
	pl_attribute_metadata(TintColor1,	PLGraphics::Color4,	PLGraphics::Color4(0.299f, 0.587f, 0.184f,  0.17f),	ReadWrite,	"Tint color 1 (green component)",		"")
	pl_attribute_metadata(TintColor2,	PLGraphics::Color4,	PLGraphics::Color4(0.299f, 0.587f, 0.184f, -0.09f),	ReadWrite,	"Tint color 2 (blue component)",		"")
		// Overloaded SNMPostProcess attributes
	pl_attribute_metadata(Filename,		PLCore::String,		"Data/PostProcesses/Sepia2.pp",						ReadWrite,	"Filename of the post process to use",	"Type='PostProcess'")
pl_class_metadata_end(SNMPostProcessSepia2)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessSepia2::SNMPostProcessSepia2(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	TintColor0(this),
	TintColor1(this),
	TintColor2(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/Sepia2.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessSepia2::~SNMPostProcessSepia2()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessSepia2::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // TintColor[0]
		Parameter *pParameter = GetParameter("TintColor[0]");
		if (pParameter)
			pParameter->SetValue4fv(TintColor0.Get());
	}

	{ // TintColor[1]
		Parameter *pParameter = GetParameter("TintColor[1]");
		if (pParameter)
			pParameter->SetValue4fv(TintColor1.Get());
	}

	{ // TintColor[2]
		Parameter *pParameter = GetParameter("TintColor[2]");
		if (pParameter)
			pParameter->SetValue4fv(TintColor2.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
