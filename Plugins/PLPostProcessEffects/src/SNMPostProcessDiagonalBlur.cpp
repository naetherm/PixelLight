/*********************************************************\
 *  File: SNMPostProcessDiagonalBlur.cpp                 *
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
#include "PLPostProcessEffects/SNMPostProcessDiagonalBlur.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPostProcessDiagonalBlur, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(ColorScale,	float,				4.0f,									ReadWrite,	"Color scale",							"")
	pl_attribute_metadata(PixelKernel0,	PLMath::Vector2,	PLMath::Vector2( 2.0f, 2.0f),			ReadWrite,	"Pixel kernel 0",						"")
	pl_attribute_metadata(PixelKernel1,	PLMath::Vector2,	PLMath::Vector2( 4.0f, 4.0f),			ReadWrite,	"Pixel kernel 1",						"")
	pl_attribute_metadata(PixelKernel2,	PLMath::Vector2,	PLMath::Vector2(-6.0f, 6.0f),			ReadWrite,	"Pixel kernel 2",						"")
	pl_attribute_metadata(PixelKernel3,	PLMath::Vector2,	PLMath::Vector2( 8.0f, 8.0f),			ReadWrite,	"Pixel kernel 3",						"")
		// Overloaded SNMPostProcess attributes
	pl_attribute_metadata(Filename,		PLCore::String,		"Data/PostProcesses/DiagonalBlur.pp",	ReadWrite,	"Filename of the post process to use",	"Type='PostProcess'")
pl_class_metadata_end(SNMPostProcessDiagonalBlur)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessDiagonalBlur::SNMPostProcessDiagonalBlur(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	ColorScale(this),
	PixelKernel0(this),
	PixelKernel1(this),
	PixelKernel2(this),
	PixelKernel3(this),
	Filename(this)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/DiagonalBlur.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessDiagonalBlur::~SNMPostProcessDiagonalBlur()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCompositing::SNMPostProcess functions ]
//[-------------------------------------------------------]
void SNMPostProcessDiagonalBlur::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // ColorScale
		Parameter *pParameter = GetParameter("ColorScale");
		if (pParameter)
			pParameter->SetValue1f(ColorScale.Get());
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
