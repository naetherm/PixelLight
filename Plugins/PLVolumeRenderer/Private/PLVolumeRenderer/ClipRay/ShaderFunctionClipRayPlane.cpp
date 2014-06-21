/*********************************************************\
 *  File: ShaderFunctionClipRayPlane.cpp                 *
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
#include "PLVolumeRenderer/ClipRay/ShaderFunctionClipRayPlane.h"


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
pl_implement_class(ShaderFunctionClipRayPlane)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionClipRayPlane::ShaderFunctionClipRayPlane()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionClipRayPlane::~ShaderFunctionClipRayPlane()
{
}

/**
*  @brief
*    Sets the clip plane parameters
*/
void ShaderFunctionClipRayPlane::SetPlane(Program &cProgram, const VisNode &cVolumeVisNode, const Plane &cViewSpaceClipPlane, int nIndex)
{
	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Bring the view space clip plane into scene node space
	Matrix4x4 mViewMatrix = cVolumeVisNode.GetWorldViewMatrix();
	mViewMatrix.Invert();
	const Plane cClipPlane = cViewSpaceClipPlane*mViewMatrix;

	// Set the clip plane in Hesse normal form
	cProgramWrapper.Set((nIndex < 0) ? "ClipPlane_x_" : (String("ClipPlane_") + nIndex + '_'), cClipPlane.a, cClipPlane.b, cClipPlane.c, cClipPlane.d);
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunctionClipRay functions        ]
//[-------------------------------------------------------]
void ShaderFunctionClipRayPlane::SetPlanes(Program &cProgram, const VisNode &cVolumeVisNode, const Array<const VisNode*> &lstClipPlanes)
{
	// None-template version

	// We only know a single clip plane, ignore the rest
	if (lstClipPlanes.GetNumOfElements()) {
		const VisNode *pVisNode = lstClipPlanes[0];

		// Invert clipping?
		const bool bInvertClipping = (pVisNode->GetSceneNode()->GetFlags() & PLVolume::SNClip::InvertClipping) != 0;

		// Get the view space clip plane
		Plane cViewSpaceClipPlane;
		cViewSpaceClipPlane.ComputeND(pVisNode->GetWorldViewMatrix()*Vector3::Zero, pVisNode->GetWorldViewMatrix().RotateVector(bInvertClipping ? Vector3::NegativeUnitZ : Vector3::UnitZ));

		// Set clip plane
		SetPlane(cProgram, cVolumeVisNode, cViewSpaceClipPlane, -1);
	}
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionClipRayPlane::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "Plane_GLSL.h"

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
		#include "Plane_Cg.h"

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
