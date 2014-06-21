/*********************************************************\
 *  File: ThemeDesktop.cpp                               *
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
#include <PLGraphics/Color/Color3.h>
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Themes/ThemeDesktop.h"


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(ThemeDesktop)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ThemeDesktop::ThemeDesktop(Gui &cGui) : Theme(cGui, "PixelLight Desktop Theme"),
	m_nControlRound(2),
	m_cColorPanelRaised				(1.0f,  1.0f,  1.0f,  1.0f),	// White
	m_cColorPanelSunken				(0.5f,  0.5f,  0.5f,  1.0f),	// Grey
	m_cColorBorder					(0.25f, 0.25f, 0.25f, 1.0f),	// Dark gray
	m_cColorTitleBar				(0.04f, 0.14f, 0.42f, 1.0f),	// Dark blue
	m_cColorTitleBarInactive		(0.31f, 0.31f, 0.31f, 1.0f),	// Dark gray
	m_cColorControlBack				(0.96f, 0.96f, 0.96f, 1.0f),	// Light  gray (nearly white)
	m_cColorControlBackDisabled		(0.58f, 0.58f, 0.58f, 1.0f),	// Middle gray
	m_cColorControlBackSelect		(0.90f, 0.92f, 0.99f, 1.0f),	// Light  blue
	m_cColorControlBackPressed		(0.3f,  0.5f,  0.8f,  1.0f),	// Middle blue
	m_cColorControlBorder			(0.25f, 0.25f, 0.25f, 1.0f),	// Dark gray
	m_cColorControlBorderSelect		(0.08f, 0.08f, 0.47f, 1.0f),	// Middle blue
	m_cImageArrowLeft	(cGui),
	m_cImageArrowRight	(cGui),
	m_cImageArrowUp		(cGui),
	m_cImageArrowDown	(cGui),
	m_cImageRadioButton(cGui),
	m_cImageRadioButtonChecked(cGui),
	m_cImageCheckBox(cGui),
	m_cImageCheckBoxChecked(cGui),
	m_cImageCheckBoxPartially(cGui),
	m_cImageMinimize(cGui),
	m_cImageMaximize(cGui),
	m_cImageRestore(cGui),
	m_cImageClose(cGui)
{
	// Load default font
	if (!m_cDefaultFont.Load("Dejavu Sans", 14, WeightBold)) {
		if (!m_cDefaultFont.Load("Sans", 14, WeightBold)) {
			if (!m_cDefaultFont.Load("Arial", 14, WeightBold)) {
			}
		}
	}

	// Load default icon
	m_cDefaultIcon.Load("PLGui/pixellight.tga");

	// Load images
	m_cImageArrowLeft.			LoadWithColorKey("PLGui/left.tga",	Color3::White);
	m_cImageArrowRight.			LoadWithColorKey("PLGui/right.tga",	Color3::White);
	m_cImageArrowUp.			LoadWithColorKey("PLGui/up.tga",	Color3::White);
	m_cImageArrowDown.			LoadWithColorKey("PLGui/down.tga",	Color3::White);
	m_cImageRadioButton.		Load("PLGui/radiobutton.tga");
	m_cImageRadioButtonChecked.	Load("PLGui/radiobutton-checked.tga");
	m_cImageCheckBox.			Load("PLGui/checkbox.tga");
	m_cImageCheckBoxChecked.	Load("PLGui/checkbox-checked.tga");
	m_cImageCheckBoxPartially.	Load("PLGui/checkbox-partially.tga");
	m_cImageMinimize.			LoadWithColorKey("PLGui/min.tga", Color3::White);
	m_cImageMaximize.			LoadWithColorKey("PLGui/max.tga", Color3::White);
	m_cImageRestore.			LoadWithColorKey("PLGui/wnd.tga", Color3::White);
	m_cImageClose.				LoadWithColorKey("PLGui/x.tga",   Color3::White);

	// Window
	m_nWindowBorderSize		= 4;
	m_nWindowTitleBarHeight	= 20;
	m_nMenuBarHeight		= 24;
	m_cWindowColor			= Color4::White;

	// Panel
	m_nPanelBorderSize		= 2;
	m_cPanelColor			= Color4(0.83f, 0.82f, 0.78f, 1.0f);

	// SystemButton
	m_vSysButtonSize		= Vector2i(16, 16);
}

/**
*  @brief
*    Destructor
*/
ThemeDesktop::~ThemeDesktop()
{
}


//[-------------------------------------------------------]
//[ Protected virtual Theme functions                     ]
//[-------------------------------------------------------]
void ThemeDesktop::DrawRect(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, ELineStyle nStyle, const Color4 &cColor, int nWidth, int nRound)
{
	// Solid
	if (nStyle == SolidLine) {
		cGraphics.DrawRect(cColor, vPos1, vPos2, nRound, nRound, nWidth);
	}

	// Dashed
	else if (nStyle == DashedLine) {
		// Horizontal
		uint32 nSize = 3 * nWidth;
		uint32 nW = vPos2.x - vPos1.x + 1;
		uint32 nX = vPos1.x;
		uint32 nCount = nW / nSize;
		for (uint32 i=0; i<nCount; i+=2) {
			cGraphics.DrawBox(cColor, Vector2i(nX, vPos1.y), Vector2i(nX+nSize-1, vPos1.y+nWidth-1), 1);
			cGraphics.DrawBox(cColor, Vector2i(nX, vPos2.y-nWidth+1), Vector2i(nX+nSize-1, vPos2.y), 1);
			nX += nSize + nSize;
		}
		cGraphics.DrawBox(cColor, Vector2i(vPos2.x - nSize + 1, vPos1.y), Vector2i(vPos2.x, vPos1.y+nWidth-1), 1);
		cGraphics.DrawBox(cColor, Vector2i(vPos2.x - nSize + 1, vPos2.y-nWidth+1), Vector2i(vPos2.x, vPos2.y), 1);

		// Vertical
		uint32 nH = vPos2.y - vPos1.y + 1;
		uint32 nY = vPos1.y;
		nCount = nH / nSize;
		for (uint32 i=0; i<nCount; i+=2) {
			cGraphics.DrawBox(cColor, Vector2i(vPos1.x, nY), Vector2i(vPos1.x+nWidth-1, nY+nSize-1), 1);
			cGraphics.DrawBox(cColor, Vector2i(vPos2.x-nWidth+1, nY), Vector2i(vPos2.x, nY+nSize-1), 1);
			nY += nSize + nSize;
		}
		cGraphics.DrawBox(cColor, Vector2i(vPos1.x, vPos2.y-nSize+1), Vector2i(vPos1.x+nWidth-1, vPos2.y), 1);
		cGraphics.DrawBox(cColor, Vector2i(vPos2.x-nWidth+1, vPos2.y-nSize+1), Vector2i(vPos2.x, vPos2.y), 1);
	}

	// Dotted
	else if (nStyle == DottedLine) {
		// Horizontal
		uint32 nW = vPos2.x - vPos1.x + 1;
		uint32 nX = vPos1.x;
		uint32 nCount = nW / nWidth;
		for (uint32 i=0; i<nCount; i+=2) {
			cGraphics.DrawBox(cColor, Vector2i(nX, vPos1.y), Vector2i(nX+nWidth-1, vPos1.y+nWidth-1), 0);
			cGraphics.DrawBox(cColor, Vector2i(nX, vPos2.y-nWidth+1), Vector2i(nX+nWidth-1, vPos2.y), 0);
			nX += nWidth + nWidth;
		}
		cGraphics.DrawBox(cColor, Vector2i(vPos2.x - nWidth + 1, vPos1.y), Vector2i(vPos2.x, vPos1.y+nWidth-1), 1);
		cGraphics.DrawBox(cColor, Vector2i(vPos2.x - nWidth + 1, vPos2.y-nWidth+1), Vector2i(vPos2.x, vPos2.y), 1);

		// Vertical
		uint32 nH = vPos2.y - vPos1.y + 1;
		uint32 nY = vPos1.y;
		nCount = nH / nWidth;
		for (uint32 i=0; i<nCount; i+=2) {
			cGraphics.DrawBox(cColor, Vector2i(vPos1.x, nY), Vector2i(vPos1.x+nWidth-1, nY+nWidth-1), 1);
			cGraphics.DrawBox(cColor, Vector2i(vPos2.x-nWidth+1, nY), Vector2i(vPos2.x, nY+nWidth-1), 1);
			nY += nWidth + nWidth;
		}
		cGraphics.DrawBox(cColor, Vector2i(vPos1.x, vPos2.y-nWidth+1), Vector2i(vPos1.x+nWidth-1, vPos2.y), 1);
		cGraphics.DrawBox(cColor, Vector2i(vPos2.x-nWidth+1, vPos2.y-nWidth+1), Vector2i(vPos2.x, vPos2.y), 1);
	}
}

void ThemeDesktop::DrawBorder(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EBorderStyle nBorderStyle)
{
	// Draw any frame at all?
	if (nBorderStyle == RaisedBorder || nBorderStyle == SunkenBorder) {
		// Choose colors
		Color4 &cColorPanel  = (nBorderStyle == RaisedBorder ? m_cPanelColor       : m_cColorPanelSunken);
		Color4 &cColorRaised = (nBorderStyle == RaisedBorder ? m_cColorPanelRaised : m_cColorBorder);
		Color4 &cColorBorder = (nBorderStyle == RaisedBorder ? m_cColorBorder      : m_cColorPanelRaised);
		Color4 &cColorSunken = (nBorderStyle == RaisedBorder ? m_cColorPanelSunken : m_cPanelColor);

		// Outer lines
		cGraphics.DrawLine(cColorPanel,  vPos1, Vector2i(vPos2.x-1, vPos1.y)  );
		cGraphics.DrawLine(cColorPanel,  vPos1, Vector2i(vPos1.x,   vPos2.y-1));
		cGraphics.DrawLine(cColorBorder, vPos2, Vector2i(vPos2.x,   vPos1.y)  );
		cGraphics.DrawLine(cColorBorder, vPos2, Vector2i(vPos1.x,   vPos2.y)  );

		// Border lines
		cGraphics.DrawLine(cColorRaised, vPos1 + Vector2i(1, 1), Vector2i(vPos2.x-1, vPos1.y+1));
		cGraphics.DrawLine(cColorRaised, vPos1 + Vector2i(1, 1), Vector2i(vPos1.x+1, vPos2.y-1));
		cGraphics.DrawLine(cColorSunken, vPos2 - Vector2i(1, 1), Vector2i(vPos2.x-1, vPos1.y+1));
		cGraphics.DrawLine(cColorSunken, vPos2 - Vector2i(1, 1), Vector2i(vPos1.x+1, vPos2.y-1));
	}
}

void ThemeDesktop::DrawFocusRect(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2)
{
	DrawRect(cGraphics, vPos1, vPos2, DottedLine, Color4::Black, 1);
}

void ThemeDesktop::DrawSeparator(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation)
{
	// Get orientation
	if (nOrientation == Horizontal) {
		// Horizontal
		uint32 nOfs = (vPos2.x - vPos1.x) / 20;
		uint32 nX1  = vPos1.x + nOfs;
		uint32 nX2  = vPos2.x - nOfs;
		uint32 nY   = (vPos2.y + vPos1.y) / 2;
		DrawRule(cGraphics, Vector2i(nX1, nY), Vector2i(nX2, nY), Horizontal);
	} else if (nOrientation == Vertical) {
		// Vertical
		uint32 nOfs = (vPos2.y - vPos1.y) / 20;
		uint32 nY1  = vPos1.y + nOfs;
		uint32 nY2  = vPos2.y - nOfs;
		uint32 nX   = (vPos2.x + vPos1.x) / 2;
		DrawRule(cGraphics, Vector2i(nX, nY1), Vector2i(nX, nY2), Vertical);
	}
}

void ThemeDesktop::DrawRule(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation)
{
	// Get orientation
	if (nOrientation == Horizontal) {
		// Horizontal
		cGraphics.DrawLine(m_cColorBorder,		Vector2i(vPos1.x, vPos1.y  ), Vector2i(vPos2.x, vPos1.y  ));
		cGraphics.DrawLine(m_cColorPanelRaised,	Vector2i(vPos1.x, vPos1.y+1), Vector2i(vPos2.x, vPos1.y+1));
	} else if (nOrientation == Vertical) {
		// Vertical
		cGraphics.DrawLine(m_cColorBorder,		Vector2i(vPos1.x,   vPos1.y), Vector2i(vPos1.x  , vPos2.y));
		cGraphics.DrawLine(m_cColorPanelRaised,	Vector2i(vPos1.x+1, vPos1.y), Vector2i(vPos1.x+1, vPos2.y));
	}
}

void ThemeDesktop::DrawPanel(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EBorderStyle nBorderStyle)
{
	// Draw a raised frame
	DrawBorder(cGraphics, vPos1, vPos2, nBorderStyle);
}

void ThemeDesktop::DrawWindow(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, bool bBorder, uint32 nWidgetState, const String &sTitle, const PLGui::Image &cIcon)
{
	// Show border?
	int nBorder = 0;
	if (bBorder) {
		// Draw raised border
		DrawBorder(cGraphics, vPos1, vPos2, RaisedBorder);

		// Draw gap between border and inside
		cGraphics.DrawRect(m_cPanelColor, vPos1 + Vector2i(2, 2), vPos2 - Vector2i(2, 2));
		cGraphics.DrawRect(m_cPanelColor, vPos1 + Vector2i(3, 3), vPos2 - Vector2i(3, 3));

		// Remember border size
		nBorder = m_nWindowBorderSize;
	}

	// Title bar
	Color4 cColor;
	if (nWidgetState & WidgetActive) cColor = m_cColorTitleBar;
	else							 cColor = m_cColorTitleBarInactive;
	cGraphics.DrawBox(cColor, vPos1 + Vector2i(nBorder, nBorder), Vector2i(vPos2.x-nBorder, vPos1.y+nBorder+19));

	// Draw icon
	cGraphics.DrawImage(cIcon.IsEmpty() ? m_cDefaultIcon : cIcon, vPos1 + Vector2i(nBorder+1, nBorder+2), Vector2i(16, 16));

	// Draw title
	cGraphics.DrawText(m_cDefaultFont, Color4::White, Color4::Transparent, vPos1 + Vector2i(nBorder+21, nBorder+3), sTitle);

	// Two lines between title and inner window
	cGraphics.DrawBox(m_cPanelColor, Vector2i(vPos1.x+nBorder, vPos1.y+nBorder+20), Vector2i(vPos2.x-nBorder, vPos1.y+nBorder+21));
}

void ThemeDesktop::DrawSystemButton(Graphics &cGraphics, const Vector2i &vPos, ESystemCommand nSystemCommand, uint32 nWidgetState)
{
	// Draw background
	Color4 cColorBackground;
		 if (!(nWidgetState & WidgetEnabled))	cColorBackground = m_cColorControlBackDisabled;
	else if (nWidgetState & WidgetMouseOver)	cColorBackground = m_cColorControlBackPressed;
	else if (nWidgetState & WidgetActive)		cColorBackground = m_cColorTitleBar;
	else										cColorBackground = m_cColorTitleBarInactive;
	cGraphics.DrawBox(cColorBackground, vPos, vPos + Vector2i(15, 15), 0, 0);

	// Draw bitmap
	Image *pImage = nullptr;
		 if (nSystemCommand == CommandMinimize)	pImage = &m_cImageMinimize;
	else if (nSystemCommand == CommandMaximize)	pImage = &m_cImageMaximize;
	else if (nSystemCommand == CommandRestore)	pImage = &m_cImageRestore;
	else if (nSystemCommand == CommandClose)	pImage = &m_cImageClose;
	cGraphics.DrawImage(*pImage, vPos, Vector2i(14, 14));
}

void ThemeDesktop::DrawButton(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, const String &sTitle, const Image &cImage, uint32 nWidgetState)
{
	// Get button state
	bool bEnabled  = ((nWidgetState & WidgetEnabled)	!= 0);
	bool bSelected = ((nWidgetState & WidgetMouseOver)	!= 0);
	bool bFocus    = ((nWidgetState & WidgetFocus)		!= 0);
	bool bPressed  = ((nWidgetState & WidgetPressed)	!= 0);

	// Select button colors
	Color4 cColorBack		= (bPressed  ? m_cColorControlBackPressed   : (bSelected ? m_cColorControlBackSelect  : (bEnabled ? m_cColorControlBack : m_cColorControlBackDisabled)));
	Color4 cColorBorder		= (bSelected ? m_cColorControlBorderSelect	: (bEnabled  ? m_cColorControlBorder	  : m_cColorControlBorder));
	Color4 cColorHighlight	= (bSelected ? m_cColorControlBackSelect	: (bEnabled  ? m_cColorControlBackPressed : m_cColorControlBorder));
	Color4 cColorText		= Color4::Black;

	// Check if an image is loaded
	Vector2i vImageSize, vImageGap;
	if (cImage.GetFilename().GetLength()) {
		vImageSize = Vector2i(16, 16);
		vImageGap  = Vector2i( 4,  4);
	}

	// Check if a text is to be displayed
	String sCaption = sTitle;
	Vector2i vTextSize(cGraphics.GetTextWidth(m_cDefaultFont, sCaption), cGraphics.GetTextHeight(m_cDefaultFont, sCaption));
	uint32 nWidth = vTextSize.x + vImageSize.x + vImageGap.x;

	// Calculate image and text position
	Vector2i vImagePos = Vector2i((vPos1.x + vPos2.x - nWidth) / 2, (vPos1.y + vPos2.y - vImageSize.y) / 2);
	Vector2i vTextPos = Vector2i(vImagePos.x + vImageSize.x + vImageGap.x, (vPos1.y + vPos2.y - vTextSize.y) / 2);
	if (bPressed) {
		vImagePos += Vector2i(1, 1);
		vTextPos  += Vector2i(1, 1);
	}

	// Draw button
	cGraphics.DrawBox(cColorBack, vPos1, vPos2, m_nControlRound);
	DrawControlBorder(cGraphics, vPos1, vPos2, cColorBorder, cColorHighlight);
	if (cImage.GetFilename().GetLength())
		cGraphics.DrawImage(cImage, vImagePos, vImageSize);
	cGraphics.DrawText(m_cDefaultFont, cColorText, Color4::Transparent, vTextPos, sCaption);

	// Draw a focus frame
	if (bFocus) {
		DrawFocusRect(cGraphics, vPos1 + Vector2i(2, 2), vPos2 - Vector2i(2, 2));
	}
}

void ThemeDesktop::DrawToggleButton(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, const String &sTitle, const Image &cImage, uint32 nWidgetState, ECheckState nChecked)
{
	// Get button state
	bool bEnabled  = ((nWidgetState & WidgetEnabled)	!= 0);
	bool bSelected = ((nWidgetState & WidgetMouseOver)	!= 0);
	bool bFocus    = ((nWidgetState & WidgetFocus)		!= 0);
	bool bPressed  = ((nWidgetState & WidgetPressed)	!= 0);
	bool bChecked  =  (nChecked == Checked);

	// Select button colors
	Color4 cColorBack		= (bChecked  ? m_cColorControlBackPressed	: (bSelected ? m_cColorControlBackSelect  : (bEnabled ? m_cColorControlBack : m_cColorControlBackDisabled)));
	Color4 cColorBorder		= (bSelected ? m_cColorControlBorderSelect	: (bEnabled  ? m_cColorControlBorder	  : m_cColorControlBorder));
	Color4 cColorHighlight	= (bSelected ? m_cColorControlBackSelect	: (bEnabled  ? m_cColorControlBackPressed : m_cColorControlBorder));
	Color4 cColorText		= Color4::Black;

	// Check if an image is loaded
	Vector2i vImageSize, vImageGap;
	if (cImage.GetFilename().GetLength()) {
		vImageSize = Vector2i(16, 16);
		vImageGap  = Vector2i( 4,  4);
	}

	// Check if a text is to be displayed
	String sCaption = sTitle;
	Vector2i vTextSize(cGraphics.GetTextWidth(m_cDefaultFont, sCaption), cGraphics.GetTextHeight(m_cDefaultFont, sCaption));
	uint32 nWidth = vTextSize.x + vImageSize.x + vImageGap.x;

	// Calculate image and text position
	Vector2i vImagePos = Vector2i((vPos1.x + vPos2.x - nWidth) / 2, (vPos1.y + vPos2.y - vImageSize.y) / 2);
	Vector2i vTextPos = Vector2i(vImagePos.x + vImageSize.x + vImageGap.x, (vPos1.y + vPos2.y - vTextSize.y) / 2);
	if (bPressed) {
		vImagePos += Vector2i(1, 1);
		vTextPos  += Vector2i(1, 1);
	}

	// Draw button
	cGraphics.DrawBox(cColorBack, vPos1, vPos2, m_nControlRound);
	DrawControlBorder(cGraphics, vPos1, vPos2, cColorBorder, cColorHighlight);
	if (cImage.GetFilename().GetLength())
		cGraphics.DrawImage(cImage, vImagePos, vImageSize);
	cGraphics.DrawText(m_cDefaultFont, cColorText, Color4::Transparent, vTextPos, sCaption);

	// Draw a focus frame
	if (bFocus) {
		DrawFocusRect(cGraphics, vPos1 + Vector2i(2, 2), vPos2 - Vector2i(2, 2));
	}
}

void ThemeDesktop::DrawRadioButton(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, const String &sTitle, uint32 nWidgetState, ECheckState nChecked)
{
	// Get state
	bool bEnabled  = ((nWidgetState & WidgetEnabled) != 0);
	bool bFocus    = ((nWidgetState & WidgetFocus)	 != 0);

	// Get font
	Font cFont = GetDefaultFont();

	// Get image
	Image cImage(*GetGui());
	if (nChecked == NotChecked)	cImage = m_cImageRadioButton;
	else						cImage = m_cImageRadioButtonChecked;

	// Select colors
	Color4 cColorText = (bEnabled ? Color4::Black : Color4::Gray);

	// Draw radio button
	cGraphics.DrawImage(cImage, vPos1 + Vector2i(4, 4), Vector2i(14, 14));
	cGraphics.DrawText(cFont, cColorText, Color4::Transparent, vPos1 + Vector2i(24, 4), sTitle);

	// Draw a focus frame
	if (bFocus) DrawFocusRect(cGraphics, vPos1 + Vector2i(2, 2), vPos2 - Vector2i(2, 0));
}

void ThemeDesktop::DrawCheckBox(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, const String &sTitle, uint32 nWidgetState, ECheckState nChecked)
{
	// Get state
	bool bEnabled  = ((nWidgetState & WidgetEnabled) != 0);
	bool bFocus    = ((nWidgetState & WidgetFocus)	 != 0);

	// Get font
	Font cFont = GetDefaultFont();

	// Get image
	Image cImage(*GetGui());
		 if (nChecked == Checked)			cImage = m_cImageCheckBoxChecked;
	else if (nChecked == PartiallyChecked)	cImage = m_cImageCheckBoxPartially;
	else									cImage = m_cImageCheckBox;

	// Select colors
	Color4 cColorText = (bEnabled ? Color4::Black : Color4::Gray);

	// Draw check box
	cGraphics.DrawImage(cImage, vPos1 + Vector2i(4, 4), Vector2i(14, 14));
	cGraphics.DrawText(cFont, cColorText, Color4::Transparent, vPos1 + Vector2i(24, 4), sTitle);

	// Draw a focus frame
	if (bFocus) DrawFocusRect(cGraphics, vPos1 + Vector2i(2, 2), vPos2 - Vector2i(2, 0));
}

void ThemeDesktop::DrawTooltip(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nWidgetState)
{
	// Draw a small black border around the tooltip
	cGraphics.DrawRect(Color4::Black, vPos1, vPos2, 0, 0, 1);
}

void ThemeDesktop::DrawSlider(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// [TODO] Draw background only outside the current slider position to avoid flickering

	// Get slider state
	bool bEnabled  = ((nWidgetState & WidgetEnabled)	!= 0);

	// Get orientation
	if (nOrientation == Horizontal)	{
		// Horizontal
		cGraphics.DrawBox(Color4::White, vPos1, Vector2i(vPos2.x, vPos1.y + 4));
		cGraphics.DrawBox(Color4::White, Vector2i(vPos1.x, vPos2.y - 4), vPos2);
		cGraphics.DrawBox(bEnabled ? m_cColorControlBack : m_cColorControlBackDisabled, vPos1 + Vector2i(0, 5), vPos2 - Vector2i(0, 5));
		DrawControlBorder(cGraphics, vPos1 + Vector2i(0, 5), vPos2 - Vector2i(0, 5), m_cColorControlBorder, m_cColorControlBackPressed);
	} else {
		// Vertical
		cGraphics.DrawBox(Color4::White, vPos1, Vector2i(vPos1.x + 4, vPos2.y));
		cGraphics.DrawBox(Color4::White, Vector2i(vPos2.x - 4, vPos1.y), vPos2);
		cGraphics.DrawBox(bEnabled ? m_cColorControlBack : m_cColorControlBackDisabled, vPos1 + Vector2i(5, 0), vPos2 - Vector2i(5, 0));
		DrawControlBorder(cGraphics, vPos1 + Vector2i(5, 0), vPos2 - Vector2i(5, 0), m_cColorControlBorder, m_cColorControlBackPressed);
	}
}

void ThemeDesktop::DrawSliderHandle(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// Get slider state
	bool bEnabled  = ((nWidgetState & WidgetEnabled)	!= 0);
	bool bSelected = ((nWidgetState & WidgetMouseOver)	!= 0);
	bool bPressed  = ((nWidgetState & WidgetPressed)	!= 0);

	// Select slider colors
	Color4 cColorBack		= (bPressed  ? m_cColorControlBackPressed	: (bSelected ? m_cColorControlBackSelect  : (bEnabled ? m_cColorControlBack	: m_cColorControlBackDisabled)));
	Color4 cColorBorder		= (bSelected ? m_cColorControlBorderSelect	: (bEnabled  ? m_cColorControlBorder	  : m_cColorControlBorder));
	Color4 cColorHighlight	= (bSelected ? m_cColorControlBackSelect	: (bEnabled  ? m_cColorControlBackPressed : m_cColorControlBorder));
	Color4 cColorText		= Color4::Black;

	// Draw handle
	if (bEnabled) {
		cGraphics.DrawBox(cColorBack, vPos1, vPos2, m_nControlRound);
		DrawControlBorder(cGraphics, vPos1, vPos2, cColorBorder, cColorHighlight);
	}
}

void ThemeDesktop::DrawScrollBar(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// Draw background
	cGraphics.DrawBox(m_cColorControlBack, vPos1, vPos2);
}

void ThemeDesktop::DrawScrollBarPlusButton(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// Draw button
	DrawButton(cGraphics, vPos1, vPos2, "", Image(*GetGui()), nWidgetState);

	// Draw icon
	if (nOrientation == Horizontal)	cGraphics.DrawImage(m_cImageArrowRight, vPos1 + Vector2i(4, 4), Vector2i(10, 10));
	else							cGraphics.DrawImage(m_cImageArrowDown,  vPos1 + Vector2i(3, 3), Vector2i(10, 10));
}

void ThemeDesktop::DrawScrollBarMinusButton(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// Draw button
	DrawButton(cGraphics, vPos1, vPos2, "", Image(*GetGui()), nWidgetState);

	// Draw icon
	if (nOrientation == Horizontal)	cGraphics.DrawImage(m_cImageArrowLeft, vPos1 + Vector2i(3, 4), Vector2i(10, 10));
	else							cGraphics.DrawImage(m_cImageArrowUp,   vPos1 + Vector2i(3, 3), Vector2i(10, 10));
}

void ThemeDesktop::DrawScrollBarHandle(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// Get slider state
	bool bEnabled  = ((nWidgetState & WidgetEnabled)	!= 0);
	bool bSelected = ((nWidgetState & WidgetMouseOver)	!= 0);
	bool bPressed  = ((nWidgetState & WidgetPressed)	!= 0);

	// Select slider colors
	Color4 cColorBack		= (bPressed  ? m_cColorControlBackPressed	: (bSelected ? m_cColorControlBackSelect  : (bEnabled ? m_cColorControlBack : m_cColorControlBackDisabled)));
	Color4 cColorBorder		= (bSelected ? m_cColorControlBorderSelect	: (bEnabled  ? m_cColorControlBorder	  : m_cColorControlBorder));
	Color4 cColorHighlight	= (bSelected ? m_cColorControlBackSelect	: (bEnabled  ? m_cColorControlBackPressed : m_cColorControlBorder));
	Color4 cColorText		= Color4::Black;

	// Draw handle
	if (bEnabled) {
		cGraphics.DrawBox(cColorBack, vPos1, vPos2, m_nControlRound);
		DrawControlBorder(cGraphics, vPos1, vPos2, cColorBorder, cColorHighlight);
	}
}

void ThemeDesktop::DrawSplitter(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation)
{
	// Choose colors
	Color4 &cColorPanel  = m_cPanelColor;
	Color4 &cColorRaised = m_cColorPanelRaised;
	Color4 &cColorBorder = m_cColorBorder;
	Color4 &cColorSunken = m_cColorPanelSunken;

	// Inside
	if (nOrientation == Vertical) {
		cGraphics.DrawBox(cColorPanel, Vector2i(vPos1.x, vPos1.y+2), Vector2i(vPos2.x, vPos2.y-2));
	} else if (nOrientation == Horizontal) {
		cGraphics.DrawBox(cColorPanel, Vector2i(vPos1.x+2, vPos1.y), Vector2i(vPos2.x-2, vPos2.y));
	}

	// Outer lines
	if (nOrientation == Vertical) {
		cGraphics.DrawLine(cColorPanel,  Vector2i(vPos1.x, vPos1.y), Vector2i(vPos2.x, vPos1.y));
		cGraphics.DrawLine(cColorBorder, Vector2i(vPos2.x, vPos2.y), Vector2i(vPos1.x, vPos2.y));
	} else if (nOrientation == Horizontal) {
		cGraphics.DrawLine(cColorPanel,  Vector2i(vPos1.x, vPos1.y), Vector2i(vPos1.x, vPos2.y));
		cGraphics.DrawLine(cColorBorder, Vector2i(vPos2.x, vPos2.y), Vector2i(vPos2.x, vPos1.y));
	}

	// Border lines
	if (nOrientation == Vertical) {
		cGraphics.DrawLine(cColorRaised, Vector2i(vPos1.x, vPos1.y+1), Vector2i(vPos2.x, vPos1.y+1));
		cGraphics.DrawLine(cColorSunken, Vector2i(vPos2.x, vPos2.y-1), Vector2i(vPos1.x, vPos2.y-1));
	} else if (nOrientation == Horizontal) {
		cGraphics.DrawLine(cColorRaised, Vector2i(vPos1.x+1, vPos1.y), Vector2i(vPos1.x+1, vPos2.y));
		cGraphics.DrawLine(cColorSunken, Vector2i(vPos2.x-1, vPos2.y), Vector2i(vPos2.x-1, vPos1.y));
	}
}

void ThemeDesktop::DrawMenuBar(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// Draw menu background
	cGraphics.DrawBox(m_cColorControlBack, vPos1, vPos2);

	// Draw light and dark line
	if (nOrientation == Horizontal) {
		// Horizontal
		cGraphics.DrawLine(m_cColorPanelRaised,	vPos1,						Vector2i(vPos2.x, vPos1.y));
		cGraphics.DrawLine(m_cColorBorder,		Vector2i(vPos1.x, vPos2.y), vPos2);
	} else {
		// Vertical
		cGraphics.DrawLine(m_cColorPanelRaised, vPos1,						Vector2i(vPos1.x, vPos2.y));
		cGraphics.DrawLine(m_cColorBorder,		Vector2i(vPos2.x, vPos1.y), vPos2);
	}
}

void ThemeDesktop::DrawPopupMenu(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, EOrientation nOrientation, uint32 nWidgetState)
{
	// Get button state
	bool bEnabled  = ((nWidgetState & WidgetEnabled)	!= 0);
	bool bSelected = ((nWidgetState & WidgetMouseOver)	!= 0);

	// Select colors
	Color4 cColorBack		= (bSelected ? m_cColorControlBack			: (bEnabled ? m_cColorControlBack		: m_cColorControlBackDisabled));
	Color4 cColorBorder		= (bSelected ? m_cColorControlBorderSelect	: (bEnabled ? m_cColorControlBorder		: m_cColorControlBorder));
	Color4 cColorHighlight	= (bSelected ? m_cColorControlBackSelect	: (bEnabled ? m_cColorControlBackPressed: m_cColorControlBorder));
	Color4 cColorText		= Color4::Black;

	// Draw menu background
	cGraphics.DrawBox(m_cColorControlBack, vPos1, vPos2);

	// Draw a border
	Color4 cColor = Color4::Black;
	DrawControlBorder(cGraphics, vPos1, vPos2, cColorBorder, cColorHighlight);
}

void ThemeDesktop::DrawMenuItem(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, const String &sTitle, const Image &cImage, bool bDrawArrow, EMenuItemType nType, EOrientation nOrientation, uint32 nWidgetState)
{
	// Get state
	bool bEnabled  = ((nWidgetState & WidgetEnabled)	!= 0);
	bool bSelected = ((nWidgetState & WidgetMouseOver)	!= 0);
	bool bPressed  = ((nWidgetState & WidgetPressed)	!= 0);

	// Select colors
	Color4 cColorBack		= (bSelected ? m_cColorControlBack			: (bEnabled ? m_cColorControlBack		: m_cColorControlBackDisabled));
	Color4 cColorBorder		= (bSelected ? m_cColorControlBorderSelect	: (bEnabled ? m_cColorControlBorder		: m_cColorControlBorder));
	Color4 cColorHighlight	= (bSelected ? m_cColorControlBackSelect	: (bEnabled ? m_cColorControlBackPressed: m_cColorControlBorder));
	Color4 cColorText		= Color4::Black;

	// Get font
	Font cFont = GetDefaultFont();

	// Get text and find the "&" inside it
	String sText = sTitle;
	int nUnderline = sText.IndexOf("&");
	if (nUnderline >= 0) {
		sText.Delete(nUnderline, 1);
	}

	// Separator
	if (nType == TypeSeparator) {
		// Draw separator
		if (nOrientation == Horizontal)
			DrawSeparator(cGraphics, vPos1, Vector2i(vPos1.x+8, vPos2.y+4), Vertical);
		else
			DrawSeparator(cGraphics, vPos1, Vector2i(vPos2.x, vPos1.y+6), Horizontal);
	}

	// Item
	else {
		// Set start position
		Vector2i vPos = vPos1 + Vector2i(4, 2);

		// Draw item
		if (bEnabled && (bPressed || bSelected)) {
			bool bOpen = false;
			if (bPressed && nType == TypeMenu) bOpen = true;
			Color4 cColor = (bOpen ? m_cColorControlBackPressed : m_cColorControlBackSelect);
			cGraphics.DrawBox(cColor, vPos1 + Vector2i(1, 1), vPos2 - Vector2i(2, 2), m_nControlRound, m_nControlRound);
			DrawControlBorder(cGraphics, vPos1 + Vector2i(1, 1), vPos2 - Vector2i(2, 2), cColorBorder, cColorHighlight);
		}

		// Draw icon
		if (!cImage.IsEmpty()) {
			cGraphics.DrawImage(cImage, Vector2i(vPos.x, vPos.y+3), Vector2i(16, 16));
			vPos.x += 20;
		}

		// Draw text
		cGraphics.DrawText(cFont, Color4::Black, Color4::Transparent, Vector2i(vPos.x, vPos.y+4), sText);
		if (nUnderline >= 0) {
			uint32 nX1 = (nUnderline > 0 ? cGraphics.GetTextWidth(cFont, sText.GetSubstring(0, nUnderline)) : 0);
			uint32 nX2 = cGraphics.GetTextWidth(cFont, sText.GetSubstring(0, nUnderline+1));
			uint32 nY  = cGraphics.GetTextHeight(cFont, sText);
			cGraphics.DrawLine(Color4::Black, vPos + Vector2i(nX1, nY+2), vPos + Vector2i(nX2, nY+2));
		}

		// Draw arrow
		bool bHasArrow = (nType == TypeMenu && bDrawArrow);
		if (bHasArrow) {
			cGraphics.DrawImage(m_cImageArrowRight, Vector2i(vPos2.x-12, vPos1.y+7), Vector2i(10, 10));
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
void ThemeDesktop::DrawControlBorder(Graphics &cGraphics, const Vector2i &vPos1, const Vector2i &vPos2, const Color4 &cColorBorder, const Color4 &cColorHighlight)
{
	cGraphics.DrawRect(cColorBorder,	vPos1, vPos2, m_nControlRound, m_nControlRound, 1);
	cGraphics.DrawRect(cColorHighlight, vPos1 + Vector2i(1, 1), vPos2 - Vector2i(1, 1), m_nControlRound, m_nControlRound, 1);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
