/*********************************************************\
 *  File: Slot.h                                         *
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


#ifndef __PLCORE_SLOT_H__
#define __PLCORE_SLOT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Event/EventHandler.h"
#include "PLCore/Base/Event/EventHandlerDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Event handler of a class
*
*  @remarks
*    This class template represents slots (event handlers that belong to objects).
*
*  @note
*    - Implementation of the observer design pattern (this class is the observer, the destination)
*/
template <typename DESC>
class Slot : public DESC::EventHandlerType {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static DESC	Desc;	/**< Event handler descriptor */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pMemFunc
		*    Pointer to member function of a class
		*  @param[in] pObject
		*    Pointer to object to which the method belongs
		*/
		Slot(const typename DESC::MethType::MemFuncType &pMemFunc, typename DESC::ClassType *pObject) :
			DESC::EventHandlerType(pMemFunc, pObject)
		{
			// Ensure that the compiler will actually create static instances
			Desc.Dummy();
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Slot()
		{
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynFunc functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get event handler descriptor
		*
		*  @return
		*    Event handler descriptor
		*/
		virtual const EventHandlerDesc *GetDesc() const override
		{
			// Return descriptor
			return &Desc;
		}


};


// Static data instances
template<typename DESC>
DESC Slot<DESC>::Desc;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SLOT_H__
