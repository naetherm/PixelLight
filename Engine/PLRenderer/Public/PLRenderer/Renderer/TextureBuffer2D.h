/*********************************************************\
 *  File: TextureBuffer2D.h                              *
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


#ifndef __PLRENDERER_TEXTUREBUFFER2D_H__
#define __PLRENDERER_TEXTUREBUFFER2D_H__
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
*    Abstract renderer 2D texture buffer resource
*/
class TextureBuffer2D : public TextureBuffer {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureBuffer2D();

		/**
		*  @brief
		*    Returns the texture buffer size
		*
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*
		*  @return
		*    Texture buffer size
		*/
		PLRENDERER_API PLMath::Vector2i GetSize(PLCore::uint32 nMipmap = 0) const;


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
		PLRENDERER_API TextureBuffer2D(Renderer &cRenderer, PLCore::uint32 nFlags);


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
		TextureBuffer2D(const TextureBuffer2D &cSource);

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
		TextureBuffer2D &operator =(const TextureBuffer2D &cSource);


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


#endif // __PLRENDERER_TEXTUREBUFFER2D_H__
