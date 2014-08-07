/*********************************************************\
 *  File: ClassMethod.cpp                                *
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
#include "PLCore/Reflection/ClassProperty.h"
#include "PLCore/Container/Array.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default ctor
*/
ClassProperty::ClassProperty() :
	m_pSetter(nullptr),
	m_pGetter(nullptr)
{}

/**
*  @brief
*    Value ctor
*/
ClassProperty::ClassProperty(const PLCore::String &sName, PLCore::FunctionBase *pSetter, PLCore::FunctionBase *pGetter) :
	m_sName(sName),
	m_pSetter(pSetter),
	m_pGetter(pGetter)
{
	PLCore::FunctionSignature sig;
	sig = pSetter->GetSignature();
	sig = pGetter->GetSignature();
	int i = 0;
	i = i;
}


//[-------------------------------------------------------]
//[ Public virtual DefaultValue functions                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Reset the property to default value
*/
void ClassProperty::RestoreDefaultValue(const DynamicObject &cObject) const
{
	if (m_pSetter && GetDefaultValue().GetTypeInfo())
	{
		static Array<DynamicObject> val;
		val.Resize(2);

		val[0] = cObject;
		val[1] = GetDefaultValue();

		Set(&val);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore