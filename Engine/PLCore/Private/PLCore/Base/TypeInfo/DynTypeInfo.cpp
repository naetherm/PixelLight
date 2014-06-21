/*********************************************************\
 *  File: DynTypeInfo.cpp                                *
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
#include "PLCore/Base/TypeInfo/DynTypeInfo.h"


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
DynTypeInfo::DynTypeInfo()
{
}

/**
*  @brief
*    Destructor
*/
DynTypeInfo::~DynTypeInfo()
{
}


//[-------------------------------------------------------]
//[ Public virtual DynTypeInfo functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get type ID
*/
int DynTypeInfo::GetTypeID() const
{
	// Unknown
	return -1;
}

/**
*  @brief
*    Get type name
*/
String DynTypeInfo::GetTypeName() const
{
	// Unknown
	return "";
}

/**
*  @brief
*    Check if this is an enum type
*/
bool DynTypeInfo::IsEnumType() const
{
	// No enum type
	return false;
}

/**
*  @brief
*    Check if this is a flag type
*/
bool DynTypeInfo::IsFlagType() const
{
	// No flag type
	return false;
}

/**
*  @brief
*    Get enum value names
*/
const List<String> &DynTypeInfo::GetEnumValues() const
{
	// Return list
	return m_lstEnumValues;
}

/**
*  @brief
*    Get enum value
*/
String DynTypeInfo::GetEnumValue(const String &sEnum) const
{
	// No enum type
	return "";
}

/**
*  @brief
*    Get enum description
*/
String DynTypeInfo::GetEnumDescription(const String &sEnum) const
{
	// No enum type
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
