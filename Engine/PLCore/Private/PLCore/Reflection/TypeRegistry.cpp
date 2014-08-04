/*********************************************************\
 *  File: TypeRegistry.cpp                               *
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
#include <PLCore/Reflection/TypeRegistry.h>
#include <PLCore/Reflection/ClassTypeInfo.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<TypeRegistry>;


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
TypeRegistry *TypeRegistry::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<TypeRegistry>::GetInstance();
}

bool TypeRegistry::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<TypeRegistry>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Register a new class into the system
*/
void TypeRegistry::RegisterClassType(const PLCore::String &sName, ClassTypeInfo *pTypeInfo)
{
	// The type may be already registered
	ClassTypeInfo *ti = m_mapClassTypes.Get(sName);
	if (ti == _ClassTypeMap::Null)
	{
		// Regiter a new type
		m_mapClassTypes.Add(sName, pTypeInfo);

		// [TODO] Fire event
	}
}

/**
*  @brief
*    Register a new primitive into the system
*/
void TypeRegistry::RegisterPrimitiveType(const PLCore::String &sName, PrimitiveTypeInfo *pTypeInfo)
{
	// The type may be already registered
	PrimitiveTypeInfo *ti = m_mapPrimitiveTypes.Get(sName);
	if (ti == _PrimitiveTypeMap::Null)
	{
		// Regiter a new type
		m_mapPrimitiveTypes.Add(sName, pTypeInfo);

		// [TODO] Fire event
	}
}

/**
*  @brief
*    Find a class by name
*/
const ClassTypeInfo *TypeRegistry::GetClassType(const PLCore::String &sName) const
{
	const ClassTypeInfo *ti = m_mapClassTypes.Get(sName);
	if (ti == _ClassTypeMap::Null)
	{
		// Class not found
		return nullptr;
	}
	else
	{
		// Class found
		return ti;
	}
}

/**
*  @brief
*    Find a class by name
*/
ClassTypeInfo *TypeRegistry::GetClassType(const PLCore::String &sName)
{
	ClassTypeInfo *ti = m_mapClassTypes.Get(sName);
	if (ti == _ClassTypeMap::Null)
	{
		// Class not found
		return nullptr;
	}
	else
	{
		// Class found
		return ti;
	}
}

/**
*  @brief
*    Find a primitive type by name
*/
const PrimitiveTypeInfo *TypeRegistry::GetPrimitiveType(const PLCore::String &sName) const
{
	const PrimitiveTypeInfo *ti = m_mapPrimitiveTypes.Get(sName);
	if (ti == _PrimitiveTypeMap::Null)
	{
		// Class not found
		return nullptr;
	}
	else
	{
		// Class found
		return ti;
	}
}

/**
*  @brief
*    Find a primitive type by name
*/
PrimitiveTypeInfo *TypeRegistry::GetPrimitiveType(const PLCore::String &sName)
{
	PrimitiveTypeInfo *ti = m_mapPrimitiveTypes.Get(sName);
	if (ti == _PrimitiveTypeMap::Null)
	{
		// Class not found
		return nullptr;
	}
	else
	{
		// Class found
		return ti;
	}
}