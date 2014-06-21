/*********************************************************\
 *  File: AnimationEvent.cpp                             *
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
#include "PLRenderer/Animation/AnimationEvent.h"


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
AnimationEvent::AnimationEvent(int nID, uint32 nFrame, AnimationEventManager *pManager, const String &sName) :
	Element<AnimationEvent>(sName, pManager),
	m_nID(nID),
	m_nFrame(nFrame)
{
	if (pManager && !sName.GetLength())
		SetName(String("Event ") + pManager->GetNumOfElements());
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Element functions              ]
//[-------------------------------------------------------]
AnimationEvent &AnimationEvent::operator =(const AnimationEvent &cSource)
{
	// Call base function
	*static_cast<Element<AnimationEvent>*>(this) = cSource;

	// Copy data
	m_nID    = cSource.m_nID;
	m_nFrame = cSource.m_nFrame;

	// Return pointer
	return *this;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
AnimationEvent *AnimationEventManager::CreateElement(const String &sName)
{
	return new AnimationEvent(0, 0, this, sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
