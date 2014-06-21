/*********************************************************\
 *  File: MemberDesc.h                                   *
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


#ifndef __PLCORE_MEMBERDESC_H__
#define __PLCORE_MEMBERDESC_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/PLCoreDefinitions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ClassReal;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Member descriptor
*
*  @remarks
*    This class contains a descriptor for a member of a class, such as a variable
*    or a method. It consists of information equal for all member types (name, description, etc.).
*/
class MemberDesc {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nMemberType
		*    Member type
		*  @param[in] sName
		*    Var name
		*  @param[in] sDescription
		*    Var description
		*  @param[in] sAnnotation
		*    Var annotation
		*/
		PLCORE_API MemberDesc(EMemberType nMemberType, const String &sName, const String &sDescription, const String &sAnnotation);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~MemberDesc();

		/**
		*  @brief
		*    Get member type
		*
		*  @return
		*    Member type
		*/
		inline EMemberType GetMemberType() const;

		/**
		*  @brief
		*    Get name
		*
		*  @return
		*    Name
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Get description
		*
		*  @return
		*    Description
		*/
		inline String GetDescription() const;

		/**
		*  @brief
		*    Get annotation
		*
		*  @return
		*    Annotation
		*/
		inline String GetAnnotation() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Register member at class
		*
		*  @param[in] pClass
		*    Pointer to class (must be valid!)
		*/
		PLCORE_API void Register(ClassReal *pClass);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		EMemberType	m_nMemberType;	/**< Member type */
		String		m_sName;		/**< Name */
		String		m_sDescription;	/**< Description */
		String		m_sAnnotation;	/**< Annotation */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Base/MemberDesc.inl"


#endif // __PLCORE_MEMBERDESC_H__
