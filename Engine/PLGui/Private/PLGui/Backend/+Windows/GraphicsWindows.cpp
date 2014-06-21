/*********************************************************\
 *  File: GraphicsWindows.cpp                            *
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
#include <PLGraphics/Color/Color4.h>
#include "PLGui/+Windows/PLGuiWindowsIncludes.h"
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Gui/Resources/Font.h"
#include "PLGui/Backend/+Windows/GraphicsWindows.h"
#include "PLGui/Backend/+Windows/ImageWindows.h"
#include "PLGui/Backend/+Windows/FontWindows.h"
#include <gdiplus.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GraphicsWindows::GraphicsWindows(HDC hDC) : GraphicsImpl(),
	m_hDC(nullptr),
	m_hMemDC(nullptr)
{
	// Create memory DC for drawing bitmaps
	HDC hDesktopDC = GetDC(nullptr);
	m_hMemDC = CreateCompatibleDC(hDesktopDC);
	ReleaseDC(nullptr, hDesktopDC);

	// Save HDC to draw in
	m_hDC = hDC;
}

/**
*  @brief
*    Constructor
*/
GraphicsWindows::GraphicsWindows(Graphics &cGraphics) : GraphicsImpl(cGraphics),
	m_hDC(nullptr),
	m_hMemDC(nullptr)
{
	// Create memory DC for drawing bitmaps
	HDC hDesktopDC = GetDC(nullptr);
	m_hMemDC = CreateCompatibleDC(hDesktopDC);
	ReleaseDC(nullptr, hDesktopDC);

	// Save HDC to draw in
	m_hDC = m_hMemDC;
}

/**
*  @brief
*    Destructor
*/
GraphicsWindows::~GraphicsWindows()
{
	DeleteDC(m_hMemDC);
}


//[-------------------------------------------------------]
//[ Public virtual GraphicsImpl functions                 ]
//[-------------------------------------------------------]
void GraphicsWindows::DrawLine(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nWidth)
{
	if (m_hDC) {
		// Set attributes
		COLORREF hColor	 = RGB(static_cast<int>(cColor.GetR()*255), static_cast<int>(cColor.GetG()*255), static_cast<int>(cColor.GetB()*255));
		HPEN	 hPen	 = CreatePen(PS_SOLID, nWidth, hColor);
		HPEN	 hOldPen = static_cast<HPEN>(SelectObject(m_hDC, hPen));

		// Draw line
		POINT sPoints[2];
		sPoints[0].x = vPos1.x; sPoints[0].y = vPos1.y;
		sPoints[1].x = vPos2.x; sPoints[1].y = vPos2.y;
		Polyline(m_hDC, sPoints, 2);

		// Draw the last pixel
		::SetPixel(m_hDC, vPos2.x, vPos2.y, hColor);

		// Restore attributes
		SelectObject(m_hDC, hOldPen);
		DeleteObject(hPen);
	}
}

void GraphicsWindows::DrawRect(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY, uint32 nWidth)
{
	if (m_hDC) {
		// Set attributes
		HPEN hPen		 = CreatePen(PS_SOLID, nWidth, RGB(static_cast<int>(cColor.GetR()*255),
														   static_cast<int>(cColor.GetG()*255),
														   static_cast<int>(cColor.GetB()*255)));
		HBRUSH hBrush	 = static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
		HPEN hOldPen	 = static_cast<HPEN>(SelectObject(m_hDC, hPen));
		HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(m_hDC, hBrush));

		// Draw box
		if (vPos1 == vPos2) {
			// Unfortunately, I was not able to draw a single pixel with Rectangle or RoundRect. So using this instead..
			POINT sPoints[2];
			sPoints[0].x = vPos1.x; sPoints[0].y = vPos1.y;
			sPoints[1].x = vPos1.x+1; sPoints[1].y = vPos1.y;
			Polyline(m_hDC, sPoints, 2);
		} else {
			// +1, because it seems that Windows excludes the second corner
			RoundRect(m_hDC, vPos1.x, vPos1.y, vPos2.x+1, vPos2.y+1, nRoundX, nRoundY);
		}

		// Restore attributes
		SelectObject(m_hDC, hOldPen);
		DeleteObject(hPen);
		SelectObject(m_hDC, hOldBrush);
	}
}

void GraphicsWindows::DrawBox(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY)
{
	if (m_hDC) {
		// Semi-transparent color?
		if (cColor.GetA() < 1.0f) {
			// Use GDI+ to draw a semi-transparent rect.
			// [TODO] No rounded edges here, sorry
			Gdiplus::Graphics cGraphics(m_hDC);
			Gdiplus::SolidBrush cBrush(Gdiplus::Color(cColor.GetAInt(), cColor.GetRInt(), cColor.GetGInt(), cColor.GetBInt()));
			cGraphics.FillRectangle(&cBrush, vPos1.x, vPos1.y, vPos2.x - vPos1.x + 1, vPos2.y - vPos1.y + 1);
		} else {
			// Set attributes
			COLORREF hColor	 = RGB(static_cast<int>(cColor.GetR()*255), static_cast<int>(cColor.GetG()*255), static_cast<int>(cColor.GetB()*255));
			HPEN hPen		 = CreatePen(PS_SOLID, 0, hColor);
			HPEN hOldPen	 = static_cast<HPEN>(SelectObject(m_hDC, hPen));
			HBRUSH hBrush	 = CreateSolidBrush(hColor);
			HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(m_hDC, hBrush));

			// Draw box (+1, because Windows excludes the second corner)
			if (vPos1 == vPos2)
				// Window does not seem to draw a single pixel with Rectangle(x, y, x+1, y+1)
				::SetPixel(m_hDC, vPos1.x, vPos1.y, hColor);
			else if (nRoundX == 0 && nRoundY == 0)
				// Draw rectangle without rounded edges
				::Rectangle(m_hDC, vPos1.x, vPos1.y, vPos2.x+1, vPos2.y+1);
			else
				// Draw rectangle with rounded edges
				::RoundRect(m_hDC, vPos1.x, vPos1.y, vPos2.x+1, vPos2.y+1, nRoundX, nRoundY);

			// Restore attributes
			SelectObject(m_hDC, hOldPen);
			DeleteObject(hPen);
			SelectObject(m_hDC, hOldBrush);
			DeleteObject(hBrush);
		}
	}
}

void GraphicsWindows::DrawGradientBox(const Color4 &cColor1, const Color4 &cColor2, float fAngle, const Vector2i &vPos1, const Vector2i &vPos2)
{
	Gdiplus::Graphics cGraphics(m_hDC);
	Gdiplus::LinearGradientBrush cBrush(
		Gdiplus::Rect(vPos1.x, vPos1.y, vPos2.x - vPos1.x, vPos2.y - vPos1.y),
		Gdiplus::Color(255, cColor1.GetRInt(), cColor1.GetGInt(), cColor1.GetBInt()),
		Gdiplus::Color(255, cColor2.GetRInt(), cColor2.GetGInt(), cColor2.GetBInt()),
		fAngle, true);
	cGraphics.FillRectangle(&cBrush, vPos1.x, vPos1.y, vPos2.x - vPos1.x + 1, vPos2.y - vPos1.y + 1);
}

void GraphicsWindows::DrawImage(const Image &cImage, const Vector2i &vPos, const Vector2i &vSize)
{
	// Check if image is valid
	if (!cImage.IsEmpty()) {
		// Bind image
		HBITMAP hBitmap = static_cast<ImageWindows*>(cImage.GetImpl())->GetBitmapHandle();
		SelectObject(m_hMemDC, hBitmap);

		// Check for alpha channel
		if (cImage.GetColorDepth() == 32) {
			// Draw transparent
			uint32 nWidth  = vSize.x ? vSize.x : cImage.GetSize().x;
			uint32 nHeight = vSize.y ? vSize.y : cImage.GetSize().y;
			BLENDFUNCTION sBlend;
			sBlend.BlendOp = AC_SRC_OVER;
			sBlend.BlendFlags = 0;
			sBlend.SourceConstantAlpha = 255;
			sBlend.AlphaFormat = AC_SRC_ALPHA;
			AlphaBlend(m_hDC, vPos.x, vPos.y, nWidth, nHeight, m_hMemDC, 0, 0, cImage.GetSize().x, cImage.GetSize().y, sBlend);
		} else {
			// Draw opaque
			int nWidth  = vSize.x ? vSize.x : cImage.GetSize().x;
			int nHeight = vSize.y ? vSize.y : cImage.GetSize().y;
			if (nWidth == cImage.GetSize().x && nHeight == cImage.GetSize().y) {
				BitBlt(m_hDC, vPos.x, vPos.y, cImage.GetSize().x, cImage.GetSize().y, m_hMemDC, 0, 0, SRCCOPY);
			} else {
				SetStretchBltMode(m_hDC, COLORONCOLOR);
				StretchBlt(m_hDC, vPos.x, vPos.y, vSize.x, vSize.y, m_hMemDC, 0, 0, cImage.GetSize().x, cImage.GetSize().y, SRCCOPY);
			}
		}
	}
}

void GraphicsWindows::DrawText(const Font &cFont, const Color4 &cTextColor, const Color4 &cBkColor, const Vector2i &vPos, const String &sText)
{
	if (m_hDC && sText.GetLength()) {
		// Save text attributes
		uint32 nTextAlign = GetTextAlign(m_hDC);
		COLORREF		  sTextColor = GetTextColor(m_hDC);
		COLORREF		  sBackColor = GetBkColor  (m_hDC);
		int				  nBackMode  = GetBkMode   (m_hDC);

		// Set text attributes
		SetTextColor(m_hDC, RGB(static_cast<int>(cTextColor.GetR()*255), static_cast<int>(cTextColor.GetG()*255), static_cast<int>(cTextColor.GetB()*255)));
		if (cBkColor.IsTransparent()) SetBkMode(m_hDC, TRANSPARENT);
		else {
			SetBkMode(m_hDC, OPAQUE);
			SetBkColor(m_hDC, RGB(static_cast<int>(cBkColor.GetR()*255), static_cast<int>(cBkColor.GetG()*255), static_cast<int>(cBkColor.GetB()*255)));
		}

		// Select font
		HFONT hFont = static_cast<FontWindows*>(cFont.GetImpl())->GetHandle();
		if (hFont) SelectObject(m_hDC, hFont);

		// Print text
		if (sText.GetFormat() == String::ASCII)
			TextOutA(m_hDC, vPos.x, vPos.y, sText.GetASCII(), sText.GetLength());
		else
			TextOutW(m_hDC, vPos.x, vPos.y, sText.GetUnicode(), sText.GetLength());

		// Restore text attributes
		SetTextAlign(m_hDC, nTextAlign);
		SetTextColor(m_hDC, sTextColor);
		SetBkColor  (m_hDC, sBackColor);
		SetBkMode   (m_hDC, nBackMode );
	}
}

uint32 GraphicsWindows::GetTextWidth(const Font &cFont, const String &sText)
{
	// Text must not be empty
	if (sText.GetLength()) {
		// Select font
		HFONT hFont = static_cast<FontWindows*>(cFont.GetImpl())->GetHandle();
		if (hFont) SelectObject(m_hDC, hFont);

		// Get text width
		if (sText.GetFormat() == String::ASCII) {
			SIZE sSize;
			if (GetTextExtentPoint32A(m_hDC, sText.GetASCII(), sText.GetLength(), &sSize))
				return sSize.cx;
		} else {
			SIZE sSize;
			if (GetTextExtentPoint32W(m_hDC, sText.GetUnicode(), sText.GetLength(), &sSize))
				return sSize.cx;
		}
	}

	// Error!
	return 0;
}

uint32 GraphicsWindows::GetTextHeight(const Font &cFont, const String &sText)
{
	// Text must not be empty
	if (sText.GetLength()) {
		// Select font
		HFONT hFont = static_cast<FontWindows*>(cFont.GetImpl())->GetHandle();
		if (hFont) SelectObject(m_hDC, hFont);

		// Get text height
		if (sText.GetFormat() == String::ASCII) {
			SIZE sSize;
			if (GetTextExtentPoint32A(m_hDC, sText.GetASCII(), sText.GetLength(), &sSize))
				return sSize.cy;
		} else {
			SIZE sSize;
			if (GetTextExtentPoint32W(m_hDC, sText.GetUnicode(), sText.GetLength(), &sSize))
				return sSize.cy;
		}
	}

	// Error!
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
