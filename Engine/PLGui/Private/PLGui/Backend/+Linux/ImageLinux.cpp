/*********************************************************\
 *  File: ImageLinux.cpp                                 *
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
#include "PLGui/Widgets/Widget.h"	// We really need to include this before the other one because else the Linux headers
									// will cause compiler errors (stupid stupid OS defines!)
#include <PLGraphics/Color/Color4.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Backend/Linux/ToolsLinux.h"
#include "PLGui/Backend/Linux/GuiLinux.h"
#include "PLGui/Backend/Linux/ImageLinux.h"


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
ImageLinux::ImageLinux(Image &cImage) : ImageImpl(cImage),
	m_pDisplay(static_cast<GuiLinux*>(cImage.GetGui()->GetImpl())->GetDisplay()),
	m_pPixmap(XLib::None),
	m_pMaskPixmap(XLib::None),
	m_pImage(new PLGraphics::Image())
{
}

/**
*  @brief
*    Destructor
*/
ImageLinux::~ImageLinux()
{
	// Free pixmap
	if (m_pPixmap)		XFreePixmap(m_pDisplay, m_pPixmap);
	if (m_pMaskPixmap)	XFreePixmap(m_pDisplay, m_pMaskPixmap);

	// Delete image
	if (m_pImage) delete m_pImage;
}

/**
*  @brief
*    Get pixmap
*/
Pixmap ImageLinux::GetPixmap() const
{
	// Return pixmap
	return m_pPixmap;
}

/**
*  @brief
*    Get mask pixmap
*/
Pixmap ImageLinux::GetMaskPixmap() const
{
	// Return mask pixmap
	return m_pMaskPixmap;
}


//[-------------------------------------------------------]
//[ Public virtual ImageImpl functions                    ]
//[-------------------------------------------------------]
bool ImageLinux::Load(const String &sFilename)
{
	// Load without color key
	return LoadPixmap(sFilename, Color4::Transparent);
}

bool ImageLinux::LoadWithColorKey(const String &sFilename, const Color3 &cColor)
{
	// Load with color key
	return LoadPixmap(sFilename, Color4(cColor));
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Load pixmap
*/
bool ImageLinux::LoadPixmap(const String &sFilename, const PLGraphics::Color4 &cColorKey)
{
	// Free pixmap
	if (m_pPixmap)		XFreePixmap(m_pDisplay, m_pPixmap);
	if (m_pMaskPixmap)	XFreePixmap(m_pDisplay, m_pMaskPixmap);
	m_pPixmap	  = NULL_HANDLE;
	m_pMaskPixmap = NULL_HANDLE;
	m_pImage	  = nullptr;

	// Check file name
	if (sFilename.GetLength()) {
		// Load image
		m_pImage = new PLGraphics::Image();
		if (m_pImage->LoadByFilename(sFilename)) {
			// Convert to pixmap
			if (ToolsLinux::CreatePixmapFromImage(m_pDisplay, *m_pImage, m_pPixmap, m_pMaskPixmap, cColorKey)) {
				// Image loaded
				m_sFilename	  = sFilename;
				m_vSize.x	  = m_pImage->GetBuffer()->GetSize().x;
				m_vSize.y	  = m_pImage->GetBuffer()->GetSize().y;
				m_nColorDepth = m_pImage->GetBuffer()->GetBytesPerPixel() * 8;
				return true;
			}
		}

		// Error, free pixmap
		if (m_pPixmap)		XFreePixmap(m_pDisplay, m_pPixmap);
		if (m_pMaskPixmap)	XFreePixmap(m_pDisplay, m_pMaskPixmap);
		m_pPixmap	  = NULL_HANDLE;
		m_pMaskPixmap = NULL_HANDLE;
		m_pImage	  = nullptr;
	}

	// Error!
	m_sFilename   = "";
	m_vSize.x	  = 0;
	m_vSize.y	  = 0;
	m_nColorDepth = 0;
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
