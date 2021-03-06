/*********************************************************\
 *  File: DockWidgetLog.cpp                              *
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
#include <QtGui/qdockwidget.h>
#include <QtGui/qplaintextedit.h>
#include <QtGui/QMainWindow>
#include <PLCore/Log/Log.h>
#include <PLCore/Base/Class.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/DockWidget/DockWidgetLog.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetLog, "PLFrontendQt", PLFrontendQt::DockWidget, "\"PLCore::Log\" Qt dock widget class")
	// Properties
	pl_properties
		pl_property("Title", "Log")
	pl_properties_end
	// Constructors
	pl_constructor_2_metadata(DefaultConstructor,	QMainWindow*,	DockWidgetManager*,	"Constructor with a pointer to the Qt main window as first parameter, pointer to the dock widget manager this dock widget should be registered to as second parameter",	"")
pl_class_metadata_end(DockWidgetLog)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetLog::DockWidgetLog(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager) : DockWidget(pQMainWindow, pDockWidgetManager),
	SlotNewEntry(&DockWidgetLog::OnNewEntry, this),
	m_pQPlainTextEdit(nullptr)
{
	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget) {
		// Get the log instance
		Log *pLog = Log::GetInstance();

		// Create Qt plain text edit instance
		m_pQPlainTextEdit = new QPlainTextEdit();
		m_pQPlainTextEdit->setReadOnly(true);
		m_pQPlainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
		m_pQPlainTextEdit->document()->setMaximumBlockCount(pLog->GetBufferCount());
		pQDockWidget->setWidget(m_pQPlainTextEdit);

		{ // Append the last log messages
			Iterator<String> cIterator = pLog->GetLastMessages().GetIterator();
			while (cIterator.HasNext())
				m_pQPlainTextEdit->appendPlainText(QtStringAdapter::PLToQt(cIterator.Next()));
		}

		// Usability: Update the Qt plain text edit cursor to in order to reduce the need for manual scrolling
		m_pQPlainTextEdit->moveCursor(QTextCursor::End);			// Move to the end of the document
		m_pQPlainTextEdit->moveCursor(QTextCursor::StartOfLine);	// Move to the start of the current line

		// Set window title
		pQDockWidget->setWindowTitle(pQDockWidget->tr(GetClass()->GetProperties().Get("Title")));

		// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
		AddDockWidgetAndTabify(*pQMainWindow, Qt::BottomDockWidgetArea, *pQDockWidget);

		// Connect event handler
		pLog->EventNewEntry.Connect(SlotNewEntry);
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetLog::~DockWidgetLog()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a new log entry had been added
*/
void DockWidgetLog::OnNewEntry()
{
	// Append the latest log message
	const Queue<String> &qLastMessages = Log::GetInstance()->GetLastMessages();
	if (qLastMessages.GetNumOfElements())
		m_pQPlainTextEdit->appendPlainText(QtStringAdapter::PLToQt(qLastMessages.Top()));

	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget && !pQDockWidget->hasFocus()) {
		// Usability: Update the Qt plain text edit cursor in order to reduce the need for manual scrolling
		// -> But only if we don't have the focus else this behavior might be annoying instead of useful
		m_pQPlainTextEdit->moveCursor(QTextCursor::End);			// Move to the end of the document
		m_pQPlainTextEdit->moveCursor(QTextCursor::StartOfLine);	// Move to the start of the current line
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
