/*********************************************************\
 *  File: ShaderCompositorPC.cpp                         *
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
#include <PLRenderer/Renderer/Renderer.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLCompositing/SRPBegin.h>
#include <PLVolume/Volume.h>
#include <PLVolume/TransferFunction.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/ClipRay/ShaderFunctionClipRay.h"
#include "PLVolumeRenderer/ClipPosition/ShaderFunctionClipPosition.h"
#include "PLVolumeRenderer/ShaderComposition.h"
#include "PLVolumeRenderer/ShaderCompositorPC.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderCompositorPC)


//[-------------------------------------------------------]
//[ Public virtual ShaderCompositor functions             ]
//[-------------------------------------------------------]
// [TODO] Just a test
ShaderComposition *ShaderCompositorPC::Generate(Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Generate shader composition ID
	ShaderCompositionID cShaderCompositionID;
	ShaderCompositionID::SData &sShaderCompositionIDData = cShaderCompositionID.m_sData;

	{ // Decide which shader language should be used (for example "GLSL" or "Cg")
		// Give the volume scene renderer setting the highest priority
		String sShaderLanguage = cSRPVolume.ShaderLanguage.Get();

		// In case the volume scene renderer specified no explicit shader language, fallback to the default shader language of the used renderer
		if (!sShaderLanguage.GetLength())
			sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

		// Get the shader language instance
		sShaderCompositionIDData.pShaderLanguage = cRenderer.GetShaderLanguage(sShaderLanguage);
		if (!sShaderCompositionIDData.pShaderLanguage)
			return nullptr;	// Error! Get us out of this method right now.
	}

	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return nullptr;	// Error! Get us out of this method right now.

	// Get the volume resource
	PLVolume::Volume *pVolume = pSNVolume->GetVolume();
	if (!pVolume)
		return nullptr;	// Early escape, there's no sense in continuing

	// Analyze the scene
	Array<const VisNode*> lstClipPlanes;
	if (!(cSRPVolume.GetFlags() & SRPVolume::NoClipRay)) {	// Analyze scene renderer
		const Class *pClass = ClassManager::GetInstance()->GetClass("PLVolume::SNClipPrimitivePlane");
		if (pClass)
			cSRPVolume.GetIntersectingInstancesOf(cCullQuery, *pSNVolume, *pClass, lstClipPlanes);
	}
	Array<const VisNode*> lstClipBoxes;
	if (!(cSRPVolume.GetFlags() & SRPVolume::NoClipPosition)) {	// Analyze scene renderer
		const Class *pClass = ClassManager::GetInstance()->GetClass("PLVolume::SNClipPrimitiveBox");
		if (pClass)
			cSRPVolume.GetIntersectingInstancesOf(cCullQuery, *pSNVolume, *pClass, lstClipBoxes);
	}
	Array<const VisNode*> lstClipEllipsoids;
	if (!(cSRPVolume.GetFlags() & SRPVolume::NoClipPosition)) {	// Analyze scene renderer
		const Class *pClass = ClassManager::GetInstance()->GetClass("PLVolume::SNClipPrimitiveEllipsoid");
		if (pClass)
			cSRPVolume.GetIntersectingInstancesOf(cCullQuery, *pSNVolume, *pClass, lstClipEllipsoids);
	}
	Array<const VisNode*> lstClipVolumeTextures;
	if (!(cSRPVolume.GetFlags() & SRPVolume::NoClipPosition)) {	// Analyze scene renderer
		const Class *pClass = ClassManager::GetInstance()->GetClass("PLVolume::SNClipVolumeTexture");
		if (pClass)
			cSRPVolume.GetIntersectingInstancesOf(cCullQuery, *pSNVolume, *pClass, lstClipVolumeTextures);
	}
	bool bLighting = !(cSRPVolume.GetFlags() & SRPVolume::NoShading);	// Analyze scene renderer
	if (bLighting) {
		// Analyze the volume scene node
		bLighting = !(pSNVolume->GetFlags() & SceneNode::NoLighting);
		if (bLighting) {
			// Get a list of all visible scene node instances intersecting with the given scene node
			const Class *pClass = ClassManager::GetInstance()->GetClass("PLScene::SNDirectionalLight");
			if (pClass) {
				Array<const VisNode*> lstIntersecting;
				cSRPVolume.GetIntersectingInstancesOf(cCullQuery, *pSNVolume, *pClass, lstIntersecting);
				if (!lstIntersecting.GetNumOfElements())
					bLighting = false;
			}
		}

		// [TODO] Give this information to the shader function instance
	}

	// Analyze scene renderer
	uint8 nNumOfDepthTextures = 0;
	if (!(cSRPVolume.GetFlags() & SRPVolume::NoDepthTexture)) {
		// Get the "PLCompositing::SRPBegin" instance
		PLCompositing::SRPBegin *pSRPBegin = static_cast<PLCompositing::SRPBegin*>(cSRPVolume.GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
		if (pSRPBegin) {
			// Depth buffer texture available?
			nNumOfDepthTextures = pSRPBegin->GetTextureBufferDepth() ? 1 : 0;
		}
	}

	// Get the class manager instance
	ClassManager &cClassManager = *ClassManager::GetInstance();

	{ // 1.0 - Ray Setup
		const SRPVolume::ERaySetup nRaySetup = (cSRPVolume.GetFlags() & SRPVolume::NoRaySetup) ? SRPVolume::RaySetupNull : cSRPVolume.RaySetup.Get();
		switch (nRaySetup) {
			// No ray setup
			default:
			case SRPVolume::RaySetupNull:
			{	// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRaySetupNull");
				sShaderCompositionIDData.pShaderFunctionRaySetupClass = pClass;
				break;
			}

			// Ray setup using a color cube
			case SRPVolume::RaySetupColorCube:
			{	// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRaySetupColorCube");
				sShaderCompositionIDData.pShaderFunctionRaySetupClass = pClass;
				break;
			}

			// Ray setup using bounding box intersection
			case SRPVolume::RaySetupBoundingBoxIntersection:
			{	// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRaySetupBoundingBoxIntersection");
				sShaderCompositionIDData.pShaderFunctionRaySetupClass = pClass;
				break;
			}

			// Ray setup using a combination of color cube and bounding box intersection
			case SRPVolume::RaySetupHybrid:
			{	// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRaySetupHybrid");
				sShaderCompositionIDData.pShaderFunctionRaySetupClass = pClass;
				break;
			}
		}
	}

	{ // 1.1 - Clip Ray
		// No depth texture and no clip planes?
		if ((cSRPVolume.GetFlags() & SRPVolume::NoClipRay) || (!nNumOfDepthTextures && !lstClipPlanes.GetNumOfElements())) {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipRayNull");
			sShaderCompositionIDData.pShaderFunctionClipRayClass = pClass;

		// No depth texture and one clip plane?
		} else if (!nNumOfDepthTextures && lstClipPlanes.GetNumOfElements() == 1) {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipRayPlane");
			sShaderCompositionIDData.pShaderFunctionClipRayClass = pClass;

		// Depth texture and no clip planes?
		} else if (nNumOfDepthTextures && !lstClipPlanes.GetNumOfElements()) {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipRayDepthTexture");
			sShaderCompositionIDData.pShaderFunctionClipRayClass = pClass;

		// Combination of multiple clip ray functions
		} else {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipRayTemplate");
			sShaderCompositionIDData.pShaderFunctionClipRayClass = pClass;
		}

		// Set template configuration
		sShaderCompositionIDData.nNumOfClipPlanes = static_cast<uint8>(lstClipPlanes.GetNumOfElements());
		sShaderCompositionIDData.nNumOfDepthTextures = nNumOfDepthTextures;
	}

	{ // 1.2 - Jitter Position
		// In case dither ray is zero, disable jitter position completely because it will not have any effect in the first place
		const SRPVolume::EJitterPosition nJitterPosition = ((cSRPVolume.GetFlags() & SRPVolume::NoJitterPosition) || !cSRPVolume.DitherRay.Get()) ? SRPVolume::JitterPositionNull : cSRPVolume.JitterPosition.Get();
		switch (nJitterPosition) {
			// No jitter position
			default:
			case SRPVolume::JitterPositionNull:
			{	// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionJitterPositionNull");
				sShaderCompositionIDData.pShaderFunctionJitterPositionClass = pClass;
				break;
			}

			// Jitter position by using a random texture
			case SRPVolume::JitterPositionStochastic:
			{	// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionJitterPositionStochastic");
				sShaderCompositionIDData.pShaderFunctionJitterPositionClass = pClass;
				break;
			}

			// Jitter position by using trigonometric functions
			case SRPVolume::JitterPositionTrigonometric:
			{	// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionJitterPositionTrigonometric");
				sShaderCompositionIDData.pShaderFunctionJitterPositionClass = pClass;
				break;
			}
		}
	}

	// 2.0 - Ray Traversal
	const PLVolume::SNVolume::ERayTraversal nRayTraversal = (cSRPVolume.GetFlags() & SRPVolume::NoRayTraversal) ? PLVolume::SNVolume::RayTraversalNull : pSNVolume->RayTraversal.Get();
	{
		switch (nRayTraversal) {
			// No ray traversal
			default:
			case PLVolume::SNVolume::RayTraversalNull:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalNull");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// Debug ray start ray traversal
			case PLVolume::SNVolume::RayTraversalDebugRayStart:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalDebugRayStart");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// Debug ray end ray traversal
			case PLVolume::SNVolume::RayTraversalDebugRayEnd:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalDebugRayEnd");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// Debug ray direction ray traversal
			case PLVolume::SNVolume::RayTraversalDebugRayDirection:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalDebugRayDirection");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// Debug ray length ray traversal
			case PLVolume::SNVolume::RayTraversalDebugRayLength:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalDebugRayLength");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// Isosurface (also known as "first-hit ray casting") ray traversal
			case PLVolume::SNVolume::RayTraversalIsosurface:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalIsosurface");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// MIP ray traversal
			case PLVolume::SNVolume::RayTraversalMIP:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalMIP");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// GMIP ray traversal
			case PLVolume::SNVolume::RayTraversalGMIP:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalGMIP");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// Back-to-front compositing (DVR) ray traversal (no early ray termination)
			case PLVolume::SNVolume::RayTraversalDVRBackToFront:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalDVRBackToFront");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// Front-to-back compositing (DVR) ray traversal (early ray termination)
			case PLVolume::SNVolume::RayTraversalDVRFrontToBack:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalDVRFrontToBack");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}

			// "Maximum intensity differences accumulation" (MIDA, hybrid combining the advantages of MIP and DVR) ray traversal (early ray termination)
			case PLVolume::SNVolume::RayTraversalMIDA:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionRayTraversalMIDA");
				sShaderCompositionIDData.pShaderFunctionRayTraversalClass = pClass;
				break;
			}
		}
	}

	{ // 2.1 - Clip Position
		if (cSRPVolume.GetFlags() & SRPVolume::NoClipPosition) {
			sShaderCompositionIDData.pShaderFunctionClipPositionClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipPositionNull");
		} else {
			// [TODO] Do only perform clipping within the shader when there's an bounding box intersection *performance*
			// [TODO] Using multiple of those clip position functions at the same time must be possible
			if (lstClipBoxes.GetNumOfElements())
				sShaderCompositionIDData.pShaderFunctionClipPositionClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipPositionBox");
			else if (lstClipEllipsoids.GetNumOfElements())
				sShaderCompositionIDData.pShaderFunctionClipPositionClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipPositionEllipsoid");
			else if (lstClipVolumeTextures.GetNumOfElements())
				sShaderCompositionIDData.pShaderFunctionClipPositionClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipPositionVolumeTexture");
			else
				sShaderCompositionIDData.pShaderFunctionClipPositionClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipPositionNull");
		//	sShaderCompositionIDData.pShaderFunctionClipPositionClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClipPositionTemplate");
		}
	}

	{ // 2.2 - Reconstruction
		const PLVolume::SNVolume::EReconstruction nReconstruction = ((cSRPVolume.GetFlags() & SRPVolume::NoReconstruction) && pSNVolume->Reconstruction.Get() > PLVolume::SNVolume::TrilinearFiltering) ? PLVolume::SNVolume::TrilinearFiltering : pSNVolume->Reconstruction.Get();
		switch (nReconstruction) {
			// Nearest neighbour filtering (1 volume texture fetch for the reconstruction)
			case PLVolume::SNVolume::NearestNeighbourFiltering:
			// Trilinear filtering (1 volume texture fetch for the reconstruction)
			case PLVolume::SNVolume::TrilinearFiltering:
			default:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionReconstructionNull");
				sShaderCompositionIDData.pShaderFunctionReconstructionClass = pClass;
				break;
			}

			// Tri-cubic B-spline filtering (8 volume texture fetches for the reconstruction, higher-order texture filtering)
			case PLVolume::SNVolume::TriCubicFiltering:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionReconstructionTriCubic");
				sShaderCompositionIDData.pShaderFunctionReconstructionClass = pClass;
				break;
			}

			// Tri-cubic kernel filtering (8 volume texture fetches + 3 kernel texture fetches for the reconstruction, higher-order texture filtering)
			case PLVolume::SNVolume::TriCubicKernelFiltering:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionReconstructionTriCubicKernel");
				sShaderCompositionIDData.pShaderFunctionReconstructionClass = pClass;
				break;
			}
		}
	}

	{ // 2.2 - Fetch Scalar
		// Get the renderer texture buffer holding the 3D voxel data
		PLRenderer::TextureBuffer *pVolumeTextureBuffer = (cSRPVolume.GetFlags() & SRPVolume::NoFetchScalar) ? nullptr : pVolume->GetVolumeTextureBuffer(cRenderer);
		if (pVolumeTextureBuffer) {
			// Evaluate the texture buffer type
			switch (pVolumeTextureBuffer->GetType()) {
				case PLRenderer::Resource::TypeTextureBuffer2D:
				{
					// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
					static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionFetchScalarSampler2D");
					sShaderCompositionIDData.pShaderFunctionFetchScalarClass = pClass;
					break;
				}

				case PLRenderer::Resource::TypeTextureBuffer2DArray:
				{
					// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
					static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionFetchScalarSampler2DArray");
					sShaderCompositionIDData.pShaderFunctionFetchScalarClass = pClass;
					break;
				}

				case PLRenderer::Resource::TypeTextureBuffer3D:
				{
					// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
					static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionFetchScalarSampler3D");
					sShaderCompositionIDData.pShaderFunctionFetchScalarClass = pClass;
					break;
				}

				case PLRenderer::Resource::TypeIndexBuffer:
				case PLRenderer::Resource::TypeVertexBuffer:
				case PLRenderer::Resource::TypeUniformBuffer:
				case PLRenderer::Resource::TypeTextureBuffer1D:
				case PLRenderer::Resource::TypeTextureBufferRectangle:
				case PLRenderer::Resource::TypeTextureBufferCube:
				case PLRenderer::Resource::TypeOcclusionQuery:
				case PLRenderer::Resource::TypeVertexShader:
				case PLRenderer::Resource::TypeTessellationControlShader:
				case PLRenderer::Resource::TypeTessellationEvaluationShader:
				case PLRenderer::Resource::TypeGeometryShader:
				case PLRenderer::Resource::TypeFragmentShader:
				case PLRenderer::Resource::TypeProgram:
				case PLRenderer::Resource::TypeFont:
				default:
				{
					// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
					static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionFetchScalarNull");
					sShaderCompositionIDData.pShaderFunctionFetchScalarClass = pClass;
					break;
				}
			}
		} else {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionFetchScalarNull");
			sShaderCompositionIDData.pShaderFunctionFetchScalarClass = pClass;
		}
	}

	{ // 2.3 - Shading
		const PLVolume::SNVolume::EShading nShading = (cSRPVolume.GetFlags() & SRPVolume::NoShading) ? PLVolume::SNVolume::ShadingNull : pSNVolume->Shading.Get();
		switch (nShading) {
			// No shading is performed, the classification result of given scalar will be returned instead
			default:
			case PLVolume::SNVolume::ShadingNull:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingNull");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Debug scalar shading
			case PLVolume::SNVolume::ShadingDebugScalar:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingDebugScalar");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Debug position shading
			case PLVolume::SNVolume::ShadingDebugPosition:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingDebugPosition");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Debug classification shading
			case PLVolume::SNVolume::ShadingDebugClassification:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingDebugClassification");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Debug gradient shading
			case PLVolume::SNVolume::ShadingDebugGradient:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingDebugGradient");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Debug normal shading: Show normalized normal as used for the illumination, similar to gradient, but in case of an invalid normal the previous valid normal will be reused
			case PLVolume::SNVolume::ShadingDebugNormal:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingDebugNormal");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Debug viewing direction shading
			case PLVolume::SNVolume::ShadingDebugViewingDirection:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingDebugViewingDirection");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Debug light direction shading
			case PLVolume::SNVolume::ShadingDebugLightDirection:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingDebugLightDirection");
				sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
				break;
			}

			// Shading with lighting
			case PLVolume::SNVolume::ShadingLighting:
				// Lighting possible/allowed?
				if (bLighting) {
					// Performance: Use illumination threshold? (does make much sense for isosurface or MIP)
					if (!(cSRPVolume.GetFlags() & SRPVolume::NoIlluminationThreshold) && cSRPVolume.IlluminationThreshold.Get() &&
						(nRayTraversal == PLVolume::SNVolume::RayTraversalDVRBackToFront || nRayTraversal == PLVolume::SNVolume::RayTraversalDVRFrontToBack || nRayTraversal == PLVolume::SNVolume::RayTraversalMIDA)) {
						// Use illumination threshold

						// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
						static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingLightingThreshold");
						sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
					} else {
						// Do not use illumination threshold

						// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
						static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingLighting");
						sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
					}
				}
				break;
		}

		// Fallback: No classification is performed, the given scalar will be returned instead
		if (!sShaderCompositionIDData.pShaderFunctionShadingClass) {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionShadingNull");
			sShaderCompositionIDData.pShaderFunctionShadingClass = pClass;
		}
	}

	{ // 2.4 - Classification
		const PLVolume::SNVolume::EClassification nClassification = (cSRPVolume.GetFlags() & SRPVolume::NoClassification) ? PLVolume::SNVolume::ClassificationNull : pSNVolume->Classification.Get();
		switch (nClassification) {
			// No classification is performed, the given scalar will be returned instead
			case PLVolume::SNVolume::ClassificationNull:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClassificationNull");
				sShaderCompositionIDData.pShaderFunctionClassificationClass = pClass;
				break;
			}

			// For given scalar values below the threshold zero will be returned, else the given scalar will be returned instead
			case PLVolume::SNVolume::ClassificationThreshold:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClassificationThreshold");
				sShaderCompositionIDData.pShaderFunctionClassificationClass = pClass;
				break;
			}

			// The given scalar value is used as index for a transfer function
			case PLVolume::SNVolume::ClassificationTransferFunction:
				// Is there a transfer function available?
				if (pVolume->GetTransferFunction().GetTextureBuffer(cRenderer, PLVolume::TransferFunction::StandardTextureBuffer)) {
					// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
					static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClassificationTransferFunction");
					sShaderCompositionIDData.pShaderFunctionClassificationClass = pClass;
				}
				break;

			// The given scalar value is used as index for a pre-multiplied (no color bleeding) transfer function
			case PLVolume::SNVolume::ClassificationTransferFunctionPreMultiplied:
				// Is there a transfer function available?
				if (pVolume->GetTransferFunction().GetTextureBuffer(cRenderer, PLVolume::TransferFunction::PreMultipliedTextureBuffer)) {
					// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
					static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClassificationTransferFunctionPreMultiplied");
					sShaderCompositionIDData.pShaderFunctionClassificationClass = pClass;
				}
				break;

			// The given scalar value is used as index for a pre-integration transfer function
			case PLVolume::SNVolume::ClassificationPreIntegration:
				// Is there a transfer function available?
				if (pVolume->GetTransferFunction().GetTextureBuffer(cRenderer, PLVolume::TransferFunction::PreIntegratedTextureBuffer)) {
					// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
					static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClassificationPreIntegration");
					sShaderCompositionIDData.pShaderFunctionClassificationClass = pClass;
				}
				break;
		}

		// Fallback: No classification is performed, the given scalar will be returned instead
		if (!sShaderCompositionIDData.pShaderFunctionClassificationClass) {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionClassificationNull");
			sShaderCompositionIDData.pShaderFunctionClassificationClass = pClass;
		}
	}

	{ // 2.5 - Gradient
		const PLVolume::SNVolume::EGradient nGradient = ((cSRPVolume.GetFlags() & SRPVolume::NoGradient) || !bLighting) ? PLVolume::SNVolume::GradientNull : pSNVolume->Gradient.Get();
		switch (nGradient) {
			// No gradient computation, returns always (0, 1, 0) instead (... at least it's normalized...)
			case PLVolume::SNVolume::GradientNull:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientNull");
				sShaderCompositionIDData.pShaderFunctionGradientClass = pClass;
				break;
			}

			// On-the-fly gradient computation approximating gradient vectors using the forward differences (finite differences) scheme (4 volume texture fetches for the gradient)
			case PLVolume::SNVolume::GradientForwardDifferences:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientForwardDifferences");
				sShaderCompositionIDData.pShaderFunctionGradientClass = pClass;
				break;
			}

			// On-the-fly gradient computation approximating gradient vectors using the backward differences (finite differences) scheme (4 volume texture fetches for the gradient)
			case PLVolume::SNVolume::GradientBackwardDifferences:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientBackwardDifferences");
				sShaderCompositionIDData.pShaderFunctionGradientClass = pClass;
				break;
			}

			// On-the-fly gradient computation approximating gradient vectors using the central differences (finite differences) scheme (6 volume texture fetches for the gradient)
			case PLVolume::SNVolume::GradientCentralDifferences:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientCentralDifferences");
				sShaderCompositionIDData.pShaderFunctionGradientClass = pClass;
				break;
			}

			// On-the-fly gradient computation approximating gradient vectors using the central differences (finite differences) scheme (gradients of neighbour voxels weighted, 6*9=54 volume texture fetches for the gradient)
			case PLVolume::SNVolume::GradientCentralDifferences9:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientCentralDifferences9");
				sShaderCompositionIDData.pShaderFunctionGradientClass = pClass;
				break;
			}
		}
	}

	{ // 2.5 - Gradient Input
		if (cSRPVolume.GetFlags() & SRPVolume::NoGradientInput) {
			// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
			static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientInputNull");
			sShaderCompositionIDData.pShaderFunctionGradientInputClass = pClass;
		} else {
			if (pSNVolume->PostClassificationGradient.Get()) {
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientInputClassified");
				sShaderCompositionIDData.pShaderFunctionGradientInputClass = pClass;
			} else {
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionGradientInputScalar");
				sShaderCompositionIDData.pShaderFunctionGradientInputClass = pClass;
			}
		}
	}

	{ // 2.6 - Illumination
		const PLVolume::SNVolume::EIllumination nIllumination = ((cSRPVolume.GetFlags() & SRPVolume::NoIllumination) || !bLighting) ? PLVolume::SNVolume::IlluminationNull : pSNVolume->Illumination.Get();
		switch (nIllumination) {
			// No illumination, returns always (0, 0, 0) instead
			case PLVolume::SNVolume::IlluminationNull:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionIlluminationNull");
				sShaderCompositionIDData.pShaderFunctionIlluminationClass = pClass;
				break;
			}

			// Illumination Lambertian reflection
			case PLVolume::SNVolume::IlluminationLambertianReflection:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionIlluminationLambertianReflection");
				sShaderCompositionIDData.pShaderFunctionIlluminationClass = pClass;
				break;
			}

			// Blinn-Phong illumination
			case PLVolume::SNVolume::IlluminationBlinnPhong:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionIlluminationBlinnPhong");
				sShaderCompositionIDData.pShaderFunctionIlluminationClass = pClass;
				break;
			}

			// Cook-Torrance illumination
			case PLVolume::SNVolume::IlluminationCookTorrance:
			{
				// Catch the RTTI class instance only once to keep the internal overhead as low as possible, but do also keep this optimization local
				static const Class *pClass = cClassManager.GetClass("PLVolumeRenderer::ShaderFunctionIlluminationCookTorrance");
				sShaderCompositionIDData.pShaderFunctionIlluminationClass = pClass;
				break;
			}
		}
	}

	// Now that we have everything, let's calculate the checksum
	cShaderCompositionID.CalculateChecksum();

	// Is there already such a shader composition?
	ShaderComposition *pShaderComposition = GetComposition(cShaderCompositionID);
	if (!pShaderComposition) {
		// Check whether or not this shader composition ID is valid, if valid, create a shader composition instance (takes over the program memory control) and return the instance
		pShaderComposition = cShaderCompositionID.IsValid() ? CreateShaderComposition(cShaderCompositionID) : nullptr;
	}
	if (!pShaderComposition)
		return nullptr;	// Error! Get us out of this method right now.

	// Set the currently used GPU program
	pShaderComposition->SetProgram(cRenderer, cCullQuery, cVisNode, cSRPVolume);

	{ // Give the shader composition current values
		const ShaderComposition::SShaderFunction &sShaderFunction = pShaderComposition->GetShaderFunctions();

		// 1.1 - Clip Ray
		// -> Tell the clip ray implementation about the found clip planes
		sShaderFunction.pShaderFunctionClipRayClass->SetPlanes(pShaderComposition->GetProgram(), cVisNode, lstClipPlanes);

		// 2.1 - Clip Position
		// -> Tell the clip position implementation about the found clip boxes, clip ellipsoids and clip volume textures
		sShaderFunction.pShaderFunctionClipPositionClass->SetBoxes(pShaderComposition->GetProgram(), cVisNode, lstClipBoxes);
		sShaderFunction.pShaderFunctionClipPositionClass->SetEllipsoids(pShaderComposition->GetProgram(), cVisNode, lstClipEllipsoids);
		sShaderFunction.pShaderFunctionClipPositionClass->SetVolumeTextures(pShaderComposition->GetProgram(), cVisNode, lstClipVolumeTextures);
	}

	// Done
	return pShaderComposition;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderCompositorPC::ShaderCompositorPC()
{
}

/**
*  @brief
*    Destructor
*/
ShaderCompositorPC::~ShaderCompositorPC()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
