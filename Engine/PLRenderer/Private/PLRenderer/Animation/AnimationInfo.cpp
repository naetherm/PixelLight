/*********************************************************\
 *  File: AnimationInfo.cpp                              *
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
#include "PLRenderer/Animation/AnimationInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AnimationInfo::AnimationInfo() :
	m_nType(0),
	m_nStart(0),
	m_nEnd(0),
	m_fSpeed(24.0f),
	m_nFlags(0)
{
}

/**
*  @brief
*    Destructor
*/
AnimationInfo::~AnimationInfo()
{
}

/**
*  @brief
*    Sets the animation start frame
*/
void AnimationInfo::SetStartFrame(uint32 nStartFrame)
{
	// Set the start frame
	m_nStart = nStartFrame;

	// Resize frame information
	int i = m_nEnd-m_nStart;
	if (i < 0)
		i = -i;
	m_lstFrameInfos.Resize(i+1);
}

/**
*  @brief
*    Sets the animation end frame
*/
void AnimationInfo::SetEndFrame(uint32 nEndFrame)
{
	// Set the end frame
	m_nEnd = nEndFrame;

	// Resize frame information
	int i = m_nEnd-m_nStart;
	if (i < 0)
		i = -i;
	m_lstFrameInfos.Resize(i+1);
}


//[-------------------------------------------------------]
//[ Public virtual AnimationInfo functions                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy operator
*/
AnimationInfo &AnimationInfo::operator =(const AnimationInfo &cSource)
{
	// Copy data
	m_nType  = cSource.GetType();
	m_nStart = cSource.GetStartFrame();
	m_nEnd   = cSource.GetEndFrame();
	m_fSpeed = cSource.GetSpeed();
	m_nFlags = cSource.GetFlags();

	// Copy frame information
	m_lstFrameInfos = cSource.m_lstFrameInfos;

	// Copy events
	m_cEventManager.Clear();
	for (uint32 i=0; i<m_cEventManager.GetNumOfElements(); i++) {
		const AnimationEvent *pAnimationEvent = m_cEventManager.GetByIndex(i);

		// Added into a manager automatically
		new AnimationEvent(pAnimationEvent->GetID(), pAnimationEvent->GetFrame(),
						   &m_cEventManager, pAnimationEvent->GetName());
	}

	// Return a reference to this instance
	return *this;
}

/**
*  @brief
*    Get the animation source name
*/
String AnimationInfo::GetSourceName() const
{
	// To implement in derived classes
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
