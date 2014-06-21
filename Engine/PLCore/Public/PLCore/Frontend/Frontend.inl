/*********************************************************\
 *  File: Frontend.inl                                   *
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
#include "PLCore/Frontend/FrontendImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get frontend context
*/
inline const FrontendContext &Frontend::GetContext() const
{
	return m_cFrontendContext;
}

/**
*  @brief
*    Get frontend implementation
*/
inline FrontendImpl *Frontend::GetImpl() const
{
	// Return implementation
	return m_pFrontendImpl;
}

/**
*  @brief
*    Get native window handle
*/
inline handle Frontend::GetNativeWindowHandle() const
{
	// Call backend
	return m_pFrontendImpl->GetNativeWindowHandle();
}

/**
*  @brief
*    Redraw frontend
*/
inline void Frontend::Redraw()
{
	// Call backend
	m_pFrontendImpl->Redraw();
}

/**
*  @brief
*    Give the frontend a chance to process OS messages
*/
inline void Frontend::Ping() const
{
	// Call backend
	m_pFrontendImpl->Ping();
}

/**
*  @brief
*    Redraw frontend and give the frontend a chance to process OS messages
*/
inline void Frontend::RedrawAndPing()
{
	// Call backend methods
	m_pFrontendImpl->Redraw();
	m_pFrontendImpl->Ping();
}

/**
*  @brief
*    Get frontend title
*/
inline String Frontend::GetTitle() const
{
	// Call backend
	return m_pFrontendImpl->GetTitle();
}

/**
*  @brief
*    Set frontend title
*/
inline void Frontend::SetTitle(const String &sTitle)
{
	// Call backend
	return m_pFrontendImpl->SetTitle(sTitle);
}

/**
*  @brief
*    Get the x position of the frontend (in screen coordinates)
*/
inline int Frontend::GetX() const
{
	// Call backend
	return m_pFrontendImpl->GetX();
}

/**
*  @brief
*    Get the y position of the frontend (in screen coordinates)
*/
inline int Frontend::GetY() const
{
	// Call backend
	return m_pFrontendImpl->GetY();
}

/**
*  @brief
*    Get frontend width
*/
inline uint32 Frontend::GetWidth() const
{
	// Call backend
	return m_pFrontendImpl->GetWidth();
}

/**
*  @brief
*    Get frontend height
*/
inline uint32 Frontend::GetHeight() const
{
	// Call backend
	return m_pFrontendImpl->GetHeight();
}

/**
*  @brief
*    Returns frontend window position and size
*/
inline void Frontend::GetWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const
{
	// Call backend
	m_pFrontendImpl->GetWindowPositionSize(nX, nY, nWidth, nHeight);
}

/**
*  @brief
*    Set frontend window position and size
*/
inline void Frontend::SetWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
	// Call backend
	m_pFrontendImpl->SetWindowPositionSize(nX, nY, nWidth, nHeight);
}

/**
*  @brief
*    Gets whether it's allowed to toggle the fullscreen mode using hotkeys
*/
inline bool Frontend::GetToggleFullscreenMode() const
{
	// Call backend
	return m_pFrontendImpl->GetToggleFullscreenMode();
}

/**
*  @brief
*    Sets whether it's allowed to toggle the fullscreen mode using hotkeys
*/
inline void Frontend::SetToggleFullscreenMode(bool bToggleFullscreenMode)
{
	// Call backend
	m_pFrontendImpl->SetToggleFullscreenMode(bToggleFullscreenMode);
}

/**
*  @brief
*    Gets whether it's allowed to use Alt-Tab if fullscreen mode is used
*/
inline bool Frontend::GetFullscreenAltTab() const
{
	// Call backend
	return m_pFrontendImpl->GetFullscreenAltTab();
}

/**
*  @brief
*    Sets whether it's allowed to use Alt-Tab if fullscreen mode is used
*/
inline void Frontend::SetFullscreenAltTab(bool bAllowed)
{
	// Call backend
	m_pFrontendImpl->SetFullscreenAltTab(bAllowed);
}

/**
*  @brief
*    Returns whether the frontend is in fullscreen mode or not
*/
inline bool Frontend::IsFullscreen() const
{
	// Call backend
	return m_pFrontendImpl->IsFullscreen();
}

/**
*  @brief
*    Sets the frontend's fullscreen mode
*/
inline void Frontend::SetFullscreen(bool bFullscreen)
{
	// Call backend
	m_pFrontendImpl->SetFullscreen(bFullscreen);
}

/**
*  @brief
*    Something related to fullscreen mode has been changed (e.g. the display resolution)
*/
inline void Frontend::RefreshFullscreen()
{
	// Call backend
	m_pFrontendImpl->RefreshFullscreen();
}

/**
*  @brief
*    Check if the mouse is currently over the frontend
*/
inline bool Frontend::IsMouseOver() const
{
	// Call backend
	return m_pFrontendImpl->IsMouseOver();
}

/**
*  @brief
*    Get current mouse cursor X position inside the frontend
*/
inline int Frontend::GetMousePositionX() const
{
	// Call backend
	return m_pFrontendImpl->GetMousePositionX();
}

/**
*  @brief
*    Get current mouse cursor Y position inside the frontend
*/
inline int Frontend::GetMousePositionY() const
{
	// Call backend
	return m_pFrontendImpl->GetMousePositionY();
}

/**
*  @brief
*    Check if the mouse cursor is visible
*/
inline bool Frontend::IsMouseVisible() const
{
	// Call backend
	return m_pFrontendImpl->IsMouseVisible();
}

/**
*  @brief
*    Set mouse cursor visibility
*/
inline void Frontend::SetMouseVisible(bool bVisible)
{
	// Call backend
	m_pFrontendImpl->SetMouseVisible(bVisible);
}

/**
*  @brief
*    Trap mouse inside the frontend
*/
inline void Frontend::SetTrapMouse(bool bTrap)
{
	// Call backend
	m_pFrontendImpl->SetTrapMouse(bTrap);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
