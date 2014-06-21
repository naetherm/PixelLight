/*********************************************************\
 *  File: OSWindowMacOSX.mm                              *
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
#import <Cocoa/Cocoa.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/Frontend/Frontend.h>
#include <PLCore/Frontend/FrontendContext.h>
#include "PLFrontendOS/Frontend.h"
#include "PLFrontendOS/OSWindowMacOSX.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendOS {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
OSWindowMacOSX::OSWindowMacOSX(Frontend &cFrontendOS) :
	m_pFrontendOS(&cFrontendOS),
	m_pNSApplication([[NSApplication alloc] init]),
	m_pNSWindow(nullptr),
	m_bInitialized(true)
{
	// Tell the frontend about this instance at once because it may already be required during frontend life cycle initialization
	m_pFrontendOS->m_pOSWindow = this;

	// Create NSWindow instance
	m_pNSWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 640, 480)
									styleMask:(NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask)
									backing:NSBackingStoreBuffered
									defer:NO];

	// Set the window title
	SetTitle(m_pFrontendOS->GetFrontend() ? m_pFrontendOS->GetFrontend()->GetContext().GetName() : m_pFrontendOS->GetFrontend());

	// Do the frontend life cycle thing - start
	m_pFrontendOS->OnStart();

	// Initialization is done
	m_bInitialized = true;

	// If the window is not visible yet, make it visible right now
	MakeVisible();

	// Do the frontend life cycle thing - resume
	m_pFrontendOS->OnResume();
}

/*
*  @brief
*    Destructor
*/
OSWindowMacOSX::~OSWindowMacOSX()
{
	// Do the frontend life cycle thing - pause
	m_pFrontendOS->OnPause();

	// Do the frontend life cycle thing - stop
	m_pFrontendOS->OnStop();

	// Destroy the NSWindow implementation
	[static_cast<NSWindow*>(m_pNSWindow) release];

	// Destroy the NSApplication implementation
	[static_cast<NSApplication*>(m_pNSApplication) release];
}

/**
*  @brief
*    If the window is not visible yet, make it visible right now
*/
void OSWindowMacOSX::MakeVisible()
{
	// Get NSWindow instance
	NSWindow *pNSWindow = static_cast<NSWindow*>(m_pNSWindow);

	// Bring the NSWindow into the foreground
	[pNSWindow setLevel:NSMainMenuWindowLevel];

	// Make the NSWindow visible
	[pNSWindow makeKeyAndOrderFront:nil];
}


//[-------------------------------------------------------]
//[ Private virtual OSWindow functions                    ]
//[-------------------------------------------------------]
handle OSWindowMacOSX::GetNativeWindowHandle() const
{
	return reinterpret_cast<handle>(m_pNSWindow);
}

void OSWindowMacOSX::Redraw()
{
	// Let the frontend draw into it's window
	if (m_bInitialized)
		m_pFrontendOS->OnDraw();
}

bool OSWindowMacOSX::Ping()
{
	// Check if we're allowed to perform an update right now, please note that an update is only allowed when the frontend is fully initialized
	if (m_bInitialized && Timing::GetInstance()->Update()) {
		// Let the frontend update it's states (do this before drawing else, e.g. the first frame may have an unwanted content)
		m_pFrontendOS->OnUpdate();
	}

	// Process all waiting OS events (non-blocking)
	NSEvent *pNSEvent = nil;
	do {
		// Get waiting event
		pNSEvent = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];

		// Process event
		[NSApp sendEvent: pNSEvent];
	}
	while (nil != pNSEvent);

	// Do not shut down the application
	return false;
}

String OSWindowMacOSX::GetTitle() const
{
	// Get the title of the NSWindow as NSString
	// [TODO] Memory: pNSString delete? The NSWindow::title documentation does not mention it
	const NSString *pNSString = [static_cast<NSWindow*>(m_pNSWindow) title];

	// Get a UTF8 representation of the NSWindow title
	// -> Memory: The documentation of NSString::UTF8String states that the UTF8 string the returned pointer is pointing to is automatically freed
	return String::FromUTF8([pNSString UTF8String]);
}

void OSWindowMacOSX::SetTitle(const String &sTitle)
{
	// Get the window title to set as NSString 
	NSString *pNSString = [NSString stringWithCString:sTitle.GetUTF8() encoding:NSUTF8StringEncoding];

	// Set the title of the NSWindow
	[static_cast<NSWindow*>(m_pNSWindow) setTitle: pNSString];

	// Release our NSString instance
	[pNSString release];
}

int OSWindowMacOSX::GetX() const
{
	// Get the x position of the NSWindow content area, meaning excluding the window border and title bar
	return [static_cast<NSWindow*>(m_pNSWindow) frame].origin.x;
}

int OSWindowMacOSX::GetY() const
{
	// Get the y position of the NSWindow content area, meaning excluding the window border and title bar
	return [static_cast<NSWindow*>(m_pNSWindow) frame].origin.y;
}

uint32 OSWindowMacOSX::GetWidth() const
{
	// Get the width of the NSWindow content area, meaning excluding the window border and title bar
	return [[static_cast<NSWindow*>(m_pNSWindow) contentView] frame].size.width;
}

uint32 OSWindowMacOSX::GetHeight() const
{
	// Get the width of the NSWindow content area, meaning excluding the window border and title bar
	return [[static_cast<NSWindow*>(m_pNSWindow) contentView] frame].size.height;
}

void OSWindowMacOSX::SetWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
	const NSRect sNSRect = { static_cast<float>(nX), static_cast<float>(nY), static_cast<float>(nWidth), static_cast<float>(nHeight) };
	[static_cast<NSWindow*>(m_pNSWindow) setFrame:sNSRect display:YES];
}

void OSWindowMacOSX::SetFullscreenAltTab(bool bAllowed)
{
	// [TODO] Implement me
}

void OSWindowMacOSX::SetFullscreen(bool bFullscreen)
{
	// [TODO] Implement me
}

void OSWindowMacOSX::RefreshFullscreen()
{
	// [TODO] Implement me
}

bool OSWindowMacOSX::IsMouseOver() const
{
	// [TODO] Implement me
	return true;
}

int OSWindowMacOSX::GetMousePositionX() const
{
	// Get NSWindow instance
	NSWindow *pNSWindow = static_cast<NSWindow*>(m_pNSWindow);

	// Get the mouse cursor position inside this window
	return [pNSWindow mouseLocationOutsideOfEventStream].x;
}

int OSWindowMacOSX::GetMousePositionY() const
{
	// Get NSWindow instance
	NSWindow *pNSWindow = static_cast<NSWindow*>(m_pNSWindow);

	// Get the mouse cursor position inside this window
	// -> Window origin is left/bottom, so we need to flip the y-axis
	return GetHeight() - [pNSWindow mouseLocationOutsideOfEventStream].y;
}

bool OSWindowMacOSX::IsMouseVisible() const
{
	// [TODO] Implement me
	return true;
}

void OSWindowMacOSX::SetMouseVisible(bool bVisible)
{
	// [TODO] Implement me
}

void OSWindowMacOSX::SetTrapMouse(bool bTrap)
{
	// [TODO] Implement me
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendOS
