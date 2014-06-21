/*********************************************************\
 *  File: ToolsLinux.cpp                                 *
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
#include <PLMath/Vector2i.h>
#include <PLGraphics/Color/Color4.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLGraphics/Image/ImageEffects.h>
#include "PLGui/Backend/Linux/ToolsLinux.h"
#include <X11/Xutil.h>
#include <X11/Xcursor/Xcursor.h>


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Color conversion
*/
// Convert color to 24bit RGB value (8, 8, 8)
#define RGB24(red, green, blue)			( (red<<16) | (green<<8) | (blue) )
// Convert color to 32bit RGBA value (8, 8, 8, 8)
#define RGBA(alpha, red, green, blue)	( (alpha<<24) | (red<<16) | (green<<8) | (blue) )
// Convert color to 16bit RGB value (5, 6, 5)
#define RGB16(red, green, blue)			( ((red>>3) << 11) | ((green>>2) << 5) | (blue>>3) )
// Convert color to 16bit ARGB value (1, 5, 5, 5)
#define RGBA16(alpha, red, green, blue) ( (alpha<<15) | ((red>>3) << 10) | ((green>>2) << 5) | (blue>>3) )

/**
*  @brief
*    MWM hints
*/
typedef struct {
	signed int flags;
	signed int funktions;
	signed int decorations;
	signed int inputMode;
	signed int unknown;
} MWM_Hints;

/**
*  @brief
*    MWM hints definitions
*/
#define MWM_HINTS_DECORATIONS (1L << 1)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Convert color to X color
*/
long ToolsLinux::GetXColor(const Color3 &cColor, int nColorDepth)
{
	// Convert color
		 if (nColorDepth >= 24)	return RGB24(cColor.GetRInt(), cColor.GetGInt(), cColor.GetBInt());
	else if (nColorDepth >= 16)	return RGB16(cColor.GetRInt(), cColor.GetGInt(), cColor.GetBInt());
	else						return 0;
}

/**
*  @brief
*    Set window decoration for a window
*/
bool ToolsLinux::SetNoWindowDecoration(Display *pDisplay, ::Window nNativeWindowHandle, bool bDecorate)
{
	// First try to set MWM hints
	Atom WM_HINTS = XInternAtom(pDisplay, "_MOTIF_WM_HINTS", True);
	if (WM_HINTS != XLib::None) {
		// Set hints
		MWM_Hints MWMHints = { MWM_HINTS_DECORATIONS, 0, 0, 0, 0 };
		if (bDecorate)	MWMHints.decorations = 1;
		else			MWMHints.decorations = 0;

		// Set hints property
		XChangeProperty(
			pDisplay,
			nNativeWindowHandle,
			WM_HINTS,
			WM_HINTS,
			32,
			PropModeReplace,
			reinterpret_cast<unsigned char*>(&MWMHints),
			sizeof(MWMHints) / 4
		);

		// Done
		return true;
	}

	// Now try to set KWM hints
	WM_HINTS = XInternAtom(pDisplay, "KWM_WIN_DECORATION", True);
	if (WM_HINTS != XLib::None) {
		// Set hints
		long KWMHints;
		if (bDecorate)	KWMHints = 1;
		else			KWMHints = 0;

		// Set hints property
		XChangeProperty(
			pDisplay,
			nNativeWindowHandle,
			WM_HINTS,
			WM_HINTS,
			32,
			PropModeReplace,
			reinterpret_cast<unsigned char*>(&KWMHints),
			sizeof(KWMHints) / 4
		);

		// Done
		return true;
	}

	// Now try to set GNOME hints
	WM_HINTS = XInternAtom(pDisplay, "_WIN_HINTS", True);
	if (WM_HINTS != XLib::None) {
		// Set hints
		long GNOMEHints;
		if (bDecorate)	GNOMEHints = 1;
		else			GNOMEHints = 0;

		// Set hints property
		XChangeProperty(
			pDisplay,
			nNativeWindowHandle,
			WM_HINTS,
			WM_HINTS,
			32,
			PropModeReplace,
			reinterpret_cast<unsigned char*>(&GNOMEHints),
			sizeof(GNOMEHints) / 4
		);

		// Done
		return true;
	}

	// Error, Could not set decoration hints
	return false;
}

/**
*  @brief
*    Create X pixmap from image
*/
bool ToolsLinux::CreatePixmapFromImage(Display *pDisplay, Image &cImage, Pixmap &pPixmap, Pixmap &pMaskPixmap, const Color4 &cColorKey)
{
	if (pDisplay) {
		// Convert image buffer to 32bit (RGBA)
		Image cImageRGBA = cImage;
		cImageRGBA.ApplyEffect(ImageEffects::Convert(DataByte, ColorRGBA));
		if (!cColorKey.IsTransparent())
			cImageRGBA.ApplyEffect(ImageEffects::ColorKey(Color3(cColorKey), 0.001f));

		// Get image data
		ImageBuffer &cImageBuffer = *cImageRGBA.GetBuffer();
		uint32  nWidth  = cImageBuffer.GetSize().x;
		uint32  nHeight = cImageBuffer.GetSize().y;
		uint8  *pData   = cImageBuffer.GetData();

		// Get output color depth
		int nScreen = DefaultScreen(pDisplay);
		int nDepth  = DefaultDepth(pDisplay, nScreen);

		// Convert image data to output color depth
		u_int32_t *pBuffer = new u_int32_t[nWidth*nHeight*4];
		for (int i=0; i<nWidth*nHeight; i++) {
			pBuffer[i] = GetXColor(Color3(static_cast<uint8>(pData[i*4]), static_cast<uint8>(pData[i*4+1]), static_cast<uint8>(pData[i*4+2])), nDepth);
		}

		// Create X image
		XImage *pXImage = XCreateImage(
							pDisplay,
							DefaultVisual(pDisplay, nScreen),
							nDepth,
							ZPixmap,
							0,
							nullptr,
							nWidth,
							nHeight,
							(nDepth > 16) ? 32 : (nDepth > 8) ? 16 : 8,
							0
						  );
		pXImage->data = reinterpret_cast<char*>(pBuffer);
		XInitImage(pXImage);

		// Create pixmap
		pPixmap = XCreatePixmap(pDisplay, XDefaultRootWindow(pDisplay), nWidth, nHeight, nDepth);
		GC sGC  = XCreateGC(pDisplay, pPixmap, 0, nullptr);
		XPutImage(pDisplay, pPixmap, sGC, pXImage, 0, 0, 0, 0, nWidth, nHeight);
		XFreeGC(pDisplay, sGC);

		// Destroy image
		XDestroyImage(pXImage);

		// Create mask image
		XImage *pXMaskImage = XCreateImage(
								pDisplay,
								DefaultVisual(pDisplay, nScreen),
								1,
								ZPixmap,
								0,
								nullptr,
								nWidth,
								nHeight,
								8,
								0
							);
		pXMaskImage->data = new char[pXMaskImage->bytes_per_line * nHeight];

		// Set transparent pixels: all pixels that have an alpha value > 0 are drawn, all other are transparent
		for (int y=0; y<nHeight; y++)
			for (int x=0; x<nWidth; x++)
				XPutPixel(pXMaskImage, x, y, pData[(y*nWidth+x)*4+3] > 0);

		// Create mask pixmap
		pMaskPixmap = XCreatePixmap(pDisplay, XDefaultRootWindow(pDisplay), nWidth, nHeight, 1);
		sGC = XCreateGC(pDisplay, pMaskPixmap, 0, nullptr);
		XPutImage(pDisplay, pMaskPixmap, sGC, pXMaskImage, 0, 0, 0, 0, nWidth, nHeight);
		XFreeGC(pDisplay, sGC);

		// Destroy mask image
		XDestroyImage(pXMaskImage);

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Create X cursor from image
*/
bool ToolsLinux::CreateCursorFromImage(Display *pDisplay, Image &cImage, const Vector2i &vHotspot, ::Cursor &pCursor)
{
	if (pDisplay) {
		// Convert image buffer to 32bit (RGBA)
		Image cImageRGBA = cImage;
		cImageRGBA.ApplyEffect(ImageEffects::Convert(DataByte, ColorRGBA));

		// Get image data
		ImageBuffer &cImageBuffer = *cImageRGBA.GetBuffer();
		uint32  nWidth  = cImageBuffer.GetSize().x;
		uint32  nHeight = cImageBuffer.GetSize().y;
		uint8  *pData   = cImageBuffer.GetData();

		// Create Xcursor image
		XcursorImage *image = XcursorImageCreate(nWidth, nHeight);

		// Copy image data to cursor image
		for (int i=0; i<nWidth*nHeight; i++) {
			unsigned char r = pData[i*4+0];
			unsigned char g = pData[i*4+1];
			unsigned char b = pData[i*4+2];
			unsigned char a = pData[i*4+3];
			image->pixels[i] = (a<<24) | (r<<16) | (g<<8) | (b);
		}

		// Set cursor hotspot
		image->xhot = vHotspot.x;
		image->yhot = vHotspot.y;

		// Create cursor
		pCursor = XcursorImageLoadCursor(pDisplay, image);

		// Release image
		XcursorImageDestroy(image);
	}

	// Error!
	return false;
}

/**
*  @brief
*    Create empty X cursor
*/
::Cursor ToolsLinux::CreateEmptyCursor(Display *pDisplay)
{
	// Create pixmap
	Pixmap sPixmap = XCreatePixmap(pDisplay, XDefaultRootWindow(pDisplay), 1, 1, 1);

	// Draw empty rect into pixmap
	XGCValues sGCValues;
	sGCValues.function = GXclear;
	GC sGC = XCreateGC(pDisplay, sPixmap, GCFunction, &sGCValues);
	XFillRectangle(pDisplay, sPixmap, sGC, 0, 0, 1, 1);

	// Create empty cursor
	XColor sColor;
	sColor.pixel = 0;
	sColor.red   = 0;
	sColor.green = 0;
	sColor.blue  = 0;
	sColor.flags = 0;
	Cursor pCursor = XCreatePixmapCursor(pDisplay, sPixmap, sPixmap, &sColor, &sColor, 0, 0);

	// Release graphics
	XFreePixmap(pDisplay, sPixmap);
	XFreeGC(pDisplay, sGC);

	// Done
	return pCursor;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
