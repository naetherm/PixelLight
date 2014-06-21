/*********************************************************\
 *  File: Weight.cpp                                     *
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
#include "PLMesh/Weight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Weight::Weight() :
	m_nJoint(-1),
	m_fBias(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
Weight::~Weight()
{
}

/**
*  @brief
*    Get the weight joint number
*/
int Weight::GetJoint() const
{
	return m_nJoint;
}

/**
*  @brief
*    Sets the weight joint number
*/
void Weight::SetJoint(int nJoint)
{
	m_nJoint = nJoint;
}

/**
*  @brief
*    Get the weight bias
*/
float Weight::GetBias() const
{
	return m_fBias;
}

/**
*  @brief
*    Sets the weight bias
*/
void Weight::SetBias(float fBias)
{
	m_fBias = fBias;
}

/**
*  @brief
*    Copy operator
*/
Weight &Weight::operator =(const Weight &cSource)
{
	// Copy data
	m_nJoint = cSource.m_nJoint;
	m_fBias  = cSource.m_fBias;

	// Return pointer
	return *this;
}

/**
*  @brief
*    Compares two weights
*/
bool Weight::operator ==(const Weight &cWeight) const
{
	return (m_nJoint == cWeight.m_nJoint && m_fBias  == cWeight.m_fBias);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
