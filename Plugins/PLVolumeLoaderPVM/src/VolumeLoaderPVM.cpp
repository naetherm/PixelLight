/*********************************************************\
 *  File: VolumeLoaderPVM.cpp                            *
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
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>
#include <PLMath/Vector3i.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLVolume/Volume.h>
#include "PLVolumeLoaderPVM/PVM_GPL_License/ddsbase.h"	// Lookout! GPL code!
#include "PLVolumeLoaderPVM/VolumeLoaderPVM.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLVolume;
namespace PLVolumeLoaderPVM {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(VolumeLoaderPVM, "PLVolumeLoaderPVM", PLVolume::VolumeLoader, "Volume loader implementation for the \"PVM\"-format. The file format is part of the V^3 volume rendering package available at http://www.stereofx.org/volume.html .")
	// Properties
	pl_properties
		pl_property("Formats",	"pvm,PVM")
		pl_property("Load",		"1")
		pl_property("Save",		"1")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_2_metadata(Load,			pl_ret_type(bool),	PLVolume::Volume&,			PLCore::File&,			"Load method, by default 8 bit is enforced",																							"")
	pl_method_3_metadata(LoadParams,	pl_ret_type(bool),	PLVolume::Volume&,			PLCore::File&,	bool,	"Load method. Parameters: First 'bool' parameter determines whether or not to enforce 8 bit (may result in a visible quality loss).",	"")
	pl_method_2_metadata(Save,			pl_ret_type(bool),	const PLVolume::Volume&,	PLCore::File&,			"Save method",																															"")
pl_class_metadata_end(VolumeLoaderPVM)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool VolumeLoaderPVM::Load(Volume &cVolume, File &cFile)
{
	return LoadParams(cVolume, cFile, true);
}

bool VolumeLoaderPVM::LoadParams(Volume &cVolume, File &cFile, bool bForce8Bit)
{
	bool bResult = false; // We're pessimism, so, error by default

	// Get the image holding the volumetric data
	Image &cImage = cVolume.GetVolumeImage();

	// [TODO] Just a test until transfer functions also properly work with 16 bit
	bForce8Bit = true;

	// Read the PVM volume
	unsigned int nWidth = 0, nHeight = 0, nDepth = 0, nComponents = 0;
	Vector3 vScale;
	unsigned char *pszPVMvolume = readPVMvolume(cFile.GetUrl().GetNativePath().GetASCII(), &nWidth, &nHeight, &nDepth, &nComponents, &vScale.x, &vScale.y, &vScale.z);
	if (pszPVMvolume) {
		// Force 8 bit?
		if (bForce8Bit && nComponents == 2) {
			// Quantize 16 bit volume to 8 bit using a non-linear mapping (by default, this function automatically frees the previous allocated memory)
			pszPVMvolume = quantize(pszPVMvolume, nWidth, nHeight, nDepth);
			nComponents = 1;
		}

		// We only support 8 bit and 16 bit
		if (nComponents == 1 || nComponents == 2) {
			// Create image - don't takeover the data because it was allocated using malloc
			cImage = Image::CreateImageAndCopyData((nComponents == 1) ? DataByte : DataWord, ColorGrayscale, Vector3i(nWidth, nHeight, nDepth), CompressionNone, pszPVMvolume);

			// Hooray, success!
			bResult = true;
		} else {
			PL_LOG(Error, "PVM volume loader: Only 8 bit and 16 bit volumes are supported")
		}

		// Cleanup
		MemoryManager::Deallocator(MemoryManager::Free, pszPVMvolume);

		// Set the size of one voxel (without metric, but usually one unit is equal to one meter)
		cVolume.SetVoxelSize(Volume::DefaultVoxelSize*vScale);
	}

	// Done
	return bResult;
}

bool VolumeLoaderPVM::Save(const Volume &cVolume, File &cFile)
{
	// Get the image holding the volumetric data
	const Image &cImage = cVolume.GetVolumeImage();

	// Get image buffer, we only support the data type "byte" and "word"
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer && (pImageBuffer->GetDataFormat() == DataByte || pImageBuffer->GetDataFormat() == DataWord)) {
		// Save the data
		const Vector3i &vSize = pImageBuffer->GetSize();
		writePVMvolume(cFile.GetUrl().GetNativePath().GetASCII(), pImageBuffer->GetData(), vSize.x, vSize.y, vSize.z, (pImageBuffer->GetDataFormat() == DataByte) ? 1 : 2);

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
VolumeLoaderPVM::VolumeLoaderPVM()
{
}

/**
*  @brief
*    Destructor
*/
VolumeLoaderPVM::~VolumeLoaderPVM()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeLoaderPVM
