/*********************************************************\
 *  File: SNMPostProcessTiles.cpp                        *
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
#include "PLPostProcessEffects/SNMPostProcessTiles.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPostProcessTiles, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(Tiles,		float,				65.0f,									ReadWrite,	"Tiles",								"")
	pl_attribute_metadata(EdgeWidth,	float,				0.15f,									ReadWrite,	"Edge width",							"")
	pl_attribute_metadata(EdgeColor,	PLGraphics::Color3,	PLGraphics::Color3(0.7f, 0.7f, 0.7f),	ReadWrite,	"Edge color",							"")
		// Overloaded SNMPostProcess attributes
	pl_attribute_metadata(Filename,		PLCore::String,		"Data/PostProcesses/Tiles.pp",			ReadWrite,	"Filename of the post process to use",	"Type='PostProcess'")
pl_class_metadata_end(SNMPostProcessTiles)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessTiles::SNMPostProcessTiles(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	Tiles(this),
	EdgeWidth(this),
	EdgeColor(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/Tiles.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessTiles::~SNMPostProcessTiles()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessTiles::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // Tiles
		Parameter *pParameter = GetParameter("Tiles");
		if (pParameter)
			pParameter->SetValue1f(Tiles.Get());
	}

	{ // EdgeWidth
		Parameter *pParameter = GetParameter("EdgeWidth");
		if (pParameter)
			pParameter->SetValue1f(EdgeWidth.Get());
	}

	{ // EdgeColor
		Parameter *pParameter = GetParameter("EdgeColor");
		if (pParameter)
			pParameter->SetValue3fv(EdgeColor.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
