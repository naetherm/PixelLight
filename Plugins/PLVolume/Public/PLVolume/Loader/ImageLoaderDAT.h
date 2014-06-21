/*********************************************************\
 *  File: ImageLoaderDAT.h                               *
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


#ifndef __PLVOLUME_IMAGELOADER_DAT_H__
#define __PLVOLUME_IMAGELOADER_DAT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Image/ImageLoader.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Image;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image loader implementation for the "DAT" volume data file format from the University of Erlangen, Germany
*
*  @note
*    - The file format specification is online available at http://openqvis.sourceforge.net/docu/fileformat.html
*    - Sample data sets are available at http://openqvis.sourceforge.net/ and http://www.voreen.org/108-Data-Sets.html
*    - The image loader is only able to deal with the volumetric image data, not with volumetric specific additional information like voxel size
*/
class ImageLoaderDAT : public PLGraphics::ImageLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, ImageLoaderDAT, "PLVolume", PLGraphics::ImageLoader, "Image loader implementation for the \"DAT\" volume data file format from the University of Erlangen, Germany")
		// Properties
		pl_properties
			pl_property("Formats",	"dat,DAT")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		// [TODO] This is just a proof-of-concept and not that useful within the master-thesis (results in a question within PLViewerQt when loading a resource)
		//pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,	pl_ret_type(bool),	PLGraphics::Image&,			PLCore::File&,	"Load method",	"")
		pl_method_2(Save,	pl_ret_type(bool),	const PLGraphics::Image&,	PLCore::File&,	"Save method",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		bool Load(PLGraphics::Image &cImage, PLCore::File &cFile);
		bool Save(const PLGraphics::Image &cImage, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ImageLoaderDAT();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ImageLoaderDAT();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_IMAGELOADER_DAT_H__
