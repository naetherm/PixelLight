/*********************************************************\
 *  File: ProgramUniformBlock.h                          *
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


#ifndef __PLRENDERER_PROGRAMUNIFORMBLOCK_H__
#define __PLRENDERER_PROGRAMUNIFORMBLOCK_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class String;
}
namespace PLRenderer {
	class UniformBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer program uniform block
*
*  @remarks
*    The behavior of the "SetUniformBuffer()"-method depends on the used internal shader API. When using Cg,
*    a call of the "SetUniformBuffer()"-method has only an effect when then program is going to be set in the
*    near future - meaning it has no effect when the program, the uniform block is part of, is already the program
*    currently used for rendering. When using GLSL, the "SetUniformBuffer()"-method is accessing a global UBO
*    binding point, meaning that it doesn't matter whether or not the program, the uniform block is part of, is
*    currently used for rendering. For performance reasons, this abstract program uniform block interface can't
*    compensate those differences. It has to be as lightweight as possible in order to keep the interface performance
*    impact as low as possible. So, to be on the safe side, do only call the "SetUniformBuffer()"-method if the program,
*    the uniform block is part of, is currently not used for rendering.
*
*  @note
*    - Do only call the "SetUniformBuffer()"-method when the program, the uniform block is part of, is currently not used for rendering
*/
class ProgramUniformBlock {


	//[-------------------------------------------------------]
	//[ Public virtual ProgramUniformBlock functions          ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets the uniform buffer feeding this uniform block with data
		*
		*  @param[in] pUniformBuffer
		*    The uniform buffer feeding this uniform block with data, can be a null pointer
		*  @param[in] nBindingPoint
		*    Binding point to use
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the number of bytes within the given uniform buffer
		*    is less than the number of bytes within this uniform block or there's a shader language mismatch)
		*
		*  @remarks
		*    The binding point is somewhat similar a texture unit. The total number of binding points
		*    which can be used at the same time during rendering are limited to e.g. 45 (just an example!)
		*    binding points.
		*/
		virtual bool SetUniformBuffer(UniformBuffer *pUniformBuffer, PLCore::uint32 nBindingPoint) = 0;

		/**
		*  @brief
		*    Returns the uniform block index
		*
		*  @return
		*    The uniform block index
		*/
		virtual PLCore::uint32 GetIndex() const = 0;

		/**
		*  @brief
		*    Returns the uniform block data size (in bytes)
		*
		*  @return
		*    The uniform block data size (in bytes)
		*/
		virtual PLCore::uint32 GetSize() const = 0;

		/**
		*  @brief
		*    Returns the offset (in bytes) of an uniform inside the uniform block
		*
		*  @param[in] sUniformName
		*    Name of the uniform
		*
		*  @return
		*    The offset (in bytes) of an uniform inside the uniform block
		*/
		virtual PLCore::uint32 GetUniformOffset(const PLCore::String &sUniformName) const = 0;

		/**
		*  @brief
		*    Returns the size (in bytes) of an uniform inside the uniform block
		*
		*  @return
		*    The size (in bytes) of an uniform inside the uniform block
		*/
		virtual PLCore::uint32 GetUniformSize(const PLCore::String &sUniformName) const = 0;

		/**
		*  @brief
		*    Returns the number of array elements of an uniform inside the uniform block
		*
		*  @return
		*    The number of array elements of an uniform inside the uniform block, 1 for none array uniforms
		*/
		virtual PLCore::uint32 GetUniformNumOfArrayElements(const PLCore::String &sUniformName) const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API ProgramUniformBlock();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~ProgramUniformBlock();


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
		ProgramUniformBlock(const ProgramUniformBlock &cSource);

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
		ProgramUniformBlock &operator =(const ProgramUniformBlock &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_PROGRAMUNIFORMBLOCK_H__
