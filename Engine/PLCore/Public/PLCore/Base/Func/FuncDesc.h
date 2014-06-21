/*********************************************************\
 *  File: FuncDesc.h                                     *
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


#ifndef __PLCORE_FUNCDESC_H__
#define __PLCORE_FUNCDESC_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Func/DynFunc.h"
#include "PLCore/Base/MemberDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Object;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Function descriptor
*
*  @remarks
*    This class contains a descriptor for a method and consists of information
*    belonging to a specific method, such as it's name and signature.
*/
class FuncDesc : public MemberDesc {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sSignature
		*    Function signature
		*  @param[in] sName
		*    Function name
		*  @param[in] sDescription
		*    Function description
		*  @param[in] sAnnotation
		*    Function annotation
		*/
		PLCORE_API FuncDesc(const String &sSignature, const String &sName, const String &sDescription, const String &sAnnotation);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~FuncDesc();

		/**
		*  @brief
		*    Initialize instance
		*
		*  @remarks
		*    This method is just here to ensure, that the compiler will actually create static instances
		*/
		PLCORE_API void Dummy();

		/**
		*  @brief
		*    Get signature
		*
		*  @return
		*    Signature
		*/
		inline String GetSignature() const;


	//[-------------------------------------------------------]
	//[ Public virtual FuncDesc functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get callable method
		*
		*  @param[in] cObject
		*    RTTI class instance
		*
		*  @return
		*    Smart pointer to callable method (can be a null pointer)
		*
		*  @note
		*    - The default implementations returns a null pointer
		*/
		PLCORE_API virtual DynFuncPtr GetMethod(Object &cObject) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		String m_sSignature;	/**< Signature */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Base/Func/FuncDesc.inl"


#endif // __PLCORE_FUNCDESC_H__
