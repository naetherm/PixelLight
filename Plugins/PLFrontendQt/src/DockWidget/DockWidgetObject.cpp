/*********************************************************\
 *  File: DockWidgetObject.cpp                           *
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
#include <QtGui/qtreeview.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/QMainWindow>
#include <PLCore/Base/Class.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/DataModels/PLIntrospectionModel.h"
#include "PLFrontendQt/DataModels/PLTreeItemsDelegate.h"
#include "PLFrontendQt/DockWidget/DockWidgetObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetObject, "PLFrontendQt", PLFrontendQt::DockWidget, "\"PLCore::Object\" Qt dock widget class")
	// Properties
	pl_properties
		pl_property("Title", "Object")
	pl_properties_end
	// Methods
	pl_method_0_metadata(GetSelectedObject,	pl_ret_type(PLCore::Object*),						"Returns the currently selected object, can be a null pointer.",	"")
	pl_method_1_metadata(SelectObject,		pl_ret_type(void),				PLCore::Object*,	"Selects the given object. Object to select as first parameter.",	"")
	// Constructors
	pl_constructor_2_metadata(DefaultConstructor,	QMainWindow*,	DockWidgetManager*,	"Constructor with a pointer to the Qt main window as first parameter, pointer to the dock widget manager this dock widget should be registered to as second parameter",	"")
	// Slots
	pl_slot_0_metadata(OnDestroyed,	"Called when the object assigned with this dock widget was destroyed",	"")
pl_class_metadata_end(DockWidgetObject)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetObject::DockWidgetObject(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager) : DockWidget(pQMainWindow, pDockWidgetManager),
	SlotOnDestroyed(this),
	m_pQTreeView(nullptr),
	m_pPLIntrospectionModel(nullptr),
	m_pObject(nullptr)
{
	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget) {
		// Create tree view and set introspection model
		m_pQTreeView = new QTreeView();
		pQDockWidget->setWidget(m_pQTreeView);
		m_pPLIntrospectionModel = new DataModels::PLIntrospectionModel(pQDockWidget);
		m_pQTreeView->setModel(m_pPLIntrospectionModel);
		m_pQTreeView->expandToDepth(0);

		// This item delegate adds additional (rudimentary) features when handling edits
		// -> See "DataModels::PLTreeItemsDelegate"-class documentation for details
		m_pQTreeView->setItemDelegate(new DataModels::PLTreeItemsDelegate);

		// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
		AddDockWidgetAndTabify(*pQMainWindow, Qt::BottomDockWidgetArea, *pQDockWidget);

		{ // Ask the RTTI dock widget fellows whether or not someone knows which is the currently selected object
			// Get a list of dock widgets registered within the same dock widget manager this dock widget is in
			const Array<DockWidget*> &lstDockWidgets = GetFellowDockWidgets();
			Object *pObject = nullptr;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements() && !pObject; i++) {
				// Get the dock widget, and ignore our own ego
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this) {
					// Get the typed dynamic parameters
					Params<Object*> cParams;

					// Call the RTTI method
					pDockWidget->CallMethod("GetSelectedObject", cParams);

					// Get the result, we're interested in any object
					pObject = cParams.Return;
					if (pObject)
						SelectObject(pObject);
				}
			}
		}
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetObject::~DockWidgetObject()
{
}

/**
*  @brief
*    Returns the currently selected object
*/
Object *DockWidgetObject::GetSelectedObject() const
{
	return m_pObject;
}

/**
*  @brief
*    Selects the given object
*/
void DockWidgetObject::SelectObject(Object *pObject)
{
	// State change?
	if (m_pObject != pObject) {
		// Disconnect event handler
		if (m_pObject)
			m_pObject->SignalDestroyed.Disconnect(SlotOnDestroyed);

		// Backup the given object pointer
		m_pObject = pObject;

		// Connect event handler
		if (m_pObject)
			m_pObject->SignalDestroyed.Connect(SlotOnDestroyed);

		// Is there a PL introspection model instance?
		if (m_pPLIntrospectionModel) {
			// Set object
			m_pPLIntrospectionModel->SetObject(m_pObject);

			{ // Usability: Resize the first tree view column given to the size of its contents
				// No need to backup current expanded state and restore it after we're done because we set new content above resulting in that all is collapsed when we're in here
				m_pQTreeView->expandAll();
				m_pQTreeView->resizeColumnToContents(0);
				m_pQTreeView->collapseAll();
			}

			// Get encapsulated Qt dock widget and set a decent window title
			QDockWidget *pQDockWidget = GetQDockWidget();
			if (pQDockWidget) {
				// Set window title
				QString sQStringWindowTitle = pQDockWidget->tr(GetClass()->GetProperties().Get("Title"));
				if (m_pObject) { 
					// Append class name
					sQStringWindowTitle += ": ";
					sQStringWindowTitle += QtStringAdapter::PLToQt('\"' + m_pObject->GetClass()->GetClassName() + '\"');	// Put it into quotes to make it possible to see e.g. trailing spaces

					// An "PLCore::Object" itself has no "name", we could show the memory address but this wouldn't be that useful to the user
					// -> Try "GetAbsoluteName()"-method to get an absolute name
					// -> In case there's no name, check whether there's an name attribute
					// -> In case there's still no name, check whether or not there's an filename attribute
					String sName;
					{ // Try "GetAbsoluteName()"-method to get an absolute name
						// Get the typed dynamic parameters
						Params<String> cParams;

						// Call the RTTI method
						m_pObject->CallMethod("GetAbsoluteName", cParams);

						// Get the result
						sName = cParams.Return;
						if (sName.GetLength())
							sName = "Name = \"" + sName + '\"';	// Put it into quotes to make it possible to see e.g. trailing spaces
					}

					// Do we already have a name?
					if (!sName.GetLength()) {
						// Check whether there's an name attribute
						DynVarPtr pDynVar = m_pObject->GetAttribute("Name");
						if (pDynVar)
							sName = "Name = \"" + pDynVar->GetString() + '\"';	// Put it into quotes to make it possible to see e.g. trailing spaces

						// In case there's still no name, check whether or not there's an filename attribute
						if (!sName.GetLength()) {
							DynVarPtr pDynVar = m_pObject->GetAttribute("Filename");
							if (pDynVar)
								sName = "Filename = \"" + pDynVar->GetString() + '\"';	// Put it into quotes to make it possible to see e.g. trailing spaces
						}
					}

					// Use the name, if we have one
					if (sName.GetLength()) {
						// We have a representable name, show it to the user
						sQStringWindowTitle += ": ";
						sQStringWindowTitle += QtStringAdapter::PLToQt(sName);
					}
				}
				pQDockWidget->setWindowTitle(sQStringWindowTitle);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the object assigned with this dock widget was destroyed
*/
void DockWidgetObject::OnDestroyed()
{
	// Argh! Mayday! We lost our object!
	// -> Now no object is currently selected
	SelectObject(nullptr);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
