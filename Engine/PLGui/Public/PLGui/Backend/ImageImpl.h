/*********************************************************\
 *  File: ImageImpl.h                                    *
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


#ifndef __PLGUI_IMAGEIMPL_H__
#define __PLGUI_IMAGEIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/RefCount.h>
#include <PLCore/String/String.h>
#include <PLMath/Vector2i.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color3;
};
namespace PLGui {
	class Image;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image implementation base class
*
*  @remarks
*    This class is the base class for platform specific image implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Image' abstraction
*/
class ImageImpl : public PLCore::RefCount<ImageImpl> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cImage
		*    Reference to platform independent image
		*/
		PLGUI_API ImageImpl(Image &cImage);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ImageImpl();

		/**
		*  @brief
		*    Get image filename
		*
		*  @return
		*    Filename of image
		*/
		PLGUI_API PLCore::String GetFilename() const;

		/**
		*  @brief
		*    Get image size
		*
		*  @return
		*    Size of image
		*/
		PLGUI_API PLMath::Vector2i GetSize() const;

		/**
		*  @brief
		*    Get color depth
		*
		*  @return
		*    Color depth
		*/
		PLGUI_API PLCore::uint32 GetColorDepth() const;


	//[-------------------------------------------------------]
	//[ Public virtual ImageImpl function                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load image
		*
		*  @param[in] sFilename
		*    Name of image file
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Load(const PLCore::String &sFilename) = 0;

		/**
		*  @brief
		*    Load image and apply a color key
		*
		*  @param[in] sFilename
		*    Name of image file
		*  @param[in] cColor
		*    Color that will be treated as transparent
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool LoadWithColorKey(const PLCore::String &sFilename, const PLGraphics::Color3 &cColor) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Image			 *m_pImage;			/**< Pointer to the platform independent image object */
		PLCore::String    m_sFilename;		/**< Image name */
		PLMath::Vector2i  m_vSize;			/**< Image size */
		PLCore::uint32    m_nColorDepth;	/**< Color depth */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_IMAGEIMPL_H__
