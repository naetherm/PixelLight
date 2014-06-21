/*********************************************************\
 *  File: ImageEffectWrapper.h                           *
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


#ifndef __PLGRAPHICS_IMAGEEFFECT_WRAPPER_H__
#define __PLGRAPHICS_IMAGEEFFECT_WRAPPER_H__
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
*    Image effect wrapper
*/
class ImageEffectWrapper: public ImageEffect {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGRAPHICS_RTTI_EXPORT, ImageEffectWrapper, "PLGraphics", PLGraphics::ImageEffect, "Image effect wrapper")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sEffect
		*    Image effect class name
		*  @param[in] sParameters
		*    Image effect parameters
		*/
		inline ImageEffectWrapper(const PLCore::String &sEffect, const PLCore::String &sParameters);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ImageEffectWrapper();

		/**
		*  @brief
		*    Get wrapped image effect
		*
		*  @return
		*    Pointer to wrapped image effect, can be a null pointer
		*/
		inline ImageEffect *GetImageEffect() const;


	//[-------------------------------------------------------]
	//[ Public virtual ImageEffect functions                  ]
	//[-------------------------------------------------------]
	public:
		virtual bool Apply(Image &cImage) const override;
		virtual bool Apply(ImagePart &cPart) const override;
		virtual bool Apply(ImageBuffer &cBuffer) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ImageEffect	*m_pImageEffect;	/**< The wrapped image effect, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLGraphics/Image/ImageEffectWrapper.inl"


#endif // __PLGRAPHICS_IMAGEEFFECT_WRAPPER_H__
