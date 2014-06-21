/*********************************************************\
 *  File: IEScale.h                                      *
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


#ifndef __PLGRAPHICS_IMAGEEFFECT_SCALE_H__
#define __PLGRAPHICS_IMAGEEFFECT_SCALE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3i.h>
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
*    Image effect: Apply scale
*
*  @note
*    - The color format "ColorPalette" is not supported
*/
class IEScale : public ImageEffect {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGRAPHICS_RTTI_EXPORT, IEScale, "PLGraphics", PLGraphics::ImageEffect, "Image effect: Apply scale")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vNewSize
		*    New size
		*  @param[in] bUseMipmaps
		*    If there are mipmaps and one of them matches the new dimension... is it allowed to
		*    just make this mipmap to the new base image and destroy the now unused other mipmaps?
		*    (extremely fast 'scale', but no set filters are applied and may cause problems on 'none standard'
		*    images)
		*/
		PLGRAPHICS_API IEScale(const PLMath::Vector3i &vNewSize, bool bUseMipmaps);

		/**
		*  @brief
		*    Destructor
		*/
		PLGRAPHICS_API virtual ~IEScale();

		/**
		*  @brief
		*    Get new size
		*
		*  @return
		*    New size
		*/
		PLGRAPHICS_API PLMath::Vector3i GetNewSize() const;

		/**
		*  @brief
		*    Is it allowed to use existing mipmaps to scale down?
		*
		*  @return
		*    'true' if it's allowed to use existing mipmaps to scale down, else 'false'
		*/
		PLGRAPHICS_API bool GetUseMipmaps() const;


	//[-------------------------------------------------------]
	//[ Public virtual ImageEffect functions                  ]
	//[-------------------------------------------------------]
	public:
		PLGRAPHICS_API virtual bool Apply(Image &cImage) const override;
		PLGRAPHICS_API virtual bool Apply(ImageBuffer &cBuffer) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3i m_vNewSize;	/**< New size */
		bool			 m_bUseMipmaps;	/**< Is it allowed to use existing mipmaps to scale down? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


#endif // __PLGRAPHICS_IMAGEEFFECT_SCALE_H__
