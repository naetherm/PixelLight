/*********************************************************\
 *  File: TextureAni.cpp                                 *
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
#include "PLRenderer/Texture/AniInfo.h"
#include "PLRenderer/Texture/TextureHandler.h"
#include "PLRenderer/Texture/TextureAni.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
TextureAni::~TextureAni()
{
	// Unload this texture animation
	Unload();
}

/**
*  @brief
*    Copy operator
*/
TextureAni &TextureAni::operator =(const TextureAni &cSource)
{
	// Unload this texture animation
	Unload();

	// Texture animation
	for (uint32 i=0; i<cSource.m_cTextureAnimationManager.GetNumOfElements(); i++) {
		const AnimationInfo *pAnimationInfo = static_cast<AnimationInfo*>(cSource.m_cTextureAnimationManager.GetByIndex(i));
		AnimationInfo *pNewInfo = static_cast<AnimationInfo*>(m_cTextureAnimationManager.Create());
		*pNewInfo = *pAnimationInfo;
	}
	for (uint32 i=0; i<cSource.m_lstTextureFrames.GetNumOfElements(); i++) {
		TextureFrame *pFrame = new TextureFrame;
		pFrame->pTextureHandler  = new TextureHandler();
		*pFrame->pTextureHandler = *cSource.m_lstTextureFrames[i]->pTextureHandler;
		m_lstTextureFrames.Add(pFrame);
	}

	// Matrix animation
	for (uint32 i=0; i<cSource.m_cMatrixAnimationManager.GetNumOfElements(); i++) {
		const AnimationInfo *pAnimationInfo = static_cast<const AnimationInfo*>(cSource.m_cMatrixAnimationManager.GetByIndex(i));
		AnimationInfo *pNewInfo = static_cast<AnimationInfo*>(m_cMatrixAnimationManager.Create());
		*pNewInfo = *pAnimationInfo;
	}
	for (uint32 i=0; i<cSource.m_lstMatrixFrames.GetNumOfElements(); i++) {
		MatrixFrame *pFrame		  = new MatrixFrame;
		MatrixFrame *pSourceFrame = cSource.m_lstMatrixFrames[i];
		pFrame->vTranslation = pSourceFrame->vTranslation;
		pFrame->vRotation    = pSourceFrame->vRotation;
		pFrame->vScale       = pSourceFrame->vScale;
		m_lstMatrixFrames.Add(pFrame);
	}

	// Color animation
	for (uint32 i=0; i<cSource.m_cColorAnimationManager.GetNumOfElements(); i++) {
		const AnimationInfo *pAnimationInfo = static_cast<const AnimationInfo*>(cSource.m_cColorAnimationManager.GetByIndex(i));
		AnimationInfo *pNewInfo = static_cast<AnimationInfo*>(m_cColorAnimationManager.Create());
		*pNewInfo = *pAnimationInfo;
	}
	for (uint32 i=0; i<cSource.m_lstColorFrames.GetNumOfElements(); i++) {
		ColorFrame *pFrame = new ColorFrame;
		pFrame->vColor = cSource.m_lstColorFrames[i]->vColor;
		m_lstColorFrames.Add(pFrame);
	}

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Texture animation                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create standard texture animation
*/
void TextureAni::CreateStandardTextureAnimation()
{
	if (!m_cTextureAnimationManager.GetNumOfElements()) {
		AnimationInfo *pAnimation = static_cast<AnimationInfo*>(m_cTextureAnimationManager.Create("Texture standard animation"));
		pAnimation->SetType(0);
		pAnimation->SetStartFrame(0);
		pAnimation->SetEndFrame(m_lstTextureFrames.GetNumOfElements()-1);
		pAnimation->SetSpeed(1.0f);
		pAnimation->SetFlags(AnimationInfo::Loop);
	}
}


//[-------------------------------------------------------]
//[ Matrix animation                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create standard matrix animation
*/
void TextureAni::CreateStandardMatrixAnimation()
{
	if (!m_cMatrixAnimationManager.GetNumOfElements()) {
		AnimationInfo *pAnimation = static_cast<AnimationInfo*>(m_cMatrixAnimationManager.Create("Matrix standard animation"));
		pAnimation->SetType(1);
		pAnimation->SetStartFrame(0);
		pAnimation->SetEndFrame(m_lstMatrixFrames.GetNumOfElements()-1);
		pAnimation->SetSpeed(1.0f);
		pAnimation->SetFlags(AnimationInfo::Loop);
	}
}


//[-------------------------------------------------------]
//[ Color animation                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create standard texture animation
*/
void TextureAni::CreateStandardColorAnimation()
{
	if (!m_cColorAnimationManager.GetNumOfElements()) {
		AnimationInfo *pAnimation = static_cast<AnimationInfo*>(m_cColorAnimationManager.Create("Color standard animation"));
		pAnimation->SetType(2);
		pAnimation->SetStartFrame(0);
		pAnimation->SetEndFrame(m_lstColorFrames.GetNumOfElements()-1);
		pAnimation->SetSpeed(1.0f);
		pAnimation->SetFlags(AnimationInfo::Loop);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureAni::TextureAni(TextureManager &cManager, const String &sName) : Texture(cManager, sName)
{
}


//[-------------------------------------------------------]
//[ Public virtual Texture functions                      ]
//[-------------------------------------------------------]
bool TextureAni::IsAnimated() const
{
	// This texture is animated
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool TextureAni::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Skip the texture implementation
	return PLCore::Resource<Texture>::LoadByFilename(sFilename, sParams, sMethod);
}

bool TextureAni::SaveByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Skip the texture implementation
	return PLCore::Resource<Texture>::SaveByFilename(sFilename, sParams, sMethod);
}

bool TextureAni::SaveByFile(File &cFile, const String &sParams, const String &sMethod)
{
	// Skip the texture implementation
	return PLCore::Resource<Texture>::SaveByFile(cFile, sParams, sMethod);
}

bool TextureAni::Unload()
{
	// Cleanup texture animation
	m_cTextureAnimationManager.Clear();
	for (uint32 i=0; i<m_lstTextureFrames.GetNumOfElements(); i++) {
		TextureFrame *pTextureFrame = m_lstTextureFrames[i];
		delete pTextureFrame->pTextureHandler;
		delete pTextureFrame;
	}
	m_lstTextureFrames.Clear();

	// Cleanup matrix animation
	m_cMatrixAnimationManager.Clear();
	for (uint32 i=0; i<m_lstMatrixFrames.GetNumOfElements(); i++)
		delete m_lstMatrixFrames[i];
	m_lstMatrixFrames.Clear();

	// Cleanup color animation
	m_cColorAnimationManager.Clear();
	for (uint32 i=0; i<m_lstColorFrames.GetNumOfElements(); i++)
		delete m_lstColorFrames[i];
	m_lstColorFrames.Clear();

	// Call base implementation
	return Texture::Unload();
}

String TextureAni::GetLoadableTypeName() const
{
	static const String sString = "TextureAni";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
