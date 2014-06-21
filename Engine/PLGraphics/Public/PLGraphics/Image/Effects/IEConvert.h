/*********************************************************\
 *  File: IEConvert.h                                    *
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


#ifndef __PLGRAPHICS_IMAGEEFFECT_CONVERT_H__
#define __PLGRAPHICS_IMAGEEFFECT_CONVERT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGraphics/Image/ImageEffect.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image effect: Convert data and color type of an image
*
*  @note
*    - The color format "ColorPalette" is not supported as destination format
*    - If an alpha channel is added, the set alpha value for every pixel is 255 for integer formats and 1.0 for floating point formats...
*      this way, such an image can usually directly be used as a solid texture
*/
class IEConvert : public ImageEffect {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGRAPHICS_RTTI_EXPORT, IEConvert, "PLGraphics", PLGraphics::ImageEffect, "Image effect: Convert data and color type of an image")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nDataFormat
		*    Desired data format
		*  @param[in] nColorFormat
		*    Desired color format
		*/
		PLGRAPHICS_API IEConvert(EDataFormat nDataFormat, EColorFormat nColorFormat);

		/**
		*  @brief
		*    Destructor
		*/
		PLGRAPHICS_API virtual ~IEConvert();


	//[-------------------------------------------------------]
	//[ Public virtual ImageEffect functions                  ]
	//[-------------------------------------------------------]
	public:
		PLGRAPHICS_API virtual bool Apply(ImageBuffer &cBuffer) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EDataFormat  m_nDataFormat;		/**< Data format of the image */
		EColorFormat m_nColorFormat;	/**< Color format of the image */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


#endif // __PLGRAPHICS_IMAGEEFFECT_CONVERT_H__
