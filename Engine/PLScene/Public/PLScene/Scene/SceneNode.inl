/*********************************************************\
 *  File: SceneNode.inl                                  *
 *      Scene node inline implementation
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Gets the node transform
*/
inline const PLMath::Transform3 &SceneNode::GetTransform() const
{
	return m_cTransform;
}

inline PLMath::Transform3 &SceneNode::GetTransform()
{
	return m_cTransform;
}

/**
*  @brief
*    Returns whether this scene node is a scene container (SceneContainer) or not
*/
inline bool SceneNode::IsContainer() const
{
	return ((m_nInternalFlags & ClassContainer) != 0);
}

/**
*  @brief
*    Returns whether this scene node is a cell (SCCell) or not
*/
inline bool SceneNode::IsCell() const
{
	return ((m_nInternalFlags & ClassCell) != 0);
}

/**
*  @brief
*    Returns whether this scene node is a portal (SNPortal) or not
*/
inline bool SceneNode::IsPortal() const
{
	return ((m_nInternalFlags & ClassPortal) != 0);
}

/**
*  @brief
*    Returns whether this scene node is a camera (SNCamera) or not
*/
inline bool SceneNode::IsCamera() const
{
	return ((m_nInternalFlags & ClassCamera) != 0);
}

/**
*  @brief
*    Returns whether this scene node is a light (SNLight) or not
*/
inline bool SceneNode::IsLight() const
{
	return ((m_nInternalFlags & ClassLight) != 0);
}

/**
*  @brief
*    Returns whether this scene node is a fog (SNFog) or not
*/
inline bool SceneNode::IsFog() const
{
	return ((m_nInternalFlags & ClassFog) != 0);
}

/**
*  @brief
*    Adds a modifier
*/
inline SceneNodeModifier *SceneNode::AddModifier(const PLCore::String &sClass, const PLCore::String &sParameters)
{
	return AddModifierAtIndex(sClass, sParameters, -1);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    The current axis align bounding box is dirty and must be updated if used next time
*/
inline void SceneNode::DirtyAABoundingBox()
{
	// We have to recalculate the current axis align bounding box in 'scene node space'
	m_nInternalFlags |= RecalculateAABoundingBox;

	// We have to recalculate the current axis align bounding box in 'scene container space' because this data is derived from the axis align bounding box
	m_nInternalFlags |= RecalculateContainerAABoundingBox;

	// We have to recalculate the bounding sphere in 'sphere node space' because this data is derived from the axis align bounding box
	m_nInternalFlags |= RecalculateBoundingSphere;

	// We have to recalculate the current bounding sphere in 'sphere container space' because this data is derived from the axis align bounding box
	m_nInternalFlags |= RecalculateContainerBoundingSphere;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
