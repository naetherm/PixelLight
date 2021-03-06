/*********************************************************\
 *  File: Frontend.cpp                                   *
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
#include <QtGui/qapplication.h>
#include <QtGui/qdesktopwidget.h>
#include <PLCore/String/UTF8Tools.h>
#include "PLFrontendQt/FrontendMainWindow.h"
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/Frontend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Frontend, "PLFrontendQt", PLCore::FrontendImpl, "Qt frontend implementation class")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(Frontend)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Frontend::Frontend() :
	m_pMainWindow(nullptr),
	m_bToggleFullscreenMode(true),
	m_bFullscreenAltTab(true),
	m_bIsFullscreen(false),
	m_pQCursorBlank(nullptr),
	m_bMouseVisible(true)
{
}

/**
*  @brief
*    Destructor
*/
Frontend::~Frontend()
{
}

/**
*  @brief
*    Get main window
*/
FrontendMainWindow *Frontend::GetMainWindow() const
{
	// Return pointer to main window
	return m_pMainWindow;
}

/**
*  @brief
*    Set main window
*/
void Frontend::SetMainWindow(FrontendMainWindow *pMainWindow)
{
	// Set pointer to main window
	m_pMainWindow = pMainWindow;
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::FrontendImpl functions      ]
//[-------------------------------------------------------]
int Frontend::Run(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// Get traditional C-arguments
	int argc = 1 + lstArguments.GetNumOfElements();
	char **argv = new char *[argc];
	{ // Fill C-arguments
		{ // First one is the executable filename
			const char *pszExecutableFilename = sExecutableFilename.GetUTF8();
			const uint32 nNumOfExecutableFilenameBytes = UTF8Tools::GetNumOfStringBytes(pszExecutableFilename);
			argv[0] = new char[nNumOfExecutableFilenameBytes + 1];	// +1 for the terminating zero
			MemoryManager::Copy(argv[0], pszExecutableFilename, nNumOfExecutableFilenameBytes);
			argv[0][nNumOfExecutableFilenameBytes] = '\0';	// The terminating zero
		}

		// ... and now the arguments...
		for (uint32 i=0; i<lstArguments.GetNumOfElements(); i++) {
			const char *pszArgument = lstArguments[i].GetUTF8();
			const uint32 nNumOfArgumentBytes = UTF8Tools::GetNumOfStringBytes(pszArgument);
			argv[i + 1] = new char[nNumOfArgumentBytes + 1];	// +1 for the terminating zero
			MemoryManager::Copy(argv[i + 1], pszArgument, nNumOfArgumentBytes);
			argv[i + 1][nNumOfArgumentBytes] = '\0';	// The terminating zero
		}
	}

	// Create the Qt application instance on the C runtime stack
	QApplication cQApplication(argc, argv);

	// Set application icon
	cQApplication.setWindowIcon(QIcon(":/pl_icon.png"));

	// Create the mouse cursors
	m_pQCursorBlank = new QCursor(Qt::BlankCursor);

	// Create and set the main window
	FrontendMainWindow *pFrontendMainWindow = new FrontendMainWindow(*this);

	// Run the Qt application
	const int nResult = cQApplication.exec();

	// Destroy the main window
	delete pFrontendMainWindow;

	// Delete the mouse cursors
	delete m_pQCursorBlank;
	m_pQCursorBlank = nullptr;

	// Cleanup traditional C-arguments
	for (int i=0; i<argc; i++)
		delete argv[i];
	delete [] argv;

	// Done
	return nResult;
}

handle Frontend::GetNativeWindowHandle() const
{
	if (m_pMainWindow) {
		// Get the render widget
		QWidget *pCentralWidget = m_pMainWindow->m_pRenderWidget;
		// Get window system identifier of the widget
		if (pCentralWidget)
			return (handle)(pCentralWidget->winId());
		else
			return (handle)(m_pMainWindow->window() ? m_pMainWindow->window()->winId() : m_pMainWindow->winId());
	} else {
		// There's no native window handle
		return NULL_HANDLE;
	}
}

void Frontend::Redraw()
{
	// Ask Qt politely to update (and repaint) the widget
	if (m_pMainWindow) {
		// If the widget is not visible yet, make it visible right now
		m_pMainWindow->MakeVisible();

		// Update
		m_pMainWindow->update();
	}
}

void Frontend::Ping()
{
	// Check if there are system messages waiting (non-blocking)
	if (QApplication::instance()->hasPendingEvents()) {
		// Process all waiting messages
		QApplication::instance()->processEvents();
	}
}

String Frontend::GetTitle() const
{
	// Query the main window
	return m_pMainWindow ? QtStringAdapter::QtToPL(m_pMainWindow->windowTitle()) : "";
}

void Frontend::SetTitle(const String &sTitle)
{
	// Query the main window
	if (m_pMainWindow)
		m_pMainWindow->setWindowTitle(QtStringAdapter::PLToQt(sTitle));
}

int Frontend::GetX() const
{
	// Query the central widget of the Qt main window
	QWidget *qQWidget = GetCentralWidget();
	return qQWidget ? qQWidget->pos().x() : 0;
}

int Frontend::GetY() const
{
	// Query the central widget of the Qt main window
	QWidget *qQWidget = GetCentralWidget();
	return qQWidget ? qQWidget->pos().y() : 0;
}

uint32 Frontend::GetWidth() const
{
	// Query the central widget of the Qt main window
	QWidget *qQWidget = GetCentralWidget();
	return qQWidget ? qQWidget->size().width() : 0;
}

uint32 Frontend::GetHeight() const
{
	// Query the central widget of the Qt main window
	QWidget *qQWidget = GetCentralWidget();
	return qQWidget ? qQWidget->size().height() : 0;
}

void Frontend::GetWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const
{
	if (m_pMainWindow) {
		nX      = m_pMainWindow->pos().x();
		nY      = m_pMainWindow->pos().y();
		nWidth  = m_pMainWindow->size().width();
		nHeight = m_pMainWindow->size().height();
	} else {
		// Set known default values
		nX = nY = nWidth = nHeight = 0;
	}
}

void Frontend::SetWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
	if (m_pMainWindow) {
		// Correct frontend position and size settings
		if (QApplication::desktop())
			CorrectPositionSize(nX, nY, nWidth, nHeight, QApplication::desktop()->pos().x(), QApplication::desktop()->pos().y(), QApplication::desktop()->width(), QApplication::desktop()->height());

		// Set position and size settings
		m_pMainWindow->move(QPoint(nX, nY));
		m_pMainWindow->resize(QSize(nWidth, nHeight));

		// If the widget is not visible yet, make it visible right now
		m_pMainWindow->MakeVisible();
	}
}

bool Frontend::GetToggleFullscreenMode() const
{
	return m_bToggleFullscreenMode;
}

void Frontend::SetToggleFullscreenMode(bool bToggleFullscreenMode)
{
	m_bToggleFullscreenMode = bToggleFullscreenMode;
}

bool Frontend::GetFullscreenAltTab() const
{
	return m_bFullscreenAltTab;
}

void Frontend::SetFullscreenAltTab(bool bAllowed)
{
	// Is there a state change?
	if (m_bFullscreenAltTab != bAllowed) {
		// Set the new state
		m_bFullscreenAltTab = bAllowed;

		// [TODO] Implement me
	}
}

bool Frontend::IsFullscreen() const
{
	return m_bIsFullscreen;
}

void Frontend::SetFullscreen(bool bFullscreen)
{
	// Is there a state change?
	if (m_bIsFullscreen != bFullscreen) {
		// Set the new state
		m_bIsFullscreen = bFullscreen;

		// [TODO] Enable/disable Alt-Tab

		// Inform that the fullscreen mode was changed (in here, usually e.g. the display resolution is updated and so on)
		OnFullscreenMode();

		// Set widget into fullscreen state?
		if (m_pMainWindow) {
			// ... just let Qt do the rest for us...
			if (bFullscreen)
				m_pMainWindow->showFullScreen();
			else
				m_pMainWindow->showNormal();
		}
	}
}

void Frontend::RefreshFullscreen()
{
	// This information is only interesting if we're currently in fullscreen mode, if not, just ignore this method call
	if (m_bIsFullscreen && m_pMainWindow) {
		// Leave fullscreen
		m_pMainWindow->showNormal();

		// Inform that the fullscreen mode was changed (in here, usually e.g. the display resolution is updated and so on)
		OnFullscreenMode();

		// Reenter fullscreen
		m_pMainWindow->showFullScreen();
	}
}

bool Frontend::IsMouseOver() const
{
	// Query the central widget of the Qt main window
	QWidget *qQWidget = GetCentralWidget();
	return qQWidget ? qQWidget->underMouse() : false;
}

int Frontend::GetMousePositionX() const
{
	// Query the central widget of the Qt main window
	QWidget *qQWidget = GetCentralWidget();
	return (qQWidget && qQWidget->underMouse()) ? qQWidget->mapFromGlobal(QCursor::pos()).x() : -1;
}

int Frontend::GetMousePositionY() const
{
	// Query the central widget of the Qt main window
	QWidget *qQWidget = GetCentralWidget();
	return (qQWidget && qQWidget->underMouse()) ? qQWidget->mapFromGlobal(QCursor::pos()).y() : -1;
}

bool Frontend::IsMouseVisible() const
{
	return m_bMouseVisible;
}

void Frontend::SetMouseVisible(bool bVisible)
{
	// Check whether or not the mouse cursor is there - and only do something if the state should be changed
	if (m_pQCursorBlank && bVisible != m_bMouseVisible) {
		// Backup the state
		m_bMouseVisible = bVisible;
		if (bVisible) {
			// This pops the last set cursor from the internal stack and restores the previous one.
			// If no other cursor was on the stack then the default widget cursor is used.
			// (see http://doc.qt.nokia.com/latest/qapplication.html#restoreOverrideCursor)
			QApplication::restoreOverrideCursor();
		} else {
			// This pushes the cursor on an internal stack (see http://doc.qt.nokia.com/latest/qapplication.html#setOverrideCursor)
			QApplication::setOverrideCursor(*m_pQCursorBlank);
		}
	}
}

void Frontend::SetTrapMouse(bool bTrap)
{
	// [TODO] Implement me
}


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the central widget of the Qt main window
*/
QWidget *Frontend::GetCentralWidget() const
{
	QWidget *pQWidget = nullptr;

	// Is there a Qt main window?
	if (m_pMainWindow) {
		// Get the render window, if there's one
		pQWidget = m_pMainWindow->m_pRenderWidget;

		// In case there's no central widget, use the Qt main window instead
		// so that we can return at least "something" which may do the job as well
		if (!pQWidget)
			pQWidget = m_pMainWindow;
	}

	// Done
	return pQWidget;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
