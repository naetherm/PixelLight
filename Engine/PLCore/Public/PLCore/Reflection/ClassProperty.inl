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
*    Set the property
*/
void ClassProperty::Set(const PLCore::Iterable<PLCore::FunctionParam> *pParams) const
{
	if (m_pSetter)
	{
		m_pSetter->DynInvoke(pParams);
	}
}

/**
*  @brief
*    Get the property
*/
PLCore::FunctionParam ClassProperty::Get(const PLCore::Iterable<PLCore::FunctionParam> *pParams) const
{
	if (m_pGetter)
	{
		return m_pGetter->DynInvoke(pParams);
	}
	else
	{
		return PLCore::FunctionParam();
	}
}

/**
*  @brief
*    Directly set the property
*/
template <typename T, class TObject>
void ClassProperty::SetDirect(TObject *pObj, T cValue) const
{
	// This method works with the assumption that every Function is derived from the
	// appropriate Invokable
	typedef PLCore::Invokable<void, TObject*, T> _InvType;
	_InvType* casted = (_InvType*)m_pSetter;

	casted->Invoke(pObj, cValue);
}

/**
*  @brief
*    Directly get the property
*/
template <typename T, class TObject>
T ClassProperty::GetDirect(TObject *pObj) const
{
	// This method works with the assumption that every Function is derived from the
	// appropriate Invokable
	typedef PLCore::Invokable<T, TObject*> _InvType;
	_InvType* casted = (_InvType*)m_pGetter;

	return casted->Invoke(pObj);
}

/**
*  @brief
*    Comparison operator
*/
bool ClassProperty::operator==(const ClassProperty &cOther) const
{
	return (m_pSetter == cOther.m_pSetter && m_pGetter == cOther.m_pGetter && m_sName == cOther.m_sName);
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl
