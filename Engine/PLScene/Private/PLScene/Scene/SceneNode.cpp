/*********************************************************\
 *  File: SceneNode.cpp                                  *
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
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/Timing.h>
#include <PLMath/Line.h>
#include <PLMath/Matrix4x4.h>
#include <PLMath/Intersect.h>
#include <PLMath/EulerAngles.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/MeshManager.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SNCellPortal.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneHierarchy.h"
#include "PLScene/Scene/SceneNodeModifier.h"
#include "PLScene/Scene/SceneHierarchyNode.h"
#include "PLScene/Scene/SceneHierarchyNodeItem.h"
#include "PLScene/Visibility/SQCull.h"
#include "PLScene/Visibility/VisContainer.h"
#include "PLScene/Scene/SceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneNode)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
uint32 SceneNode::GetFlags() const
{
	return m_nFlags;
}

void SceneNode::SetFlags(uint32 nValue)
{
	if (m_nFlags != nValue) {
		// Update active and visible state 'correctly'
		SetActive(!(nValue & Inactive));
		SetVisible(!(nValue & Invisible));

		// Set the new flags
		m_nFlags = nValue;
	}
}

uint32 SceneNode::GetDebugFlags() const
{
	return m_nDebugFlags;
}

void SceneNode::SetDebugFlags(uint32 nValue)
{
	m_nDebugFlags = nValue;
}

const Vector3 &SceneNode::GetPosition() const
{
	return m_cTransform.GetPosition();
}

void SceneNode::SetPosition(const Vector3 &vValue)
{
	m_cTransform.SetPosition(vValue);
}

Vector3 SceneNode::GetRotation() const
{
	// Internally, the rotation is stored as quaternion, so we now have to calculate an reasonable Euler angles representation...

	// Get an Euler angles representation of the rotation in radian
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fAngleZ = 0.0f;
	EulerAngles::FromQuaternion(m_cTransform.GetRotation(), fAngleX, fAngleY, fAngleZ);

	// Something like "-0" just looks stupid within for instance a GUI, so make it to "0"
	if (fAngleX == -0.0f)
		fAngleX = 0.0f;
	if (fAngleY == -0.0f)
		fAngleY = 0.0f;
	if (fAngleZ == -0.0f)
		fAngleZ = 0.0f;

	// To have well defined values, ensure that the Euler angles are between [0, 360]
	static const float Min = 0.0f;
	static const float Max = 360.0f;
	fAngleX = Math::WrapToInterval(static_cast<float>(fAngleX*Math::RadToDeg), Min, Max);
	fAngleY = Math::WrapToInterval(static_cast<float>(fAngleY*Math::RadToDeg), Min, Max);
	fAngleZ = Math::WrapToInterval(static_cast<float>(fAngleZ*Math::RadToDeg), Min, Max);

	// Return the Euler angles representation of the rotation in degree
	return Vector3(fAngleX, fAngleY, fAngleZ);
}

void SceneNode::SetRotation(const Vector3 &vValue)
{
	// Internally, the rotation is stored as quaternion, so we have to construct a quaternion by using the given Euler angles representation...
	// To have well defined values, ensure that the Euler angles are between [0, 360]
	Quaternion qRotation;
	static const float Min = 0.0f;
	static const float Max = 360.0f;
	EulerAngles::ToQuaternion(static_cast<float>(Math::WrapToInterval(vValue.x, Min, Max)*Math::DegToRad),
							  static_cast<float>(Math::WrapToInterval(vValue.y, Min, Max)*Math::DegToRad),
							  static_cast<float>(Math::WrapToInterval(vValue.z, Min, Max)*Math::DegToRad), qRotation);
	m_cTransform.SetRotation(qRotation);
}

const Vector3 &SceneNode::GetScale() const
{
	return m_cTransform.GetScale();
}

void SceneNode::SetScale(const Vector3 &vValue)
{
	m_cTransform.SetScale(vValue);
}

const Vector3 &SceneNode::GetAABBMin() const
{
	return m_cAABoundingBox.vMin;
}

void SceneNode::SetAABBMin(const Vector3 &vValue)
{
	m_cAABoundingBox.vMin = vValue;
	SetAABoundingBox(m_cAABoundingBox);
}

const Vector3 &SceneNode::GetAABBMax() const
{
	return m_cAABoundingBox.vMax;
}

void SceneNode::SetAABBMax(const Vector3 &vValue)
{
	m_cAABoundingBox.vMax = vValue;
	SetAABoundingBox(m_cAABoundingBox);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the scene context the scene node is in
*/
SceneContext *SceneNode::GetSceneContext() const
{
	// Is this a scene container?
	if (IsContainer()) {
		return static_cast<const SceneContainer*>(this)->m_pSceneContext;
	} else {
		// Get the scene context the parent scene container is in
		SceneContainer *pSceneContainer = static_cast<SceneContainer*>(GetManager());
		return pSceneContainer ? pSceneContainer->GetSceneContext() : nullptr;
	}
}

/**
*  @brief
*    Returns the scene container the scene node is in
*/
SceneContainer *SceneNode::GetContainer() const
{
	return static_cast<SceneContainer*>(GetManager());
}

/**
*  @brief
*    Sets the scene container the scene node is in
*/
bool SceneNode::SetContainer(SceneContainer &cSceneContainer)
{
	// Container change?
	if (&cSceneContainer != GetContainer()) {
		// Remove the scene node from the old parent container
		GetContainer()->Remove(*this, false);

		// Add the scene node to the new parent container
		cSceneContainer.Add(*this, GetName(), false);

		// Emit signal
		SignalContainer();
	}

	// Done
	return true;
}

/**
*  @brief
*    Returns the scene root container
*/
SceneContainer *SceneNode::GetRootContainer() const
{
	// Search for the scene root container
	const SceneNode *pSceneNode = static_cast<const SceneNode*>(this);
	while (pSceneNode->GetContainer() && pSceneNode->GetContainer()->GetContainer())
		pSceneNode = pSceneNode->GetContainer();

	// Return the found scene root container
	return (pSceneNode && pSceneNode->IsContainer()) ? const_cast<SceneContainer*>(static_cast<const SceneContainer*>(pSceneNode)) : nullptr;
}

/**
*  @brief
*    Gets the common container of this and another scene node
*/
SceneContainer *SceneNode::GetCommonContainer(SceneNode &cSceneNode) const
{
	// Same scene node?
	if (&cSceneNode == this) {
		return IsContainer() ? static_cast<SceneContainer*>(&cSceneNode) : nullptr;
	} else {
		// Go down the parents of this scene node
		for (const SceneContainer *pThisSceneContainer=GetContainer(); pThisSceneContainer!=nullptr; pThisSceneContainer=pThisSceneContainer->GetContainer()) {
			// Find the first common parent - go down the parents of the other scene node
			for (SceneContainer *pOtherSceneContainer=cSceneNode.GetContainer(); pOtherSceneContainer!=nullptr; pOtherSceneContainer=pOtherSceneContainer->GetContainer()) {
				// Common parent found?
				if (pThisSceneContainer == pOtherSceneContainer)
					return pOtherSceneContainer;
			}
		}

		// No common parent found
		return nullptr;
	}
}

/**
*  @brief
*    Returns the index of this scene node within the scene node list of the scene container this scene node is in
*/
int SceneNode::GetContainerIndex()
{
	return GetManager() ? static_cast<SceneContainer*>(GetManager())->GetIndex(*this) : -1;
}

/**
*  @brief
*    Returns the scene hierarchy this scene node is linked into
*/
SceneHierarchy *SceneNode::GetHierarchy() const
{
	// Is there a scene hierarchy node item and is there a scene hierarchy node?
	if (m_pFirstSceneHierarchyNodeItem && m_pFirstSceneHierarchyNodeItem->GetHierarchyNode()) {
		// Return the scene hierarchy
		return m_pFirstSceneHierarchyNodeItem->GetHierarchyNode()->GetHierarchy();
	} else {
		// Error!
		return nullptr;
	}
}

/**
*  @brief
*   Returns the unique absolute name of the scene node
*/
String SceneNode::GetAbsoluteName() const
{
	// Get a list of all parent containers
	Array<const SceneContainer*> lstContainers;
	const SceneContainer *pParent = GetContainer();
	while (pParent && pParent->GetContainer()) {
		lstContainers.Add(pParent);
		pParent = pParent->GetContainer();
	}

	// Construct the absolute scene node name
	String sAbsoluteName = "Root.";
	for (int i=lstContainers.GetNumOfElements()-1; i>=0; i--) {
		sAbsoluteName += lstContainers[i]->GetName();
		sAbsoluteName += '.';
	}
	sAbsoluteName += GetName();

	// Return the absolute scene node name
	return sAbsoluteName;
}

/**
*  @brief
*    Is the scene node initialized?
*/
bool SceneNode::IsInitialized() const
{
	return ((m_nInternalFlags & Initialized) != 0);
}

/**
*  @brief
*    Returns whether the scene node is active or not
*/
bool SceneNode::IsActive() const
{
	return !((m_nFlags & Inactive) != 0);
}

/**
*  @brief
*    Sets whether the scene node is active or not
*/
void SceneNode::SetActive(bool bActive)
{
	// Is it currently allowed to change this state?
	if (!(m_nInternalFlags & DestroyThis)) {
		// Change active state?
		if ((bActive && (m_nFlags & Inactive)) || (!bActive && !(m_nFlags & Inactive))) {
			// Set the flag
			if (bActive)
				m_nFlags &= ~Inactive;
			else
				m_nFlags |=  Inactive;

			// Add/remove the scene node from the scene container hierarchy
			const SceneContainer *pContainer = GetContainer();
			if (pContainer) {
				SceneHierarchy *pHierarchy = pContainer->m_pHierarchy;
				if (pHierarchy) {
					if (IsActive())
						pHierarchy->AddSceneNode(*this);
					else
						pHierarchy->RemoveSceneNode(*this);
				}
			}

			// Call the "OnActivate()"-method
			OnActivate(EvaluateGlobalActiveState());

			// Emit signal
			SignalActive();
		}
	}
}

/**
*  @brief
*    Evaluates whether or not the scene node is active in respect of the complete scene graph
*/
bool SceneNode::EvaluateGlobalActiveState() const
{
	// Go down the rabbit-hole (iterative implementation instead of recursive implementation)
	const SceneNode *pSceneNode = this;
	do {
		// Is the current scene node active and not frozen?
		const uint32 nFlags = pSceneNode->GetFlags();
		if ((nFlags & Inactive) || (nFlags & Frozen))
			return false;	// Not active

		// Go deeper...
		pSceneNode = pSceneNode->GetContainer();
	} while (pSceneNode);

	// Still here? Then it's active!
	return true;
}

/**
*  @brief
*    Returns whether the scene node is visible or not
*/
bool SceneNode::IsVisible() const
{
	if (m_nFlags & Inactive)
		return false; // Not active = not visible
	else
		return !((m_nFlags & Invisible) != 0);
}

/**
*  @brief
*    Sets whether the scene node is visible or not
*/
void SceneNode::SetVisible(bool bVisible)
{
	// Change visible state?
	if ((bVisible && (m_nFlags & Invisible)) || (!bVisible && !(m_nFlags & Invisible))) {
		// Set the flag
		if (bVisible)
			m_nFlags &= ~Invisible;
		else
			m_nFlags |=  Invisible;

		// Emit signal
		SignalVisible();
	}
}

/**
*  @brief
*    Returns whether the scene node is frozen or not
*/
bool SceneNode::IsFrozen() const
{
	return ((m_nFlags & Frozen) != 0);
}

/**
*  @brief
*    Sets whether the scene node is frozen or not
*/
void SceneNode::SetFrozen(bool bFrozen)
{
	// State change?
	if (((m_nFlags & Frozen) != 0) != bFrozen) {
		// Set the new frozen state
		if (bFrozen)
			m_nFlags |=  Frozen;
		else
			m_nFlags &= ~Frozen;

		// Call the "OnActivate()"-method
		OnActivate(EvaluateGlobalActiveState());
	}
}

/**
*  @brief
*    Gets the scene node draw function flags
*/
uint8 SceneNode::GetDrawFunctionFlags() const
{
	return m_nDrawFunctionFlags;
}

/**
*  @brief
*    Sets the scene node draw function flags
*/
void SceneNode::SetDrawFunctionFlags(uint8 nFlags)
{
	m_nDrawFunctionFlags = nFlags;
}


//[-------------------------------------------------------]
//[ Transform                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Moves the node to a new position
*/
void SceneNode::MoveTo(const Vector3 &vPosition)
{
	// Is this scene node within a cell? If not, so special cell-portal stuff must be done :)
	if (GetContainer() && !GetContainer()->IsCell()) {
		m_cTransform.SetPosition(vPosition);
	} else {
		Line cLine(m_cTransform.GetPosition(), vPosition);
		for (uint32 i=0; i<GetContainer()->GetNumOfElements(); i++) {
			SceneNode &cSceneNode = *GetContainer()->GetByIndex(i);

			// Is this an active cell-portal?
			if (&cSceneNode != this && cSceneNode.IsActive() && cSceneNode.IsPortal() &&
				cSceneNode.IsInstanceOf("PLScene::SNCellPortal") && !(cSceneNode.GetFlags() & SNCellPortal::NoPassThrough)) {
				// Do we intersect the cell-portal? We use a sphere intersection because a axis aligned bounding box
				// one may fail if the box dimension is small and the 'movement velocity' low...
				// [TODO] Fails sometimes, too...
	//			if (Intersect::SphereLine(cSceneNode.GetContainerBoundingSphere(), cLine.vStart, cLine.vEnd)) {
				if (Intersect::AABoxLine(cSceneNode.GetContainerAABoundingBox().vMin, cSceneNode.GetContainerAABoundingBox().vMax, cLine)) {
					// Get the target cell
					SNCellPortal	&cCellPortal = static_cast<SNCellPortal&>(cSceneNode);
					const SceneNode *pTargetCell = reinterpret_cast<const SceneNode*>(cCellPortal.GetTargetCellInstance());
					if (pTargetCell) {
						// Transform the line into the scene node space
						Line cLineT(cLine);
						cLineT *= cCellPortal.GetTransform().GetInverseMatrix();

						// 'Walk through' the cell-portal?
						const Plane &cPlane = cCellPortal.GetPolygon().GetPlane();
						if (cPlane.GetSide(cLineT.vStart) == Plane::Behind &&
							cPlane.GetSide(cLineT.vEnd)   != Plane::Behind) {
							// Set the new parent container
							SetContainer(*const_cast<SceneContainer*>(static_cast<const SceneContainer*>(pTargetCell)));

							// The new position, rotation and scale
							Vector3    vNewPosition = m_cTransform.GetPosition();
							Quaternion qNewRotation = m_cTransform.GetRotation();
							Vector3    vNewScale    = m_cTransform.GetScale();

							// We need to 'jump out of the cell-portal' a bit to avoid a 'black out' :)
							Vector3 vDir = vPosition;
							vDir -= vNewPosition;
							vDir.Normalize();
							vDir *= 0.001f;
							vNewPosition += vDir;

							// Update position, scale and rotation using the given warp matrix of the cell-portal
							const Matrix3x4 &mWarp = cCellPortal.GetWarpMatrix();
							// Position
							vNewPosition *= mWarp;
							m_cTransform.SetPosition(vNewPosition);
							// Rotation
							qNewRotation *= mWarp;
							m_cTransform.SetRotation(qNewRotation);
							// Scale
							mWarp.RotateVector(vNewScale);
							m_cTransform.SetScale(vNewScale);

							// Done, get out of here!
							return;
						}
					}
				}
			}
		}

		// There was no cell-portal, so, just set then position
		m_cTransform.SetPosition(vPosition);
	}
}


//[-------------------------------------------------------]
//[ Bounding volume                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the axis align bounding box in 'scene node space'
*/
const AABoundingBox &SceneNode::GetAABoundingBox()
{
	// Calculate the axis align bounding box in 'scene node space' if required
	if (m_nInternalFlags & RecalculateAABoundingBox) {
		// Update the axis align bounding box in 'scene node space'
		UpdateAABoundingBox();

		// Recalculation done
		m_nInternalFlags &= ~RecalculateAABoundingBox;
	}

	// Return the axis align bounding box in 'scene node space'
	return m_cAABoundingBox;
}

/**
*  @brief
*    Sets the axis align bounding box in 'scene node space'
*/
void SceneNode::SetAABoundingBox(const AABoundingBox &cAABoundingBox)
{
	// Set new axis align bounding box
	m_cAABoundingBox = cAABoundingBox;

	// We have to recalculate the current axis align bounding box in 'scene container space' because this data is derived from the axis align bounding box
	m_nInternalFlags |= RecalculateContainerAABoundingBox;

	// We have to recalculate the bounding sphere in 'sphere node space' because this data is derived from the axis align bounding box
	m_nInternalFlags |= RecalculateBoundingSphere;

	// We have to recalculate the current bounding sphere in 'sphere container space' because this data is derived from the axis align bounding box
	m_nInternalFlags |= RecalculateContainerBoundingSphere;

	// We need a hierarchy refresh for this scene node
	HierarchyRefreshRequired();

	// Is this a container? We could do this within a scene container by using scene node listeners,
	// but this here should also be ok...
	if (IsContainer()) {
		// Destroy the current hierarchy, but do NOT create the new one at once - this
		// is done if the new hierarchy is requested the first time  :)
		SceneContainer *pContainer = static_cast<SceneContainer*>(this);
		if (pContainer->m_pHierarchy) {
			delete pContainer->m_pHierarchy;
			pContainer->m_pHierarchy = nullptr;
		}
	}

	// Emit signal
	SignalAABoundingBox();
}

/**
*  @brief
*    Returns the current axis align bounding box in 'scene container space'
*/
const AABoundingBox &SceneNode::GetContainerAABoundingBox()
{
	// Calculate current axis align bounding box in 'scene container space' if required
	if (m_nInternalFlags & RecalculateContainerAABoundingBox) {
		const AABoundingBox &cAABB = GetAABoundingBox();
		const Vector3 &vMin = cAABB.vMin, &vMax = cAABB.vMax;
		const Matrix3x4 &mTrans = m_cTransform.GetMatrix();

		// Transform ALL eight corners bounding box corners into 'scene container space'
		// and get min/max values of the 'scene container space' bounding box
		Vector3 vT = vMin;
		vT *= mTrans;
		m_cContainerAABoundingBox.vMin = m_cContainerAABoundingBox.vMax = vT;
		vT.SetXYZ(vMax.x, vMin.y, vMin.z);
		vT *= mTrans;
		m_cContainerAABoundingBox.AppendToCubicHull(vT);
		vT.SetXYZ(vMax.x, vMax.y, vMin.z);
		vT *= mTrans;
		m_cContainerAABoundingBox.AppendToCubicHull(vT);
		vT.SetXYZ(vMin.x, vMax.y, vMin.z);
		vT *= mTrans;
		m_cContainerAABoundingBox.AppendToCubicHull(vT);
		vT.SetXYZ(vMin.x, vMin.y, vMax.z);
		vT *= mTrans;
		m_cContainerAABoundingBox.AppendToCubicHull(vT);
		vT.SetXYZ(vMax.x, vMin.y, vMax.z);
		vT *= mTrans;
		m_cContainerAABoundingBox.AppendToCubicHull(vT);
		vT.SetXYZ(vMax.x, vMax.y, vMax.z);
		vT *= mTrans;
		m_cContainerAABoundingBox.AppendToCubicHull(vT);
		vT.SetXYZ(vMin.x, vMax.y, vMax.z);
		vT *= mTrans;
		m_cContainerAABoundingBox.AppendToCubicHull(vT);

		// Recalculation done
		m_nInternalFlags &= ~RecalculateContainerAABoundingBox;
	}

	// Return the current axis align bounding box in 'scene container space'
	return m_cContainerAABoundingBox;
}

/**
*  @brief
*    Returns the bounding sphere in 'scene node space'
*/
const Sphere &SceneNode::GetBoundingSphere()
{
	// Calculate the bounding sphere in 'scene node space' if required
	if (m_nInternalFlags & RecalculateBoundingSphere) {
		// Get the bounding sphere in 'scene node space'
		GetBoundingSphere(m_cBoundingSphere);

		// Recalculation done
		m_nInternalFlags &= ~RecalculateBoundingSphere;
	}

	// Return the bounding sphere in 'scene node space'
	return m_cBoundingSphere;
}

/**
*  @brief
*    Returns the current bounding sphere in 'scene container space'
*/
const Sphere &SceneNode::GetContainerBoundingSphere()
{
	// Calculate the current bounding sphere in 'scene container space' if required
	if (m_nInternalFlags & RecalculateContainerBoundingSphere) {
		// Get the bounding sphere in 'scene container space'
		GetContainerBoundingSphere(m_cContainerBoundingSphere);

		// Recalculation done
		m_nInternalFlags &= ~RecalculateContainerBoundingSphere;
	}

	// Return the current bounding sphere in 'scene container space'
	return m_cContainerBoundingSphere;
}


//[-------------------------------------------------------]
//[ Modifier                                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of modifiers
*/
uint32 SceneNode::GetNumOfModifiers(const String &sClass) const
{
	// Return total number of modifiers?
	if (!sClass.GetLength())
		return m_lstModifiers.GetNumOfElements();

	// Get number of certain modifier instances
	uint32 nInstances = 0;
	for (uint32 i=0; i<m_lstModifiers.GetNumOfElements(); i++) {
		if (m_lstModifiers[i]->IsInstanceOf(sClass))
			nInstances++;
	}

	// Return the number of certain modifier instances
	return nInstances;
}

/**
*  @brief
*    Adds a modifier at a certain index inside the scene node modifier list
*/
SceneNodeModifier *SceneNode::AddModifierAtIndex(const String &sClass, const String &sParameters, int nPosition)
{
	// Check parameter
	if (sClass.GetLength()) {
		// To keep things as fast as possible we store a pointer to the base class
		// (no hash map operation + quite efficient internal string comparison)
		static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLScene::SceneNodeModifier");

		// Get class and check it
		if (pBaseClass) {
			const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
			if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
				// Create class instance
				Object *pObject = pClass->Create(Params<Object*, SceneNode&>(*this));
				if (pObject) {
					SceneNodeModifier *pModifier = static_cast<SceneNodeModifier*>(pObject);

					// Check the scene node class
					if (IsInstanceOf(pModifier->GetSceneNodeClass())) {
						// Add the new modifier to the scene node modifier list
						if (nPosition < 0 || nPosition >= static_cast<int>(m_lstModifiers.GetNumOfElements()))
							m_lstModifiers.Add(pModifier);
						else
							m_lstModifiers.AddAtIndex(pModifier, nPosition);

						// Set variables to the desired values
						if (sParameters.GetLength())
							pModifier->SetValues(sParameters);

						// If the scene node is already initialized, initialize the modifier, too
						if (IsInitialized())
							pModifier->InformedOnInit();

						// Call the "OnActivate()"-method of the modifier, don't forget to also respect the active state of the scene node modifier itself
						pModifier->OnActivate(EvaluateGlobalActiveState() && pModifier->IsActive());

						// Return the created modifier
						return pModifier;
					} else {
						// Invalid scene node class!
						delete pModifier;
					}
				}
			}
		}
	}

	// Error
	return nullptr;
}

/**
*  @brief
*    Returns a modifier
*/
SceneNodeModifier *SceneNode::GetModifier(const String &sClass, uint32 nIndex) const
{
	if (sClass.GetLength()) {
		uint32 nInstances = 0;
		for (uint32 i=0; i<m_lstModifiers.GetNumOfElements(); i++) {
			SceneNodeModifier *pModifier = m_lstModifiers[i];
			if (pModifier->IsInstanceOf(sClass)) {
				if (nInstances == nIndex)
					return pModifier;
				nInstances++;
			}
		}
	} else {
		SceneNodeModifier *pModifier = m_lstModifiers[nIndex];
		if (pModifier)
			return pModifier;
	}

	// Sorry, requested modifier not found :(
	return nullptr;
}

/**
*  @brief
*    Removes a modifier by using a given reference to the modifier to remove
*/
bool SceneNode::RemoveModifierByReference(SceneNodeModifier &cModifier)
{
	// Check parameter
	if (&cModifier.GetSceneNode() != this)
		return false; // Error!

	// Remove this modifier
	m_lstModifiers.Remove(&cModifier);
	delete &cModifier;

	// Done
	return true;
}

/**
*  @brief
*    Removes a modifier
*/
bool SceneNode::RemoveModifier(const String &sClass, uint32 nIndex)
{
	if (sClass.GetLength()) {
		uint32 nInstances = 0;
		for (uint32 i=0; i<m_lstModifiers.GetNumOfElements(); i++) {
			SceneNodeModifier *pModifier = m_lstModifiers[i];
			if (pModifier->IsInstanceOf(sClass)) {
				if (nInstances == nIndex) {
					// Remove modifier
					delete pModifier;
					m_lstModifiers.RemoveAtIndex(i);

					// Done
					return true;
				}
				nInstances++;
			}
		}
	} else {
		SceneNodeModifier *pModifier = m_lstModifiers[nIndex];
		if (pModifier) {
			// Remove modifier
			delete pModifier;
			m_lstModifiers.RemoveAtIndex(nIndex);

			// Done
			return true;
		}
	}

	// Sorry, requested modifier not found :(
	return false;
}

/**
*  @brief
*    Clears all modifiers
*/
void SceneNode::ClearModifiers()
{
	for (uint32 i=0; i<m_lstModifiers.GetNumOfElements(); i++)
		delete m_lstModifiers[i];
	m_lstModifiers.Clear();
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a clone of this scene node within the scene container this scene node is in
*/
SceneNode *SceneNode::Clone()
{
	return GetManager() ? CloneSceneNode(*static_cast<SceneContainer*>(GetManager()), *this, "_Clone") : nullptr;
}

/**
*  @brief
*    Creates a clone of this scene node within the scene container this scene node is in at a certain index inside the scene node list
*/
SceneNode *SceneNode::CloneAtIndex(int nPosition)
{
	return GetManager() ? CloneSceneNode(*static_cast<SceneContainer*>(GetManager()), *this, "_Clone", nPosition) : nullptr;
}

/**
*  @brief
*    Returns a pointer to the mesh handler
*/
MeshHandler *SceneNode::GetMeshHandler()
{
	// The default implementation is empty
	return nullptr;
}

/**
*  @brief
*    Get input controller
*/
PLInput::Controller *SceneNode::GetInputController() const
{
	// The default implementation is empty
	return nullptr;
}

/**
*  @brief
*    This function is called before any solid parts of the scene are drawn
*/
void SceneNode::DrawPre(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Emit signal
	SignalDrawPre(cRenderer, pVisNode);
}

/**
*  @brief
*    This function is called when the solid parts of the scene are drawn
*/
void SceneNode::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Emit signal
	SignalDrawSolid(cRenderer, pVisNode);
}

/**
*  @brief
*    This function is called when the transparent parts of the scene are drawn
*/
void SceneNode::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Emit signal
	SignalDrawTransparent(cRenderer, pVisNode);
}

/**
*  @brief
*    This function is called when the debug parts of the scene are drawn
*/
void SceneNode::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Inform listeners?
	if (!(m_nDebugFlags & DebugNoDrawSignal)) {
		// Emit signal
		SignalDrawDebug(cRenderer, pVisNode);
	}

	// Draw
	if (pVisNode) {
		// Fixed functions support still required for some stuff
		FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();

		// Get the effect manager
		EffectManager &cEffectManager = cRenderer.GetRendererContext().GetEffectManager();

		// Draw (the white) 'scene container space' bounding box or sphere?
		if ((m_nDebugFlags & DebugContainerAABBox) || (m_nDebugFlags & DebugContainerSphere)) {
			// Calculate world matrix
			Matrix4x4 mWorld = pVisNode->GetWorldMatrix();
			mWorld *= m_cTransform.GetInverseMatrix();

			// Setup render states
			cEffectManager.Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Draw the box?
			if ((m_nDebugFlags & DebugContainerAABBox) && pVisNode->GetParent()) {
				const AABoundingBox &cAABB = GetContainerAABoundingBox();
				cRenderer.GetDrawHelpers().DrawBox(Color4::White, cAABB.vMin, cAABB.vMax, pVisNode->GetParent()->GetWorldViewProjectionMatrix(), 1.0f);
			}

			// Draw 'scene container space' bounding box sphere?
			if (pFixedFunctions && (m_nDebugFlags & DebugContainerSphere)) {
				// Create the sphere mesh if required
				static MeshHandler cMeshHandler;
				if (!cMeshHandler.GetMesh())
					cMeshHandler.SetMesh(GetSceneContext()->GetMeshManager().CreateMesh("PLMesh::MeshCreatorSphere", true, "Detail=26"));
				if (cMeshHandler.GetMesh()) {
					// Setup world transform matrix
					mWorld.SetTranslation(mWorld.GetTranslation() + GetContainerBoundingSphere().GetPos());
					Matrix3x4 mScale;
					const float fRadius = GetContainerBoundingSphere().GetRadius()*2;
					mScale.SetScaleMatrix(fRadius, fRadius, fRadius);
					mWorld *= mScale;
					// [TODO] Remove the direct usage of fixed functions in here
					pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);

					// Setup render states
					cRenderer.SetTextureBuffer();
					cRenderer.SetProgram();
					cRenderer.SetRenderState(RenderState::CullMode,    Cull::None);
					cRenderer.SetRenderState(RenderState::BlendEnable, true);
					pFixedFunctions->SetColor(Color4(1.0f, 1.0f, 1.0f, 0.1f));

					// Draw
					cMeshHandler.Draw(false, false);
				}
			}
		}

		// Get cull query (if available) which enables us to cull 3D debug texts
		const SQCull *pCullQuery = nullptr;
		if (pVisNode) {
			const VisNode *pParentVisNode = pVisNode->GetParent();
			if (pParentVisNode && pParentVisNode->IsContainer()) {
				pCullQuery = static_cast<const VisContainer*>(pParentVisNode)->GetCullQuery();
			}
		}

		// Set world matrix
		if (pFixedFunctions)
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, pVisNode->GetWorldMatrix());

		// Draw local coordinate axis?
		if (!(m_nDebugFlags & DebugNoLocalCoordinateAxis)) {
			// Set standard material
			cEffectManager.Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Draw direction vectors
			// Center point
			cRenderer.GetDrawHelpers().DrawPoint(Color4::White, Vector3::Zero, pVisNode->GetWorldViewProjectionMatrix(), 5.0f);
			// Direction vector
			cRenderer.GetDrawHelpers().DrawLine(Color4::Blue, Vector3::Zero, Vector3::UnitZ, pVisNode->GetWorldViewProjectionMatrix(), 1.0f);
			// Left vector
			cRenderer.GetDrawHelpers().DrawLine(Color4::Red, Vector3::Zero, Vector3::UnitX, pVisNode->GetWorldViewProjectionMatrix(), 1.0f);
			// Up vector
			cRenderer.GetDrawHelpers().DrawLine(Color4::Green, Vector3::Zero, Vector3::UnitY, pVisNode->GetWorldViewProjectionMatrix(), 1.0f);

			// Draw texts
			Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
			if (pFont) {
				if (!pCullQuery || Intersect::PlaneSetPoint(pCullQuery->GetViewFrustum(), m_cTransform.GetMatrix()*Vector3::UnitZ))
					cRenderer.GetDrawHelpers().DrawText(*pFont, "z (dir)", Color4::Blue, Vector3::UnitZ, pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
				if (!pCullQuery || Intersect::PlaneSetPoint(pCullQuery->GetViewFrustum(), m_cTransform.GetMatrix()*Vector3::UnitX))
					cRenderer.GetDrawHelpers().DrawText(*pFont, "x (left)", Color4::Red, Vector3::UnitX, pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
				if (!pCullQuery || Intersect::PlaneSetPoint(pCullQuery->GetViewFrustum(), m_cTransform.GetMatrix()*Vector3::UnitY))
					cRenderer.GetDrawHelpers().DrawText(*pFont, "y (up)", Color4::Green, Vector3::UnitY, pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
			}
		}

		// Show the scene node name? (not required if ALL names are shown :)
		if (pVisNode && !(m_nDebugFlags & DebugNoName) &&
			(!pCullQuery || Intersect::PlaneSetPoint(pCullQuery->GetViewFrustum(), m_cTransform.GetPosition()))) {
			Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
			if (pFont) {
				cEffectManager.Use();
				cRenderer.SetRenderState(RenderState::ZEnable,      false);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
				cRenderer.GetDrawHelpers().DrawText(*pFont, GetAbsoluteName(), Color4::White, Vector3::Zero, pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
			}
		}

		// Draw (the yellow) 'scene node space' bounding box?
		if (!(m_nDebugFlags & DebugNoAABBox)) {
			cEffectManager.Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
			const AABoundingBox &cAABB = GetAABoundingBox();
			cRenderer.GetDrawHelpers().DrawBox(Color4::Yellow, cAABB.vMin, cAABB.vMax, pVisNode->GetWorldViewProjectionMatrix(), 1.0f);
		}

		// Draw debug text?
		if (m_nDebugFlags & DebugText) {
			// Get the font
			Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
			if (pFont) {
				// Setup render states
				cEffectManager.Use();
				cRenderer.SetRenderState(RenderState::ZEnable,      false);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

				// Begin 2D mode
				DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
				cDrawHelpers.Begin2DMode(0.0f, 0.0f, 800.0f, 600.0f);

					// Name
					cRenderer.GetDrawHelpers().DrawText(*pFont, "Name: " + GetName(), Color4::White, Vector2(10.0f, 570.0f));

					// General
					cRenderer.GetDrawHelpers().DrawText(*pFont, String::Format("Active: %d", IsActive()), Color4::White, Vector2(10.0f, 550.0f));

					// Position
					cRenderer.GetDrawHelpers().DrawText(*pFont, "Position: " + m_cTransform.GetPosition().ToString(), Color4::White, Vector2(10.0f, 530.0f));

					// Show an Euler angles representation of the rotation in degree
					cRenderer.GetDrawHelpers().DrawText(*pFont, "Rotation: " + GetRotation().ToString(), Color4::White, Vector2(10.0f, 510.0f));

					// Scale
					cRenderer.GetDrawHelpers().DrawText(*pFont, "Scale: " + m_cTransform.GetScale().ToString(), Color4::White, Vector2(10.0f, 490.0f));

				// End 2D mode
				cDrawHelpers.End2DMode();
			}
		}
	}
}

/**
*  @brief
*    This function is called after transparent parts of the scene are drawn
*/
void SceneNode::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Emit signal
	SignalDrawPost(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SceneNode::SceneNode() :
	Flags(this),
	DebugFlags(this),
	Position(this),
	Rotation(this),
	Scale(this),
	MaxDrawDistance(this),
	AABBMin(this),
	AABBMax(this),
	Name(this),
	EventHandlerPosition(&SceneNode::OnPosition, this),
	EventHandlerRotation(&SceneNode::OnRotation, this),
	EventHandlerScale	(&SceneNode::OnScale,    this),
	m_nFlags(0),
	m_nDebugFlags(0),
	m_nDrawFunctionFlags(UseDrawDebug),
	m_nCounter(0),
	m_nInternalFlags(RecalculateContainerAABoundingBox | RecalculateContainerBoundingSphere),
	m_pFirstSceneHierarchyNodeItem(nullptr)
{
	// Connect transform event handlers
	m_cTransform.EventPosition.Connect(EventHandlerPosition);
	m_cTransform.EventRotation.Connect(EventHandlerRotation);
	m_cTransform.EventScale   .Connect(EventHandlerScale);
}

/**
*  @brief
*    Destructor
*/
SceneNode::~SceneNode()
{
	// De-initialize the scene node if required
	if (IsInitialized())
		DeInitSceneNode();

	// Clear modifiers
	ClearModifiers();
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    This function is called when the scene node is initialized
*/
void SceneNode::InitFunction()
{
	// Initialization allowed?
	if (!(m_nInternalFlags & DestroyThis)) {
		// If scene node is already initialized, de-initialize it first!
		if (IsInitialized())
			DeInitFunction();

		// Scene node is initialized
		m_nInternalFlags |= Initialized;

		// Emit signal
		SignalInit();

		// Call the "OnActivate()"-method
		OnActivate(EvaluateGlobalActiveState());
	}
}

/**
*  @brief
*    This function is called when the scene node is de-initialized
*/
void SceneNode::DeInitFunction()
{
	// Set initialization flag
	m_nInternalFlags &= ~Initialized;

	// Emit signal
	SignalDeInit();

	// Clear modifiers
	ClearModifiers();
}

/**
*  @brief
*    Called when the scene node has been activated or deactivated
*/
void SceneNode::OnActivate(bool bActivate)
{
	// Loop through all modifiers
	for (uint32 i=0; i<m_lstModifiers.GetNumOfElements(); i++) {
		// Get the scene node modifier
		SceneNodeModifier *pSceneNodeModifier = m_lstModifiers[i];

		// Evaluate the global active state, don't forget to also respect the active state of the scene node modifier itself
		pSceneNodeModifier->OnActivate(bActivate && pSceneNodeModifier->IsActive());
	}
}

/**
*  @brief
*    Updates the axis align bounding box in 'scene node space'
*/
void SceneNode::UpdateAABoundingBox()
{
	// No default implementation
}

/**
*  @brief
*    Returns the bounding sphere in 'scene node space'
*/
void SceneNode::GetBoundingSphere(Sphere &cSphere)
{
	// Use the 'scene node space' axis aligned bounding box
	const AABoundingBox &cAABB = GetAABoundingBox();
	Vector3 vMin = cAABB.vMin, vMax = cAABB.vMax;

	// Get the center of the box and make the minimum/maximum relative to it
	const Vector3 vCenter((vMax.x+vMin.x)*0.5f, (vMax.y+vMin.y)*0.5f, (vMax.z+vMin.z)*0.5f);
	vMin -= vCenter;
	vMax -= vCenter;

	// Get the minimum/maximum squared length
	const float fMinSquaredLength = vMin.GetSquaredLength();
	const float fMaxSquaredLength = vMax.GetSquaredLength();

	// The greater one has to be used for the radius
	cSphere.SetRadius((fMaxSquaredLength > fMinSquaredLength) ? Math::Sqrt(fMaxSquaredLength) : Math::Sqrt(fMinSquaredLength));

	// Set the position of the sphere
	cSphere.SetPos(m_cTransform.GetPosition() + vCenter);
}

/**
*  @brief
*    Returns the current bounding sphere in 'scene container space'
*/
void SceneNode::GetContainerBoundingSphere(Sphere &cSphere)
{
	// Use the 'scene node space' axis aligned bounding box, the 'container space' one
	// may be bigger than required...
	const AABoundingBox &cAABB = GetAABoundingBox();
	Vector3 vMin = cAABB.vMin, vMax = cAABB.vMax;

	// Apply the scale of the scene node
	vMin *= m_cTransform.GetScale();
	vMax *= m_cTransform.GetScale();

	// Get the center of the box and make the minimum/maximum relative to it
	const Vector3 vCenter((vMax.x+vMin.x)*0.5f, (vMax.y+vMin.y)*0.5f, (vMax.z+vMin.z)*0.5f);
	vMin -= vCenter;
	vMax -= vCenter;

	// Get the minimum/maximum squared length
	const float fMinSquaredLength = vMin.GetSquaredLength();
	const float fMaxSquaredLength = vMax.GetSquaredLength();

	// The greater one has to be used for the radius
	cSphere.SetRadius((fMaxSquaredLength > fMinSquaredLength) ? Math::Sqrt(fMaxSquaredLength) : Math::Sqrt(fMinSquaredLength));

	// Set the position of the sphere and don't forget to take the scene node rotation into account :D
	cSphere.SetPos(m_cTransform.GetPosition() + m_cTransform.GetRotation()*vCenter);
}

/**
*  @brief
*    Called when the scene node was added to a visibility tree
*/
void SceneNode::OnAddedToVisibilityTree(VisNode &cVisNode)
{
	// Emit signal
	SignalAddedToVisibilityTree(cVisNode);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes the scene node
*/
void SceneNode::InitSceneNode()
{
	// Initialization allowed?
	if (!(m_nInternalFlags & DestroyThis)) {
		// Call the virtual initialization function
		InitFunction();
	}
}

/**
*  @brief
*    De-Initializes the scene node
*/
void SceneNode::DeInitSceneNode()
{
	// Call the virtual de-initialization function
	if (IsInitialized())
		DeInitFunction();
}

/**
*  @brief
*    Clones the given scene node
*/
SceneNode *SceneNode::CloneSceneNode(SceneContainer &cTargetSceneContainer, const SceneNode &cSceneNode, const String &sNameExtension, int nPosition)
{
	// Clone scene node
	SceneNode *pSceneNodeClone = cTargetSceneContainer.CreateAtIndex(cSceneNode.GetClass()->GetClassName(), cSceneNode.GetName() + sNameExtension, cSceneNode.GetValues(), nPosition);
	if (pSceneNodeClone) {
		// Reset debug flags of the clone
		pSceneNodeClone->SetDebugFlags(0);

		// Is it a scene container?
		if (cSceneNode.IsContainer()) {
			// Get the scene container
			const SceneContainer &cSceneContainer = static_cast<const SceneContainer&>(cSceneNode);

			// Clone the scene nodes, but only in case this scene container was not loaded from a file
			if (!cSceneContainer.Filename.Get().GetLength()) {
				// Loop through all scene nodes of the scene container
				for (uint32 i=0; i<cSceneContainer.GetNumOfElements(); i++) {
					// Get the scene node
					const SceneNode &cSourceSceneNode = *cSceneContainer.GetByIndex(i);

					// Do not clone automatic scene nodes
					if (!(cSourceSceneNode.GetFlags() & SceneNode::Automatic)) {
						// Clone the scene node
						CloneSceneNode(static_cast<SceneContainer&>(*pSceneNodeClone), cSourceSceneNode, "");
					}
				}
			}
		}

		// Clone the scene node modifiers
		for (uint32 i=0; i<cSceneNode.GetNumOfModifiers(); i++) {
			// Get the scene node modifier
			SceneNodeModifier *pSceneNodeModifier = cSceneNode.GetModifier("", i);

			// Do not clone automatic scene node modifiers
			if (!(pSceneNodeModifier->GetFlags() & SceneNodeModifier::Automatic)) {
				// Clone the scene node modifier
				pSceneNodeClone->AddModifier(pSceneNodeModifier->GetClass()->GetClassName(), pSceneNodeModifier->GetValues());
			}
		}
	}

	// Return the created clone
	return pSceneNodeClone;
}

/**
*  @brief
*    Call this function if the scene node bounding box was changed and the
*    hierarchy the scene node is in needs an update
*/
void SceneNode::HierarchyRefreshRequired()
{
	// Is this node within a hierarchy and is this flag not already set?
	if (m_pFirstSceneHierarchyNodeItem && !(m_nInternalFlags & RecalculateHierarchy)) {
		// Set the flag
		m_nInternalFlags |= RecalculateHierarchy;

		// Add this node to the scene container list of scene nodes which needs a hierarchy refresh
		GetContainer()->m_lstHierarchyRefresh.Add(this);
	}
}

/**
*  @brief
*    Called on position transform change
*/
void SceneNode::OnPosition()
{
	// We have to recalculate the current axis align bounding box in 'scene container space'
	m_nInternalFlags |= RecalculateContainerAABoundingBox;

	// We have to recalculate the current bounding sphere in 'sphere container space'
	m_nInternalFlags |= RecalculateContainerBoundingSphere;

	// We need a hierarchy refresh for this scene node
	HierarchyRefreshRequired();
}

/**
*  @brief
*    Called on rotation transform change
*/
void SceneNode::OnRotation()
{
	// We have to recalculate the current axis align bounding box in 'scene container space'
	m_nInternalFlags |= RecalculateContainerAABoundingBox;

	// We have to recalculate the current bounding sphere in 'sphere container space'
	m_nInternalFlags |= RecalculateContainerBoundingSphere;

	// We need a hierarchy refresh for this scene node
	HierarchyRefreshRequired();
}

/**
*  @brief
*    Called on scale transform change
*/
void SceneNode::OnScale()
{
	// We have to recalculate the current axis align bounding box in 'scene container space'
	m_nInternalFlags |= RecalculateContainerAABoundingBox;

	// We have to recalculate the current bounding sphere in 'sphere container space'
	m_nInternalFlags |= RecalculateContainerBoundingSphere;

	// We need a hierarchy refresh for this scene node
	HierarchyRefreshRequired();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Element functions              ]
//[-------------------------------------------------------]
bool SceneNode::Delete(bool bProtectedToo)
{
	if (!(m_nInternalFlags & DestroyThis) && (bProtectedToo || !m_bProtected)) {
		// Set the 'I am going to die'-flag
		m_nInternalFlags |= DestroyThis;

		// Emit signal
		SignalDestroy();

		// Is this scene node within a container?
		if (GetContainer()) {
			// Delay destruction to avoid reference problems, or at least try it
			SceneContext *pSceneContext = GetSceneContext();
			if (pSceneContext) {
				pSceneContext->m_lstDeleteNodes.Add(this);

				// Remove scene node from scene node container, the scene node is now de-initialized automatically
				GetContainer()->Remove(*this);

				// 'SceneContext::Cleanup()' will do the rest later...
			}
			else {
				// Remove scene node from scene node container, the scene node is now de-initialized automatically
				GetContainer()->Remove(*this);

				// Destroy this scene node right now!
				delete this;
			}
		} else {
			// De-initialize the scene node
			DeInitSceneNode();

			// Delete the scene node right now!
			delete this;
		}

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

bool SceneNode::SetName(const String &sName)
{
	// Check parameter
	if (m_sName != sName && sName.GetLength()) {
		// Is this the root scene node? The name if it can NOT be changed!
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext && this == pSceneContext->GetRoot()) {
			// Set the name of the root scene container to 'Root' (initialization)
			m_sName = "Root";

			// Hm, return an error for sure :)
		} else {
			// The names 'Root' and 'Parent' are NOT allowed!
			if (sName != "Root" && sName != "Parent") {
				// Check for the character '.' within the name - this isn't allowed, too!
				const int nIndex = sName.IndexOf('.');
				if (nIndex < 0) {
					// Call base implementation
					return Element<SceneNode>::SetName(sName);
				}
			}
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::Element functions             ]
//[-------------------------------------------------------]
void SceneNode::DeleteElement()
{
	Delete(true);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
