/*********************************************************\
 *  File: ImageEffects.h                                 *
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


#ifndef __PLGRAPHICS_IMAGEEFFECTS_H__
#define __PLGRAPHICS_IMAGEEFFECTS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGraphics/Image/ImageEffectWrapper.h"
#include "PLGraphics/Image/Effects/IEConvert.h"
#include "PLGraphics/Image/Effects/IEFilter.h"
#include "PLGraphics/Image/Effects/IEScale.h"
#include "PLGraphics/Image/Effects/IEMonochrome.h"
#include "PLGraphics/Image/Effects/IEColorKey.h"
#include "PLGraphics/Image/Effects/IEFlipXAxis.h"
#include "PLGraphics/Image/Effects/IEFlipYAxis.h"
#include "PLGraphics/Image/Effects/IERemovePalette.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Color3;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static image effects class
*/
class ImageEffects {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create image effect dynamically
		*
		*  @param[in] sEffect
		*    Image effect class name
		*  @param[in] sParameters
		*    Image effect parameters
		*
		*  @return
		*    Image effect
		*/
		static inline ImageEffectWrapper Effect(const PLCore::String &sEffect, const PLCore::String &sParameters);

		/**
		*  @brief
		*    Create image filter
		*
		*  @return
		*    Image filter
		*/
		static inline IEFilter Filter();

		/**
		*  @brief
		*    Create scale image effect
		*
		*  @param[in] vNewSize
		*    The new size
		*  @param[in] bUseMipmaps
		*    If there are mipmaps and one of them matches the new dimension... is it allowed to
		*    just make this mipmap to the new base image and destroy the now unused other mipmaps?
		*    (extremely fast 'scale', but no set filters are applied and may cause problems on 'none standard'
		*    images)
		*
		*  @return
		*    Scale image effect
		*/
		static inline IEScale Scale(const PLMath::Vector3i &vNewSize, bool bUseMipmaps);

		/**
		*  @brief
		*    Create image conversion effect
		*
		*  @param[in] nDataFormat
		*    Desired data format
		*  @param[in] nColorFormat
		*    Desired color format
		*
		*  @return
		*    Image conversion effect
		*/
		static inline IEConvert Convert(EDataFormat nDataFormat, EColorFormat nColorFormat);

		/**
		*  @brief
		*    Create monochrome image effect
		*
		*  @return
		*    Monochrome image effect
		*/
		static inline IEMonochrome Monochrome();

		/**
		*  @brief
		*    Create color key image effect
		*
		*  @param[in] cColor
		*    Color
		*  @param[in] fTolerance
		*    Tolerance (0 .. 1)
		*
		*  @return
		*    Color key image effect
		*/
		static inline IEColorKey ColorKey(const Color3 &cColor, float fTolerance);

		/**
		*  @brief
		*    Create flip the image around the x axis image effect
		*
		*  @return
		*    Flip the image around the x axis image effect
		*/
		static inline IEFlipXAxis FlipXAxis();

		/**
		*  @brief
		*    Create flip the image around the y axis image effect
		*
		*  @return
		*    Flip the image around the y axis image effect
		*/
		static inline IEFlipYAxis FlipYAxis();

		/**
		*  @brief
		*    Create remove palette image effect
		*
		*  @return
		*    Remove palette image effect
		*/
		static inline IERemovePalette RemovePalette();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLGraphics/Image/ImageEffects.inl"


#endif // __PLGRAPHICS_IMAGEEFFECTS_H__
