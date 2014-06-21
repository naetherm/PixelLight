/*********************************************************\
 *  File: <Filename>                            *
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
#include "PLPhysicsBullet/Helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsBullet {


btQuaternion Helper::PLQuaternionTobtQuaternion(const PLMath::Quaternion& qPLQuat)
{
	return btQuaternion(qPLQuat.x, qPLQuat.y, qPLQuat.z, qPLQuat.w);
}

PLMath::Quaternion Helper::btQuaternionToPLQuaternion(const btQuaternion& qbtQuat)
{
	return PLMath::Quaternion(qbtQuat.w(), qbtQuat.x(), qbtQuat.y(), qbtQuat.z());
}

btVector3 Helper::PLVector3TobtVector3(const PLMath::Vector3& vPLVector3)
{
	return btVector3(vPLVector3.x, vPLVector3.y, vPLVector3.z);
}

PLMath::Vector3 Helper::btVector3ToPLVector3(const btVector3& vbtVector3)
{
	return PLMath::Vector3(vbtVector3.x(), vbtVector3.y(), vbtVector3.z());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
