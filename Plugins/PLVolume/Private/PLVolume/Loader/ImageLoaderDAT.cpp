/*********************************************************\
 *  File: ImageLoaderDAT.cpp                             *
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
#include <PLCore/File/File.h>
#include <PLMath/Vector3i.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLVolume/Loader/ImageLoaderDAT.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ImageLoaderDAT)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderDAT::Load(Image &cImage, File &cFile)
{
	Url cObjectFilename;
	Vector3i vResolution;
	EDataFormat nFormat = DataByte;

	{ // Use the tokenizer in order to gather all required information, ignore the rest
		// A "dat"-file (simple ASCII) looks like this
		/*
			ObjectFileName: Teddybear.raw
			TaggedFileName: ---
			Resolution:     128 128 62
			SliceThickness: 2.8 2.8 5
			Format:         UCHAR
			NbrTags:        0
			ObjectType:     TEXTURE_VOLUME_OBJECT
			ObjectModel:    RGBA
			GridType:       EQUIDISTANT
		*/

		// Startup the tokenizer
		Tokenizer cTokenizer;
		cTokenizer.Start(cFile.GetContentAsString());

		// Loop through all tokens
		String sToken = cTokenizer.GetNextToken();
		while (sToken.GetLength()) {
			// ObjectFileName
			if (sToken == "ObjectFileName:") {
				// The file format specification says:
				//	"The object file name refers to the name of the data file, which contains the raw voxel data.
				//	 This can be either an absolute path or a relative path with respect to the storage position of the dat file."

				// Get the value
				cObjectFilename = cTokenizer.GetNextToken();

			// Resolution
			} else if (sToken == "Resolution:") {
				// The file format specification says:
				//	"The volume data set consists of a large array of voxel values. The resolution of the data set is given
				//	 by the number of voxels in x-, y- and z- direction."

				// Get the values
				vResolution.x = cTokenizer.GetNextToken().GetInt();
				vResolution.y = cTokenizer.GetNextToken().GetInt();
				vResolution.z = cTokenizer.GetNextToken().GetInt();

			// Format
			} else if (sToken == "Format:") {
				// The file format specification says:
				//	"The data format of one voxel. Can be either UCHAR (8 bit) or USHORT (16 bit)."

				// Get the value
				const String sFormat = cTokenizer.GetNextToken();
				if (sFormat == "UCHAR")
					nFormat = DataByte;	// Byte (8 bit)
				else if (sFormat == "USHORT")
					nFormat = DataWord;	// Word (16 bit)
			}

			// Next token, please
			sToken = cTokenizer.GetNextToken();
		}
	}

	// Valid settings? If so, open and read in the raw data...
	if (!cObjectFilename.IsEmpty() && vResolution.x > 0 && vResolution.y > 0 && vResolution.z > 0) {
		// The file format specification says:
		//	"The data file simply contains the raw voxel data as a large binary array which is indexed as
		//	 voxel(x,y,z) = array[z * YDIM * XDIM + y * XDIM + x],
		//	 with XDIM, YDIM and ZDIM referring to the resolution of the data set, as specified in line 3
		//	 of the header file. For 16 bit data, the data may be stored either in big endian or little endian format."
		//	-> We expect "Little Endian First"

		// Get the filename of the raw file
		const String sFilename = cObjectFilename.IsAbsolute() ? cObjectFilename.GetUrl() : (cFile.GetUrl().CutFilename() + cObjectFilename.GetUrl());

		// Open the raw file
		File cRawFile(sFilename);
		if (cRawFile.Open(File::FileRead)) {
			// Create image buffer
			ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
			pImageBuffer->CreateImage(nFormat, ColorGrayscale, vResolution);

			// Read the data
			cRawFile.Read(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ImageLoaderDAT::Save(const Image &cImage, File &cFile)
{
	// Get image buffer, we only support the data type "byte" and "word"
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer && (pImageBuffer->GetDataFormat() == DataByte || pImageBuffer->GetDataFormat() == DataWord)) {
		const Vector3i &vSize = pImageBuffer->GetSize();

		// Construct the object filename
		const String sObjectFilename = cFile.GetUrl().GetTitle() + ".raw";

		// A "dat"-file has simple ASCII content
		cFile.PutS("ObjectFileName: " + sObjectFilename + '\n');	// Example:	"ObjectFileName: Teddybear.raw"
		cFile.PutS("TaggedFileName: ---\n");						// Example:	"TaggedFileName: ---"
		cFile.PutS("Resolution:     " + vSize.ToString() + '\n');	// Example:	"Resolution:     128 128 62"
		cFile.PutS("SliceThickness: 1 1 1\n");						// Example:	"SliceThickness: 2.8 2.8 5"
		if (pImageBuffer->GetDataFormat() == DataByte)				// Example:	"Format:         UCHAR"
			cFile.PutS("Format:         UCHAR\n");
		else
			cFile.PutS("Format:         USHORT\n");
		cFile.PutS("NbrTags:        0\n");							// Example:	"NbrTags:        0"
		cFile.PutS("ObjectType:     TEXTURE_VOLUME_OBJECT\n");		// Example:	"ObjectType:     TEXTURE_VOLUME_OBJECT"
		cFile.PutS("ObjectModel:    RGBA\n");						// Example:	"ObjectModel:    RGBA"
		cFile.PutS("GridType:       EQUIDISTANT\n");				// Example:	"GridType:       EQUIDISTANT"

		{ // Raw data...
			// Get the absolute filename of the raw file
			const String sFilename = cFile.GetUrl().CutFilename() + sObjectFilename;

			// Open the raw file
			File cRawFile(sFilename);
			if (cRawFile.Open(File::FileCreate | File::FileWrite)) {
				// Save the data
				cRawFile.Write(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());

				// Done
				return true;
			}
		}
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
ImageLoaderDAT::ImageLoaderDAT()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderDAT::~ImageLoaderDAT()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
