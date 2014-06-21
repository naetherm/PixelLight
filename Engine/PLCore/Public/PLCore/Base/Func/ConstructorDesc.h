/*********************************************************\
 *  File: ConstructorDesc.h                              *
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


#ifndef __PLCORE_CONSTRUCTORDESC_H__
#define __PLCORE_CONSTRUCTORDESC_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/MemberDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Object;
class DynFunc;
class DynParams;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor descriptor
*
*  @remarks
*    This class contains a descriptor for a specific constructor of a class
*/
class ConstructorDesc : public MemberDesc {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sSignature
		*    Signature
		*  @param[in] sName
		*    Name
		*  @param[in] sDescription
		*    Description
		*  @param[in] sAnnotation
		*    Annotation
		*/
		PLCORE_API ConstructorDesc(const String &sSignature, const String &sName, const String &sDescription, const String &sAnnotation);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ConstructorDesc();

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

		/**
		*  @brief
		*    Check if this is a default constructor
		*
		*  @return
		*    'true' if the constructor is a default constructor, else 'false'
		*/
		inline bool IsDefaultConstructor() const;


	//[-------------------------------------------------------]
	//[ Public virtual ConstructorDesc functions              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get constructor
		*
		*  @return
		*    Pointer to constructor (can be a null pointer)
		*/
		PLCORE_API virtual DynFunc *GetConstructor() const;

		/**
		*  @brief
		*    Create object by using typed constructor parameters
		*
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to new object (can be a null pointer)
		*/
		PLCORE_API virtual Object *Create(const DynParams &cParams);

		/**
		*  @brief
		*    Create object by using typeless constructor parameters
		*
		*  @param[in] sParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to new object (can be a null pointer)
		*/
		PLCORE_API virtual Object *Create(const String &sParams);


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
#include "PLCore/Base/Func/ConstructorDesc.inl"


#endif // __PLCORE_CONSTRUCTORDESC_H__
