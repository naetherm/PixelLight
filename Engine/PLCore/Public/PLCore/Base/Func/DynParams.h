/*********************************************************\
 *  File: DynParams.h                                    *
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


#ifndef __PLCORE_DYNPARAMS_H__
#define __PLCORE_DYNPARAMS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Func/DynSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for parameters
*
*  @remarks
*    This is the virtual base class to pass parameters from and to functions.
*/
class DynParams : public DynSignature {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API DynParams();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~DynParams();


	//[-------------------------------------------------------]
	//[ Public virtual DynParams functions                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get a pointer to the return value
		*
		*  @return
		*    Pointer to the return value, null pointer if there's no return value
		*
		*  @note
		*    - Whenever possible, don't use this method, use typed access instead
		*    - If you really need to use this generic method, use it at least very carefully and always use "GetReturnTypeID()" to check for the real type
		*/
		PLCORE_API virtual void *GetPointerToReturnValue();

		/**
		*  @brief
		*    Get a pointer to a parameter value
		*
		*  @param[in] nIndex
		*    Index of the parameter to return a pointer to the value from
		*
		*  @return
		*    Pointer to the parameter, null pointer on error
		*
		*  @note
		*    - Whenever possible, don't use this method, use typed access instead
		*    - If you really need to use this generic method, use it at least very carefully and always use "GetParameterTypeID()" to check for the real type
		*/
		PLCORE_API virtual void *GetPointerToParameterValue(uint32 nIndex);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_DYNPARAMS_H__
