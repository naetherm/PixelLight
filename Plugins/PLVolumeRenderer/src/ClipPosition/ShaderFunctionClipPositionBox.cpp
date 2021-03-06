/*********************************************************\
 *  File: ShaderFunctionClipPositionBox.cpp              *
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
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Scene/SNClip.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/ClipPosition/ShaderFunctionClipPositionBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ShaderFunctionClipPositionBox, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionClipPosition, "Box shader function implementation class for clip position")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(ShaderFunctionClipPositionBox)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionClipPositionBox::ShaderFunctionClipPositionBox()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionClipPositionBox::~ShaderFunctionClipPositionBox()
{
}

/**
*  @brief
*    Sets the clip box parameters
*/
void ShaderFunctionClipPositionBox::SetBox(Program &cProgram, const VisNode &cVolumeVisNode, const VisNode &cBoxVisNode, int nIndex)
{
	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Calculate the volume space to clip box space matrix
	// Volume space to view space
	Matrix4x4 mVolumeSpaceToClipBoxSpace = cVolumeVisNode.GetWorldViewMatrix();
	// View space to clip box space
	mVolumeSpaceToClipBoxSpace = cBoxVisNode.GetWorldViewMatrix().GetInverted()*mVolumeSpaceToClipBoxSpace;

	// Set the volume space to clip box space matrix
	cProgramWrapper.Set((nIndex < 0) ? "VolumeSpaceToClipBoxSpace_x_" : (String("VolumeSpaceToClipBoxSpace_") + nIndex + '_'), mVolumeSpaceToClipBoxSpace);

	// Set invert clipping
	cProgramWrapper.Set((nIndex < 0) ? "InvertClipping_x_" : (String("InvertClipping_") + nIndex + '_'), (cBoxVisNode.GetSceneNode()->GetFlags() & PLVolume::SNClip::InvertClipping) != 0);
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunctionClipPosition functions   ]
//[-------------------------------------------------------]
void ShaderFunctionClipPositionBox::SetBoxes(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipBoxes)
{
	// None-template version

	// We only know a single clip box, ignore the rest
	if (lstClipBoxes.GetNumOfElements())
		SetBox(cProgram, cVolumeVisNode, *lstClipBoxes[0], -1);
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionClipPositionBox::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "Box_GLSL.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case FragmentShaderTemplate:
				return sSourceCode_Fragment_Template;

			case VertexShaderHeader:
			case VertexShaderBody:
			case VertexShaderTemplate:
			case FragmentShaderHeader:
				// Nothing to do in here
				break;
		}
	} else if (sShaderLanguage == Cg) {
		#include "Box_Cg.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case FragmentShaderTemplate:
				return sSourceCode_Fragment_Template;

			case VertexShaderHeader:
			case VertexShaderBody:
			case VertexShaderTemplate:
			case FragmentShaderHeader:
				// Nothing to do in here
				break;
		}
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
