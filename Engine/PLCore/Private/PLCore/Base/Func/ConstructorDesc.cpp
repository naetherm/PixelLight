/*********************************************************\
 *  File: ConstructorDesc.cpp                            *
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
#include "PLCore/Base/Class.h"
#include "PLCore/Base/Func/ConstructorDesc.h"


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
ConstructorDesc::ConstructorDesc(const String &sSignature, const String &sName, const String &sDescription, const String &sAnnotation) :
	MemberDesc(MemberConstructor, sName, sDescription, sAnnotation),
	m_sSignature(sSignature)
{
}

/**
*  @brief
*    Destructor
*/
ConstructorDesc::~ConstructorDesc()
{
}

/**
*  @brief
*    Initialize instance
*/
void ConstructorDesc::Dummy()
{
	// This method is just here to ensure, that the compiler will actually create static instances
}


//[-------------------------------------------------------]
//[ Public virtual ConstructorDesc functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get constructor
*/
DynFunc *ConstructorDesc::GetConstructor() const
{
	// To be implemented in derived classes
	return nullptr;
}

/**
*  @brief
*    Create object by using typed constructor parameters
*/
Object *ConstructorDesc::Create(const DynParams &cParams)
{
	// To be implemented in derived classes
	return nullptr;
}

/**
*  @brief
*    Create object by using typeless constructor parameters
*/
Object *ConstructorDesc::Create(const String &sParams)
{
	// To be implemented in derived classes
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
