/*********************************************************\
 *  File: Joint.cpp                                      *
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
#include "PLMesh/Joint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Joint::Joint(const String &sName, ElementManager<Joint> *pManager) : Element<Joint>(sName, pManager),
	m_nID(-1),
	m_nParent(-1)
{
}

/**
*  @brief
*    Destructor
*/
Joint::~Joint()
{
}

/**
*  @brief
*    Get the ID of this joint
*/
int Joint::GetID() const
{
	return m_nID;
}

/**
*  @brief
*    Set the ID this joint
*/
void Joint::SetID(int nID)
{
	m_nID = nID;
}

/**
*  @brief
*    Get the parent joint number of this joint
*/
int Joint::GetParent() const
{
	return m_nParent;
}

/**
*  @brief
*    Set the parent joint number of this joint
*/
void Joint::SetParent(int nParent)
{
	m_nParent = nParent;
}

/**
*  @brief
*    Gets an array with the child joints
*/
const Array<uint32> &Joint::GetChildren() const
{
	return m_lstChildren;
}

/**
*  @brief
*    Copy operator
*/
Joint &Joint::operator =(const Joint &cSource)
{
	// Copy data
	m_nID		  = cSource.m_nID;
	m_nParent	  = cSource.m_nParent;
	m_lstChildren = cSource.m_lstChildren;

	// Return this
	return *this;
}

/**
*  @brief
*    Compares two joints
*/
bool Joint::operator ==(const Joint &cJoint) const
{
	return (m_nID		  == cJoint.m_nID	  &&
		    m_nParent	  == cJoint.m_nParent &&
		    m_lstChildren == cJoint.m_lstChildren);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
