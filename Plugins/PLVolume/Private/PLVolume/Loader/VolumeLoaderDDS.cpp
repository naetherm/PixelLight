/*********************************************************\
 *  File: VolumeLoaderDDS.cpp                            *
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
#include <PLGraphics/Image/Image.h>
#include <PLRenderer/Renderer/TextureBuffer.h>
#include "PLVolume/Volume.h"
#include "PLVolume/Loader/VolumeLoaderDDS.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(VolumeLoaderDDS)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool VolumeLoaderDDS::Load(Volume &cVolume, File &cFile)
{
	return LoadParams(cVolume, cFile, Volume::DefaultVoxelSize.x, Volume::DefaultVoxelSize.y, Volume::DefaultVoxelSize.z);
}

bool VolumeLoaderDDS::LoadParams(Volume &cVolume, File &cFile, float fVoxelSizeX, float fVoxelSizeY, float fVoxelSizeZ)
{
	// Load in the dds image
	if (cVolume.GetVolumeImage().LoadByFile(cFile)) {
		// Set the size of one voxel (without metric, but usually one unit is equal to one meter)
		cVolume.SetVoxelSize(Vector3(fVoxelSizeX, fVoxelSizeY, fVoxelSizeZ));

		// Done
		return true;
	}

	// Error!
	return false;
}

bool VolumeLoaderDDS::Save(const Volume &cVolume, File &cFile)
{
	// First: Try to save the volume texture buffer from the GPU
	TextureBuffer *pTextureBuffer = cVolume.GetVolumeTextureBuffer();
	if (pTextureBuffer) {
		// Download the texture buffer data from the GPU and save it
		Image cImage;
		pTextureBuffer->CopyDataToImage(cImage);
		return cImage.SaveByFile(cFile);
	} else {
		// Second: Try to save the CPU side volume image
		return const_cast<Image&>(cVolume.GetVolumeImage()).SaveByFile(cFile);	// Ugly const-cast, this volume loader is just a wrapper
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
VolumeLoaderDDS::VolumeLoaderDDS()
{
}

/**
*  @brief
*    Destructor
*/
VolumeLoaderDDS::~VolumeLoaderDDS()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
