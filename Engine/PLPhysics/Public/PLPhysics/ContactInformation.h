/*********************************************************\
 *  File: ContactInformation.h                           *
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


#ifndef __PLPHYSICS_CONTACTINFORMATION_H__
#define __PLPHYSICS_CONTACTINFORMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/PLPhysics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Body;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Contact information class
*/
class ContactInformation {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFirstBody
		*    The first physics body
		*  @param[in] cSecondBody
		*    The second physics body
		*/
		PLPHYSICS_API ContactInformation(Body &cFirstBody, Body &cSecondBody);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API ~ContactInformation();

		/**
		*  @brief
		*    Returns the first physics body
		*
		*  @return
		*    The first physics body
		*/
		PLPHYSICS_API Body &GetFirstBody() const;

		/**
		*  @brief
		*    Returns the first second body
		*
		*  @return
		*    The second physics body
		*/
		PLPHYSICS_API Body &GetSecondBody() const;

		/**
		*  @brief
		*    Returns whether or not the contact is marked to be ignored
		*
		*  @return
		*    'true' if the contact is marked as to be ignored, else 'false'
		*/
		PLPHYSICS_API bool IsContactIgnored() const;

		/**
		*  @brief
		*    Marks the contact to be ignored
		*
		*  @remarks
		*    By default, detected contacts are used by the physics. Using this function one can
		*    tell the physics not to use this contact.
		*/
		PLPHYSICS_API void IgnoreContact();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Body *m_pFirstBody;		/**< The first physics body, always valid! */
		Body *m_pSecondBody;	/**< The second physics body, always valid! */
		bool  m_bIgnoreContact;	/**< Is the contact marked to be ignored? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_CONTACTINFORMATION_H__
