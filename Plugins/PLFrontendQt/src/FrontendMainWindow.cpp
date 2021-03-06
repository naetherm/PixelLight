/*********************************************************\
 *  File: FrontendMainWindow.cpp                         *
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
#include <QtCore/qurl.h>
#include <QtCore/qcoreevent.h>
#include <QtGui/qevent.h>
#include <QtGui/qboxlayout.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/Frontend/Frontend.h>
#include <PLCore/Frontend/FrontendContext.h>
#include "PLFrontendQt/Frontend.h"
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/FrontendRenderWindow.h"
#include "PLFrontendQt/DockWidget/DockWidgetManager.h"
#include "PLFrontendQt/FrontendMainWindow.h"
#if defined(Q_WS_WIN)
	#include <PLCore/PLCoreWindowsIncludes.h>
	#include <QtGui/qwindowdefs_win.h>	// For QWidget::winEvent() usage
#elif defined(Q_WS_X11)
	#include <X11/Xlib.h> // For the X11 event type definitions
	// Undefine some identifiers, their names are also used by other headers
	#undef Bool
	#undef None
	// Put the X11 defines for FocusIn/Out event type into a enum and undefine the defines because the names are also used as names for enum values by Qt QEvent
	enum X11 {
		FocusInX11 = FocusIn,
		FocusOutX11 = FocusOut
	};
	#undef FocusIn
	#undef FocusOut
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the dock widget manager of this main window
*/
DockWidgetManager &FrontendMainWindow::GetDockWidgetManager()
{
	// If required, create the dock widget manager of this main window right now
	if (!m_pDockWidgetManager)
		m_pDockWidgetManager = new DockWidgetManager(this);

	// Return the dock widget manager of this main window
	return *m_pDockWidgetManager;
}

/**
*  @brief
*    Sets the update interval
*/
void FrontendMainWindow::SetUpdateInterval(int nUpdateInterval)
{
	// State change?
	if (m_nUpdateInterval != nUpdateInterval) {
		// Stop the previous timer (if there's one)
		if (m_nUpdateTimerID) {
			killTimer(m_nUpdateTimerID);
			m_nUpdateTimerID = 0;
		}

		// Backup the given update interval
		m_nUpdateInterval = nUpdateInterval;

		// Start timer with new interval? (only if the frontend is already initialized)
		if (m_bInitialized && m_nUpdateInterval)
			m_nUpdateTimerID = startTimer(m_nUpdateInterval);
	}
}

/**
*  @brief
*    Gets the render window which is used by this main window
*/
QWidget *FrontendMainWindow::GetRenderWindow() const
{
	return m_pRenderWidget;
}


//[-------------------------------------------------------]
//[ Public virtual QObject methods                        ]
//[-------------------------------------------------------]
bool FrontendMainWindow::eventFilter(QObject *pQObject, QEvent *pQEvent)
{
	if (pQObject == m_pRenderWidget) {
		switch (pQEvent->type()) {
			case QEvent::FocusIn:
				// Do the frontend life cycle thing - resume
				m_pFrontendQt->OnResume();

				// We do not filter out the event because we add only additional handling
				break;

			case QEvent::FocusOut:
				// Do the frontend life cycle thing - pause
				m_pFrontendQt->OnPause();

				// We do not filter out the event because we add only additional handling
				break;
		}
	}

	// Pass the event on to the parent class
	return QObject::eventFilter(pQObject, pQEvent);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendMainWindow::FrontendMainWindow(Frontend &cFrontendQt) :
	m_pFrontendQt(&cFrontendQt),
	m_bVisible(false),
	m_nUpdateInterval(10),
	m_nUpdateTimerID(0),
	m_pDockWidgetManager(nullptr),
	m_bInitialized(false),
	m_pRenderWidget(nullptr)
{
	// Tell the frontend about this instance at once because it may already be required during frontend life cycle initialization
	m_pFrontendQt->SetMainWindow(this);

	{ // Create a dump QWidget as the central widget for the QMainWindow and add the FrontendRenderWindow as a child of this widget.
		// -> This solves, in conjunction with the change that a DockWidget can receive input focus, the "problem" that the "render window"
		//    processes mouse move input event while a user drags a QDockWidget.
		// -> Note: The central widget of a QMainWindow receives the input focus when a docked QDockWidget gets dragged by the user
		QWidget *pQCentralWidget = new QWidget(this);
		QVBoxLayout *pQVBoxLayout = new QVBoxLayout;
		pQVBoxLayout->setContentsMargins(0, 0, 0, 0);	// No visible border, please
		pQCentralWidget->setLayout(pQVBoxLayout);
		m_pRenderWidget = new FrontendRenderWindow(this);

		// Set focus policy to "Qt::ClickFocus". Normally a QWidget doesn't get input focus via mouse click on the widget itself.
		// With this policy the widget gets the input focus when the user performs a mouse click while the cursor is inside the widget area.
		m_pRenderWidget->setFocusPolicy(Qt::ClickFocus);
		pQCentralWidget->layout()->addWidget(m_pRenderWidget);

		// Install an event filter onto the render window.
		// -> The event filter will do the frontend pause/resume cycle when the render window gets/loose the input focus
		m_pRenderWidget->installEventFilter(this);

		// Set central widget
		setCentralWidget(pQCentralWidget);
	}

	// Set window title and size
	setWindowTitle(m_pFrontendQt->GetFrontend() ? QtStringAdapter::PLToQt(m_pFrontendQt->GetFrontend()->GetContext().GetName()) : "");
	resize(640, 480);

	// Drop events are enabled for this widget
	setAcceptDrops(true);

	// Do the frontend life cycle thing - start
	m_pFrontendQt->OnStart();
	m_bInitialized = true;

	// If the widget is not visible yet, make it visible right now
	MakeVisible();

	// Ready to rumble, start the update-timer
	m_nUpdateTimerID = startTimer(m_nUpdateInterval);
}

/**
*  @brief
*    Destructor
*/
FrontendMainWindow::~FrontendMainWindow()
{
	// Stop window redraw timer before entering frontend deinitialization phase
	if (m_nUpdateTimerID) {
		killTimer(m_nUpdateTimerID);
		m_nUpdateTimerID = 0;
	}

	// Do the frontend life cycle thing - stop
	m_pFrontendQt->OnStop();

	// Destroy the dock widget manager of this main window, if there's one
	if (m_pDockWidgetManager)
		delete m_pDockWidgetManager;
}

/**
*  @brief
*    If the widget is not visible yet, make it visible right now
*/
void FrontendMainWindow::MakeVisible()
{
	if (!m_bVisible) {
		// The widget is now considered to be visible
		m_bVisible = true;

		// Show the widget
		show();

		// Activate the window by giving it the focus
		m_pRenderWidget->setFocus(Qt::ActiveWindowFocusReason);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual QObject functions                   ]
//[-------------------------------------------------------]
void FrontendMainWindow::timerEvent(QTimerEvent *pQTimerEvent)
{
	if (pQTimerEvent->timerId() == m_nUpdateTimerID) {
		// Is the frontend still running?
		if (m_pFrontendQt->GetFrontend() && m_pFrontendQt->GetFrontend()->IsRunning()) {
			// Check if we're allowed to perform an update right now
			if (PLCore::Timing::GetInstance()->Update()) {
				// Let the frontend update it's states (do this before drawing else, e.g. the first frame may have an unwanted content)
				m_pFrontendQt->OnUpdate();
			}

			// Ask Qt politely to update (and repaint) the widget
			update();
		} else {
			// The frontend is no longer running -> Close this Qt widget
			close();
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual QWidget functions                   ]
//[-------------------------------------------------------]
void FrontendMainWindow::keyPressEvent(QKeyEvent *pQKeyEvent)
{
	// Is it allowed to toggle the fullscreen mode using hotkeys? If so, toggle fullscreen right now? (Alt-Return or AltGr-Return)
	if (m_pFrontendQt->GetToggleFullscreenMode() && pQKeyEvent->key() == Qt::Key_Return && (pQKeyEvent->modifiers() & Qt::AltModifier)) {
		// Toggle fullscreen mode
		m_pFrontendQt->SetFullscreen(!m_pFrontendQt->IsFullscreen());
	}
	// The Escape key minimizes a window, not closes
	if(pQKeyEvent->key() == Qt::Key_Escape) {
		QWidget::showMinimized();
	}
}

void FrontendMainWindow::paintEvent(QPaintEvent *)
{
	// Qt only calls this method if the draw area isn't null

	// Let the frontend draw into it's window
	m_pFrontendQt->OnDraw();
}

void FrontendMainWindow::resizeEvent(QResizeEvent *)
{
	// Inform that the window size has been changed
	m_pFrontendQt->OnSize();
}

void FrontendMainWindow::dragEnterEvent(QDragEnterEvent *pQDragEnterEvent)
{
	if (pQDragEnterEvent->mimeData()->hasUrls()) {
		// We only support drop elements with URLs
		pQDragEnterEvent->acceptProposedAction();
	}
}

void FrontendMainWindow::dropEvent(QDropEvent *pQDropEvent)
{
	const QMimeData *pQMimeData = pQDropEvent->mimeData();
	if (pQMimeData) {
		// Loop through the provided list
		PLCore::Array<PLCore::String> lstFiles;
		QList<QUrl> lstUrls = pQMimeData->urls();
		lstFiles.Resize(lstUrls.size());
		for (int i=0; i<lstUrls.size(); i++)
			lstFiles[i] = QtStringAdapter::QtToPL(lstUrls.at(i).toString());

		// Inform the frontend
		m_pFrontendQt->OnDrop(lstFiles);
	}
}

#if defined(Q_WS_WIN)
	// Qt doesn't have a WM_EXITSIZEMOVE/WM_ENTERSIZEMOVE (MS Windows) equivalent - so we have to use the native stuff in here
	//   Background: When moving/sizing the window, the application will also be paused during this period (WM_EXITSIZEMOVE/WM_ENTERSIZEMOVE MS Windows events)...
	//               it's just annoying when you move or size a window and the controlled scene camera is spinning around while you do so...
	bool FrontendMainWindow::winEvent(MSG *message, long *result)
	{
		switch (message->message) {
			case WM_EXITSIZEMOVE:
				// Do the frontend life cycle thing - resume
				// -> Do only do this when the render window has the input focus to avoid a double call to "OnResume()"
				// -> The frontend life cycle thing is also done when the render window itself gets/looses the input focus (see "FrontendMainWindow::eventFilter()")
				if (m_pRenderWidget->hasFocus())
					m_pFrontendQt->OnResume();

				// Do not return true in here in order to filter the event out (i.e. stop it being handled further, this might cause unexpected behavior)
				break;

			case WM_ENTERSIZEMOVE:
				// Do the frontend life cycle thing - pause
				// -> Do only do this when the render window has the input focus to avoid a double call to "OnResume()"
				// -> The frontend life cycle thing is also done when the render window itself gets/looses the input focus (see "FrontendMainWindow::eventFilter()")
				if (m_pRenderWidget->hasFocus())
					m_pFrontendQt->OnPause();

				// Do not return true in here in order to filter the event out (i.e. stop it being handled further, this might cause unexpected behavior)
				break;
		}

		// Let the event being handled by Qt
		return false;
	}
#elif defined(Q_WS_X11)
	// The Linux part of the workaround for the problem described by the comment of the "FrontendMainWindow::winEvent()".
	// -> Under Linux the top level window receives an "FocusOut"-event when the user moves/resize the window via window manager frame
	bool FrontendMainWindow::x11Event(XEvent *pXEvent)
	{
		switch (pXEvent->type) {
			case FocusInX11:
				// Do the frontend life cycle thing - resume
				// -> Do only do this when the render window has the input focus to avoid a double call to "OnResume()"
				// -> The frontend life cycle thing is also done when the render window itself gets/looses the input focus (see "FrontendMainWindow::eventFilter()")
				if (m_pRenderWidget->hasFocus())
					m_pFrontendQt->OnResume();

				// Do not return true in here in order to filter the event out (i.e. stop it being handled further, this might cause unexpected behavior)
				break;

			case FocusOutX11:
				// Do the frontend life cycle thing - pause
				// -> Do only do this when the render window has the input focus to avoid a double call to "OnResume()"
				// -> The frontend life cycle thing is also done when the render window itself gets/looses the input focus (see "FrontendMainWindow::eventFilter()")
				if (m_pRenderWidget->hasFocus())
					m_pFrontendQt->OnPause();

				// Do not return true in here in order to filter the event out (i.e. stop it being handled further, this might cause unexpected behavior)
				break;
		}

		// Let the event being handled by Qt
		return false;
	}
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
