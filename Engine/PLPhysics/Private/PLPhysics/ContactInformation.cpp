/*********************************************************\
 *  File: ContactInformation.cpp                         *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/ContactInformation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ContactInformation::ContactInformation(Body &cFirstBody, Body &cSecondBody) :
	m_pFirstBody(&cFirstBody),
	m_pSecondBody(&cSecondBody),
	m_bIgnoreContact(false)
{
}

/**
*  @brief
*    Destructor
*/
ContactInformation::~ContactInformation()
{
	// Nothing to do in here
}

/**
*  @brief
*    Returns the first physics body
*/
Body &ContactInformation::GetFirstBody() const
{
	return *m_pFirstBody;
}

/**
*  @brief
*    Returns the first second body
*/
Body &ContactInformation::GetSecondBody() const
{
	return *m_pSecondBody;
}

/**
*  @brief
*    Returns whether or not the contact is marked to be ignored
*/
bool ContactInformation::IsContactIgnored() const
{
	return m_bIgnoreContact;
}

/**
*  @brief
*    Marks the contact to be ignored
*/
void ContactInformation::IgnoreContact()
{
	m_bIgnoreContact = true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
