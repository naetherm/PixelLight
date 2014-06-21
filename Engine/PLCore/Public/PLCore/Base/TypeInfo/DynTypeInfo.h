/*********************************************************\
 *  File: DynTypeInfo.h                                  *
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


#ifndef __PLCORE_DYNTYPEINFO_H__
#define __PLCORE_DYNTYPEINFO_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/List.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for types
*
*  @remarks
*    This is the virtual base class to access dynamic type information.
*    Usually it is better to use the static type template classes
*    such as Type<int> etc. However, sometimes it is necessary go get the
*    type e.g. of a variable in a dynamic (polymorphic) way.
*/
class DynTypeInfo {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API DynTypeInfo();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~DynTypeInfo();


	//[-------------------------------------------------------]
	//[ Public virtual DynTypeInfo functions                  ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get type ID
		*
		*  @return
		*    Type ID
		*/
		PLCORE_API virtual int GetTypeID() const;

		/**
		*  @brief
		*    Get type name
		*
		*  @return
		*    Type name
		*/
		PLCORE_API virtual String GetTypeName() const;

		/**
		*  @brief
		*    Check if this is an enum type
		*
		*  @return
		*    'true' if type is enum type, else 'false'
		*/
		PLCORE_API virtual bool IsEnumType() const;

		/**
		*  @brief
		*    Check if this is an flag type
		*
		*  @return
		*    'true' if type is flag type, else 'false'
		*/
		PLCORE_API virtual bool IsFlagType() const;

		/**
		*  @brief
		*    Get enum value names
		*
		*  @return
		*    List of enum value names
		*/
		PLCORE_API virtual const List<String> &GetEnumValues() const;

		/**
		*  @brief
		*    Get enum value
		*
		*  @param[in] sEnum
		*    Enum name
		*
		*  @return
		*    Enum value as string
		*/
		PLCORE_API virtual String GetEnumValue(const String &sEnum) const;

		/**
		*  @brief
		*    Get enum description
		*
		*  @param[in] sEnum
		*    Enum name
		*
		*  @return
		*    Description for that enum value
		*/
		PLCORE_API virtual String GetEnumDescription(const String &sEnum) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		List<String> m_lstEnumValues;	/**< List of enum values (empty but for enum-types) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_DYNTYPEINFO_H__
