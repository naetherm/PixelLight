/*********************************************************\
 *  File: DrawHelpersBackend.cpp                         *
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
#include <PLMath/Rectangle.h>
#include <PLMath/Matrix4x4.h>
#include "PLRenderer/Renderer/Font.h"
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/VertexBuffer.h"
#include "PLRenderer/Renderer/Backend/DrawHelpersBackend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DrawHelpersBackend::~DrawHelpersBackend()
{
	// Destroy the internal temp vertex buffer
	if (m_pTempVertexBuffer)
		delete m_pTempVertexBuffer;
}


//[-------------------------------------------------------]
//[ Public virtual DrawHelpers functions                  ]
//[-------------------------------------------------------]
bool DrawHelpersBackend::Is2DMode() const
{
	return m_b2DMode;
}

bool DrawHelpersBackend::Get2DMode(float &fX1, float &fY1, float &fX2, float &fY2) const
{
	if (!m_b2DMode)
		return false; // Error!

	// Get the virtual screen size
	fX1 = m_fVirtualScreen[0];
	fY1 = m_fVirtualScreen[1];
	fX2 = m_fVirtualScreen[2];
	fY2 = m_fVirtualScreen[3];

	// Done
	return true;
}

float DrawHelpersBackend::Get2DZValue() const
{
	return m_fZValue2D;
}

void DrawHelpersBackend::Set2DZValue(float fZValue)
{
	m_fZValue2D = fZValue;
}

const Matrix4x4 &DrawHelpersBackend::GetObjectSpaceToClipSpaceMatrix() const
{
	return m_mObjectSpaceToClipSpace;
}

void DrawHelpersBackend::DrawText(Font &cFont, const String &sText, const Color4 &cColor, const Vector2 &vPosition, uint32 nFlags, const Vector2 &vScale, const Vector2 &vBias)
{
	// Is there any text to draw?
	if (sText.GetLength()) {

		// [TODO] Rethink the font transform into clip space

		// [TODO] Check previous font size usage - this looks to big?!
		const uint32 nFontHeightInPixels = cFont.GetSize();
//		const uint32 nFontHeightInPixels = cFont.GetHeightInPixels();	// Should be this?!

		// Get the viewport (the real display size)
		const PLMath::Rectangle &cViewport = m_pRenderer->GetViewport();

		// Get the virtual screen size
		float fWidth  = 1.0f;
		float fHeight = 1.0f;
		float fClipSpaceFontWidth = 0.0f;
		float fClipSpaceFontHeight = 0.0f;
		if (Is2DMode()) {
			// [TODO] X/Y offset if virtual screen doesn't start at 0/0?
			fWidth  = m_fVirtualScreen[2] - m_fVirtualScreen[0];
			fHeight = m_fVirtualScreen[3] - m_fVirtualScreen[1];

			// [HACK] 'Font size in pixel' => 'normalized window size' => NOT working!
			if (fWidth == 1.0f)
				fClipSpaceFontWidth = static_cast<float>(nFontHeightInPixels)/1024.0f;
			else
				fClipSpaceFontWidth = static_cast<float>(nFontHeightInPixels)/fWidth;
			if (fHeight == 1.0f)
				fClipSpaceFontHeight = static_cast<float>(nFontHeightInPixels)/768.0f;
			else
				fClipSpaceFontHeight = static_cast<float>(nFontHeightInPixels)/fHeight;
			fClipSpaceFontWidth *= 1.75f;
			fClipSpaceFontHeight *= 1.75f;

		} else {
			// [TODO] X/Y offset if renderer viewport doesn't start at 0/0?
			// Just use the renderer viewport
			fWidth  = cViewport.GetWidth();
			fHeight = cViewport.GetHeight();
			fClipSpaceFontWidth  = static_cast<float>(nFontHeightInPixels)/fWidth;
			fClipSpaceFontHeight = static_cast<float>(nFontHeightInPixels)/fHeight;
			fClipSpaceFontWidth *= 1.75f;
			fClipSpaceFontHeight *= 1.75f;
		}

		// Position
		float fX = vPosition.x;
		float fY = vPosition.y;

		// From this space
			/* 2D draw helper space
					 (0, 0)
						|---
						|
						|	(Width, Height)
			*/
		// Into this space:
			/* Clip space
						| (1, 1)
						|
					 ---|---
						|
			   (-1, -1) |
			*/

		// Get normalized 2D draw helper space position
		fX /= fWidth;
		fY /= fHeight;

		// Normalized 2D draw helper space position into clip space
		fX =   fX*2.0f - 1.0f;
		fY = -(fY*2.0f - 1.0f);

		// Feed the transform matrix with the calculated clip space position
		Matrix4x4 mTransform;
		mTransform.SetTranslationMatrix(fX, fY - fClipSpaceFontHeight, 0.0f);

		// Calculate the bias
		Vector2 vFontBias = vBias;
		if (nFlags & Font::CenterText)
			vFontBias.x -= cFont.GetTextWidth(sText)/2;

		// Draw the text
		cFont.Draw(sText, cColor, mTransform, Vector2(fClipSpaceFontWidth/nFontHeightInPixels, fClipSpaceFontHeight/nFontHeightInPixels)*vScale, vFontBias);
	}
}

void DrawHelpersBackend::DrawText(Font &cFont, const String &sText, const Color4 &cColor, const Vector3 &vPosition, const Matrix4x4 &mObjectSpaceToClipSpace, uint32 nFlags, const Vector2 &vScale, const Vector2 &vBias)
{
	// Is there any text to draw?
	if (sText.GetLength()) {
		// Get text screen position
		const Vector3 v2D = vPosition.To2DCoordinate(mObjectSpaceToClipSpace, m_pRenderer->GetViewport());

		// Draw the text
		DrawText(cFont, sText, cColor, v2D, nFlags, vScale, vBias);
	}
}

float DrawHelpersBackend::GetTextWidth(Font &cFont, const String &sText) const
{
	// Get the width of the screen
	float fWidth;
	if (Is2DMode()) {
		// Get the virtual screen width
		fWidth  = m_fVirtualScreen[2] - m_fVirtualScreen[0];


		// [HACK] 'Font size in pixel' => 'normalized window size' => NOT working!
		if (fWidth == 1.0f)
			fWidth = 1024.0f;


	} else {
		const PLMath::Rectangle &cViewportRect = m_pRenderer->GetViewport();
		fWidth = cViewportRect.vMax.x-cViewportRect.vMin.x;
	}
	if (fWidth) {
		// Get width on a scale from 0.0 to 1.0
		return cFont.GetTextWidth(sText) * (1.0f/fWidth);
	} else {
		// Error!
		return 0.0f;
	}
}

float DrawHelpersBackend::GetTextHeight(Font &cFont) const
{
	// Get the height of the screen
	float fHeight;
	if (Is2DMode()) {
		// Get the virtual screen height
		fHeight = m_fVirtualScreen[3] - m_fVirtualScreen[1];


		// [HACK] 'Font size in pixel' => 'normalized window size' => NOT working!
		if (fHeight == 1.0f)
			fHeight = 768;


	} else {
		const PLMath::Rectangle &cViewportRect = m_pRenderer->GetViewport();
		fHeight = cViewportRect.vMax.y-cViewportRect.vMin.y;
	}
	if (fHeight) {
		// Get height on a scale from 0.0 to 1.0
		// [TODO] Check previous font size usage - this looks to big?!
//		return cFont.GetHeight() * (1.0f/fHeight);
		return cFont.GetSize() * (1.0f/fHeight);
	} else {
		// Error!
		return 0.0f;
	}
}

void DrawHelpersBackend::DrawBox(const Color4 &cColor, const Vector3 &vMin, const Vector3 &vMax, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	if (fWidth) {
		// Left side
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMin.z), Vector3(vMin.x, vMin.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMax.y, vMin.z), Vector3(vMin.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMin.z), Vector3(vMin.x, vMax.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMax.z), Vector3(vMin.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);

		// Right side
		DrawLine(cColor, Vector3(vMax.x, vMin.y, vMin.z), Vector3(vMax.x, vMin.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMax.x, vMax.y, vMin.z), Vector3(vMax.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMax.x, vMin.y, vMin.z), Vector3(vMax.x, vMax.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMax.x, vMin.y, vMax.z), Vector3(vMax.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);

		// Other lines
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMin.z), Vector3(vMax.x, vMin.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMax.z), Vector3(vMax.x, vMin.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMax.y, vMin.z), Vector3(vMax.x, vMax.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMax.y, vMax.z), Vector3(vMax.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
	} else {
		// Left side
		DrawQuad(cColor,
				 Vector3(vMin.x, vMax.y, vMax.z),
				 Vector3(vMin.x, vMax.y, vMin.z),
				 Vector3(vMin.x, vMin.y, vMax.z),
				 Vector3(vMin.x, vMin.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Right side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMin.y, vMax.z),
				 Vector3(vMax.x, vMin.y, vMin.z),
				 Vector3(vMax.x, vMax.y, vMax.z),
				 Vector3(vMax.x, vMax.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Top side
		DrawQuad(cColor,
				 Vector3(vMin.x, vMin.y, vMax.z),
				 Vector3(vMin.x, vMin.y, vMin.z),
				 Vector3(vMax.x, vMin.y, vMax.z),
				 Vector3(vMax.x, vMin.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Bottom side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMax.y, vMax.z),
				 Vector3(vMax.x, vMax.y, vMin.z),
				 Vector3(vMin.x, vMax.y, vMax.z),
				 Vector3(vMin.x, vMax.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Front side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMin.y, vMax.z),
				 Vector3(vMax.x, vMax.y, vMax.z),
				 Vector3(vMin.x, vMin.y, vMax.z),
				 Vector3(vMin.x, vMax.y, vMax.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Back side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMin.y, vMin.z),
				 Vector3(vMin.x, vMin.y, vMin.z),
				 Vector3(vMax.x, vMax.y, vMin.z),
				 Vector3(vMin.x, vMax.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);
	}
}

void DrawHelpersBackend::DrawPlane(const Color4 &cColor, const Vector3 &vN, float fD, const Matrix4x4 &mObjectSpaceToClipSpace, float fLineWidth, float fSize)
{
	Vector3 vRight, vUp, vV = vN*(-fD);
	vN.GetRightUp(vRight, vUp);
	DrawQuad(cColor, vRight*fSize+vV, vUp*fSize+vV, -vUp*fSize+vV, -vRight*fSize+vV, mObjectSpaceToClipSpace, 0.0f);
	if (fLineWidth)
		DrawLine(cColor, Vector3::Zero, vV, mObjectSpaceToClipSpace, fLineWidth);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DrawHelpersBackend::DrawHelpersBackend(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_pTempVertexBuffer(nullptr),
	m_b2DMode(false),
	m_fZValue2D(0.0f)
{
	m_fVirtualScreen[0] = m_fVirtualScreen[1] = m_fVirtualScreen[2] = m_fVirtualScreen[3] = 0.0f;
}

/**
*  @brief
*    Creates the internally used vertex buffer if it's not created yet
*/
bool DrawHelpersBackend::CreateTempBuffes()
{
	// Create vertex buffer
	if (!m_pTempVertexBuffer) {
		m_pTempVertexBuffer = m_pRenderer->CreateVertexBuffer();
		if (m_pTempVertexBuffer) {
			m_pTempVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
			m_pTempVertexBuffer->AddVertexAttribute(VertexBuffer::Color,    0, VertexBuffer::RGBA);
			m_pTempVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
			m_pTempVertexBuffer->Allocate(4, Usage::WriteOnly);
		} else {
			return false; // Error!
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
