/*********************************************************\
 *  File: SNSphereFog.cpp                                *
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
#include "PLScene/Scene/SNSphereFog.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNSphereFog)


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
const float SNSphereFog::MinRange = 0.0001f;


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
float SNSphereFog::GetRange() const
{
	return m_fRange;
}

void SNSphereFog::SetRange(float fValue)
{
	// Clamp minimum range
	if (fValue < MinRange)
		fValue = MinRange;

	// Same value?
	if (m_fRange != fValue) {
		// Set the new range
		m_fRange = fValue;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNSphereFog::SNSphereFog() :
	Range(this),
	Volumetricy(this),
	m_fRange(1.0f)
{
	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}

/**
*  @brief
*    Destructor
*/
SNSphereFog::~SNSphereFog()
{
}


//[-------------------------------------------------------]
//[ Public virtual SNFog functions                        ]
//[-------------------------------------------------------]
bool SNSphereFog::IsSphereFog() const
{
	return true;
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNSphereFog::UpdateAABoundingBox()
{
	SetAABoundingBox(AABoundingBox(-m_fRange, -m_fRange, -m_fRange, m_fRange, m_fRange, m_fRange));
}

void SNSphereFog::GetBoundingSphere(Sphere &cSphere)
{
	// The sphere has always the 'range' as radius
	cSphere.SetRadius(m_fRange);

	// The sphere is always within the scene node origin
	cSphere.SetPos(GetTransform().GetPosition());
}

void SNSphereFog::GetContainerBoundingSphere(Sphere &cSphere)
{
	// The sphere has always the 'range' as radius
	cSphere.SetRadius(m_fRange);

	// The sphere is always within the scene node origin
	cSphere.SetPos(GetTransform().GetPosition());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
