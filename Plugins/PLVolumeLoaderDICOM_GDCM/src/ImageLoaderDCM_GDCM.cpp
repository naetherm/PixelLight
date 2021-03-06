/*********************************************************\
 *  File: ImageLoaderDCM_GDCM.cpp                        *
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
PL_WARNING_PUSH
	PL_WARNING_DISABLE(4530)	// "warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc"
	PL_WARNING_DISABLE(4541)	// "warning C4541: 'dynamic_cast' used on polymorphic type 'gdcm::Value' with /GR-; unpredictable behavior may result"
	PL_WARNING_DISABLE(4512)	// "warning C4512: 'gdcm::ByteSwapFilter' : assignment operator could not be generated"
	#include <gdcm/MediaStorageAndFileFormat/gdcmImageReader.h>
PL_WARNING_POP
#include "PLVolumeLoaderDICOM_GDCM/Context_GDCM.h"
#include "PLVolumeLoaderDICOM_GDCM/ImageLoaderDCM_GDCM.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLVolumeLoaderDICOM_GDCM {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ImageLoaderDCM_GDCM, "PLVolumeLoaderDICOM_GDCM", PLGraphics::ImageLoader, "Image loader implementation for the \"dcm\" image data file format by using GDCM (Grassroots DICOM) (http://sourceforge.net/projects/gdcm/)")
	// Properties
	pl_properties
		pl_property("Formats",	",dcm,DCM,dicom,DICOM")	// DICOM data can be within a given directory instead of a file, the comma at the beginning of the string denotes that no extension is also valid
		pl_property("Load",		"1")
		pl_property("Save",		"0")
	pl_properties_end
	// Constructors
	// [TODO] This is just a proof-of-concept and not that useful within the master-thesis (results in a question within PLViewerQt when loading a resource)
	//pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_2_metadata(Load,	pl_ret_type(bool),	PLGraphics::Image&,			PLCore::File&,	"Load method",	"")
	pl_method_2_metadata(Save,	pl_ret_type(bool),	const PLGraphics::Image&,	PLCore::File&,	"Save method",	"")
pl_class_metadata_end(ImageLoaderDCM_GDCM)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderDCM_GDCM::Load(Image &cImage, File &cFile)
{
	// Load in the DICOM image
	// [TODO] Check: Is there a way to overwrite the file handling in DCMTK?
	const String sFilename = cFile.GetUrl().GetNativePath();
	gdcm::ImageReader gdcmImageReader;
	gdcmImageReader.SetFileName(sFilename.GetASCII());
	if (!gdcmImageReader.Read()) {
		// Error!
		// [TODO] Error description?
		PL_LOG(Error, "Failed to load in DICOM file \"" + sFilename + '\"')
//		PL_LOG(Error, "Failed to load in DICOM file \"" + sFilename + "\" (" + cOFCondition.text() + ')')

		// Error! Get us out of here right now!
		return false;
	}

	// Get the DICOM image
	const gdcm::Image &gdcmImage = gdcmImageReader.GetImage();

	// Get the DICOM image dimension
	const unsigned long nWidth  = gdcmImage.GetDimensions()[0];
	const unsigned long nHeight = gdcmImage.GetDimensions()[1];

	// Allocate the DICOM image buffer
	char *gdcmImageBuffer = new char[gdcmImage.GetBufferLength()];
	gdcmImage.GetBuffer(gdcmImageBuffer);

//	gdcm::PixelFormat::ScalarType gdcmPixelFormatScalarType = gdcmImage.GetPixelFormat();

	// Let's start with the easy case:
	if (gdcmImage.GetPhotometricInterpretation() == gdcm::PhotometricInterpretation::RGB) {
		if (gdcmImage.GetPixelFormat() != gdcm::PixelFormat::UINT8) {
			// [TODO] Handle this case
		} else {
			// Create image and takeover the data
			cImage = Image::CreateImageAndTakeoverData(DataByte, ColorRGB, Vector3i(nWidth, nHeight, 1), CompressionNone, reinterpret_cast<uint8*>(gdcmImageBuffer));
			gdcmImageBuffer = nullptr;
		}
	} else if (gdcmImage.GetPhotometricInterpretation() == gdcm::PhotometricInterpretation::MONOCHROME2) {
		if (gdcmImage.GetPixelFormat() == gdcm::PixelFormat::UINT8) {
			// Create image and takeover the data
			cImage = Image::CreateImageAndTakeoverData(DataByte, ColorGrayscale, Vector3i(nWidth, nHeight, 1), CompressionNone, reinterpret_cast<uint8*>(gdcmImageBuffer));
			gdcmImageBuffer = nullptr;
		} else if (gdcmImage.GetPixelFormat() == gdcm::PixelFormat::UINT16) {
			// Create image and takeover the data
			cImage = Image::CreateImageAndTakeoverData(DataWord, ColorGrayscale, Vector3i(nWidth, nHeight, 1), CompressionNone, reinterpret_cast<uint8*>(gdcmImageBuffer));
			gdcmImageBuffer = nullptr;
		} else {
			// [TODO] Handle this case
			const gdcm::PixelFormat &gdcmPixelFormat = gdcmImage.GetPixelFormat();
		//	std::cerr << "Pixel Format is: " << gdcmImage.GetPixelFormat() << std::endl;
			return false;
		}
	} else {
			// [TODO] Handle this case
//		std::cerr << "Unhandled PhotometricInterpretation: " << gimage.GetPhotometricInterpretation() << std::endl;
		return false;
	}

	// Deallocate the DICOM image buffer
	if (gdcmImageBuffer)
		delete [] gdcmImageBuffer;

	// Done
	return true;
}

bool ImageLoaderDCM_GDCM::Save(const Image &cImage, File &cFile)
{
	// Not implemented

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
ImageLoaderDCM_GDCM::ImageLoaderDCM_GDCM()
{
	// Add a context reference
	Context_GDCM::AddReference();
}

/**
*  @brief
*    Destructor
*/
ImageLoaderDCM_GDCM::~ImageLoaderDCM_GDCM()
{
	// Release a context reference
	Context_GDCM::ReleaseReference();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeLoaderDICOM_GDCM
