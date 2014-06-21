/*********************************************************\
 *  File: VisPortal.cpp                                  *
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
#include "PLScene/Scene/SNCellPortal.h"
#include "PLScene/Visibility/VisContainer.h"
#include "PLScene/Visibility/VisPortal.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the target visibility container the portal links to
*/
VisContainer *VisPortal::GetTargetVisContainer() const
{
	// Is it possible to 'see through' this portal?
	const SceneNode *pSceneNode = GetSceneNode();
	if (!pSceneNode || !(pSceneNode->GetFlags() & SNCellPortal::NoSeeThrough))
		return m_pTargetCell;
	else
		return nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VisPortal::VisPortal(VisNode &cParent) : VisNode(&cParent),
	EventHandlerDestroy(&VisPortal::OnDestroy, this),
	m_pTargetCell(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
VisPortal::~VisPortal()
{
	if (m_pTargetCell)
		delete m_pTargetCell;
}

/**
*  @brief
*    Called when the scene node assigned with this visibility container was destroyed
*/
void VisPortal::OnDestroy()
{
	SceneNode *pSceneNode = GetSceneNode();
	if (pSceneNode && pSceneNode->IsPortal() && pSceneNode->IsInstanceOf("PLScene::SNCellPortal")) {
		// Get the parent visibility container
		const VisNode *pParent = GetParent();
		if (pParent && pParent->IsContainer()) {
			VisContainer *pParentVisContainer = const_cast<VisContainer*>(static_cast<const VisContainer*>(pParent));

			// Unregister within the parent container
			VisPortal *pPortal = pParentVisContainer->m_mapPortals.Get(pSceneNode->GetName());
			if (pPortal) {
				pParentVisContainer->m_lstNodes.Remove(pPortal);
				delete pPortal;
				pParentVisContainer->m_mapPortals.Remove(pSceneNode->GetName());
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual VisNode functions                      ]
//[-------------------------------------------------------]
bool VisPortal::IsPortal() const
{
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
