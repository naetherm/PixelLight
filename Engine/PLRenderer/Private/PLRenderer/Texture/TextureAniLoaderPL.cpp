/*********************************************************\
 *  File: TextureAniLoaderPL.cpp                         *
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
#include "PLRenderer/Texture/AniInfo.h"
#include "PLRenderer/Texture/TextureAni.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/TextureAniLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(TextureAniLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool TextureAniLoaderPL::Load(TextureAni &cTextureAni, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get texture animation element
		const XmlElement *pTextureAniElement = cDocument.GetFirstChildElement("TextureAnimation");
		if (pTextureAniElement) {
			// Get the format version
			const int nVersion = pTextureAniElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cTextureAni, *pTextureAniElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cTextureAni, *pTextureAniElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'TextureAnimation' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool TextureAniLoaderPL::Save(const TextureAni &cTextureAni, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add texture animation
	XmlElement *pTextureAniElement = new XmlElement("TextureAnimation");
	pTextureAniElement->SetAttribute("Version", "1");

	// Add texture frames
	if (cTextureAni.GetTextureFrames().GetNumOfElements()) {
		XmlElement *pTextureFramesElement = new XmlElement("TextureFrames");

		// Loop through all frames
		for (uint32 i=0; i<cTextureAni.GetTextureFrames().GetNumOfElements(); i++) {
			// Get the frame information
			const TextureAni::TextureFrame *pTextureFrame = cTextureAni.GetTextureFrames()[i];
			if (pTextureFrame) {
				XmlElement *pFrameElement = new XmlElement("Frame");
				if (pTextureFrame->pTextureHandler && pTextureFrame->pTextureHandler->GetResource()) {
					// Add value
					XmlText *pValue = new XmlText(pTextureFrame->pTextureHandler->GetResource()->GetName());
					pFrameElement->LinkEndChild(*pValue);
				}

				// Link frame element
				pTextureFramesElement->LinkEndChild(*pFrameElement);
			}
		}

		// Link texture frames element
		pTextureAniElement->LinkEndChild(*pTextureFramesElement);
	}

	// Add matrix frames
	if (cTextureAni.GetMatrixFrames().GetNumOfElements()) {
		XmlElement *pMatrixFramesElement = new XmlElement("MatrixFrames");

		// Loop through all frames
		for (uint32 i=0; i<cTextureAni.GetMatrixFrames().GetNumOfElements(); i++) {
			// Get the frame information
			const TextureAni::MatrixFrame *pMatrixFrame = cTextureAni.GetMatrixFrames()[i];
			if (pMatrixFrame) {
				XmlElement *pFrameElement = new XmlElement("Frame");

				// Add position
				if (pMatrixFrame->vTranslation != Vector3::Zero)
					pFrameElement->SetAttribute("Position", pMatrixFrame->vTranslation.ToString());

				// Add rotation
				if (pMatrixFrame->vRotation != Vector3::Zero)
					pFrameElement->SetAttribute("Rotation", pMatrixFrame->vRotation.ToString());

				// Add scale
				if (pMatrixFrame->vScale != Vector3::One)
					pFrameElement->SetAttribute("Scale", pMatrixFrame->vScale.ToString());

				// Link frame element
				pMatrixFramesElement->LinkEndChild(*pFrameElement);
			}
		}

		// Link matrix frames element
		pTextureAniElement->LinkEndChild(*pMatrixFramesElement);
	}

	// Add color frames
	if (cTextureAni.GetColorFrames().GetNumOfElements()) {
		XmlElement *pColorFramesElement = new XmlElement("ColorFrames");

		// Loop through all frames
		for (uint32 i=0; i<cTextureAni.GetColorFrames().GetNumOfElements(); i++) {
			// Get the frame information
			const TextureAni::ColorFrame *pColorFrame = cTextureAni.GetColorFrames()[i];
			if (pColorFrame) {
				XmlElement *pFrameElement = new XmlElement("Frame");

				// Add value
				XmlText *pValue = new XmlText(pColorFrame->vColor.ToString());
				pFrameElement->LinkEndChild(*pValue);

				// Link frame element
				pColorFramesElement->LinkEndChild(*pFrameElement);
			}
		}

		// Link color frames element
		pTextureAniElement->LinkEndChild(*pColorFramesElement);
	}

	// Add texture animation information
	AddAniInfoManager(*pTextureAniElement, cTextureAni.GetTextureAnimationManager(), "Texture");

	// Add matrix animation information
	AddAniInfoManager(*pTextureAniElement, cTextureAni.GetMatrixAnimationManager(), "Matrix");

	// Add color animation information
	AddAniInfoManager(*pTextureAniElement, cTextureAni.GetColorAnimationManager(), "Color");

	// Link texture animation element
	cDocument.LinkEndChild(*pTextureAniElement);

	// Save settings
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
TextureAniLoaderPL::TextureAniLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
TextureAniLoaderPL::~TextureAniLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool TextureAniLoaderPL::LoadV1(TextureAni &cTextureAni, const XmlElement &cTextureAniElement) const
{
	// Iterate through all texture frames
	const XmlNode *pTextureFramesNode = cTextureAniElement.GetFirstChild("TextureFrames");
	while (pTextureFramesNode) {
		// Iterate through all texture frames
		const XmlNode *pTextureFrameNode = pTextureFramesNode->GetFirstChild("Frame");
		while (pTextureFrameNode) {
			// Get value
			const XmlNode *pNode = pTextureFrameNode->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					// Create texture frame
					TextureAni::TextureFrame *pTextureFrame = new TextureAni::TextureFrame;
					pTextureFrame->pTextureHandler = new TextureHandler();
					pTextureFrame->pTextureHandler->Load(cTextureAni.GetTextureManager(), sValue);

					// Add texture frame
					cTextureAni.GetTextureFrames().Add(pTextureFrame);
				}
			}

			// Next node, please
			pTextureFrameNode = pTextureFramesNode->IterateChildren("Frame", pTextureFrameNode);
		}

		// Next node, please
		pTextureFramesNode = cTextureAniElement.IterateChildren("TextureFrames", pTextureFramesNode);
	}

	// Iterate through all matrix frames
	const XmlNode *pMatrixFramesNode = cTextureAniElement.GetFirstChild("MatrixFrames");
	while (pMatrixFramesNode) {
		// Iterate through all matrix frames
		const XmlElement *pMatrixFrameElement = pMatrixFramesNode->GetFirstChildElement("Frame");
		while (pMatrixFrameElement) {
			// Create and add matrix frame
			TextureAni::MatrixFrame *pMatrixFrame = new TextureAni::MatrixFrame;
			cTextureAni.GetMatrixFrames().Add(pMatrixFrame);
			pMatrixFrame->vScale = Vector3::One;

			// Position
			String sValue = pMatrixFrameElement->GetAttribute("Position");
			if (sValue.GetLength())
				pMatrixFrame->vTranslation.FromString(sValue);
			// Rotation
			sValue = pMatrixFrameElement->GetAttribute("Rotation");
			if (sValue.GetLength())
				pMatrixFrame->vRotation.FromString(sValue);
			// Scale
			sValue = pMatrixFrameElement->GetAttribute("Scale");
			if (sValue.GetLength())
				pMatrixFrame->vScale.FromString(sValue);

			// Next element, please
			pMatrixFrameElement = pMatrixFrameElement->GetNextSiblingElement("Frame");
		}

		// Next node, please
		pMatrixFramesNode = cTextureAniElement.IterateChildren("MatrixFrames", pMatrixFramesNode);
	}

	// Iterate through all color frames
	const XmlNode *pColorFramesNode = cTextureAniElement.GetFirstChild("ColorFrames");
	while (pColorFramesNode) {
		// Iterate through all color frames
		const XmlNode *pColorFrameNode = pColorFramesNode->GetFirstChild("Frame");
		while (pColorFrameNode) {
			// Get value
			const XmlNode *pNode = pColorFrameNode->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				const String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					// Create and add color frame
					TextureAni::ColorFrame *pColorFrame = new TextureAni::ColorFrame;
					cTextureAni.GetColorFrames().Add(pColorFrame);
					pColorFrame->vColor.FromString(sValue);
				}
			}

			// Next node, please
			pColorFrameNode = pColorFramesNode->IterateChildren("Frame", pColorFrameNode);
		}

		// Next element, please
		pColorFramesNode = cTextureAniElement.IterateChildren("ColorFrames", pColorFramesNode);
	}

	// Iterate through all animations
	int nValue;
	float fValue;
	const XmlElement *pAnimationElement = cTextureAniElement.GetFirstChildElement("Animation");
	while (pAnimationElement) {
		AniInfo *pAnimation = nullptr;
		uint32 nFrames = 0;

		// Type
		String sValue = pAnimationElement->GetAttribute("Type");
		if (sValue.GetLength()) {
			// Texture
			if (sValue == "Texture") {
				cTextureAni.CreateStandardTextureAnimation();
				pAnimation = cTextureAni.GetTextureAnimationManager().Create();
				pAnimation->SetType(0);
			// Matrix
			} else if (sValue == "Matrix") {
				cTextureAni.CreateStandardMatrixAnimation();
				pAnimation = cTextureAni.GetMatrixAnimationManager().Create();
				pAnimation->SetType(1);
			// Color
			} else if (sValue == "Color") {
				cTextureAni.CreateStandardColorAnimation();
				pAnimation = cTextureAni.GetColorAnimationManager().Create();
				pAnimation->SetType(2);
			}
			if (pAnimation) {
				switch (pAnimation->GetType()) {
					case 0:
						nFrames = cTextureAni.GetTextureFrames().GetNumOfElements();
						break;

					case 1:
						nFrames = cTextureAni.GetMatrixFrames().GetNumOfElements();
						break;

					case 2:
						nFrames = cTextureAni.GetColorFrames().GetNumOfElements();
						break;
				}
			}
		}
		if (pAnimation) {
			// Name
			sValue = pAnimationElement->GetAttribute("Name");
			if (sValue.GetLength())
				pAnimation->SetName(sValue);

			// Start
			if (pAnimationElement->QueryIntAttribute("Start", &nValue) == XmlBase::Success) {
				if (nValue >= static_cast<int>(nFrames))
					pAnimation->SetStartFrame(nFrames-1);
				else {
					if (nValue < 0)
						nValue = 0;
					pAnimation->SetStartFrame(nValue);
				}
			}

			// End
			if (pAnimationElement->QueryIntAttribute("End", &nValue) == XmlBase::Success) {
				if (nValue >= static_cast<int>(nFrames))
					pAnimation->SetEndFrame(nFrames-1);
				else {
					if (nValue < 0)
						nValue = 0;
					pAnimation->SetEndFrame(nValue);
				}
			}

			// Speed
			if (pAnimationElement->QueryFloatAttribute("Speed", &fValue) == XmlBase::Success)
				pAnimation->SetSpeed(fValue);

			// Loop
			sValue = pAnimationElement->GetAttribute("Loop");
			if (sValue.GetLength()) {
				if (sValue.GetBool())
					pAnimation->SetFlags(pAnimation->GetFlags() |  AnimationInfo::Loop);
				else
					pAnimation->SetFlags(pAnimation->GetFlags() & ~AnimationInfo::Loop);
			}

			// PingPong
			sValue = pAnimationElement->GetAttribute("PingPong");
			if (sValue.GetLength()) {
				if (sValue.GetBool())
					pAnimation->SetFlags(pAnimation->GetFlags() |  AnimationInfo::PingPong);
				else
					pAnimation->SetFlags(pAnimation->GetFlags() & ~AnimationInfo::PingPong);
			}

			// Iterate through all frames
			const XmlNode *pFrameNode = pAnimationElement->GetFirstChild("Frame");
			while (pFrameNode) {
				// Is this an XML element?
				if (pFrameNode->GetType() == XmlNode::Element) {
					const XmlElement *pFrameElement = static_cast<const XmlElement*>(pFrameNode);

					// ID
					nValue = -1;
					if (pFrameElement->QueryIntAttribute("ID", &nValue) == XmlBase::Success &&
						nValue >= 0 && nValue < static_cast<int>(nFrames)) {
						// Speed
						float fSpeed = 1.0f;
						sValue = pFrameElement->GetAttribute("Speed");
						if (sValue.GetLength())
							fSpeed = sValue.GetFloat();

						// Set
						pAnimation->GetFrameInfo(nValue)->SetSpeed(fSpeed);
					}
				}

				// Next node, please
				pFrameNode = pAnimationElement->IterateChildren("Frame", pFrameNode);
			}

			// Iterate through all events
			const XmlNode *pEventNode = pAnimationElement->GetFirstChild("Event");
			while (pEventNode) {
				// Is this an XML element?
				if (pEventNode->GetType() == XmlNode::Element) {
					const XmlElement *pEventElement = static_cast<const XmlElement*>(pEventNode);

					// FrameID
					int nFrameID = -1;
					if (pEventElement->QueryIntAttribute("FrameID", &nFrameID) == XmlBase::Success &&
						nFrameID >= 0 && nFrameID < static_cast<int>(nFrames)) {
						// ID
						int nID = 0;
						if (pEventElement->QueryIntAttribute("ID", &nID) == XmlBase::Success) {
							// Create the event, added automatically to the event manager
							new AnimationEvent(nID, nFrameID, &pAnimation->GetEventManager());
						}
					}
				}

				// Next node, please
				pEventNode = pAnimationElement->IterateChildren("Event", pEventNode);
			}
		}

		// Next element, please
		pAnimationElement = pAnimationElement->GetNextSiblingElement("Animation");
	}

	// Check if there is at least one frame per animation type
	if (!cTextureAni.GetTextureFrames().GetNumOfElements()) { // No texture frames found
		// Create and add standard texture frame
		TextureAni::TextureFrame *pTextureFrame = new TextureAni::TextureFrame;
		pTextureFrame->pTextureHandler = new TextureHandler();
		pTextureFrame->pTextureHandler->Load(cTextureAni.GetTextureManager(), TextureManager::Default);
		cTextureAni.GetTextureFrames().Add(pTextureFrame);
	}
	if (!cTextureAni.GetMatrixFrames().GetNumOfElements()) { // No matrix frames found
		// Create and add standard matrix frame
		TextureAni::MatrixFrame *pMatrixFrame = new TextureAni::MatrixFrame;
		pMatrixFrame->vScale = Vector3::One;
		cTextureAni.GetMatrixFrames().Add(pMatrixFrame);
	}
	if (!cTextureAni.GetColorFrames().GetNumOfElements()) { // No color frames found
		// Create and add standard color frame
		TextureAni::ColorFrame *pColorFrame = new TextureAni::ColorFrame;
		pColorFrame->vColor = 1.0f;
		cTextureAni.GetColorFrames().Add(pColorFrame);
	}

	// Create standard animations (if there aren't created until now)
	cTextureAni.CreateStandardTextureAnimation();
	cTextureAni.CreateStandardMatrixAnimation();
	cTextureAni.CreateStandardColorAnimation();

	// Done
	return true;
}

/**
*  @brief
*    Adds an animation information manager to the given XML element
*/
void TextureAniLoaderPL::AddAniInfoManager(XmlElement &cElement, const AniInfoManager &cAniInfoManager, const String &sType) const
{
	// Loop through all animation information
	for (uint32 i=0; i<cAniInfoManager.GetNumOfElements(); i++) {
		// Get animation information
		const AniInfo *pAnimation = cAniInfoManager.GetByIndex(i);
		if (pAnimation) {
			XmlElement *pAnimationElement = new XmlElement("Animation");

			// Type
			pAnimationElement->SetAttribute("Type", sType);

			// Name
			if (pAnimation->GetName().GetLength())
				pAnimationElement->SetAttribute("Name", pAnimation->GetName());

			// Start
			if (pAnimation->GetStartFrame() != 0)
				pAnimationElement->SetAttribute("Start", pAnimation->GetStartFrame());

			// End
			if (pAnimation->GetEndFrame() != 0)
				pAnimationElement->SetAttribute("End", pAnimation->GetEndFrame());

			// Speed
			if (pAnimation->GetSpeed() != 24.0f)
				pAnimationElement->SetAttribute("Speed", String(pAnimation->GetSpeed()));

			// Loop
			if (pAnimation->GetFlags() & AnimationInfo::Loop)
				pAnimationElement->SetAttribute("Loop", "1");

			// PingPong
			if (pAnimation->GetFlags() & AnimationInfo::PingPong)
				pAnimationElement->SetAttribute("PingPong", "1");

			// Add frame information
			uint32 nNumOfFrames = pAnimation->GetNumOfFrames();
			for (uint32 nFrameID=0; nFrameID<nNumOfFrames; nFrameID++) {
				// Get frame information
				const AnimationFrameInfo *pFrameInfo = pAnimation->GetFrameInfo(nFrameID);

				// Add?
				if (pFrameInfo && pFrameInfo->GetSpeed() != 1.0f) {
					XmlElement *pFrameInfoElement = new XmlElement("Frame");

					// ID
					pAnimationElement->SetAttribute("ID", nFrameID);

					// Speed
					pAnimationElement->SetAttribute("Speed", String(pFrameInfo->GetSpeed()));

					// Link frame information element
					pAnimationElement->LinkEndChild(*pFrameInfoElement);
				}
			}

			// Add events
			const AnimationEventManager &cEventManager = pAnimation->GetEventManager();
			for (uint32 nEvent=0; nEvent<cEventManager.GetNumOfElements(); nEvent++) {
				// Get the event
				const AnimationEvent *pEvent = cEventManager.GetByIndex(nEvent);
				if (pEvent) {
					XmlElement *pEventElement = new XmlElement("Event");

					// FrameID
					pEventElement->SetAttribute("FrameID", pEvent->GetFrame());

					// ID
					pEventElement->SetAttribute("ID", pEvent->GetID());

					// Link event element
					pAnimationElement->LinkEndChild(*pEventElement);
				}
			}

			// Link animation element
			cElement.LinkEndChild(*pAnimationElement);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
