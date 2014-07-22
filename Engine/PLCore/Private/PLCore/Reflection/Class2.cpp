/*********************************************************\
 *  File: Rtti.h                                         *
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
#include <PLCore/Reflection/Class.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLRefl;

//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Find method by name
*/
const ClassMethod *Class::GetMethod(const PLCore::String &sName) const
{
	const ClassMethod &meth = m_mapMethods.Get(sName);
	if (meth == _MethodMap::Null)
	{
		// Method not found, try base classes
		auto it = m_lstBases.GetConstIterator();
		while (it.HasNext())
		{
			const ClassMethod *baseMeth = it.Next()->GetMethod(sName);
			if (baseMeth)
				return baseMeth;
		}

		return nullptr;
	}
	else
	{
		// Method found
		return &meth;
	}
}

/**
*  @brief
*    Find field by name
*/
const ClassField *Class::GetField(const PLCore::String &sName) const
{
	const ClassField &field = m_mapFields.Get(sName);
	if (field == _FieldMap::Null)
	{
		// Field not found, try base classes
		auto it = m_lstBases.GetConstIterator();
		while (it.HasNext())
		{
			const ClassField *baseField = it.Next()->GetField(sName);
			if (baseField)
				return baseField;
		}

		return nullptr;
	}
	else
	{
		// Field found
		return &field;
	}
}

/**
*  @brief
*    Find property by name
*/
const ClassProperty *Class::GetProperty(const PLCore::String &sName) const
{
	const ClassProperty &prop = m_mapProperties.Get(sName);
	if (prop == _PropertyMap::Null)
	{
		// Property not found, try base classes
		auto it = m_lstBases.GetConstIterator();
		while (it.HasNext())
		{
			const ClassProperty *baseProp = it.Next()->GetProperty(sName);
			if (baseProp)
				return baseProp;
		}

		return nullptr;
	}
	else
	{
		// Property found
		return &prop;
	}
}