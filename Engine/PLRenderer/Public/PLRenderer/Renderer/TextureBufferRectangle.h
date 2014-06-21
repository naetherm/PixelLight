/*********************************************************\
 *  File: TextureBufferRectangle.h                       *
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


#ifndef __PLRENDERER_TEXTUREBUFFERRECTANGLE_H__
#define __PLRENDERER_TEXTUREBUFFERRECTANGLE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include "PLRenderer/Renderer/TextureBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer rectangle texture buffer resource
*
*  @remarks
*    The hardware texturing is limited to images with power-of-two dimensions and an optional
*    1-texel border. But there’s also an special kind of texture buffers called ’rectangle textures’ without
*    requiring power-of-two dimensions.\n
*    Non-power-of-two dimensioned texture buffers are useful for storing video images that do not
*    have power-of-two dimensions. Re-sampling artifacts are avoided and less texture buffer memory
*    may be required by using non-power-of-two dimensioned texture buffers. Non-power-of-two dimensioned
*    texture buffers are also useful for shadow maps and window-space texturing.\n
*    However, non-power-of-two dimensioned (NPOTD) texture buffers have limitations that do not
*    apply to power-of-two dimensioned (POT) texture buffers. NPOTD texture buffers may not use mipmap
*    filtering; POTD texture buffers support both mipmapped and non-mipmapped filtering. NPOTD
*    texture buffers support only the CLAMP, CLAMP TO EDGE, and CLAMP TO BORDER wrap
*    modes; POTD texture buffers support CLAMP TO EDGE, REPEAT, CLAMP, MIRRORED
*    REPEAT, and CLAMP TO BORDER. NPOTD texture buffers do not support an optional 1-
*    texel border; POTD texture buffers do support an optional 1-texel border.\n
*/
class TextureBufferRectangle : public TextureBuffer {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureBufferRectangle();

		/**
		*  @brief
		*    Returns the texture buffer size
		*
		*  @return
		*    Texture buffer size
		*/
		inline const PLMath::Vector2i &GetSize() const;


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
		*    Texture buffer flags (see EFlags)
		*/
		PLRENDERER_API TextureBufferRectangle(Renderer &cRenderer, PLCore::uint32 nFlags);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLMath::Vector2i m_vSize;	/**< Texture buffer size */


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
		TextureBufferRectangle(const TextureBufferRectangle &cSource);

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
		TextureBufferRectangle &operator =(const TextureBufferRectangle &cSource);


	//[-------------------------------------------------------]
	//[ Public virtual TextureBuffer functions                ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual bool IsPowerOfTwo() const override;
		PLRENDERER_API virtual PLCore::uint32 GetNumOfPixels(PLCore::uint32 nMipmap = 0) const override;
		PLRENDERER_API virtual PLCore::uint32 GetNumOfBytes(PLCore::uint32 nMipmap = 0, EPixelFormat nFormat = Unknown) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/TextureBufferRectangle.inl"


#endif // __PLRENDERER_TEXTUREBUFFERRECTANGLE_H__
