/*********************************************************\
 *  File: TextureBuffer2DArray.h                         *
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


#ifndef __PLRENDEREROPENGL_TEXTUREBUFFER2DARRAY_H__
#define __PLRENDEREROPENGL_TEXTUREBUFFER2DARRAY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/Types.h>
#include <PLRenderer/Renderer/TextureBuffer2DArray.h>
#include "PLRendererOpenGL/PLRendererOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL 2D array texture buffer resource
*/
class TextureBuffer2DArray : public PLRenderer::TextureBuffer2DArray {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Renderer;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDEREROPENGL_API virtual ~TextureBuffer2DArray();

		/**
		*  @brief
		*    Returns the OpenGL resource ID of the texture buffer
		*
		*  @return
		*    OpenGL resource ID of the texture buffer
		*/
		PLRENDEREROPENGL_API PLCore::uint32 GetOpenGLTexture() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags (see EFlags)
		*
		*  @note
		*    - The image is assumed to be valid because it was already checked by the renderer texture buffer
		*      create function
		*/
		TextureBuffer2DArray(PLRenderer::Renderer &cRenderer, PLGraphics::Image &cImage, EPixelFormat nInternalFormat, PLCore::uint32 nFlags);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32 m_nOpenGLTexture;								/**< OpenGL texture ID */
		PLCore::uint32 m_nSamplerState[PLRenderer::Sampler::Number];	/**< Sampler states of this texture */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::TextureBuffer functions    ]
	//[-------------------------------------------------------]
	public:
		virtual bool CopyDataFrom(PLCore::uint32 nMipmap, EPixelFormat nFormat, const void *pData, PLCore::uint8 nFace = 0) override;
		virtual bool CopyDataTo(PLCore::uint32 nMipmap, EPixelFormat nFormat, void *pData, PLCore::uint8 nFace = 0) const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::TextureBuffer functions   ]
	//[-------------------------------------------------------]
	private:
		virtual bool MakeCurrent(PLCore::uint32 nStage) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::Resource functions        ]
	//[-------------------------------------------------------]
	private:
		virtual void BackupDeviceData(PLCore::uint8 **ppBackup) override;
		virtual void RestoreDeviceData(PLCore::uint8 **ppBackup) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_TEXTUREBUFFER2DARRAY_H__
