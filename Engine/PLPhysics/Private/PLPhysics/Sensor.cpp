/*********************************************************\
 *  File: Sensor.cpp                                     *
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
#include "PLPhysics/World.h"
#include "PLPhysics/Sensor.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Sensor::~Sensor()
{
	ClearHitList();
}

/**
*  @brief
*    Returns the flags
*/
uint32 Sensor::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Sets the flags
*/
void Sensor::SetFlags(uint32 nFlags)
{
	m_nFlags = nFlags;
}

/**
*  @brief
*    Clears the list of hit bodies
*/
void Sensor::ClearHitList()
{
	for (uint32 i=0; i<m_lstBodyInfo.GetNumOfElements(); i++)
		FreeBodyInfo(*m_lstBodyInfo[i]);
	m_lstBodyInfo.Clear();
	m_pClosedBodyInfo = nullptr;
}

/**
*  @brief
*    Returns the number of hit physics bodies
*/
uint32 Sensor::GetNumOfHitBodies() const
{
	return m_lstBodyInfo.GetNumOfElements();
}

/**
*  @brief
*    Returns a hit physics body by index
*/
const Sensor::BodyInfo *Sensor::GetHitBody(uint32 nIndex) const
{
	return m_lstBodyInfo[nIndex];
}

/**
*  @brief
*    Returns the closest hit physics body
*/
const Sensor::BodyInfo *Sensor::GetClosestBody() const
{
	return m_pClosedBodyInfo;
}


//[-------------------------------------------------------]
//[ Public virtual Sensor functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Performs an immediate sensor check
*/
uint32 Sensor::Check()
{
	ClearHitList();

	// Done
	return 0;
}


//[-------------------------------------------------------]
//[ Public virtual Element functions                      ]
//[-------------------------------------------------------]
bool Sensor::IsBody() const
{
	return false;
}

bool Sensor::IsJoint() const
{
	return false;
}

bool Sensor::IsSensor() const
{
	return true;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Sensor::Sensor(World &cWorld, uint32 nFlags) : Element(cWorld),
	m_pClosedBodyInfo(nullptr),
	m_nFlags(nFlags)
{
}

/**
*  @brief
*    Returns a free body information
*/
Sensor::BodyInfo &Sensor::GetFreeBodyInfo()
{
	Pool<void*> &lstFreeBodyInformation = GetWorld().m_lstFreeBodyInformation;

	if (lstFreeBodyInformation.IsEmpty())
		return *(new BodyInfo);
	else {
		BodyInfo *pBodyInfo = static_cast<BodyInfo*>(lstFreeBodyInformation.Get(0));
		lstFreeBodyInformation.RemoveAtIndex(0);
		return *pBodyInfo;
	}
}

/**
*  @brief
*    Frees a body information
*/
void Sensor::FreeBodyInfo(Sensor::BodyInfo &cBodyInfo)
{
	GetWorld().m_lstFreeBodyInformation.Add(&cBodyInfo);
}


//[-------------------------------------------------------]
//[ Protected virtual Sensor functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sensor callback function
*/
bool Sensor::Callback(Body &cBody, float fDistance, const Vector3 &vNormal, int nCollisionID)
{
	BodyInfo &cBodyInfo = GetFreeBodyInfo();
	cBodyInfo.pBody        = &cBody;
	cBodyInfo.fDistance    = fDistance;
	cBodyInfo.vNormal      = vNormal;
	cBodyInfo.nCollisionID = nCollisionID;
	m_lstBodyInfo.Add(&cBodyInfo);

	// Check closest body
	if (m_pClosedBodyInfo) {
		if (m_pClosedBodyInfo->fDistance > cBodyInfo.fDistance)
			m_pClosedBodyInfo = &cBodyInfo;
	} else {
		m_pClosedBodyInfo = &cBodyInfo;
	}

	// Continue the check
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
