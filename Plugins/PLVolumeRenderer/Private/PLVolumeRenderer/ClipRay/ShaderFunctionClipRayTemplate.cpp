/*********************************************************\
 *  File: ShaderFunctionClipRayTemplate.cpp              *
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
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Scene/SNClip.h>
#include "PLVolumeRenderer/ShaderCompositor.h"
#include "PLVolumeRenderer/ClipRay/ShaderFunctionClipRayPlane.h"
#include "PLVolumeRenderer/ClipRay/ShaderFunctionClipRayTemplate.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionClipRayTemplate)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ShaderFunctionClipRayTemplate::ShaderFunctionClipRayTemplate(uint8 nNumOfClipPlanes, uint8 nNumOfDepthTextures) :
	m_nNumOfClipPlanes(nNumOfClipPlanes),
	m_nNumOfDepthTextures(nNumOfDepthTextures)
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionClipRayTemplate::~ShaderFunctionClipRayTemplate()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunctionClipRay functions        ]
//[-------------------------------------------------------]
void ShaderFunctionClipRayTemplate::SetPlanes(PLRenderer::Program &cProgram, const VisNode &cVisNode, const Array<const VisNode*> &lstClipPlanes)
{
	// [TODO] Just a test
	for (uint32 i=0; i<m_lstShaderFunction.GetNumOfElements(); i++) {
		const VisNode *pVisNode = lstClipPlanes[i];
		if (pVisNode) {
			// Invert clipping?
			const bool bInvertClipping = (pVisNode->GetSceneNode()->GetFlags() & PLVolume::SNClip::InvertClipping) != 0;

			// Get the view space clip plane
			Plane cViewSpaceClipPlane;
			cViewSpaceClipPlane.ComputeND(pVisNode->GetWorldViewMatrix()*Vector3::Zero, pVisNode->GetWorldViewMatrix().RotateVector(bInvertClipping ? Vector3::NegativeUnitZ : Vector3::UnitZ));

			// Set clip plane
			static_cast<ShaderFunctionClipRayPlane*>(m_lstShaderFunction[i])->SetPlane(cProgram, cVisNode, cViewSpaceClipPlane, i);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionClipRayTemplate::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Return the requested source code
	return (nSourceCodeType == FragmentShaderBody) ? GenerateSourceCode(sShaderLanguage) : "";
}

void ShaderFunctionClipRayTemplate::SetProgram(PLRenderer::Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Let the shader functions do their job (e.g. setting program uniforms)
	for (uint32 i=0; i<m_lstShaderFunction.GetNumOfElements(); i++)
		m_lstShaderFunction[i]->SetProgram(cProgram, cCullQuery, cVisNode, cSRPVolume);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generates a template instance source code
*/
String ShaderFunctionClipRayTemplate::GenerateSourceCode(const String &sShaderLanguage)
{
	// Get the shader compositor owning this shader function instance
	ShaderCompositor *pShaderCompositor = GetShaderCompositor();
	if (pShaderCompositor) {
		String sSourceCode_Template;

		// Check requested shader language
		if (sShaderLanguage == GLSL) {
			#include "Template_GLSL.h"

			// Get the template source code
			sSourceCode_Template = sSourceCode_Fragment_Template;
		} else if (sShaderLanguage == Cg) {
			#include "Template_Cg.h"

			// Get the template source code
			sSourceCode_Template = sSourceCode_Fragment_Template;
		}

		// Generate source code
		if (sSourceCode_Template.GetLength()) {
			static const String StringToReplace = "_x_";
			String sSourceCode;
			String sFunctionCalls;

			// [TODO] For now, only one depth texture
			if (m_nNumOfDepthTextures) {
				ShaderFunction *pShaderFunction = pShaderCompositor->GetShaderFunction("PLVolumeRenderer::ShaderFunctionClipRayDepthTexture");
				if (pShaderFunction) {
					String sSourceCode_Template = pShaderFunction->GetSourceCode(sShaderLanguage, ShaderFunction::FragmentShaderTemplate);
					sSourceCode += sSourceCode_Template;
					m_lstShaderFunction.Add(pShaderFunction);
					sFunctionCalls += "ClipRay_x_(RayOrigin, RayDirection, MaximumTravelLength);";
				}
			}

			for (uint32 i=0; i<m_nNumOfClipPlanes; i++) {
				ShaderFunction *pShaderFunction = pShaderCompositor->GetShaderFunction("PLVolumeRenderer::ShaderFunctionClipRayPlane");
				if (pShaderFunction) {
					String sSourceCode_Template = pShaderFunction->GetSourceCode(sShaderLanguage, ShaderFunction::FragmentShaderTemplate);
					sSourceCode_Template.Replace(StringToReplace, String::Format("_%d_", i));
					sSourceCode += sSourceCode_Template;
					m_lstShaderFunction.Add(pShaderFunction);
					sFunctionCalls += String("ClipRay_") + i + "_(RayOrigin, RayDirection, MaximumTravelLength);";
				}
			}

			{
				sSourceCode_Template.Replace(StringToReplace, sFunctionCalls);
				sSourceCode += sSourceCode_Template;
			}

			// Return the generated source code
			return sSourceCode;
		}
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
