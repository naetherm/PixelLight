/*********************************************************\
 *  File: Chunk.cpp                                      *
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
#include "PLCore/Tools/Loader.h"
#include "PLCore/Tools/Chunk.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Chunk::Chunk() :
	m_nSemantic(Unknown),
	m_nElementType(Float),
	m_nNumOfComponentsPerElement(0),
	m_nNumOfElements(0),
	m_pData(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
Chunk::~Chunk()
{
	Clear();
}

/**
*  @brief
*    Allocates the chunk
*/
bool Chunk::Allocate(EElementType nElementType, uint32 nNumOfComponentsPerElement, uint32 nNumOfElements)
{
	// Clear the chunk
	Clear();

	// Backup the settings
	m_nElementType				 = nElementType;
	m_nNumOfComponentsPerElement = nNumOfComponentsPerElement;
	m_nNumOfElements			 = nNumOfElements;

	// Get the total number of bytes of the chunk
	uint32 nTotalNumOfBytes = GetTotalNumOfBytes();
	if (nTotalNumOfBytes) {
		// Allocate the chunk
		m_pData = new uint8[nTotalNumOfBytes];
	}

	// Done
	return true;
}

/**
*  @brief
*    Clears the chunk
*/
void Chunk::Clear()
{
	// Destroy the chunk data
	if (m_pData) {
		delete [] m_pData;
		m_pData = nullptr;
	}

	// Reset
	m_nElementType				 = Float;
	m_nNumOfComponentsPerElement = 0;
	m_nNumOfElements			 = 0;
}

/**
*  @brief
*    Returns the number of bytes per component of an element
*/
uint32 Chunk::GetNumOfBytesPerElementComponent() const
{
	switch (m_nElementType) {
		case Int8:
			return sizeof(int8);

		case Int16:
			return sizeof(int16);

		case Int32:
			return sizeof(int32);

		case UInt8:
			return sizeof(uint8);

		case UInt16:
			return sizeof(uint16);

		case UInt32:
			return sizeof(uint32);

		case Float:
			return sizeof(float);

		case Double:
			return sizeof(double);

		default:
			return 0;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Chunk::Chunk(const Chunk &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Chunk &Chunk::operator =(const Chunk &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual Loadable functions                     ]
//[-------------------------------------------------------]
bool Chunk::Unload()
{
	// Cleanup
	Clear();

	// Call base implementation
	return Loadable::Unload();
}

String Chunk::GetLoadableTypeName() const
{
	return "Chunk";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
