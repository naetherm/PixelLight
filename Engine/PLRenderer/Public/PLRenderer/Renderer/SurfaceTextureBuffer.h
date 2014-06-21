/*********************************************************\
 *  File: SurfaceTextureBuffer.h                         *
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


#ifndef __PLRENDERER_SURFACE_TEXTUREBUFFER_H__
#define __PLRENDERER_SURFACE_TEXTUREBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/TextureBuffer.h"
#include "PLRenderer/Renderer/Surface.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A texture buffer renderer surface where we can render in (RTT -> Render To Texture)
*/
class SurfaceTextureBuffer : public Surface {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Texture buffer surface flags
		*/
		enum EFlags {
			Depth                     = 1<<0,	/**< Depth buffer */
			Stencil                   = 1<<1,	/**< Stencil buffer */
			Mipmaps                   = 1<<2,	/**< Mipmaps (created "on the fly" by the GPU) */
			NoMultisampleAntialiasing = 1<<3	/**< Do not use multisample antialiasing */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~SurfaceTextureBuffer();

		/**
		*  @brief
		*    Returns the texture buffer format
		*
		*  @return
		*    The texture buffer format
		*/
		inline TextureBuffer::EPixelFormat GetFormat() const;

		/**
		*  @brief
		*    Returns the texture buffer surface flags
		*
		*  @return
		*    The texture buffer surface flags (see EFlags)
		*/
		inline PLCore::uint32 GetFlags() const;

		/**
		*  @brief
		*    Returns the maximum number of color render targets
		*
		*  @return
		*    The maximum number of color render targets
		*/
		inline PLCore::uint8 GetMaxColorTargets() const;

		/**
		*  @brief
		*    Sets the maximum number of color render targets
		*
		*  @param[in] nMaxColorTargets
		*    The maximum number of color render targets
		*/
		inline void SetMaxColorTargets(PLCore::uint8 nMaxColorTargets);


	//[-------------------------------------------------------]
	//[ Public virtual SurfaceTextureBuffer functions         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the renderer texture buffer this surface will render in
		*
		*  @return
		*    The renderer texture buffer this surface will render in, can be a null pointer
		*
		*  @note
		*    - If you currently render into the texture buffer it can't be used for texturing!
		*/
		virtual PLRenderer::TextureBuffer *GetTextureBuffer() const = 0;

		/**
		*  @brief
		*    Returns the current renderer texture buffer face this surface will render in
		*
		*  @return
		*    The current renderer texture buffer face this surface will render in
		*
		*  @note
		*    - Only valid if the target texture buffer is a cube map!
		*
		*  @see
		*    - 'TextureBufferCube.h' for more information about cube maps
		*/
		virtual PLCore::uint8 GetTextureBufferFace() const = 0;

		/**
		*  @brief
		*    Takes over the control of the depth buffer of the given surface texture buffer
		*
		*  @param[in] cSurfaceTextureBuffer
		*    Surface texture buffer we take the depth buffer away
		*/
		virtual void TakeDepthBufferFromSurfaceTextureBuffer(SurfaceTextureBuffer &cSurfaceTextureBuffer) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] nFlags
		*    Texture buffer surface flags (see EFlags)
		*  @param[in] nMaxColorTargets
		*    Maximum number of color render targets. This must be at least 1 - main renderer
		*    target color.
		*/
		PLRENDERER_API SurfaceTextureBuffer(Renderer &cRenderer, PLCore::uint32 nFlags = Depth | Stencil, PLCore::uint8 nMaxColorTargets = 1);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::uint32 m_nFlags;			/**< Texture buffer surface flags (see EFlags) */
		PLCore::uint8  m_nMaxColorTargets;	/**< Maximum number of color render targets */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		SurfaceTextureBuffer(const SurfaceTextureBuffer &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		SurfaceTextureBuffer &operator =(const SurfaceTextureBuffer &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/SurfaceTextureBuffer.inl"


#endif // __PLRENDERER_SURFACE_TEXTUREBUFFER_H__
