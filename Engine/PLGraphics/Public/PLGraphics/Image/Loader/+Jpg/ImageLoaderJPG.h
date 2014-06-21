/*********************************************************\
 *  File: ImageLoaderJPG.h                               *
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


#ifndef __PLGRAPHICS_IMAGELOADER_JPG_H__
#define __PLGRAPHICS_IMAGELOADER_JPG_H__
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


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image loader implementation for JPEG ("Joint Photographic Experts Group") file formats
*
*  @note
*    - Using libjpeg (http://www.ijg.org/)
*/
class ImageLoaderJPG : public ImageLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGRAPHICS_RTTI_EXPORT, ImageLoaderJPG, "PLGraphics", PLGraphics::ImageLoader, "Image loader implementation for JPEG (\"Joint Photographic Experts Group\") file formats")
		// Properties
		pl_properties
			pl_property("Formats",	"jpg,jpeg,jpe,jif,jfif,jfi,JPG,JPEG,JPE,JIF,JFIF,JFI")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	Image&,			PLCore::File&,					"Load method, no fancy upsampling and no interblock smoothing is applied",																											"")
		pl_method_4(LoadParams,	pl_ret_type(bool),	Image&,			PLCore::File&,	bool,	bool,	"Load method. Parameters: First 'bool' parameter determines whether or not fancy upsampling is applied, second 'bool' parameter whether or not interblock smoothing is applied.",	"")
		pl_method_2(Save,		pl_ret_type(bool),	const Image&,	PLCore::File&,					"Save method, using 100% quality setting",																																			"")
		pl_method_3(SaveParams,	pl_ret_type(bool),	const Image&,	PLCore::File&,	PLCore::uint32,	"Save method, quality (0...100) as first parameter",																																"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLGRAPHICS_API bool Load(Image &cImage, PLCore::File &cFile);
		PLGRAPHICS_API bool LoadParams(Image &cImage, PLCore::File &cFile, bool bBlockSmoothing, bool bFancyUpsampling);
		PLGRAPHICS_API bool Save(const Image &cImage, PLCore::File &cFile);
		PLGRAPHICS_API bool SaveParams(const Image &cImage, PLCore::File &cFile, PLCore::uint32 nQuality);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLGRAPHICS_API ImageLoaderJPG();

		/**
		*  @brief
		*    Destructor
		*/
		PLGRAPHICS_API virtual ~ImageLoaderJPG();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


#endif // __PLGRAPHICS_IMAGELOADER_JPG_H__
