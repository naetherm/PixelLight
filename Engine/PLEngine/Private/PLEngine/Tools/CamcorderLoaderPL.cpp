/*********************************************************\
 *  File: CamcorderLoaderPL.cpp                          *
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
#include <PLCore/File/File.h>
#include <PLRenderer/Animation/Animation.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodeModifiers/SNMPositionKeyframeAnimation.h>
#include <PLScene/Scene/SceneNodeModifiers/SNMRotationKeyframeAnimation.h>
#include "PLEngine/Tools/Camcorder.h"
#include "PLEngine/Tools/CamcorderLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(CamcorderLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool CamcorderLoaderPL::Load(Camcorder &cCamcorder, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get camcorder element
		const XmlElement *pCamcorderElement = cDocument.GetFirstChildElement("Camcorder");
		if (pCamcorderElement) {
			// Get the format version
			const int nVersion = pCamcorderElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cCamcorder, *pCamcorderElement, cFile);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cCamcorder, *pCamcorderElement, cFile);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'Camcorder' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool CamcorderLoaderPL::Save(Camcorder &cCamcorder, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add camcorder
	XmlElement *pCamcorderElement = new XmlElement("Camcorder");
	pCamcorderElement->SetAttribute("Version", "1");

	// Write camera scene container name attribute
	pCamcorderElement->SetAttribute("SceneContainer", cCamcorder.m_sCameraStartSceneContainer);

	// Write camera scene node name attribute
	pCamcorderElement->SetAttribute("SceneNode", cCamcorder.m_sCameraStartName);

	// Get the recorded camera
	SceneNode *pCameraSceneNode = cCamcorder.m_cCameraSceneNodeHandler.GetElement();
	if (pCameraSceneNode) {
		// Get the position keyframe record modifier from the camera scene node
		SceneNodeModifier *pSceneNodeModifier = pCameraSceneNode->GetModifier("PLEngine::SNMPositionKeyframeRecord");
		if (pSceneNodeModifier) {
			// Create the position keys element
			XmlElement *pElement = new XmlElement("PositionKeys");

			// Set coordinate system attribute
			pElement->SetAttribute("CoordinateSystem", pSceneNodeModifier->GetAttribute("CoordinateSystem")->GetString());

			// Set frames per second attribute
			pElement->SetAttribute("FramesPerSecond", pSceneNodeModifier->GetAttribute("FramesPerSecond")->GetString());

			// Add value
			XmlText *pValue = new XmlText(pSceneNodeModifier->GetAttribute("Keys")->GetString());
			pElement->LinkEndChild(*pValue);

			// Link position keys element to parent
			pCamcorderElement->LinkEndChild(*pElement);
		}

		// Get the rotation keyframe record modifier from the camera scene node
		pSceneNodeModifier = pCameraSceneNode->GetModifier("PLEngine::SNMRotationKeyframeRecord");
		if (pSceneNodeModifier) {
			// Create the rotation keys element
			XmlElement *pElement = new XmlElement("RotationKeys");

			// Set frames per second attribute
			pElement->SetAttribute("FramesPerSecond", pSceneNodeModifier->GetAttribute("FramesPerSecond")->GetString());

			// Add value
			XmlText *pValue = new XmlText(pSceneNodeModifier->GetAttribute("Keys")->GetString());
			pElement->LinkEndChild(*pValue);

			// Link rotation keys element to parent
			pCamcorderElement->LinkEndChild(*pElement);
		}
	}

	// Link camcorder element to parent
	cDocument.LinkEndChild(*pCamcorderElement);

	// Save the document
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
CamcorderLoaderPL::CamcorderLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
CamcorderLoaderPL::~CamcorderLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool CamcorderLoaderPL::LoadV1(Camcorder &cCamcorder, const XmlElement &cCamcorderElement, File &cFile) const
{
	// Get the recorded camera
	SceneNode *pCameraSceneNode = cCamcorder.m_cCameraSceneNodeHandler.GetElement();
	if (pCameraSceneNode) {
		// Get the scene container the camera is in
		const String sSceneContainer = cCamcorderElement.GetAttribute("SceneContainer");

		// Set the camera into this scene container
		SceneNode *pSceneNode = pCameraSceneNode->GetContainer()->GetByName(sSceneContainer);
		if (pSceneNode && pSceneNode->IsContainer())
			pCameraSceneNode->SetContainer(*static_cast<SceneContainer*>(pSceneNode));

		// Get and set the camera start name - not really required, the scene node name is just there to be complete
		//const String sSceneNode = cCamcorderElement.GetAttribute("SceneNode");
		//pCameraSceneNode->SetName(sSceneNode);

		// Get position keys
		const XmlElement *pXmlPositionKeysElement = cCamcorderElement.GetFirstChildElement("PositionKeys");
		if (pXmlPositionKeysElement) {
			// Get the coordinate system
			const String sCoordinateSystem = pXmlPositionKeysElement->GetAttribute("CoordinateSystem");

			// Get frames per second attribute
			const String sFramesPerSecond = pXmlPositionKeysElement->GetAttribute("FramesPerSecond");

			// Get value
			const XmlNode *pNode = pXmlPositionKeysElement->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				const String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					// Add position keyframe animation modifiers to the camera scene node
					SNMPositionKeyframeAnimation *pSNMPositionKeyframeAnimation = static_cast<SNMPositionKeyframeAnimation*>(pCameraSceneNode->AddModifier("PLScene::SNMPositionKeyframeAnimation", "Flags=\"PlaybackNoLoop\" Keys=\"" + sValue + "\" CoordinateSystem=\"" + sCoordinateSystem + "\" FramesPerSecond=\"" + sFramesPerSecond + '\"'));

					// Connect the event handler
					if (pSNMPositionKeyframeAnimation)
						pSNMPositionKeyframeAnimation->GetAnimation().EventStop.Connect(cCamcorder.SlotOnAnimationStop);
				}
			}
		}

		// Get rotation keys
		const XmlElement *pXmlRotationKeysElement = cCamcorderElement.GetFirstChildElement("RotationKeys");
		if (pXmlRotationKeysElement) {
			// Get frames per second attribute
			const String sFramesPerSecond = pXmlRotationKeysElement->GetAttribute("FramesPerSecond");

			// Get value
			const XmlNode *pNode = pXmlRotationKeysElement->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				const String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					// Add rotation keyframe animation modifiers to the camera scene node
					SNMRotationKeyframeAnimation *pSNMRotationKeyframeAnimation = static_cast<SNMRotationKeyframeAnimation*>(pCameraSceneNode->AddModifier("PLScene::SNMRotationKeyframeAnimation", "Flags=\"PlaybackNoLoop\" Keys=\"" + sValue +  + "\" FramesPerSecond=\"" + sFramesPerSecond + '\"'));

					// Connect the event handler
					if (pSNMRotationKeyframeAnimation)
						pSNMRotationKeyframeAnimation->GetAnimation().EventStop.Connect(cCamcorder.SlotOnAnimationStop);
				}
			}
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
