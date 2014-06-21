/*********************************************************\
 *  File: Skeleton.cpp                                   *
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
#include "PLMesh/Joint.h"
#include "PLMesh/Skeleton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Skeleton::~Skeleton()
{
}


//[-------------------------------------------------------]
//[ Joints                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the skeleton root joint indices
*/
const Array<uint32> &Skeleton::GetRootJoints() const
{
	return m_lstRootJoints;
}

/**
*  @brief
*    Updates the joint information
*/
void Skeleton::UpdateJointInformation()
{
	// Check joints
	if (m_lstElements.GetNumOfElements()) {
		// Loop through all joints to count and reserve children
		uint32 nNumOfRootJoints = 0;
		for (uint32 i=0; i<m_lstElements.GetNumOfElements(); i++) {
			Joint *pJoint = m_lstElements[i];
			if (!m_lstElements[pJoint->GetParent()])
				nNumOfRootJoints++;
			uint32 nNumOfChildren = 0;
			for (uint32 j=0; j<m_lstElements.GetNumOfElements(); j++) {
				if (m_lstElements[j]->GetParent() == static_cast<int>(i))
					nNumOfChildren++;
			}
			pJoint->m_lstChildren.Resize(nNumOfChildren);
		}
		m_lstRootJoints.Resize(nNumOfRootJoints);
		nNumOfRootJoints = 0;

		// Loop through all joints to fill children
		for (uint32 i=0; i<m_lstElements.GetNumOfElements(); i++) {
			const Joint *pJoint = m_lstElements[i];
			if (!m_lstElements[pJoint->GetParent()])
				m_lstRootJoints[nNumOfRootJoints++] = i;
			uint32 nNumOfChildren = 0;
			for (uint32 j=0; j<m_lstElements.GetNumOfElements(); j++) {
				if (m_lstElements[j]->GetParent() == static_cast<int>(i)) {
					pJoint->m_lstChildren[nNumOfChildren] = j;
					nNumOfChildren++;
				}
			}
		}

		// Set initial absolute joint states
		// Loop through all root joints
		for (uint32 i=0; i<m_lstRootJoints.GetNumOfElements(); i++) {
			Joint *pJoint = m_lstElements[m_lstRootJoints[i]];
			if (pJoint)
				RecCalculateStates(*pJoint, nullptr);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Skeleton::Skeleton(ResourceManager<Skeleton> &cManager, const String &sName) :
	Resource<Skeleton>(sName, &cManager), JointAni()
{
}

/**
*  @brief
*    Recursive joint state calculations
*/
void Skeleton::RecCalculateStates(Joint &cJoint, Joint *pParentJoint) const
{
	// Check and calculate states of this joint
	cJoint.CalculateInitialStates(pParentJoint);

	// Transform children
	for (uint32 i=0; i<cJoint.GetChildren().GetNumOfElements(); i++) {
		Joint *pJoint = m_lstElements[cJoint.GetChildren()[i]];
		if (pJoint)
			RecCalculateStates(*pJoint, &cJoint);
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
Joint *Skeleton::CreateElement(const String &sName)
{
	return new Joint(sName, this);
}


//[-------------------------------------------------------]
//[ Public virtual AnimationInfo functions                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy operator
*/
Skeleton &Skeleton::operator =(const Skeleton &cSource)
{
	// Unload the skeleton
	Unload();

	// Call base function
	*static_cast<Resource<Skeleton>*>(this)    = cSource;
	*static_cast<ElementManager<Joint>*>(this) = cSource;

	// Call base function
	*static_cast<JointAni*>(this) = *static_cast<const JointAni*>(&cSource);

/* [TODO]
	// Copy data
	m_lstJoints.Resize(cSource.GetNumOfJoints(), false, false);
	for (uint32 i=0; i<cSource.GetNumOfJoints(); i++) {
		Joint *pJoint = new Joint();
		*pJoint = *cSource.GetJoint(i);
		m_lstJoints.Add(pJoint);
	}
	m_lstRootJoints = cSource.m_lstRootJoints;
	m_cAnimation = cSource.m_cAnimation;
*/

	// Return pointer
	return *this;
}

/**
*  @brief
*    Get the animation source name
*/
String Skeleton::GetSourceName() const
{
	// To implement in derived classes
	return GetName();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Skeleton::Unload()
{
	// Clear the skeleton
	ElementManager<Joint>::Clear();
	m_lstRootJoints.Clear();

	// Call base implementation
	return Resource<Skeleton>::Unload();
}

String Skeleton::GetLoadableTypeName() const
{
	static const String sString = "Skeleton";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
