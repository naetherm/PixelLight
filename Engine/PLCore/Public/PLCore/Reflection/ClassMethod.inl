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
namespace PLCore {

/**
*  @brief
*    Comparison operator
*/
bool ClassMethod::operator==(const ClassMethod &cOther) const
{
	return (m_pFunc == cOther.m_pFunc && m_sName == cOther.m_sName);
}

/**
*  @brief
*    Invoke the method
*/
DynamicObject ClassMethod::Call(PLCore::Iterable<DynamicObject> *pParams) const
{
	if (m_pFunc)
	{
		return m_pFunc->DynInvoke(pParams);
	}
	else
	{
		return DynamicObject();
	}
}

/**
*  @brief
*    Directly invoke the method
*/
template <typename TRet, class TObject, typename... TArgs>
TRet ClassMethod::CallDirect(TObject *pObj, TArgs... args) const
{
	PL_TODO(ananta, "Upgrade this as with properties")

	// This method works with the assumption that every Function is derived from the
	// appropriate Invokable
	typedef PLCore::Invokable<TRet, TObject*, TArgs...> _InvType;
	_InvType* casted = (_InvType*)m_pFunc;

	return casted->Invoke(pObj, args...);
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
