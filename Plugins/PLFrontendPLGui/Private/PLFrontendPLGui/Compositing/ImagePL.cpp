/*********************************************************\
 *  File: ImagePL.cpp                                    *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageEffects.h>
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Resources/Image.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include "PLFrontendPLGui/Compositing/GuiPL.h"
#include "PLFrontendPLGui/Compositing/ImagePL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLGui;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ImagePL::ImagePL(PLGui::Image &cImage) : ImageImpl(cImage),
	m_pImage(&cImage),
	m_pTextureBuffer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ImagePL::~ImagePL()
{
	// Clean up
	Unload();
}

/**
*  @brief
*    Unload image
*/
void ImagePL::Unload()
{
	// Destroy the renderer texture buffer
	if (m_pTextureBuffer) {
		delete m_pTextureBuffer;
		m_pTextureBuffer = nullptr;
	}
}

/**
*  @brief
*    Returns the texture buffer
*/
PLRenderer::TextureBuffer *ImagePL::GetTextureBuffer()
{
	return m_pTextureBuffer;
}


//[-------------------------------------------------------]
//[ Public virtual PLGui::ImageImpl functions             ]
//[-------------------------------------------------------]
bool ImagePL::Load(const String &sFilename)
{
	// Check parameter
	if (sFilename.GetLength()) {
		// Delete image
		Unload();

		// Get the PixelLight ingame GUI implementation
		GuiPL *pGuiPL = static_cast<GuiPL*>(m_pImage->GetGui()->GetImpl());
		if (pGuiPL) {
			// Get the renderer instance
			PLRenderer::Renderer *pRenderer = pGuiPL->GetRenderer();
			if (pRenderer) {
				// Load the image
				PLGraphics::Image cImage;
				if (cImage.LoadByFilename(sFilename)) {
					// Create rectangle texture buffer
					m_pTextureBuffer = pRenderer->CreateTextureBufferRectangle(cImage);
					if (m_pTextureBuffer) {
						m_sFilename   = sFilename;
						m_nColorDepth = 32;

						// Get width and height
						if (m_pTextureBuffer->GetType() == PLRenderer::Resource::TypeTextureBuffer2D)
							m_vSize = static_cast<const PLRenderer::TextureBuffer2D*>(m_pTextureBuffer)->GetSize();
						else
							m_vSize = static_cast<const PLRenderer::TextureBufferRectangle*>(m_pTextureBuffer)->GetSize();

						// Done
						return true;
					}
				}
			}
		}
	}

	// Error!
	m_sFilename		= "";
	m_vSize.x		= 0;
	m_vSize.y		= 0;
	m_nColorDepth	= 0;
	return false;
}

bool ImagePL::LoadWithColorKey(const String &sFilename, const Color3 &cColor)
{
	// Check parameter
	if (sFilename.GetLength()) {
		// Delete image
		Unload();

		// Get the PixelLight ingame GUI implementation
		GuiPL *pGuiPL = static_cast<GuiPL*>(m_pImage->GetGui()->GetImpl());
		if (pGuiPL) {
			// Get the renderer instance
			PLRenderer::Renderer *pRenderer = pGuiPL->GetRenderer();
			if (pRenderer) {
				// Load the image
				PLGraphics::Image cImage;
				if (cImage.LoadByFilename(sFilename)) {
					// Apply color key
					cImage.ApplyEffect(ImageEffects::ColorKey(cColor, 0.0f));

					// Create rectangle texture buffer
					m_pTextureBuffer = pRenderer->CreateTextureBufferRectangle(cImage);
					if (m_pTextureBuffer) {
						m_sFilename   = sFilename;
						m_nColorDepth = 32;

						// Get width and height
						if (m_pTextureBuffer->GetType() == PLRenderer::Resource::TypeTextureBuffer2D)
							m_vSize = static_cast<const PLRenderer::TextureBuffer2D*>(m_pTextureBuffer)->GetSize();
						else
							m_vSize = static_cast<const PLRenderer::TextureBufferRectangle*>(m_pTextureBuffer)->GetSize();

						// Done
						return true;
					}
				}
			}
		}
	}

	// Error!
	m_sFilename		= "";
	m_vSize.x		= 0;
	m_vSize.y		= 0;
	m_nColorDepth	= 0;
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
