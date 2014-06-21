/*********************************************************\
 *  File: ImageLoaderDDS.h                               *
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


#ifndef __PLGRAPHICS_IMAGELOADER_DDS_H__
#define __PLGRAPHICS_IMAGELOADER_DDS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGraphics/Image/ImageLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Image;
struct DDSHeader;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image loader implementation for DDS file formats
*
*  @remarks
*    This file format supports GPU texture compression formats like DXT3 and therefore makes it possible to load in
*    compressed image data as efficient as possible by directly passing thru the compressed loaded data to the GPU
*    without need for any further processing.
*
*  @note
*    - DirectDraw Surface file format from Microsoft
*    - Use e.g. "The Compressonator" from AMD (http://developer.amd.com/tools/compressonator/pages/default.aspx) to open, view, edit and save the dds-file
*    - BGR(A) color format is automatically converted to RGB(A)
*/
class ImageLoaderDDS : public ImageLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGRAPHICS_RTTI_EXPORT, ImageLoaderDDS, "PLGraphics", PLGraphics::ImageLoader, "Image loader implementation for DDS file formats")
		// Properties
		pl_properties
			pl_property("Formats",	"dds,DDS")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,	pl_ret_type(bool),	Image&,			PLCore::File&,	"Load method",	"")
		pl_method_2(Save,	pl_ret_type(bool),	const Image&,	PLCore::File&,	"Save method",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLGRAPHICS_API bool Load(Image &cImage, PLCore::File &cFile);
		PLGRAPHICS_API bool Save(const Image &cImage, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLGRAPHICS_API ImageLoaderDDS();

		/**
		*  @brief
		*    Destructor
		*/
		PLGRAPHICS_API virtual ~ImageLoaderDDS();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Returns bits from a given bit mask
		*
		*  @param[in]  nMask
		*    Bit mask
		*  @param[out] nShiftLeft
		*    Receives the shift left
		*  @param[out] nShiftRight
		*    Receives the shift right
		*/
		void GetBitsFromMask(PLCore::uint32 nMask, PLCore::uint32 &nShiftLeft, PLCore::uint32 &nShiftRight) const;

		/**
		*  @brief
		*    Decompresses a RGB/RGBA image
		*
		*  @param[in]      sHeader
		*    DDS header
		*  @param[in, out] cImageBuffer
		*    Image buffer
		*  @param[in] pnCompressedData
		*    Data to decompress
		*/
		void DecompressRGBA(const DDSHeader &sHeader, ImageBuffer &cImageBuffer, const PLCore::uint8 *pnCompressedData) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


#endif // __PLGRAPHICS_IMAGELOADER_DDS_H__
