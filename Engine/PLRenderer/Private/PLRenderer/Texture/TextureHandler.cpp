/*********************************************************\
 *  File: TextureHandler.cpp                             *
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
#include <PLCore/Log/Log.h>
#include <PLCore/File/Url.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/String/Tokenizer.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Quaternion.h>
#include <PLMath/EulerAngles.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/FixedFunctions.h"
#include "PLRenderer/Animation/Animation.h"
#include "PLRenderer/Texture/AniInfo.h"
#include "PLRenderer/Texture/Texture.h"
#include "PLRenderer/Texture/TextureAni.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/TextureHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ TextureHandler::AnimationState                        ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the texture handlers texture
*/
Texture *TextureHandler::AnimationState::GetTexture() const
{
	const TextureAni *pTextureAni = static_cast<const TextureAni*>(m_pTextureHandler->GetResource());
	if (pTextureAni) {
		const uint32 nFrame = m_pTextureAnimation->GetCurrentFrame();
		if (nFrame < pTextureAni->GetTextureFrames().GetNumOfElements())
			return pTextureAni->GetTextureFrames()[nFrame]->pTextureHandler->GetTexture();
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns the texture handler from a texture animation frame
*/
TextureHandler *TextureHandler::AnimationState::GetFrameTextureHandler(int nIndex) const
{
	// Texture animation played?
	if (nIndex >= 0 || m_pTextureAnimation->IsActive()) {
		const TextureAni *pTextureAni = static_cast<const TextureAni*>(m_pTextureHandler->GetResource());
		if (pTextureAni) {
			const uint32 nFrame = nIndex < 0 ? m_pTextureAnimation->GetCurrentFrame() : nIndex;
			if (nFrame < pTextureAni->GetTextureFrames().GetNumOfElements())
				return pTextureAni->GetTextureFrames()[nFrame]->pTextureHandler;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns the matrix from a matrix animation frame
*/
Matrix4x4 TextureHandler::AnimationState::GetFrameMatrix(int nIndex) const
{
	// Matrix animation played?
	if (nIndex >= 0 || m_pMatrixAnimation->IsActive()) {
		const TextureAni *pTextureAni = static_cast<const TextureAni*>(m_pTextureHandler->GetResource());
		if (pTextureAni) {
			const uint32 nFrame = nIndex < 0 ? m_pMatrixAnimation->GetCurrentFrame() : nIndex;
			if (nFrame < pTextureAni->GetMatrixFrames().GetNumOfElements()) {
				// Set rotation
				Quaternion qRotation;
				const Vector3 &vRot = pTextureAni->GetMatrixFrames()[nFrame]->vRotation;
				EulerAngles::ToQuaternion(static_cast<float>(vRot.x*Math::DegToRad), static_cast<float>(vRot.y*Math::DegToRad), static_cast<float>(vRot.z*Math::DegToRad), qRotation);

				// Set position and rotation
				Matrix4x4 mTrans;
				mTrans.FromQuatTrans(qRotation, pTextureAni->GetMatrixFrames()[nFrame]->vTranslation);

				// Apply scale
				Matrix3x3 mScale;
				mScale.SetScaleMatrix(pTextureAni->GetMatrixFrames()[nFrame]->vScale);
				mTrans *= mScale;

				// Return the final transform matrix
				return mTrans;
			}
		}
	}

	// Error!
	return Matrix4x4::Identity;
}

/**
*  @brief
*    Copy operator
*/
TextureHandler::AnimationState &TextureHandler::AnimationState::operator =(const TextureHandler::AnimationState &cSource)
{
	*m_pTextureAnimation = *cSource.m_pTextureAnimation;
	m_mTrans			 = cSource.m_mTrans;
	*m_pMatrixAnimation  = *cSource.m_pMatrixAnimation;
	m_cColor			 = cSource.m_cColor;
	*m_pColorAnimation   = *cSource.m_pColorAnimation;

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureHandler::AnimationState::AnimationState(TextureHandler &cTextureHandler) :
	EventHandlerUpdate(&AnimationState::OnUpdate, this),
	m_pTextureHandler(&cTextureHandler),
	m_pTextureAnimation(new Animation()),
	m_pMatrixAnimation(new Animation()),
	m_cColor(Color4::White),
	m_pColorAnimation(new Animation())
{
	const TextureAni *pTextureAni = static_cast<TextureAni*>(m_pTextureHandler->GetResource());
	if (pTextureAni) {
		AnimationInfo *pAniInfo;

		// Start texture animation
		if (pTextureAni->GetTextureAnimationManager().GetNumOfElements() == 1)
			pAniInfo = static_cast<AnimationInfo*>(pTextureAni->GetTextureAnimationManager().GetByIndex(0)); // Default animation
		else
			pAniInfo = static_cast<AnimationInfo*>(pTextureAni->GetTextureAnimationManager().GetByIndex(1));
		if (pAniInfo) {
			m_pTextureAnimation->Start(*pAniInfo);
			if (m_pTextureAnimation->GetCurrentFrame() == m_pTextureAnimation->GetNextFrame())
				m_pTextureAnimation->Stop();
		}

		// Start matrix animation
		if (pTextureAni->GetMatrixAnimationManager().GetNumOfElements() == 1)
			pAniInfo = static_cast<AnimationInfo*>(pTextureAni->GetMatrixAnimationManager().GetByIndex(0)); // Default animation
		else
			pAniInfo = static_cast<AnimationInfo*>(pTextureAni->GetMatrixAnimationManager().GetByIndex(1));
		if (pAniInfo) {
			m_pMatrixAnimation->Start(*pAniInfo);
			if (m_pMatrixAnimation->GetCurrentFrame() == m_pMatrixAnimation->GetNextFrame())
				m_pMatrixAnimation->Stop();
		}

		// Start color animation
		if (pTextureAni->GetColorAnimationManager().GetNumOfElements() == 1)
			pAniInfo = static_cast<AnimationInfo*>(pTextureAni->GetColorAnimationManager().GetByIndex(0)); // Default animation
		else
			pAniInfo = static_cast<AnimationInfo*>(pTextureAni->GetColorAnimationManager().GetByIndex(1));
		if (pAniInfo) {
			m_pColorAnimation->Start(*pAniInfo);
			if (m_pColorAnimation->GetCurrentFrame() == m_pColorAnimation->GetNextFrame())
				m_pColorAnimation->Stop();
		}

		// Connect event handler
		pTextureAni->GetTextureManager().GetRendererContext().EventUpdate.Connect(EventHandlerUpdate);
	}
}

/**
*  @brief
*    Destructor
*/
TextureHandler::AnimationState::~AnimationState()
{
	// Destroy data
	delete m_pTextureAnimation;
	delete m_pMatrixAnimation;
	delete m_pColorAnimation;
}

/**
*  @brief
*    Called on update
*/
void TextureHandler::AnimationState::OnUpdate()
{
	const TextureAni *pTextureAni = static_cast<const TextureAni*>(m_pTextureHandler->GetResource());
	if (pTextureAni) {
		// Get time difference
		const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

		// Texture animation
		m_pTextureAnimation->Update(fTimeDiff); // Update texture animation

		// Matrix animation
		m_pMatrixAnimation->Update(fTimeDiff); // Update matrix animation
		{ // Set current matrix
			const float fFrame = m_pMatrixAnimation->GetFrame()-m_pMatrixAnimation->GetCurrentFrame();
			const TextureAni::MatrixFrame *pMatrixFrame1 = pTextureAni->GetMatrixFrames()[m_pMatrixAnimation->GetCurrentFrame()];
			const TextureAni::MatrixFrame *pMatrixFrame2 = pTextureAni->GetMatrixFrames()[m_pMatrixAnimation->GetNextFrame()];
			if (pMatrixFrame1 && pMatrixFrame2) {
				// Set rotation
				Quaternion qRotation;
				Vector3 vRot = pMatrixFrame1->vRotation+(pMatrixFrame2->vRotation-pMatrixFrame1->vRotation)*fFrame;
				EulerAngles::ToQuaternion(static_cast<float>(vRot.x*Math::DegToRad), static_cast<float>(vRot.y*Math::DegToRad), static_cast<float>(vRot.z*Math::DegToRad), qRotation);

				// Set position and rotation
				m_mTrans.FromQuatTrans(qRotation, pMatrixFrame1->vTranslation+(pMatrixFrame2->vTranslation-pMatrixFrame1->vTranslation)*fFrame);

				// Apply scale
				Matrix3x3 mScale;
				mScale.SetScaleMatrix(pMatrixFrame1->vScale+(pMatrixFrame2->vScale-pMatrixFrame1->vScale)*fFrame);
				m_mTrans *= mScale;
			}
		}

		// Color animation
		m_pColorAnimation->Update(fTimeDiff); // Update color animation
		{ // Set current color
			const float fFrame = m_pColorAnimation->GetFrame()-m_pColorAnimation->GetCurrentFrame();
			const TextureAni::ColorFrame *pColorFrame1 = pTextureAni->GetColorFrames()[m_pColorAnimation->GetCurrentFrame()];
			const TextureAni::ColorFrame *pColorFrame2 = pTextureAni->GetColorFrames()[m_pColorAnimation->GetNextFrame()];
			if (pColorFrame1 && pColorFrame2)
				m_cColor = pColorFrame1->vColor+(pColorFrame2->vColor-pColorFrame1->vColor)*fFrame;
		}
	}
}




//[-------------------------------------------------------]
//[ TextureHandler                                        ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureHandler::TextureHandler() :
	m_pAnimationState(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
TextureHandler::~TextureHandler()
{
	Unload();
}

/**
*  @brief
*    Load texture
*/
bool TextureHandler::Load(TextureManager &cTextureManager, const String &sFilename, bool bReloadTexture)
{
	// Unload old texture
	Unload();

	// Check if it's a texture animation
	const String sExtension = Url(sFilename).GetExtension();
	if (sExtension == "tani") {
		if (m_pAnimationState) {
			delete m_pAnimationState;
			m_pAnimationState = nullptr;
		}

		Texture *pTexture = cTextureManager.GetByName(sFilename);
		if (pTexture && !pTexture->IsAnimated()) {
			SetResource(pTexture);
		} else {
			TextureAni *pTextureAni = static_cast<TextureAni*>(pTexture);
			if (!pTextureAni) {
				pTextureAni = new TextureAni(cTextureManager, sFilename);
				if (!pTextureAni->LoadByFilename(sFilename)) {
					delete pTextureAni;

					// Error!
					return false;
				}
			}

			SetResource(pTextureAni);
			m_pAnimationState = new AnimationState(*this);
		}

		// Done
		return true;
	}

	// Load texture
	if (bReloadTexture) {
		// There are file formats without an extension, so no extension must also be valid
		if (cTextureManager.GetByName(sFilename))
			SetResource(cTextureManager.LoadResource(sFilename));
		else {
			SetResource(cTextureManager.Create(sFilename));
			if (GetTexture())
				GetTexture()->LoadByFilename(sFilename);
		}

		// Done
		return true;
	} else {
		// Create the texture by using a texture creator?
		Texture *pTexture = nullptr;
		if (sFilename.Compare("Create ", 0, 7)) {
			// Get texture creator class to use
			Tokenizer cTokenizer;
			cTokenizer.Start(sFilename);
			cTokenizer.GetNextToken(); // Skip 'Create '
			String sClass = cTokenizer.GetNextToken();
			if (sClass.GetLength()) {
				// Get texture name
				bool bUseName = false;
				String sName;
				cTokenizer.PushState();
				if (cTokenizer.FindToken("Name")) {
					// Skip '='
					cTokenizer.GetNextToken();

					// Get name
					if (cTokenizer.GetNextToken().GetLength()) {
						if (cTokenizer.GetNextToken().GetLength() == 1 && cTokenizer.GetQuotes().IsSubstring(cTokenizer.GetToken())) {
							sName = cTokenizer.GetNextToken();
							// Read "
							if (!cTokenizer.GetNextToken().GetLength() == 1 && cTokenizer.GetQuotes().IsSubstring(cTokenizer.GetToken()))
								PL_LOG(Error, "Name var closing \" is missed! (" + sFilename + ')')
						} else {
							sName = cTokenizer.GetToken();
						}
						bUseName = true;
					}
				} else {
					// There's no name! :(
					cTokenizer.PopState();
				}

				// Check whether the texture is already within the texture manager
				pTexture = bUseName ? cTextureManager.GetByName(sName) : nullptr;
				if (!pTexture) {
					// Get the parameters
					String sParameters = sFilename.GetSubstring(cTokenizer.GetPosition());

					// Try to create the texture
					pTexture = cTextureManager.CreateTexture(sClass, sParameters);
					if (pTexture && bUseName)
						pTexture->SetName(sName);
				}
			}
			cTokenizer.Stop();

		// Try to get/load the texture
		// -> There are file formats without an extension, so no extension must also be valid
		} else {
			pTexture = cTextureManager.GetByName(sFilename);
			if (!pTexture) {
				pTexture = cTextureManager.Create(sFilename);
				if (pTexture && !pTexture->LoadByFilename(sFilename)) {
					// Can't load texture...
					delete pTexture;
					pTexture = nullptr;
				}
			}
		}

		// Set resource
		return (pTexture && SetResource(pTexture));
	}
}

/**
*  @brief
*    Unload texture
*/
bool TextureHandler::Unload()
{
	// Destroy texture animation state
	if (m_pAnimationState) {
		delete m_pAnimationState;
		m_pAnimationState = nullptr;
	}

	// Unload texture
	SetResource();

	// Done
	return true;
}

/**
*  @brief
*    Makes the texture handlers texture to the current renderer texture
*/
bool TextureHandler::Bind(uint32 nStage) const
{
	// Is there a texture animation?
	if (m_pAnimationState) {
		// Bind current texture
		const Texture *pTexture = GetTexture();
		if (pTexture && pTexture->Bind(nStage)) {
			// Fixed functions
			FixedFunctions *pFixedFunctions = pTexture->GetTextureManager().GetRendererContext().GetRenderer().GetFixedFunctions();
			if (pFixedFunctions) {
				// Set texture matrix
				pFixedFunctions->SetTransformState(static_cast<FixedFunctions::Transform::Enum>(static_cast<uint32>(FixedFunctions::Transform::Texture0) + nStage), m_pAnimationState->GetMatrix());
			}

			// Done
			return true;
		}
	} else {
		// Bind texture
		const Texture *pTexture = GetResource();
		if (pTexture && pTexture->Bind(nStage)) {
			// Fixed functions
			FixedFunctions *pFixedFunctions = pTexture->GetTextureManager().GetRendererContext().GetRenderer().GetFixedFunctions();
			if (pFixedFunctions) {
				// Set identity texture matrix
				pFixedFunctions->SetTransformState(static_cast<FixedFunctions::Transform::Enum>(static_cast<uint32>(FixedFunctions::Transform::Texture0) + nStage), Matrix4x4::Identity);
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the texture handlers texture
*/
Texture *TextureHandler::GetTexture() const
{
	// Is there a texture animation?
	return m_pAnimationState ? m_pAnimationState->GetTexture() : GetResource();
}

/**
*  @brief
*    Copy operator
*/
TextureHandler &TextureHandler::operator =(const TextureHandler &cSource)
{
	// Texture
	if (cSource.GetTexture())
		SetResource(cSource.GetTexture());

	// Texture animation state
	if (cSource.m_pAnimationState) {
		if (!m_pAnimationState)
			m_pAnimationState = new AnimationState(*this);
		*m_pAnimationState = *cSource.m_pAnimationState;
	} else {
		if (m_pAnimationState) {
			delete m_pAnimationState;
			m_pAnimationState = nullptr;
		}
	}

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
