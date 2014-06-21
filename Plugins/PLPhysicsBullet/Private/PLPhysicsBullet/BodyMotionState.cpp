/*********************************************************\
 *  File: BodyMotionState.cpp                            *
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
#include "PLPhysicsBullet/BodyMotionState.h"
#include "PLPhysicsBullet/Helper.h"
#include <PLPhysics/Body.h>
#include <PLMath/Vector3.h>
#include <PLMath/Quaternion.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsBullet {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
BodyMotionState::BodyMotionState(PLPhysics::Body *pBody)
	: m_pBody(pBody)
{
}

void BodyMotionState::setWorldTransform(const btTransform& worldTrans)
{
	if (!m_pBody)
		return;
	m_pBody->SetPosition(Helper::btVector3ToPLVector3(worldTrans.getOrigin()));
	m_pBody->SetRotation(Helper::btQuaternionToPLQuaternion(worldTrans.getRotation()));
	m_pBody->EventTransform();
}

void BodyMotionState::getWorldTransform(btTransform& worldTrans) const
{
	PLMath::Vector3 vPos;
	m_pBody->GetPosition(vPos);
	btVector3 btPos(Helper::PLVector3TobtVector3(vPos));
	worldTrans.setOrigin(btPos);
	
	PLMath::Quaternion qRot;
	m_pBody->GetRotation(qRot);
	btQuaternion btRot(Helper::PLQuaternionTobtQuaternion(qRot));
	worldTrans.setRotation(btRot);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
