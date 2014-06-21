/*********************************************************\
 *  File: DynFunc.h                                      *
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


#ifndef __PLCORE_DYNFUNC_H__
#define __PLCORE_DYNFUNC_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Core/SmartPtr.h"
#include "PLCore/Base/Func/DynSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FuncDesc;
class DynParams;
class XmlElement;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for function objects (functoids)
*
*  @remarks
*    This is the virtual base class to access functions and function objects dynamically.
*    It is a virtual interface that allows you to call a function or function like object
*    regardless of it's actual type (e.g. static function, method of an object or function object).
*/
class DynFunc : public DynSignature {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~DynFunc();


	//[-------------------------------------------------------]
	//[ Public virtual DynFunc functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get function descriptor
		*
		*  @return
		*    Descriptor (can be a null pointer)
		*/
		PLCORE_API virtual const FuncDesc *GetDesc() const;

		/**
		*  @brief
		*    Call function
		*
		*  @param[in] cParams
		*    Parameters
		*/
		PLCORE_API virtual void Call(DynParams &cParams);

		/**
		*  @brief
		*    Call function
		*
		*  @param[in] cParams
		*    Parameters
		*/
		PLCORE_API virtual void Call(const DynParams &cParams);

		/**
		*  @brief
		*    Call function
		*
		*  @param[in] sParams
		*    Parameters as string
		*/
		PLCORE_API virtual void Call(const String &sParams);

		/**
		*  @brief
		*    Call function
		*
		*  @param[in] cElement
		*    Parameters as XML
		*/
		PLCORE_API virtual void Call(const XmlElement &cElement);

		/**
		*  @brief
		*    Call function with return as string
		*
		*  @param[in] sParams
		*    Parameters as string
		*
		*  @return
		*    Return of the function as string, empty string if there's no return
		*/
		PLCORE_API virtual String CallWithReturn(const String &sParams);

		/**
		*  @brief
		*    Call function with return as string
		*
		*  @param[in] cElement
		*    Parameters as XML
		*
		*  @return
		*    Return of the function as string, empty string if there's no return
		*/
		PLCORE_API virtual String CallWithReturn(const XmlElement &cElement);

		/**
		*  @brief
		*    Clone function object
		*
		*  @return
		*    Copy of this functoid (can be a null pointer!)
		*/
		PLCORE_API virtual DynFunc *Clone() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API DynFunc();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cDynFunc
		*    Source to copy from
		*/
		PLCORE_API DynFunc(const DynFunc &cDynFunc);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cDynFunc
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		PLCORE_API DynFunc &operator =(const DynFunc &cDynFunc);


};


//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
typedef SmartPtr<DynFunc> DynFuncPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_DYNFUNC_H__
