/*********************************************************\
 *  File: SNClipPrimitiveEllipsoid.cpp                   *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/MeshManager.h>
#include "PLScene/Scene/SceneContext.h"
#include <PLScene/Visibility/VisNode.h>
#include "PLVolume/Scene/SNClipPrimitiveEllipsoid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNClipPrimitiveEllipsoid)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNClipPrimitiveEllipsoid::SNClipPrimitiveEllipsoid() :
	AABBMin(this),
	AABBMax(this)
{
	// Overwritten PLScene::SceneNode variables
	SetAABoundingBox(AABoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));
}

/**
*  @brief
*    Destructor
*/
SNClipPrimitiveEllipsoid::~SNClipPrimitiveEllipsoid()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNClipPrimitiveEllipsoid::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNClipPrimitive::DrawDebug(cRenderer, pVisNode);

	// Draw the ellipsoid visualization?
	if (pVisNode && !(GetDebugFlags() & DebugNoVisualization)) {
		// [TODO] Remove the direct usage of fixed functions in here
		// Fixed functions support still required for some stuff
		FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
		if (pFixedFunctions) {
			// Setup render states
			// -> Lookout! Enable depth buffer else we will see both transparent box sides which is quite confusing
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      true);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, true);
			cRenderer.SetRenderState(RenderState::BlendEnable,  true);

			// Create the sphere mesh if required
			// -> In 'scene node space' it's a sphere, in 'scene container space' it can be an ellipsoid due to axis scaling
			static MeshHandler cMeshHandler;
			if (!cMeshHandler.GetMesh())
				cMeshHandler.SetMesh(GetSceneContext()->GetMeshManager().CreateMesh("PLMesh::MeshCreatorSphere", true, "Detail=26"));
			if (cMeshHandler.GetMesh()) {
				// Setup render states
				cRenderer.SetTextureBuffer();
				cRenderer.SetProgram();

				// Invert clipping?
				const bool bInvertClipping = (GetFlags() & InvertClipping) != 0;

				// View side allowing to see the clipped part *camera outside the clipping primitive*
				// -> Negative
				// -> Make this a little bit bigger to avoid depth buffer issues
				pFixedFunctions->SetColor(bInvertClipping ? DebugColorPositive.Get() : DebugColorNegative.Get());
				cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);
				cMeshHandler.Draw(false, false);

				// View side allowing to see the non-clipped rest *camera inside the clipping primitive*
				// -> Positive
				pFixedFunctions->SetColor(bInvertClipping ? DebugColorNegative.Get() : DebugColorPositive.Get());
				cRenderer.SetRenderState(RenderState::CullMode, Cull::CW);
				cMeshHandler.Draw(false, false);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
