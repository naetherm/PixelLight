/*********************************************************\
 *  File: DockWidgetRTTIBrowser.cpp                      *
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
#include <QtGui/QMainWindow>
#include <PLCore/Base/Class.h>
#include "PLFrontendQt/Widget/RTTIBrowserWidget.h"
#include "PLFrontendQt/DockWidget/DockWidgetRTTIBrowser.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetRTTIBrowser, "PLFrontendQt", PLFrontendQt::DockWidget, "RTTI browser Qt dock widget class")
	// Properties
	pl_properties
		pl_property("Title", "RTTI Browser")
	pl_properties_end
	// Constructors
	pl_constructor_2_metadata(DefaultConstructor,	QMainWindow*,	DockWidgetManager*,	"Constructor with a pointer to the Qt main window as first parameter, pointer to the dock widget manager this dock widget should be registered to as second parameter",	"")
pl_class_metadata_end(DockWidgetRTTIBrowser)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetRTTIBrowser::DockWidgetRTTIBrowser(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager) : DockWidget(pQMainWindow, pDockWidgetManager)
{
	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget) {
		// Create RTTI browser widget
		pQDockWidget->setWidget(new RTTIBrowserWidget());

		// Set window title
		pQDockWidget->setWindowTitle(pQDockWidget->tr(GetClass()->GetProperties().Get("Title")));

		// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
		AddDockWidgetAndTabify(*pQMainWindow, Qt::BottomDockWidgetArea, *pQDockWidget);
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetRTTIBrowser::~DockWidgetRTTIBrowser()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
