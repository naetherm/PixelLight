/*********************************************************\
 *  File: IEColorKey.h                                   *
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


#ifndef __PLGRAPHICS_IMAGEEFFECT_COLORKEY_H__
#define __PLGRAPHICS_IMAGEEFFECT_COLORKEY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGraphics/Color/Color3.h"
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
*    Image effect: Apply color key
*
*  @note
*    - The color format "ColorPalette" is not supported
*    - The value in the alpha channel will be set to 0 if the RGB value at this pixel
*      is within the tolerance of the color key RGB value, else it is set to max value
*    - If the image has no alpha channel an alpha channel is add automatically
*/
class IEColorKey : public ImageEffect {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGRAPHICS_RTTI_EXPORT, IEColorKey, "PLGraphics", PLGraphics::ImageEffect, "Image effect: Apply color key")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cColor
		*    Color
		*  @param[in] fTolerance
		*    fTolerance (0 .. 1)
		*/
		PLGRAPHICS_API IEColorKey(const Color3 &cColor, float fTolerance);

		/**
		*  @brief
		*    Destructor
		*/
		PLGRAPHICS_API virtual ~IEColorKey();

		/**
		*  @brief
		*    Get color
		*
		*  @return
		*    Color
		*/
		PLGRAPHICS_API Color3 GetColor() const;

		/**
		*  @brief
		*    Get tolerance
		*
		*  @return
		*    Tolerance (0 .. 1)
		*/
		PLGRAPHICS_API float GetTolerance() const;


	//[-------------------------------------------------------]
	//[ Public virtual ImageEffect functions                  ]
	//[-------------------------------------------------------]
	public:
		PLGRAPHICS_API virtual bool Apply(ImageBuffer &cBuffer) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Color3 m_cColor;		/**< Color used for transparent pixels */
		float  m_fTolerance;	/**< Tolerance */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


#endif // __PLGRAPHICS_IMAGEEFFECT_COLORKEY_H__
