/*********************************************************\
 *  File: Image.h                                        *
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


#ifndef __PLGUI_IMAGE_H__
#define __PLGUI_IMAGE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color3;
}
namespace PLMath {
	class Vector2i;
}
namespace PLGui {
	class Gui;
	class ImageImpl;
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
*    Image class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Image {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] sFilename
		*    Image filename
		*/
		PLGUI_API Image(Gui &cGui, const PLCore::String &sFilename = "");

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cImage
		*    Image that is copied
		*/
		PLGUI_API Image(const Image &cImage);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~Image();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cImage
		*    Image that is copied
		*/
		PLGUI_API Image &operator =(const Image &cImage);

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object (never a null pointer)
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get implementation
		*
		*  @return
		*    Pointer to platform  specific implementation
		*/
		PLGUI_API ImageImpl *GetImpl() const;

		/**
		*  @brief
		*    Check if image has been loaded
		*
		*  @return
		*    'true' if image is empty, else 'false'
		*/
		PLGUI_API bool IsEmpty() const;

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
		PLGUI_API bool Load(const PLCore::String &sFilename);

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
		PLGUI_API bool LoadWithColorKey(const PLCore::String &sFilename, const PLGraphics::Color3 &cColor);

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
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui				*m_pGui;		/**< GUI instance */
		ImageImpl		*m_pImageImpl;	/**< Image implementation object */
		PLCore::String	 m_sFilename;	/**< Image filename */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_IMAGE_H__
