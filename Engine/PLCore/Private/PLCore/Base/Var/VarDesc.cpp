/*********************************************************\
 *  File: VarDesc.cpp                                    *
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
#include "PLCore/PLCoreDefinitions.h"
#include "PLCore/Base/Class.h"
#include "PLCore/Base/Var/VarDesc.h"


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
VarDesc::VarDesc(int nType, const String &sType, const String &sName, const String &sDescription, const String &sAnnotation) :
	MemberDesc(MemberAttribute, sName, sDescription, sAnnotation),
	m_nType(nType),
	m_sType(sType)
{
}

/**
*  @brief
*    Destructor
*/
VarDesc::~VarDesc()
{
}

/**
*  @brief
*    Initialize instance
*/
void VarDesc::Dummy()
{
	// This method is just here to ensure, that the compiler will actually create static instances
}


//[-------------------------------------------------------]
//[ Public virtual VarDesc functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get default value as string
*/
String VarDesc::GetDefault() const
{
	// To be implemented in derived classes
	return "";
}

/**
*  @brief
*    Get attribute
*/
DynVar *VarDesc::GetAttribute(const Object &cObject) const
{
	// To be implemented in derived classes
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
