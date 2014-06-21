/*********************************************************\
 *  File: DynSignature.h                                 *
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


#ifndef __PLCORE_DYNSIGNATURE_H__
#define __PLCORE_DYNSIGNATURE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for signatures
*/
class DynSignature {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API DynSignature();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~DynSignature();


	//[-------------------------------------------------------]
	//[ Public virtual DynSignature functions                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get signature as string
		*
		*  @return
		*    Signature (e.g. "void(int,float)")
		*/
		PLCORE_API virtual String GetSignature() const;

		/**
		*  @brief
		*    Get the return type ID
		*
		*  @return
		*    Return type ID (e.g. "TypeNull" for "void()" or "TypeInt" for "int()"), "TypeInvalid" if there's no return type
		*/
		PLCORE_API virtual int GetReturnTypeID() const;

		/**
		*  @brief
		*    Return the number of parameters
		*
		*  @return
		*    Number of parameters
		*/
		PLCORE_API virtual uint32 GetNumOfParameters() const;

		/**
		*  @brief
		*    Get a parameter type ID
		*
		*  @param[in] nIndex
		*    Index of the parameter to return the type ID from
		*
		*  @return
		*    Parameter type ID (e.g. "TypeInt" for "void(int)"), "TypeInvalid" on error
		*/
		PLCORE_API virtual int GetParameterTypeID(uint32 nIndex) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_DYNSIGNATURE_H__
