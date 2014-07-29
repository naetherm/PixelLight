/*********************************************************\
 *  File: ClassMehod.inl                                 *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRefl {

/**
*  @brief
*    Comparison operator
*/
bool ClassConstructor::operator==(const ClassConstructor &cOther) const
{
	return (m_pFunc == cOther.m_pFunc);
}

/**
*  @brief
*    Invoke the method
*/
DynamicObject ClassConstructor::Construct(const PLCore::Iterable<PLCore::FunctionParam> *pParams) const
{
	if (m_pFunc)
	{
		return DynamicObject(m_pFunc->DynInvoke(pParams), m_cSignature.GetReturnType());
	}
	else
	{
		return DynamicObject();
	}
}

/**
*  @brief
*    Get this ctor's signature
*/
const PLCore::FunctionSignature &ClassConstructor::GetSignature() const
{
	return m_cSignature;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl
