/*********************************************************\
 *  File: AnchorPoint.cpp                                *
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
#include "PLMesh/AnchorPoint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public AnchorPoint functions                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AnchorPoint::AnchorPoint(const String &sName, bool bType, uint32 nID, AnchorPointManager *pManager) :
	Element<AnchorPoint>(sName, pManager),
	m_bType(bType),
	m_nID(nID)
{
}

/**
*  @brief
*    Destructor
*/
AnchorPoint::~AnchorPoint()
{
}

/**
*  @brief
*    Returns the anchor type
*/
bool AnchorPoint::GetType() const
{
	return m_bType;
}

/**
*  @brief
*    Sets the anchor type
*/
void AnchorPoint::SetType(bool bType)
{
	m_bType = bType;
}

/**
*  @brief
*    Returns the anchors vertex/bone ID
*/
uint32 AnchorPoint::GetID() const
{
	return m_nID;
}

/**
*  @brief
*    Sets the anchors vertex/bone ID
*/
void AnchorPoint::SetID(uint32 nID)
{
	m_nID = nID;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Element functions              ]
//[-------------------------------------------------------]
AnchorPoint &AnchorPoint::operator =(const AnchorPoint &cSource)
{
	// Call base function
	*static_cast<Element<AnchorPoint>*>(this) = cSource;

	// Copy data
	m_bType = cSource.m_bType;
	m_nID   = cSource.m_nID;

	// Return pointer
	return *this;
}




//[-------------------------------------------------------]
//[ Public AnchorPointManager functions                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AnchorPointManager::AnchorPointManager()
{
}

/**
*  @brief
*    Destructor
*/
AnchorPointManager::~AnchorPointManager()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
AnchorPoint *AnchorPointManager::CreateElement(const String &sName)
{
	return new AnchorPoint(sName, 0, 0, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
