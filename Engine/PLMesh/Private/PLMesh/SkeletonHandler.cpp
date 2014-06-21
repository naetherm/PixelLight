/*********************************************************\
 *  File: SkeletonHandler.cpp                            *
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
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include "PLMesh/Joint.h"
#include "PLMesh/Skeleton.h"
#include "PLMesh/SkeletonHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SkeletonHandler::SkeletonHandler()
{
}

/**
*  @brief
*    Destructor
*/
SkeletonHandler::~SkeletonHandler()
{
}

/**
*  @brief
*    Draws the joints
*/
void SkeletonHandler::DrawJoints(Renderer &cRenderer, const Color4 &cColor, const Matrix4x4 &mWorldViewProjection, float fPointSize) const
{
	for (uint32 i=0; i<m_lstJointHandlers.GetNumOfElements(); i++)
		 cRenderer.GetDrawHelpers().DrawPoint(cColor, m_lstJointHandlers[i].GetTranslationAbsolute(), mWorldViewProjection, fPointSize);
}

/**
*  @brief
*    Draws the joint names
*/
void SkeletonHandler::DrawJointNames(Font &cFont, const Color4 &cColor, const Matrix4x4 &mWorldViewProjection) const
{
	// Get skeleton
	const Skeleton *pSkeleton = GetResource();
	if (pSkeleton) {
		// Draw joint names
		DrawHelpers &cDrawHelpers = cFont.GetRenderer().GetDrawHelpers();
		for (uint32 i=0; i<pSkeleton->GetNumOfElements(); i++) {
			const Joint        *pJoint        = pSkeleton->GetByIndex(i);
			const JointHandler &cJointHandler = m_lstJointHandlers[i];
			if (&cJointHandler != &Array<JointHandler>::Null)
				cDrawHelpers.DrawText(cFont, pJoint->GetName(), cColor, cJointHandler.GetTranslationAbsolute(), mWorldViewProjection, Font::CenterText);
		}
	}
}

/**
*  @brief
*    Draws the skeleton
*/
void SkeletonHandler::DrawSkeleton(Renderer &cRenderer, const Color4 &cColor, const Matrix4x4 &mWorldViewProjection, float fLineWidth) const
{
	// Get skeleton
	const Skeleton *pSkeleton = GetResource();
	if (pSkeleton) {
		// Draw skeleton
		for (uint32 i=0; i<pSkeleton->GetNumOfElements(); i++) {
			const Joint        *pJoint        = pSkeleton->GetByIndex(i);
			const JointHandler &cJointHandler = m_lstJointHandlers[i];
			if (&cJointHandler != &Array<JointHandler>::Null) {
				const JointHandler &cParentJointState = m_lstJointHandlers[pJoint->GetParent()];
				if (&cParentJointState != &Array<JointHandler>::Null)
					cRenderer.GetDrawHelpers().DrawLine(cColor, cJointHandler.GetTranslationAbsolute(), cParentJointState.GetTranslationAbsolute(), mWorldViewProjection, fLineWidth);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Joint states                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the base joint handlers
*/
Array<JointHandler> &SkeletonHandler::GetBaseJointHandlers()
{
	return m_lstBaseJointHandlers;
}

/**
*  @brief
*    Returns a base joint handler by name
*/
JointHandler *SkeletonHandler::GetBaseJointHandler(const String &sJointName) const
{
	// Get the skeleton
	const Skeleton *pSkeleton = GetResource();
	if (pSkeleton) {
		// Get the joint
		const Joint *pJoint = pSkeleton->GetByName(sJointName);
		if (pJoint) {
			// Return the base joint handler for this joint
			JointHandler &cJointHandler = m_lstBaseJointHandlers[pJoint->GetID()];
			return (&cJointHandler != &Array<JointHandler>::Null) ? &cJointHandler : nullptr;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns the current joint handlers
*/
Array<JointHandler> &SkeletonHandler::GetJointHandlers()
{
	return m_lstJointHandlers;
}

const Array<JointHandler> &SkeletonHandler::GetJointHandlers() const
{
	return m_lstJointHandlers;
}

/**
*  @brief
*    Returns a joint handler by name
*/
JointHandler *SkeletonHandler::GetJointHandler(const String &sJointName) const
{
	// Get the skeleton
	const Skeleton *pSkeleton = GetResource();
	if (pSkeleton) {
		// Get the joint
		const Joint *pJoint = pSkeleton->GetByName(sJointName);
		if (pJoint) {
			// Return the joint handler for this joint
			JointHandler &cJointHandler = m_lstJointHandlers[pJoint->GetID()];
			return (&cJointHandler != &Array<JointHandler>::Null) ? &cJointHandler : nullptr;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Resets all current joint states
*/
bool SkeletonHandler::ResetJointStates(bool bForceAll)
{
	// Get skeleton
	const Skeleton *pSkeleton = GetResource();
	if (!pSkeleton)
		return false; // Error!

	// Resize current joint handlers array if required (sure is sure :)
	m_lstJointHandlers.Resize(pSkeleton->GetNumOfElements());

	// Set all base joint states to null
	for (uint32 i=0; i<pSkeleton->GetNumOfElements(); i++) {
		JointHandler &cJH    = m_lstJointHandlers[i];
		Joint        *pJoint = pSkeleton->GetByIndex(i);
		cJH.SetElement(pJoint);

		// Is this joint user controlled?
		if (bForceAll || !cJH.GetElement() || !cJH.IsUserControlled()) {
			// Reset absolute states
			cJH.SetTranslationAbsolute(pJoint->GetTranslationAbsolute());
			cJH.SetRotationAbsolute(pJoint->GetRotationAbsolute());

			// Reset relative states
			cJH.SetTranslation(Vector3::Zero);
			cJH.SetRotation(Quaternion::Identity);
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Resets all base joint states to the skeleton 'orginal' base joint states
*/
bool SkeletonHandler::ResetBaseJointStates()
{
	// Get skeleton
	const Skeleton *pSkeleton = GetResource();
	if (!pSkeleton)
		return false; // Error!

	// Resize base joint handlers array if required (sure is sure :)
	m_lstBaseJointHandlers.Resize(pSkeleton->GetNumOfElements());

	// Set all base joint states to the skeleton 'orginal' base joint states
	for (uint32 i=0; i<pSkeleton->GetNumOfElements(); i++) {
		JointHandler &cJH   = m_lstBaseJointHandlers[i];
		Joint       *pJoint = pSkeleton->GetByIndex(i);
		cJH.SetElement(pJoint);
		cJH.SetTranslation(pJoint->GetTranslation());
		cJH.SetRotation(pJoint->GetRotation());
	}

	// Done
	return true;
}

/**
*  @brief
*    Applies the joint states from the given joint animation frame
*    to the current joint states
*/
bool SkeletonHandler::ApplyJointStates(uint32 nFrame, float fWeight)
{
	// Get skeleton
	const Skeleton *pSkeleton = GetResource();
	if (!pSkeleton)
		return false; // Error!

	// Apply joint states
	return pSkeleton->ApplyJointStates(m_lstJointHandlers, nFrame, fWeight);
}

/**
*  @brief
*    Applies the blended joint states from the given joint animation frames
*    to the current joint states
*/
bool SkeletonHandler::ApplyJointStates(uint32 nFrame1, uint32 nFrame2, float fTime, float fWeight)
{
	// Get skeleton
	const Skeleton *pSkeleton = GetResource();
	if (!pSkeleton)
		return false; // Error!

	// Apply joint states
	return pSkeleton->ApplyJointStates(m_lstJointHandlers, nFrame1, nFrame2, fTime, fWeight);
}

/**
*  @brief
*    Applies the current joint states from the given skeleton handler
*    to the current joint states
*/
bool SkeletonHandler::ApplyJointStates(SkeletonHandler &cSkeletonHandler, float fWeight)
{
	// Check weight of this states
	if (fWeight) {
		// Get skeleton
		const Skeleton *pSkeleton = GetResource();
		if (!pSkeleton)
			return false; // Error!

		// Get joint handlers
		const Array<JointHandler> &lstJointHandlers = cSkeletonHandler.GetJointHandlers();

		// Loop through all joint handlers and add deltas
		for (uint32 i=0; i<lstJointHandlers.GetNumOfElements(); i++) {
			const JointHandler &cJH = lstJointHandlers[i];
			if (cJH.GetElement()) {
				// We have to find the joint handler of this skeleton handler which has loaded
				// the joint from this skeleton which has the same name as the joint the given
				// joint handler has
				const Joint *pJoint = pSkeleton->GetByName(cJH.GetElement()->GetName());
				if (pJoint) {
					JointHandler *pJHBase = nullptr;
					for (uint32 j=0; j<m_lstJointHandlers.GetNumOfElements(); j++) {
						pJHBase = &m_lstJointHandlers[j];
						if (pJHBase->GetElement() == pJoint)
							break;
						pJHBase = nullptr;
					}
					if (pJHBase) {
						// Add deltas
						pJHBase->SetTranslation(pJHBase->GetTranslation() + cJH.GetTranslation()*fWeight);
						Quaternion qDelta;
						qDelta.Slerp(Quaternion::Identity, cJH.GetRotation(), fWeight);
						pJHBase->SetRotation(pJHBase->GetRotation()*qDelta);
					}
				}
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Applies the base joint states to the current joint states
*/
bool SkeletonHandler::ApplyBaseJointStates()
{
	// Apply the base joint states to the current joint states
	for (uint32 i=0; i<m_lstBaseJointHandlers.GetNumOfElements(); i++) {
		JointHandler &cJH = m_lstJointHandlers[i];

		// Is this joint user controlled?
		if (!cJH.GetElement() || !cJH.IsUserControlled()) {
			const JointHandler &cBJH = m_lstBaseJointHandlers[i];
			cJH.SetTranslation(cJH.GetTranslation() + cBJH.GetTranslation());
			cJH.SetRotation(cJH.GetRotation()*cBJH.GetRotation());
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Calculates all current absolute joint states
*/
bool SkeletonHandler::CalculateStates()
{
	// Get skeleton and root joints
	const Skeleton *pSkeleton = GetResource();
	if (!pSkeleton)
		return false; // Error!

	// Loop through all root joints
	const Array<uint32> &lstRootJoints = pSkeleton->GetRootJoints();
	for (uint32 i=0; i<lstRootJoints.GetNumOfElements(); i++) {
		JointHandler &cJointHandler = m_lstJointHandlers[lstRootJoints[i]];
		if (&cJointHandler != &Array<JointHandler>::Null)
			RecCalculateStates(cJointHandler, nullptr);
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Recursive joint state calculations
*/
void SkeletonHandler::RecCalculateStates(JointHandler &cJointHandler, JointHandler *pParentJointHandler)
{
	// Check and calculate states of this joint
	const Joint *pJoint = cJointHandler.GetElement();
	if (pJoint) {
		cJointHandler.CalculateStates(pParentJointHandler);

		// Transform children
		for (uint32 i=0; i<pJoint->GetChildren().GetNumOfElements(); i++) {
			JointHandler &cCurrentJointHandler = m_lstJointHandlers[pJoint->GetChildren()[i]];
			if (&cCurrentJointHandler != &Array<JointHandler>::Null)
				RecCalculateStates(cCurrentJointHandler, &cJointHandler);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::ResourceHandler functions      ]
//[-------------------------------------------------------]
bool SkeletonHandler::SetResource(Skeleton *pResource)
{
	// Call base implementation
	const bool bResult = PLCore::ResourceHandler<Skeleton>::SetResource(pResource);

	// Set current joint handlers
	ResetJointStates();

	// Set base joint handlers
	ResetBaseJointStates();

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
