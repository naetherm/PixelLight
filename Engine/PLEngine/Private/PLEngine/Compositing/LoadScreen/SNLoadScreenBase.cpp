/*********************************************************\
 *  File: SNLoadScreenBase.cpp                           *
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
#include <PLScene/Scene/SceneContainer.h>
#include "PLEngine/Compositing/LoadScreen/SNLoadScreenBase.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNLoadScreenBase)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNLoadScreenBase::SNLoadScreenBase() :
	Flags(this),
	EventHandlerContainer   (&SNLoadScreenBase::OnContainer,	this),
	EventHandlerLoadProgress(&SNLoadScreenBase::OnLoadProgress, this),
	m_pContainer(nullptr),
	m_fLoadProgress(0.0f)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoCulling);

	// Connect event handler
	SignalContainer.Connect(EventHandlerContainer);
}

/**
*  @brief
*    Destructor
*/
SNLoadScreenBase::~SNLoadScreenBase()
{
}

/**
*  @brief
*    Returns the current load progress
*/
float SNLoadScreenBase::GetLoadProgress() const
{
	return m_fLoadProgress;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node container changed
*/
void SNLoadScreenBase::OnContainer()
{
	// Disconnect event handler
	if (m_pContainer)
		m_pContainer->SignalLoadProgress.Disconnect(EventHandlerLoadProgress);

	// Connect event handler
	m_pContainer = GetContainer();
	if (m_pContainer)
		m_pContainer->SignalLoadProgress.Connect(EventHandlerLoadProgress);
}

/**
*  @brief
*    Called on load progress
*/
void SNLoadScreenBase::OnLoadProgress(float fLoadProgress)
{
	m_fLoadProgress = fLoadProgress;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNLoadScreenBase::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Show the load screen?
	if (m_fLoadProgress >= 0.0f && m_fLoadProgress <= 1.0f) {
		// Call base implementation
		SceneNode::DrawPost(cRenderer, pVisNode);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
