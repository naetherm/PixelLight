/*********************************************************\
 *  File: SNClipVolumeTexture.cpp                        *
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
#include <PLMath/Vector3i.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLVolume/VolumeHandler.h"
#include "PLVolume/VolumeManager.h"
#include "PLVolume/Scene/SNClipVolumeTexture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNClipVolumeTexture)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNClipVolumeTexture::GetVolumeFilename() const
{
	return m_sVolumeFilename;
}

void SNClipVolumeTexture::SetVolumeFilename(const String &sValue)
{
	if (m_sVolumeFilename != sValue) {
		m_sVolumeFilename = sValue;

		// Load/reload the volume
		Load();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNClipVolumeTexture::SNClipVolumeTexture() :
	LoaderParameters(this),
	VolumeFilename(this),
	ClipThreshold(this),
	Flags(this),
	AABBMin(this),
	AABBMax(this),
	m_pVolumeHandler(new VolumeHandler())
{
	// Overwritten PLScene::SceneNode variables
	SetAABoundingBox(AABoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));
}

/**
*  @brief
*    Destructor
*/
SNClipVolumeTexture::~SNClipVolumeTexture()
{
	// Destroy the volume resource handler instance
	delete m_pVolumeHandler;
}

/**
*  @brief
*    Returns the used volume resource
*/
Volume *SNClipVolumeTexture::GetVolume() const
{
	return m_pVolumeHandler->GetResource();
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNClipVolumeTexture::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNClip::DrawDebug(cRenderer, pVisNode);

	// Draw the box visualization?
	if (pVisNode && !(GetDebugFlags() & DebugNoVisualization)) {
		// Get the 'scene node space' bounding box?
		const AABoundingBox &cAABB = GetAABoundingBox();

		// Setup render states
		// -> Lookout! Enable depth buffer else we will see both transparent box sides which is quite confusing
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      true);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, true);
		cRenderer.SetRenderState(RenderState::BlendEnable,  true);

		// Invert clipping?
		const bool bInvertClipping = (GetFlags() & InvertClipping) != 0;

		// View side allowing to see the clipped part *camera outside the clipping primitive*
		// -> Negative
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);
		cRenderer.GetDrawHelpers().DrawBox(bInvertClipping ? DebugColorPositive.Get() : DebugColorNegative.Get(), cAABB.vMin, cAABB.vMax, pVisNode->GetWorldViewProjectionMatrix());

		// View side allowing to see the non-clipped rest *camera inside the clipping primitive*
		// -> Positive
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CW);
		cRenderer.GetDrawHelpers().DrawBox(bInvertClipping ? DebugColorNegative.Get() : DebugColorPositive.Get(), cAABB.vMin, cAABB.vMax, pVisNode->GetWorldViewProjectionMatrix());
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads/reloads the sound
*/
void SNClipVolumeTexture::Load()
{
	// Get/create a new volume resource instance (if there was a previous one, it will be destroyed by the manager as soon as no longer referenced)
	bool bNewCreated = false;
	Volume *pVolume = VolumeManager::GetInstance()->GetByName(m_sVolumeFilename);
	if (!pVolume) {
		pVolume = VolumeManager::GetInstance()->Create(m_sVolumeFilename);
		bNewCreated = true;
	}

	// Give our volume resource handler the new resource (it's possible that the previous volume resource is now going to be destroyed)
	m_pVolumeHandler->SetResource(pVolume);

	// [TODO] Handle varying loader parameters
	if (pVolume && bNewCreated) {
		// Setup volume
		if (pVolume) {
			// Load in the volume data
			if (pVolume->LoadByFilename(m_sVolumeFilename, LoaderParameters.Get())) {
				// [TODO] Check this situation when the loader parameters do not match
				// Try again without parameters?
				if (!pVolume->GetVolumeImage().GetBuffer())
					pVolume->LoadByFilename(m_sVolumeFilename);

				// Create the texture buffer instance by using our image data
				SceneContext *pSceneContext = GetSceneContext();
				if (pSceneContext) {
					// Get the renderer and renderer capabilities instance
					Renderer &cRenderer = pSceneContext->GetRendererContext().GetRenderer();

					// [TODO] Just a test (volume image to texture buffer)
					TextureBuffer *pTextureBuffer = pVolume->GetVolumeTextureBuffer(cRenderer, !(GetFlags() & NoTextureCompression), !(GetFlags() & NoTextureMipmapping));
				}
			}
		}
	}

	// Set the scale of the scene node?
	if (pVolume && !(GetFlags() & NoVolumeScale)) {
		// [TODO] Don't use the image data for this, may e.g. been unloaded within "GetVolumeTextureBuffer()"
		ImageBuffer *pImageBuffer = pVolume->GetVolumeImage().GetBuffer();
		if (pImageBuffer) {
			// Get the size of one voxel (without metric, but usually one unit is equal to one meter)
			const Vector3 &vVoxelSize = pVolume->GetVoxelSize();

			// Get the image size aka the number of voxels along each diagonal
			const Vector3i &vImageSize = pImageBuffer->GetSize();

			// Set the scale of the scene node
			SetScale(Vector3(vVoxelSize.x*vImageSize.x, vVoxelSize.y*vImageSize.y, vVoxelSize.z*vImageSize.z));
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
