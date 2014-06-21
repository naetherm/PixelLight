/*********************************************************\
 *  File: SceneNodeModifier.inl                          *
 *      Scene node modifier inline implementation
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
*    Returns the owner scene node
*/
inline SceneNode &SceneNodeModifier::GetSceneNode() const
{
	return *m_pSceneNode;
}

/**
*  @brief
*    Returns the index of the given modifier
*/
inline int SceneNodeModifier::GetSceneNodeIndex()
{
	return m_pSceneNode->m_lstModifiers.GetIndex(this);
}

/**
*  @brief
*    Returns the scene context the owner scene node is in
*/
inline SceneContext *SceneNodeModifier::GetSceneContext() const
{
	// There's always a valid owner scene node, so we don't need to check for a null pointer!
	return m_pSceneNode->GetSceneContext();
}

/**
*  @brief
*    Is the scene node modifier active?
*/
inline bool SceneNodeModifier::IsActive() const
{
	return !(m_nFlags & Inactive);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
