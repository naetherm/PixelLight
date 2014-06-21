/*********************************************************\
 *  File: RagdollLoaderPL.cpp                            *
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
#include <PLCore/Xml/Xml.h>
#include <PLCore/Log/Log.h>
#include <PLCore/String/ParseTools.h>
#include "PLPhysics/SceneNodes/SNRagdoll.h"
#include "PLPhysics/SceneNodes/RagdollBody.h"
#include "PLPhysics/SceneNodes/RagdollJoint.h"
#include "PLPhysics/SceneNodes/RagdollLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(RagdollLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool RagdollLoaderPL::Load(SNRagdoll &cSNRagdoll, File &cFile)
{
	return LoadParams(cSNRagdoll, cFile, true);
}

bool RagdollLoaderPL::LoadParams(SNRagdoll &cSNRagdoll, File &cFile, bool bAllowAttachToWorld)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get ragdoll element
		const XmlElement *pRagdollElement = cDocument.GetFirstChildElement("Ragdoll");
		if (pRagdollElement) {
			// Get the format version
			int nVersion = pRagdollElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cSNRagdoll, *pRagdollElement, bAllowAttachToWorld);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cSNRagdoll, *pRagdollElement, bAllowAttachToWorld);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else PL_LOG(Error, "Can't find 'Ragdoll' element")
	} else PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())

	// Error!
	return false;
}

bool RagdollLoaderPL::Save(SNRagdoll &cSNRagdoll, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add ragdoll
	XmlElement *pRagdollElement = new XmlElement("Ragdoll");
	pRagdollElement->SetAttribute("Version", "1");

	// Add bodies
	if (cSNRagdoll.GetNumOfBodies()) {
		XmlElement *pBodiesElement = new XmlElement("Bodies");
		for (uint32 i=0; i<cSNRagdoll.GetNumOfBodies(); i++) {
			// Get the ragdoll body
			const RagdollBody *pBody = cSNRagdoll.GetBody(i);
			if (pBody) {
				// Add the XML element
				XmlElement *pBodyElement = new XmlElement("Body");

				// Add attributes
				pBodyElement->SetAttribute("Name",     pBody->sName);
				pBodyElement->SetAttribute("Enabled",  pBody->bEnabled);
				pBodyElement->SetAttribute("Size",     pBody->vSize.ToString());
				pBodyElement->SetAttribute("Position", pBody->vPos.ToString());
				pBodyElement->SetAttribute("Rotation", String::Format("%g %g %g %g", pBody->qRot.w, pBody->qRot.x, pBody->qRot.y, pBody->qRot.z));
				pBodyElement->SetAttribute("Mass",     String::Format("%g",			pBody->fMass));

				// Link body element to parent
				pBodiesElement->LinkEndChild(*pBodyElement);
			}
		}

		// Link bodies element to parent
		pRagdollElement->LinkEndChild(*pBodiesElement);
	}

	// Add joints
	if (cSNRagdoll.GetNumOfJoints()) {
		XmlElement *pJointsElement = new XmlElement("Joints");
		for (uint32 i=0; i<cSNRagdoll.GetNumOfJoints(); i++) {
			// Get the ragdoll joint
			const RagdollJoint *pJoint = cSNRagdoll.GetJoint(i);
			if (pJoint) {
				// Add the XML element
				XmlElement *pJointElement = new XmlElement("Joint");

				// Add attributes
				pJointElement->SetAttribute("Name",          pJoint->sName);
				pJointElement->SetAttribute("Attached",      pJoint->sAttached);
				pJointElement->SetAttribute("Parent",        pJoint->sParent);
				pJointElement->SetAttribute("AttachToWorld", pJoint->bAttachToWorld);
				switch (pJoint->nJointType) {
					case RagdollJoint::Hinge:     pJointElement->SetAttribute("Type", "hinge");     break;
					case RagdollJoint::Universal: pJointElement->SetAttribute("Type", "universal"); break;
				}
				pJointElement->SetAttribute("LowJointRange1",  String::Format("%g", pJoint->fLowJointRange1));
				pJointElement->SetAttribute("HighJointRange1", String::Format("%g", pJoint->fHighJointRange1));
				pJointElement->SetAttribute("LowJointRange2",  String::Format("%g", pJoint->fLowJointRange2));
				pJointElement->SetAttribute("HighJointRange2", String::Format("%g", pJoint->fHighJointRange2));
				pJointElement->SetAttribute("Anchor",		   pJoint->vJointAnchor.ToString());
				pJointElement->SetAttribute("RotAxis1",		   pJoint->vRotAxis1.ToString());
				pJointElement->SetAttribute("RotAxis2",		   pJoint->vRotAxis2.ToString());

				// Link joint element to parent
				pJointsElement->LinkEndChild(*pJointElement);
			}
		}

		// Link joints element to parent
		pRagdollElement->LinkEndChild(*pJointsElement);
	}

	// Add no collisions
	if (cSNRagdoll.GetNumOfNoCollisions()) {
		XmlElement *pNoCollisionsElement = new XmlElement("NoCollisions");
		for (uint32 i=0; i<cSNRagdoll.GetNumOfNoCollisions(); i++) {
			// Get the ragdoll no collision
			const NoCollision *pNoCollision = cSNRagdoll.GetNoCollision(i);
			if (pNoCollision) {
				// Add the XML element
				XmlElement *pNoCollisionElement = new XmlElement("NoCollision");
				pNoCollisionElement->SetAttribute("First",  pNoCollision->sFirst);
				pNoCollisionElement->SetAttribute("Second", pNoCollision->sSecond);

				// Link no collision element to parent
				pNoCollisionsElement->LinkEndChild(*pNoCollisionElement);
			}
		}

		// Link no collisions element to parent
		pRagdollElement->LinkEndChild(*pNoCollisionsElement);
	}

	// Add controllers
	if (cSNRagdoll.GetNumOfControllers()) {
		XmlElement *pControllersElement = new XmlElement("Controllers");
		for (uint32 i=0; i<cSNRagdoll.GetNumOfControllers(); i++) {
			// Get the ragdoll controller
			const JointController *pController = cSNRagdoll.GetController(i);
			if (pController) {
				// Add the XML element
				XmlElement *pControllerElement = new XmlElement("Controller");
				pControllerElement->SetAttribute("Joint",			pController->sJoint);
				pControllerElement->SetAttribute("IsControllable",	pController->bIsControllable);
				pControllerElement->SetAttribute("ks1",				String::Format("%g", pController->ks1));
				pControllerElement->SetAttribute("kd1",				String::Format("%g", pController->kd1));
				pControllerElement->SetAttribute("ks2",				String::Format("%g", pController->ks2));
				pControllerElement->SetAttribute("kd2",				String::Format("%g", pController->kd2));

				// Link controller element to parent
				pControllersElement->LinkEndChild(*pControllerElement);
			}
		}

		// Link controllers element to parent
		pRagdollElement->LinkEndChild(*pControllersElement);
	}

	// Link ragdoll element to parent
	cDocument.LinkEndChild(*pRagdollElement);

	// Save ragdoll
	cDocument.Save(cFile);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
RagdollLoaderPL::RagdollLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
RagdollLoaderPL::~RagdollLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool RagdollLoaderPL::LoadV1(SNRagdoll &cSNRagdoll, const XmlElement &cRagdollElement, bool bAllowAttachToWorld) const
{
	// Iterate through all bodies
	const XmlNode *pBodiesNode = cRagdollElement.GetFirstChild("Bodies");
	while (pBodiesNode) {
		// Iterate through each body
		const XmlElement *pBodyElement = pBodiesNode->GetFirstChildElement("Body");
		while (pBodyElement) {
			// Get the name of the ragdoll body
			String sBodyName = pBodyElement->GetAttribute("Name");

			// Is there already a ragdoll body with this name?
			if (cSNRagdoll.GetBody(sBodyName)) {
				// Write a warning into the log
				PL_LOG(Warning, "Name '" + sBodyName + "' of ragdoll body at row " + pBodyElement->GetRow() + ", column " + pBodyElement->GetColumn() + " is already used! (this ragdoll body is ignored)")

			// Add new body
			} else {
				RagdollBody *pBody = cSNRagdoll.AddBody(sBodyName, false);
				if (pBody) {
					// Get settings
					const XmlAttribute *pAttribute = pBodyElement->GetFirstAttribute();
					while (pAttribute) {
						// Get the name of the attribute
						String sName = pAttribute->GetName();

						// Known attribute?
							 if (sName == "Enabled")  pBody->bEnabled = pAttribute->GetValue().GetBool();
						else if (sName == "Size")     ParseTools::ParseFloatArray(pAttribute->GetValue(), pBody->vSize, 3);
						else if (sName == "Position") ParseTools::ParseFloatArray(pAttribute->GetValue(), pBody->vPos, 3);
						else if (sName == "Rotation") ParseTools::ParseFloatArray(pAttribute->GetValue(), pBody->qRot, 4);
						else if (sName == "Mass")     pBody->fMass = pAttribute->GetValue().GetFloat();

						// Get the next attribute
						pAttribute = pAttribute->GetNext();
					}
				}
			}

			// Next element, please
			pBodyElement = pBodyElement->GetNextSiblingElement("Body");
		}

		// Next node, please
		pBodiesNode = cRagdollElement.IterateChildren("Bodies", pBodiesNode);
	}

	// Iterate through all joints
	const XmlNode *pJointsNode = cRagdollElement.GetFirstChild("Joints");
	while (pJointsNode) {
		// Iterate through each joint
		const XmlElement *pJointElement = pJointsNode->GetFirstChildElement("Joint");
		while (pJointElement) {
			// Get the name of the ragdoll joint
			String sJointName = pJointElement->GetAttribute("Name");

			// Is there already a ragdoll joint with this name?
			if (cSNRagdoll.GetJoint(sJointName)) {
				// Write a warning into the log
				PL_LOG(Warning, "Name '" + sJointName + "' of ragdoll joint at row " + pJointElement->GetRow() + ", column " + pJointElement->GetColumn() + " is already used! (this ragdoll joint is ignored)")

			// Add new joint
			} else {
				RagdollJoint *pJoint = cSNRagdoll.AddJoint(sJointName, false);
				if (pJoint) {
					// Get settings
					const XmlAttribute *pAttribute = pJointElement->GetFirstAttribute();
					while (pAttribute) {
						// Get the name of the attribute
						String sName = pAttribute->GetName();

						// Known attribute?
						if (sName == "Attached") {
							pJoint->sAttached = pAttribute->GetValue();

						} else if (sName == "Parent") {
							pJoint->sParent = pAttribute->GetValue();

						} else if (sName == "AttachToWorld") {
							pJoint->bAttachToWorld = pAttribute->GetValue().GetBool();

						} else if (sName == "Type") {
							if (pAttribute->GetValue() == "Hinge") {
								pJoint->nJointType = RagdollJoint::Hinge;
							} else if (pAttribute->GetValue() == "Universal") {
								pJoint->nJointType = RagdollJoint::Universal;
							}

						} else if (sName == "LowJointRange1") {
							pJoint->fLowJointRange1 = pAttribute->GetValue().GetFloat();

						} else if (sName == "HighJointRange1") {
							pJoint->fHighJointRange1 = pAttribute->GetValue().GetFloat();

						} else if (sName == "LowJointRange2") {
							pJoint->fLowJointRange2 = pAttribute->GetValue().GetFloat();

						} else if (sName == "HighJointRange2") {
							pJoint->fHighJointRange2 = pAttribute->GetValue().GetFloat();

						} else if (sName == "Anchor") {
							ParseTools::ParseFloatArray(pAttribute->GetValue(), pJoint->vJointAnchor, 3);

						} else if (sName == "RotAxis1") {
							ParseTools::ParseFloatArray(pAttribute->GetValue(), pJoint->vRotAxis1, 3);

						} else if (sName == "RotAxis2") {
							ParseTools::ParseFloatArray(pAttribute->GetValue(), pJoint->vRotAxis2, 3);
						}

						// Get the next attribute
						pAttribute = pAttribute->GetNext();
					}

					// Inform the body about it's joint
					RagdollBody *pBody = cSNRagdoll.GetBody(pJoint->sAttached);
					if (pBody) {
						// If the body has already an assigned joint, we have to decide which one should be used...
						if (pBody->sJoint.GetLength()) {
							if (!pJoint->bAttachToWorld)
								pBody->sJoint = pJoint->sName;
						} else pBody->sJoint = pJoint->sName;
					}
				}
			}

			// Next element, please
			pJointElement = pJointElement->GetNextSiblingElement("Joint");
		}

		// Next node, please
		pJointsNode = cRagdollElement.IterateChildren("Joints", pJointsNode);
	}

	// Iterate through all no collisions
	const XmlNode *pNoCollisionsNode = cRagdollElement.GetFirstChild("NoCollisions");
	while (pNoCollisionsNode) {
		// Iterate through each no collisions
		const XmlElement *pNoCollisionElement = pNoCollisionsNode->GetFirstChildElement("NoCollision");
		while (pNoCollisionElement) {
			// Add new no collision
			NoCollision *pNoCollision = cSNRagdoll.AddNoCollision();
			if (pNoCollision) {
				// Get settings
				const XmlAttribute *pAttribute = pNoCollisionElement->GetFirstAttribute();
				while (pAttribute) {
					// Get the name of the attribute
					String sName = pAttribute->GetName();

					// Known attribute?
						 if (sName == "First")  pNoCollision->sFirst  = pAttribute->GetValue();
					else if (sName == "Second") pNoCollision->sSecond = pAttribute->GetValue();

					// Get the next attribute
					pAttribute = pAttribute->GetNext();
				}
			}

			// Next element, please
			pNoCollisionElement = pNoCollisionElement->GetNextSiblingElement("NoCollision");
		}

		// Next node, please
		pNoCollisionsNode = cRagdollElement.IterateChildren("NoCollisions", pNoCollisionsNode);
	}

	// Iterate through all controllers
	const XmlNode *pControllersNode = cRagdollElement.GetFirstChild("Controllers");
	while (pControllersNode) {
		// Iterate through each controller
		const XmlElement *pControllerElement = pControllersNode->GetFirstChildElement("Controller");
		while (pControllerElement) {
			// Get the name of the ragdoll joint to control
			String sJointName = pControllerElement->GetAttribute("Joint");

			// Is there already a controller with this name?
			if (cSNRagdoll.GetController(sJointName)) {
				// Write a warning into the log
				PL_LOG(Warning, "Name '" + sJointName + "' of controller at row " + pControllerElement->GetRow() + ", column " + pControllerElement->GetColumn() + " is already used! (this controller is ignored)")

			// Add new controller
			} else {
				JointController *pController = cSNRagdoll.AddController(sJointName);
				if (pController) {
					// Get settings
					const XmlAttribute *pAttribute = pControllerElement->GetFirstAttribute();
					while (pAttribute) {
						// Get the name of the attribute
						String sName = pAttribute->GetName();

						// Known attribute?
							 if (sName == "IsControllable")	pController->bIsControllable = pAttribute->GetValue().GetBool();
						else if (sName == "ks1") 			pController->ks1 = pAttribute->GetValue().GetFloat();
						else if (sName == "kd1") 			pController->kd1 = pAttribute->GetValue().GetFloat();
						else if (sName == "ks2") 			pController->ks2 = pAttribute->GetValue().GetFloat();
						else if (sName == "kd2") 			pController->kd2 = pAttribute->GetValue().GetFloat();

						// Get the next attribute
						pAttribute = pAttribute->GetNext();
					}
				}
			}

			// Next element, please
			pControllerElement = pControllerElement->GetNextSiblingElement("Controller");
		}

		// Next node, please
		pControllersNode = cRagdollElement.IterateChildren("Controllers", pControllersNode);
	}

	// Now create the physics ragdoll
	cSNRagdoll.CreatePhysicsRagdoll(bAllowAttachToWorld);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
