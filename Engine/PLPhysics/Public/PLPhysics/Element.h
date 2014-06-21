/*********************************************************\
 *  File: Element.h                                      *
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


#ifndef __PLPHYSICS_ELEMENT_H__
#define __PLPHYSICS_ELEMENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Element.h>
#include "PLPhysics/PLPhysics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class World;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics element base class
*/
class Element : public PLCore::Element<Element> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the world this element is in
		*
		*  @return
		*    The world this element is in
		*/
		PLPHYSICS_API World &GetWorld() const;

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~Element();


	//[-------------------------------------------------------]
	//[ Public virtual Element functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether or not this is a body
		*
		*  @return
		*    'true' if this is a body, else 'false'
		*/
		PLPHYSICS_API virtual bool IsBody() const = 0;

		/**
		*  @brief
		*    Returns whether or not this is a joint
		*
		*  @return
		*    'true' if this is a joint, else 'false'
		*/
		PLPHYSICS_API virtual bool IsJoint() const = 0;

		/**
		*  @brief
		*    Returns whether or not this is a sensor
		*
		*  @return
		*    'true' if this is a sensor, else 'false'
		*/
		PLPHYSICS_API virtual bool IsSensor() const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    World this element is in
		*/
		PLPHYSICS_API Element(World &cWorld);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_ELEMENT_H__
