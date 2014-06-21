/*********************************************************\
 *  File: ImagePL.h                                      *
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


#ifndef __PLFRONTENDPLGUI_COMPOSITING_GUI_IMAGE_H__
#define __PLFRONTENDPLGUI_COMPOSITING_GUI_IMAGE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Backend/ImageImpl.h>
#include "PLFrontendPLGui/PLFrontendPLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class TextureBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight ingame GUI image implementation
*/
class ImagePL : public PLGui::ImageImpl {


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
		PLFRONTENDPLGUI_API ImagePL(PLGui::Image &cImage);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~ImagePL();

		/**
		*  @brief
		*    Unload image
		*/
		PLFRONTENDPLGUI_API void Unload();

		/**
		*  @brief
		*    Returns the texture buffer
		*
		*  @return
		*    The texture buffer, a null pointer on error
		*/
		PLFRONTENDPLGUI_API PLRenderer::TextureBuffer *GetTextureBuffer();


	//[-------------------------------------------------------]
	//[ Public virtual PLGui::ImageImpl functions             ]
	//[-------------------------------------------------------]
	public:
		virtual bool Load(const PLCore::String &sFilename) override;
		virtual bool LoadWithColorKey(const PLCore::String &sFilename, const PLGraphics::Color3 &cColor) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGui::Image			  *m_pImage;			/**< Pointer to the platform independent image object */
		PLRenderer::TextureBuffer *m_pTextureBuffer;	/**< The texture buffer, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui


#endif // __PLFRONTENDPLGUI_COMPOSITING_GUI_IMAGE_H__
