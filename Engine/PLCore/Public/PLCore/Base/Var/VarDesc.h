/*********************************************************\
 *  File: VarDesc.h                                      *
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


#ifndef __PLCORE_VARDESC_H__
#define __PLCORE_VARDESC_H__
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
class DynVar;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Variable descriptor
*
*  @remarks
*    This class contains a descriptor for a variable and consists of information
*    belonging to a specific variable, such as it's name, type and description.
*/
class VarDesc : public MemberDesc {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nType
		*    Type ID
		*  @param[in] sType
		*    Type name
		*  @param[in] sName
		*    Var name
		*  @param[in] sDescription
		*    Var description
		*  @param[in] sAnnotation
		*    Var annotation
		*/
		PLCORE_API VarDesc(int nType, const String &sType, const String &sName, const String &sDescription, const String &sAnnotation);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~VarDesc();

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
		*    Get type ID
		*
		*  @return
		*    Type ID
		*/
		inline int GetTypeID() const;

		/**
		*  @brief
		*    Get type name
		*
		*  @return
		*    Type name
		*/
		inline String GetTypeName() const;


	//[-------------------------------------------------------]
	//[ Public virtual VarDesc functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get default value as string
		*
		*  @return
		*    Default value as string
		*/
		PLCORE_API virtual String GetDefault() const;

		/**
		*  @brief
		*    Get attribute
		*
		*  @param[in] cObject
		*    Object to return the attribute pointer from
		*
		*  @return
		*    Pointer to var (can be a null pointer, do not destroy the returned instance)
		*/
		PLCORE_API virtual DynVar *GetAttribute(const Object &cObject) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		int		m_nType;	/**< Type ID */
		String	m_sType;	/**< Type name */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Base/Var/VarDesc.inl"


#endif // __PLCORE_VARDESC_H__