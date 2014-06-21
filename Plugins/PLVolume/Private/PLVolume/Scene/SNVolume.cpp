/*********************************************************\
 *  File: SNVolume.cpp                                   *
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
#include <PLScene/Scene/SceneContext.h>
#include "PLVolume/VolumeHandler.h"
#include "PLVolume/VolumeManager.h"
#include "PLVolume/TransferFunctionControl/TransferFunctionChannel.h"
#include "PLVolume/TransferFunctionControl/TransferFunctionControl.h"
#include "PLVolume/Scene/SNVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNVolume)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNVolume::GetVolumeFilename() const
{
	return m_sVolumeFilename;
}

void SNVolume::SetVolumeFilename(const String &sValue)
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
SNVolume::SNVolume() :
	VolumeFilenameSave(this),
	LoaderParameters(this),
	VolumeFilename(this),
	SampleRateFactor(this),
	RayTraversal(this),
	Opacity(this),
	IsosurfaceValue(this),
	Reconstruction(this),
	VolumeTextureLOD(this),
	Shading(this),
	Classification(this),
	ScalarClassificationThreshold(this),
	Gradient(this),
	PostClassificationGradient(this),
	Illumination(this),
	Flags(this),
	AABBMin(this),
	AABBMax(this),
	m_pVolumeHandler(new VolumeHandler())
{
	// Overwritten SceneNode variables
	SetAABoundingBox(AABoundingBox(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
}

/**
*  @brief
*    Destructor
*/
SNVolume::~SNVolume()
{
	// Destroy the volume resource handler instance
	delete m_pVolumeHandler;
}

/**
*  @brief
*    Returns the used volume resource
*/
Volume *SNVolume::GetVolume() const
{
	return m_pVolumeHandler->GetResource();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads/reloads the sound
*/
void SNVolume::Load()
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

					{ // Save volume?
						const String sVolumeFilenameSave = VolumeFilenameSave.Get();
						if (sVolumeFilenameSave.GetLength())
							pVolume->SaveByFilename(sVolumeFilenameSave);
					}
				}
			}
		}
	}

	// [TODO] Just a test
	if (pVolume) {
		// Set the default transfer function configuration
		pVolume->GetTransferFunctionControl().SetDefaultConfiguration();
	}

	// Set the scale of the scene node?
	if (pVolume && !(GetFlags() & NoVolumeScale)) {
		// Get the size of one voxel (without metric, but usually one unit is equal to one meter)
		const Vector3 &vVoxelSize = pVolume->GetVoxelSize();

		// Get the image size aka the number of voxels along each diagonal
		const Vector3i &vImageSize = pVolume->GetVolumeImageSize();

		// Set the scale of the scene node
		SetScale(Vector3(vVoxelSize.x*vImageSize.x, vVoxelSize.y*vImageSize.y, vVoxelSize.z*vImageSize.z));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
