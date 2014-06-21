/*********************************************************\
 *  File: Signal.h                                       *
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


#ifndef __PLCORE_SIGNAL_H__
#define __PLCORE_SIGNAL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Event/Event.h"
#include "PLCore/Base/Event/EventDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Event of a class
*
*  @remarks
*    This class template represents signals (events that belong to objects).
*
*  @note
*    - Implementation of the observer design pattern (this class is the subject/observable, the source)
*/
template <typename DESC>
class Signal : public DESC::EventType {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static DESC	Desc;	/**< Event descriptor */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		Signal() :
			DESC::EventType()
		{
			// Ensure that the compiler will actually create static instances
			Desc.Dummy();
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Signal()
		{
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynEvent functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get event descriptor
		*
		*  @return
		*    Event descriptor
		*/
		virtual const EventDesc *GetDesc() const override
		{
			// Return descriptor
			return &Desc;
		}


};


// Static data instances
template<typename DESC>
DESC Signal<DESC>::Desc;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SIGNAL_H__
