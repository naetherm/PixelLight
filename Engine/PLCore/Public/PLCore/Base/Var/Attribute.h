/*********************************************************\
 *  File: Attribute.h                                    *
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


#ifndef __PLCORE_ATTRIBUTE_H__
#define __PLCORE_ATTRIBUTE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Var/Var.h"
#include "PLCore/Base/Var/VarDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Attribute of a class
*
*  @remarks
*    This class template represents attributes (variables that belong to objects).
*/
template <typename T, typename ACCESS, typename STORAGE, typename DESC>
class Attribute : public Var<T, ACCESS, STORAGE> {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage type
		typedef typename Type<T>::_Type _Type;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static DESC	Desc;	/**< Attribute descriptor */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] DefaultValue
		*    Default value for the attribute
		*  @param[in] pObject
		*    Pointer to object to which the attribute belongs
		*/
		Attribute(_Type DefaultValue, Object *pObject) : Var<T, ACCESS, STORAGE>(DefaultValue, pObject)
		{
			// Ensure that the compiler will actually create static instances
			Desc.Dummy();
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Attribute()
		{
		}

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] Value
		*    New value
		*
		*  @remarks
		*    Unfortunately, it is necessary to define assignment operators in the 'leaf'-classes,
		*    as they are not inherited from base classes. So we have to define it here and then
		*    call the implementation in the base class, which makes everything a bit more complicated
		*/
		Attribute &operator =(const _Type &Value)
		{
			// Call base implementation
			return static_cast<Attribute&>(Var<T, ACCESS, STORAGE>::operator =(Value));
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynVar functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get var descriptor
		*
		*  @return
		*    Var descriptor
		*/
		virtual const VarDesc *GetDesc() const override
		{
			// Return descriptor
			return &Desc;
		}


};


// Static data instances
template<typename T, typename ACCESS, typename STORAGE, typename DESC>
DESC Attribute<T, ACCESS, STORAGE, DESC>::Desc;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_ATTRIBUTE_H__
