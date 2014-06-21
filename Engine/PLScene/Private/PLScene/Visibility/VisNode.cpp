/*********************************************************\
 *  File: VisNode.cpp                                    *
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
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Visibility/SQCull.h"
#include "PLScene/Visibility/VisContainer.h"
#include "PLScene/Visibility/VisNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the visibility root container
*/
const VisContainer *VisNode::GetVisRootContainer() const
{
	if (m_pParent) {
		const SQCull *pSceneQuery = nullptr;
		if (m_pParent->IsContainer())
			pSceneQuery = static_cast<const VisContainer*>(m_pParent)->GetCullQuery();
		else if (m_pParent->IsPortal())
			pSceneQuery = static_cast<const VisContainer*>(m_pParent->GetParent())->GetCullQuery();
		return pSceneQuery ? pSceneQuery->GetVisRootContainer() : nullptr;
	} else {
		return static_cast<const VisContainer*>(this);
	}
}

/**
*  @brief
*    Returns the parent visibility node
*/
const VisNode *VisNode::GetParent() const
{
	return m_pParent;
}

/**
*  @brief
*    Returns the scene node this visibility node links to
*/
SceneNode *VisNode::GetSceneNode() const
{
	return m_pSceneNodeHandler->GetElement();
}

/**
*  @brief
*    Returns the used projection matrix
*/
const Matrix4x4 &VisNode::GetProjectionMatrix() const
{
	const VisNode *pVisNodeParent = GetParent();
	if (pVisNodeParent && pVisNodeParent->IsContainer())
		return static_cast<const VisContainer*>(pVisNodeParent)->GetCullQuery()->GetProjectionMatrix();
	else
		return Matrix4x4::Identity;
}

/**
*  @brief
*    Returns the used view matrix
*/
const Matrix4x4 &VisNode::GetViewMatrix() const
{
	const VisNode *pVisNodeParent = GetParent();
	if (pVisNodeParent && pVisNodeParent->IsContainer())
		return static_cast<const VisContainer*>(pVisNodeParent)->GetCullQuery()->GetViewMatrix();
	else
		return Matrix4x4::Identity;
}

/**
*  @brief
*    Returns the used view projection matrix
*/
const Matrix4x4 &VisNode::GetViewProjectionMatrix() const
{
	const VisNode *pVisNodeParent = GetParent();
	if (pVisNodeParent && pVisNodeParent->IsContainer())
		return static_cast<const VisContainer*>(pVisNodeParent)->GetCullQuery()->GetViewProjectionMatrix();
	else
		return Matrix4x4::Identity;
}

/**
*  @brief
*    Returns the absolute world matrix of the scene node
*/
const Matrix3x4 &VisNode::GetWorldMatrix() const
{
	return m_mWorld;
}

// [TODO] VisNode refactoring -> currently, this method should not be used if possible because it doesn't update for example GetInverseWorldMatrix()
void VisNode::SetWorldMatrix(const Matrix3x4 &mWorld)
{
	// Set the new world matrix
	m_mWorld = mWorld;

	// We now have to recalculate some derived data
	m_nInternalFlags |= RecalculateInvWorld;
	m_nInternalFlags |= RecalculateWorldView;
	m_nInternalFlags |= RecalculateWorldViewProj;
}

/**
*  @brief
*    Returns the absolute inverse world matrix of the scene node
*/
const Matrix3x4 &VisNode::GetInverseWorldMatrix() const
{
	// Recalculation required?
	if (m_nInternalFlags & RecalculateInvWorld) {
		// Calculate the matrix
		m_mInvWorld = m_mWorld.GetInverted();

		// Recalculation done
		m_nInternalFlags &= ~RecalculateInvWorld;
	}

	// Return the matrix
	return m_mInvWorld;
}

/**
*  @brief
*    Returns the absolute world view matrix of the scene node
*/
const Matrix4x4 &VisNode::GetWorldViewMatrix() const
{
	// Recalculation required?
	if (m_nInternalFlags & RecalculateWorldView) {
		// Calculate the world view transform matrix
		m_mWorldView = GetViewMatrix()*m_mWorld;

		// Recalculation done
		m_nInternalFlags &= ~RecalculateWorldView;
	}

	// Return the matrix
	return m_mWorldView;
}

/**
*  @brief
*    Returns the absolute world view projection matrix of the scene node
*/
const Matrix4x4 &VisNode::GetWorldViewProjectionMatrix() const
{
	// Recalculation required?
	if (m_nInternalFlags & RecalculateWorldViewProj) {
		// Calculate the world view projection transform matrix
		m_mWorldViewProj = GetViewProjectionMatrix()*m_mWorld;

		// Recalculation done
		m_nInternalFlags &= ~RecalculateWorldViewProj;
	}

	// Return the matrix
	return m_mWorldViewProj;
}

/**
*  @brief
*    Returns the squared distance to the camera
*/
float VisNode::GetSquaredDistanceToCamera() const
{
	return m_fSquaredDistanceToCamera;
}


//[-------------------------------------------------------]
//[ Public virtual VisNode functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether or not this is a container visibility node (VisContainer, links to a SceneContainer scene node)
*/
bool VisNode::IsContainer() const
{
	return false;
}

/**
*  @brief
*    Returns whether or not this is a cell visibility node (VisContainer, links to a SNCell scene node)
*/
bool VisNode::IsCell() const
{
	return false;
}

/**
*  @brief
*    Returns whether or not this is a portal visibility node (VisPortal, links to a SNCellPortal scene node)
*/
bool VisNode::IsPortal() const
{
	return false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VisNode::VisNode(VisNode *pParent) :
	m_pParent(pParent),
	m_pSceneNodeHandler(new SceneNodeHandler()),
	m_fSquaredDistanceToCamera(0.0f),
	m_nInternalFlags(0)
{
}

/**
*  @brief
*    Destructor
*/
VisNode::~VisNode()
{
	delete m_pSceneNodeHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
