/*********************************************************\
 *  File: ShaderCompositor.cpp                           *
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
#include <PLCore/Base/Class.h>
//#include <PLCore/File/File.h>			// [TODO] Test: Write out the composed shader
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include "PLVolumeRenderer/ShaderFunction.h"
#include "PLVolumeRenderer/ShaderComposition.h"
#include "PLVolumeRenderer/ShaderCompositor.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderCompositor)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
ShaderCompositor::~ShaderCompositor()
{
	// Clear the cache
	ClearCache();
}

// [TODO] Find a more flexible template solution
ShaderFunction *ShaderCompositor::GetShaderFunction(const String &sClassName, uint8 nNumOfClipPlanes, uint8 nNumOfDepthTextures)
{
	// Get RTTI class
	const Class *pClass = ClassManager::GetInstance()->GetClass(sClassName);
	if (pClass && pClass->IsDerivedFrom("PLVolumeRenderer::ShaderFunction")) {
		if ((pClass->GetClassName() == "PLVolumeRenderer::ShaderFunctionClipRayTemplate")) {
			// Get instance name
			const String sInstanceName = sClassName + nNumOfClipPlanes + nNumOfDepthTextures;

			// Is there already an instance of this shader function?
			ShaderFunction *pShaderFunction = m_mapShaderFunction.Get(sInstanceName);
			if (pShaderFunction)
				return pShaderFunction;

			// Create shader function instance
			pShaderFunction = static_cast<ShaderFunction*>(pClass->Create(Params<Object*, uint8, uint8>(nNumOfClipPlanes, nNumOfDepthTextures)));
			if (pShaderFunction) {
				m_lstShaderFunction.Add(pShaderFunction);
				m_mapShaderFunction.Add(sInstanceName, pShaderFunction);
				pShaderFunction->m_pShaderCompositor = this;
				return pShaderFunction;
			}
		} else {
			// Get instance name
			const String sInstanceName = sClassName;

			// Is there already an instance of this shader function?
			ShaderFunction *pShaderFunction = m_mapShaderFunction.Get(sInstanceName);
			if (pShaderFunction)
				return pShaderFunction;

			// Create shader function instance
			pShaderFunction = static_cast<ShaderFunction*>(pClass->Create());
			if (pShaderFunction) {
				m_lstShaderFunction.Add(pShaderFunction);
				m_mapShaderFunction.Add(sInstanceName, pShaderFunction);
				pShaderFunction->m_pShaderCompositor = this;
				return pShaderFunction;
			}
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Clears the cache
*/
void ShaderCompositor::ClearCache()
{
	// Destroy the shader compositions using the shader functions
	for (uint32 i=0; i<m_lstShaderCompositions.GetNumOfElements(); i++)
		delete m_lstShaderCompositions[i];
	m_lstShaderCompositions.Clear();
	m_mapShaderCompositions.Clear();

	// Destroy shader functions
	for (uint32 i=0; i<m_lstShaderFunction.GetNumOfElements(); i++)
		delete m_lstShaderFunction[i];
	m_lstShaderFunction.Clear();
	m_mapShaderFunction.Clear();
}

/**
*  @brief
*    Returns a shader composition instance by using the provided shader composition ID
*/
ShaderComposition *ShaderCompositor::GetComposition(const ShaderCompositionID &cShaderCompositionID) const
{
	// [TODO] This might not work correctly when there's a checksum collision
	return m_mapShaderCompositions.Get(cShaderCompositionID.GetChecksum());
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderCompositor::ShaderCompositor()
{
}

/**
*  @brief
*    Creates a shader composition instance
*/
ShaderComposition *ShaderCompositor::CreateShaderComposition(const ShaderCompositionID &cShaderCompositionID)
{
	const ShaderCompositionID::SData &sShaderCompositionIDData = cShaderCompositionID.GetData();

	// We don't have to maintain a certain order, but in here we can make the shader compilers life easier by just inverting the shader functions order
	Array<String> lstShaderFunctions;
	{
		// 2.6 - Illumination
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionIlluminationClass->GetClassName());
		// 2.5 - Gradient Input
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionGradientInputClass->GetClassName());
		// 2.5 - Gradient
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionGradientClass->GetClassName());
		// 2.4 - Classification
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionClassificationClass->GetClassName());
		// 2.3 - Shading
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionShadingClass->GetClassName());
		// 2.2 - Reconstruction
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionReconstructionClass->GetClassName());
		// 2.2 - Fetch Scalar
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionFetchScalarClass->GetClassName());
		// 2.1 - Clip Position
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionClipPositionClass->GetClassName());
		// 2.0 - Ray Traversal
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionRayTraversalClass->GetClassName());
		// 1.2 - Jitter Position
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionJitterPositionClass->GetClassName());
		// 1.1 - Clip Ray
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionClipRayClass->GetClassName());
		// 1.0 - Ray Setup
		lstShaderFunctions.Add(sShaderCompositionIDData.pShaderFunctionRaySetupClass->GetClassName());
	}

	// Get the used shader language
	ShaderLanguage &cShaderLanguage = *sShaderCompositionIDData.pShaderLanguage;
	const String sShaderLanguage = cShaderLanguage.GetShaderLanguage();

	// Construct the vertex and fragment shader
	String sVertexShaderSourceCode, sFragmentShaderSourceCode;
	Array<ShaderFunction*> lstShaderFunctionInstances;
	if (sShaderLanguage == ShaderFunction::GLSL) {
		// GLSL (OpenGL 3.3 ("#version 330")
		sVertexShaderSourceCode   += "#version 330 core\n";
		sFragmentShaderSourceCode += "#version 330 core\n";

		{ // Add shader function signatures
		  // -> If we don't do this, the order of the shader functions would be important, might cause troubles in certain situations
			#include "ShaderFunctionSignatures_GLSL.h"
			sFragmentShaderSourceCode += sSourceCode_Fragment;
		}
	} else if (sShaderLanguage == ShaderFunction::Cg) {
		// Add shader function signatures
		// -> If we don't do this, the order of the shader functions would be important, might cause troubles in certain situations
		#include "ShaderFunctionSignatures_Cg.h"
		sFragmentShaderSourceCode += sSourceCode_Fragment;
	}
	for (uint32 i=0; i<lstShaderFunctions.GetNumOfElements(); i++) {
		ShaderFunction *pShaderFunction = GetShaderFunction(lstShaderFunctions[i], sShaderCompositionIDData.nNumOfClipPlanes, sShaderCompositionIDData.nNumOfDepthTextures);
		if (pShaderFunction) {
			lstShaderFunctionInstances.Add(pShaderFunction);
			sVertexShaderSourceCode   += pShaderFunction->GetSourceCode(sShaderLanguage, ShaderFunction::VertexShaderHeader);
			sFragmentShaderSourceCode += pShaderFunction->GetSourceCode(sShaderLanguage, ShaderFunction::FragmentShaderHeader);
		}
	}
	for (uint32 i=0; i<lstShaderFunctions.GetNumOfElements(); i++) {
		ShaderFunction *pShaderFunction = GetShaderFunction(lstShaderFunctions[i], sShaderCompositionIDData.nNumOfClipPlanes, sShaderCompositionIDData.nNumOfDepthTextures);
		if (pShaderFunction) {
			sVertexShaderSourceCode   += pShaderFunction->GetSourceCode(sShaderLanguage, ShaderFunction::VertexShaderBody);
			sFragmentShaderSourceCode += pShaderFunction->GetSourceCode(sShaderLanguage, ShaderFunction::FragmentShaderBody);
		}
	}

	// Create a vertex shader instance
	VertexShader *pVertexShader = cShaderLanguage.CreateVertexShader(sVertexShaderSourceCode, "glslv", "version=150");

	// Create a fragment shader instance
	FragmentShader *pFragmentShader = cShaderLanguage.CreateFragmentShader(sFragmentShaderSourceCode, "glslf", "version=150");

	// Create a program instance and assign the created vertex and fragment shaders to it
	Program *pProgram = cShaderLanguage.CreateProgram(pVertexShader, pFragmentShader);

	// Create new shader composition instance
	ShaderComposition *pShaderComposition = new ShaderComposition(cShaderCompositionID, *pProgram, lstShaderFunctionInstances);

	// Register new shader composition
	m_lstShaderCompositions.Add(pShaderComposition);
	m_mapShaderCompositions.Add(cShaderCompositionID.GetChecksum(), pShaderComposition);

	{ // Setup direct access to shader function instances this shader composition consists of
		ShaderComposition::SShaderFunction &sShaderFunction = pShaderComposition->m_sShaderFunction;

		// 1.0 - Ray Setup
		sShaderFunction.pShaderFunctionRaySetupClass		= reinterpret_cast<ShaderFunctionRaySetup*>		 (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionRaySetupClass->GetClassName()));
		// 1.1 - Clip Ray
		sShaderFunction.pShaderFunctionClipRayClass			= reinterpret_cast<ShaderFunctionClipRay*>		 (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionClipRayClass->GetClassName(), sShaderCompositionIDData.nNumOfClipPlanes, sShaderCompositionIDData.nNumOfDepthTextures));
		// 1.2 - Jitter Position
		sShaderFunction.pShaderFunctionJitterPositionClass	= reinterpret_cast<ShaderFunctionJitterPosition*>(GetShaderFunction(sShaderCompositionIDData.pShaderFunctionJitterPositionClass->GetClassName()));
		// 2.0 - Ray Traversal
		sShaderFunction.pShaderFunctionRayTraversalClass	= reinterpret_cast<ShaderFunctionRayTraversal*>  (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionRayTraversalClass->GetClassName()));
		// 2.1 - Clip Position
		sShaderFunction.pShaderFunctionClipPositionClass	= reinterpret_cast<ShaderFunctionClipPosition*>	 (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionClipPositionClass->GetClassName()));
		// 2.2 - Reconstruction
		sShaderFunction.pShaderFunctionReconstructionClass	= reinterpret_cast<ShaderFunctionReconstruction*>(GetShaderFunction(sShaderCompositionIDData.pShaderFunctionReconstructionClass->GetClassName()));
		// 2.2 - Fetch Scalar
		sShaderFunction.pShaderFunctionFetchScalarClass		= reinterpret_cast<ShaderFunctionFetchScalar*>	 (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionFetchScalarClass->GetClassName()));
		// 2.3 - Shading
		sShaderFunction.pShaderFunctionShadingClass			= reinterpret_cast<ShaderFunctionShading*>		 (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionShadingClass->GetClassName()));
		// 2.4 - Classification
		sShaderFunction.pShaderFunctionClassificationClass	= reinterpret_cast<ShaderFunctionClassification*>(GetShaderFunction(sShaderCompositionIDData.pShaderFunctionClassificationClass->GetClassName()));
		// 2.5 - Gradient
		sShaderFunction.pShaderFunctionGradientClass		= reinterpret_cast<ShaderFunctionGradient*>		 (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionGradientClass->GetClassName()));
		// 2.5 - Gradient Input
		sShaderFunction.pShaderFunctionGradientInputClass	= reinterpret_cast<ShaderFunctionGradientInput*> (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionGradientInputClass->GetClassName()));
		// 2.6 - Illumination
		sShaderFunction.pShaderFunctionIlluminationClass	= reinterpret_cast<ShaderFunctionIllumination*>	 (GetShaderFunction(sShaderCompositionIDData.pShaderFunctionIlluminationClass->GetClassName()));
	}

	/*
	{	// [TODO] Test: Write out the composed shader
		String sResult = pShaderComposition->ToString();
		if (sResult.GetLength()) {
			File cFile("D:\\_ShaderComposition.txt");
			if (cFile.Open(File::FileWrite | File::FileCreate)) {
				cFile.Write(sResult.GetASCII(), 1, sResult.GetLength());
			}
		}
	}*/

	// Done
	return pShaderComposition;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
