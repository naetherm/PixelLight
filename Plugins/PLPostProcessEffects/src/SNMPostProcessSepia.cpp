/*********************************************************\
 *  File: SNMPostProcessSepia.cpp                        *
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
#include "PLPostProcessEffects/SNMPostProcessSepia.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPostProcessSepia, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(Desat,		float,				0.5f,									ReadWrite,	"Desaturation",							"")
	pl_attribute_metadata(Toned,		float,				1.0f,									ReadWrite,	"Toning",								"")
	pl_attribute_metadata(LightColor,	PLGraphics::Color3,	PLGraphics::Color3(1.0f, 0.90f, 0.5f),	ReadWrite,	"Paper tone",							"")
	pl_attribute_metadata(DarkColor,	PLGraphics::Color3,	PLGraphics::Color3(0.2f, 0.05f, 0.0f),	ReadWrite,	"Stain tone",							"")
		// Overloaded SNMPostProcess attributes
	pl_attribute_metadata(Filename,		PLCore::String,		"Data/PostProcesses/Sepia.pp",			ReadWrite,	"Filename of the post process to use",	"Type='PostProcess'")
pl_class_metadata_end(SNMPostProcessSepia)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessSepia::SNMPostProcessSepia(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	Desat(this),
	Toned(this),
	LightColor(this),
	DarkColor(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/Sepia.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessSepia::~SNMPostProcessSepia()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessSepia::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // Desat
		Parameter *pParameter = GetParameter("Desat");
		if (pParameter)
			pParameter->SetValue1f(Desat.Get());
	}

	{ // Toned
		Parameter *pParameter = GetParameter("Toned");
		if (pParameter)
			pParameter->SetValue1f(Toned.Get());
	}

	{ // LightColor
		Parameter *pParameter = GetParameter("LightColor");
		if (pParameter)
			pParameter->SetValue3fv(LightColor.Get());
	}
	
	{ // DarkColor
		Parameter *pParameter = GetParameter("DarkColor");
		if (pParameter)
			pParameter->SetValue3fv(DarkColor.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
