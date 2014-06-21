/*********************************************************\
 *  File: Geometry.cpp                                   *
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
#include "PLMesh/Geometry.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Geometry::Geometry() :
	m_nFlags(0),
	m_bActive(true),
	m_nPrimitiveType(Primitive::Unknown),
	m_nMaterial(0),
	m_nStartIndex(0),
	m_nIndexSize(0)
{
}

/**
*  @brief
*    Destructor
*/
Geometry::~Geometry()
{
}

/**
*  @brief
*    Returns the geometrie's name
*/
String Geometry::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Sets the geometry name
*/
void Geometry::SetName(const String &sName)
{
	m_sName = sName;
}

/**
*  @brief
*    Returns the geometry flags
*/
uint32 Geometry::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Sets the geometry flags
*/
void Geometry::SetFlags(uint32 nFlags)
{
	m_nFlags = nFlags;
}

/**
*  @brief
*    Returns the geometry active flag
*/
bool Geometry::IsActive() const
{
	return m_bActive;
}

/**
*  @brief
*    Sets the geometry active flag
*/
void Geometry::SetActive(bool bActive)
{
	m_bActive = bActive;
}

/**
*  @brief
*    Returns the type of the primitives
*/
Primitive::Enum Geometry::GetPrimitiveType() const
{
	return m_nPrimitiveType;
}

/**
*  @brief
*    Sets the type of the primitives
*/
void Geometry::SetPrimitiveType(Primitive::Enum nPrimitiveType)
{
	m_nPrimitiveType = nPrimitiveType;
}

/**
*  @brief
*    Returns the material used by this geometry
*/
uint32 Geometry::GetMaterial() const
{
	return m_nMaterial;
}

/**
*  @brief
*    Sets the material used by this geometry
*/
void Geometry::SetMaterial(uint32 nMaterial)
{
	m_nMaterial = nMaterial;
}

/**
*  @brief
*    Returns the start index
*/
uint32 Geometry::GetStartIndex() const
{
	return m_nStartIndex;
}

/**
*  @brief
*    Sets the start index
*/
void Geometry::SetStartIndex(uint32 nStartIndex)
{
	m_nStartIndex = nStartIndex;
}

/**
*  @brief
*    Returns the index size
*/
uint32 Geometry::GetIndexSize() const
{
	return m_nIndexSize;
}

/**
*  @brief
*    Sets the index size
*/
void Geometry::SetIndexSize(uint32 nIndexSize)
{
	m_nIndexSize = nIndexSize;
}

/**
*  @brief
*    Returns the number of triangles the geometry consists of
*/
uint32 Geometry::GetNumOfTriangles() const
{
	// Get num of triangles
	if (m_nIndexSize > 2) {
		switch (m_nPrimitiveType) {
			case Primitive::TriangleList:
				return m_nIndexSize/3;

			case Primitive::TriangleStrip:
				return m_nIndexSize-2;

			case Primitive::TriangleFan:
				return m_nIndexSize-2;

			case Primitive::PointList:
			case Primitive::LineList:
			case Primitive::LineStrip:
			case Primitive::Number:
			case Primitive::Unknown:
			default:
				// Only triangle based primitives are relevant in this switch
				break;
		}
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Copy operator
*/
Geometry &Geometry::operator =(const Geometry &cSource)
{
	// Copy data
	m_sName			 = cSource.m_sName;
	m_nFlags		 = cSource.m_nFlags;
	m_bActive		 = cSource.m_bActive;
	m_nPrimitiveType = cSource.m_nPrimitiveType;
	m_nMaterial		 = cSource.m_nMaterial;
	m_nStartIndex	 = cSource.m_nStartIndex;
	m_nIndexSize	 = cSource.m_nIndexSize;

	// Return pointer
	return *this;
}

/**
*  @brief
*    Compares two geometries
*/
bool Geometry::operator ==(const Geometry &cGeometry) const
{
	return (m_sName			 == cGeometry.m_sName		   &&
		    m_nFlags		 == cGeometry.m_nFlags		   &&
		    m_bActive		 == cGeometry.m_bActive		   &&
		    m_nPrimitiveType == cGeometry.m_nPrimitiveType &&
		    m_nMaterial		 == cGeometry.m_nMaterial	   &&
		    m_nStartIndex	 == cGeometry.m_nStartIndex    &&
		    m_nIndexSize	 == cGeometry.m_nIndexSize);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
