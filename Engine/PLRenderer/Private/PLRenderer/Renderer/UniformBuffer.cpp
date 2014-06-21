/*********************************************************\
 *  File: UniformBuffer.cpp                              *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/UniformBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
UniformBuffer::~UniformBuffer()
{
}

/**
*  @brief
*    Copy operator
*/
UniformBuffer &UniformBuffer::operator =(const UniformBuffer &cSource)
{
	// Lock the source buffer
	if (const_cast<UniformBuffer&>(cSource).Lock(Lock::ReadOnly)) {
		// Clear old uniform buffer
		Clear();

		// Copy
		Allocate(cSource.GetNumOfElements(), cSource.GetUsage(), cSource.IsManaged());
		if (Lock(Lock::WriteOnly)) {
			MemoryManager::Copy(GetData(), const_cast<UniformBuffer&>(cSource).GetData(), cSource.GetSize());
			Unlock();
		}

		// Unlock the source buffer
		const_cast<UniformBuffer&>(cSource).Unlock();
	}

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
UniformBuffer::UniformBuffer(Renderer &cRenderer) : Buffer(cRenderer, TypeUniformBuffer)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
UniformBuffer::UniformBuffer(const UniformBuffer &cSource) : Buffer(cSource.GetRenderer(), TypeUniformBuffer)
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Public virtual Buffer functions                       ]
//[-------------------------------------------------------]
void *UniformBuffer::GetData()
{
	// We need this dummy default implementation because this function is used within this file!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
