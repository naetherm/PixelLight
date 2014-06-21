/*********************************************************\
 *  File: VertexWeights.cpp                              *
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
#include "PLMesh/VertexWeights.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VertexWeights::VertexWeights()
{
}

/**
*  @brief
*    Destructor
*/
VertexWeights::~VertexWeights()
{
}

/**
*  @brief
*    Get the vertex weight indices index array
*/
Array<uint32> &VertexWeights::GetWeights()
{
	return m_lstWeights;
}

const Array<uint32> &VertexWeights::GetWeights() const
{
	return m_lstWeights;
}

/**
*  @brief
*    Copy operator
*/
VertexWeights &VertexWeights::operator =(const VertexWeights &cSource)
{
	// Copy data
	m_lstWeights = cSource.m_lstWeights;

	// Return pointer
	return *this;
}

/**
*  @brief
*    Compares two vertex weights
*/
bool VertexWeights::operator ==(const VertexWeights &cVertexWeights) const
{
	return (m_lstWeights == cVertexWeights.m_lstWeights);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
