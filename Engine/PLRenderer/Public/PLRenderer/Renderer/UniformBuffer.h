/*********************************************************\
 *  File: UniformBuffer.h                                *
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


#ifndef __PLRENDERER_UNIFORMBUFFER_H__
#define __PLRENDERER_UNIFORMBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Buffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer uniform buffer (UBO, aka "constant buffer") resource
*/
class UniformBuffer : public Buffer {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~UniformBuffer();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this object
		*/
		PLRENDERER_API UniformBuffer &operator =(const UniformBuffer &cSource);

		/**
		*  @brief
		*    Copies data from a given buffer into this uniform buffer
		*
		*  @param[in] pData
		*    Data to copy into this uniform buffer, must be valid and must have at least "GetSize()" bytes
		*
		*  @remarks
		*    This ease of use method locks the uniform buffer, copies the
		*    given data into it and unlocks the uniform buffer when done.
		*/
		inline void CopyDataFrom(const void *pData);

		/**
		*  @brief
		*    Copies data from this uniform buffer into a given buffer
		*
		*  @param[out] pData
		*    Buffer to copy into, must be valid and must have at least "GetSize()" bytes
		*
		*  @remarks
		*    This ease of use method locks the uniform buffer, copies the uniform buffer
		*    data into the given buffer and unlocks the uniform buffer when done.
		*/
		inline void CopyDataTo(void *pData);


	//[-------------------------------------------------------]
	//[ Public virtual UniformBuffer functions                ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the name of the shader language the uniform buffer is using
		*
		*  @return
		*    The name of the shader language the uniform buffer is using (for example "GLSL" or "Cg")
		*
		*  @remarks
		*    In theory, the uniform buffer is independent of the used shader language because it's just a buffer.
		*    In practice, e.g. the Cg shader language comes with it's own way how to create and use uniform buffers,
		*    that's the only reason PixelLight connects an uniform buffer with a shader language.
		*/
		virtual PLCore::String GetShaderLanguage() const = 0;


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
		*/
		PLRENDERER_API UniformBuffer(Renderer &cRenderer);


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
		UniformBuffer(const UniformBuffer &cSource);


	//[-------------------------------------------------------]
	//[ Public virtual Buffer functions                       ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual void *GetData() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/UniformBuffer.inl"


#endif // __PLRENDERER_UNIFORMBUFFER_H__
