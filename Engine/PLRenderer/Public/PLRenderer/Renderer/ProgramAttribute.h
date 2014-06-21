/*********************************************************\
 *  File: ProgramAttribute.h                             *
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


#ifndef __PLRENDERER_PROGRAMATTRIBUTE_H__
#define __PLRENDERER_PROGRAMATTRIBUTE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/VertexBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer program attribute (also known as "varying parameter")
*
*  @note
*    - For per-vertex data
*    - It doesn't matter whether or not the program, the attribute is part of, is currently used for rendering
*/
class ProgramAttribute {


	//[-------------------------------------------------------]
	//[ Public virtual ProgramAttribute functions             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets a attribute by using an index to reference the vertex buffer attribute to use
		*
		*  @param[in] pVertexBuffer
		*    Vertex buffer to use, can be a null pointer
		*  @param[in] nIndex
		*    Index of the vertex buffer attribute to connect with the vertex shader attribute
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Set(PLRenderer::VertexBuffer *pVertexBuffer, PLCore::uint32 nIndex) = 0;

		/**
		*  @brief
		*    Sets a attribute by using a semantic to reference the vertex buffer attribute to use
		*
		*  @param[in] pVertexBuffer
		*    Vertex buffer to use, can be a null pointer
		*  @param[in] nSemantic
		*    Semantic of the vertex buffer attribute to connect with the vertex shader attribute
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic, maximum see MaxPipelineChannels)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Set(PLRenderer::VertexBuffer *pVertexBuffer, PLRenderer::VertexBuffer::ESemantic nSemantic, PLCore::uint32 nChannel = 0) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API ProgramAttribute();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~ProgramAttribute();


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
		ProgramAttribute(const ProgramAttribute &cSource);

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
		ProgramAttribute &operator =(const ProgramAttribute &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_PROGRAMATTRIBUTE_H__
