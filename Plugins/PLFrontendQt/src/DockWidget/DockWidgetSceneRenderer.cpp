/*********************************************************\
 *  File: DockWidgetSceneRenderer.cpp                    *
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
#include <PLScene/Compositing/SceneRenderer.h>
#include "PLFrontendQt/DataModels/PLTreeItemsDelegate.h"
#include "PLFrontendQt/DataModels/SceneRendererDataModel/SceneRendererDataModel.h"
#include "PLFrontendQt/DockWidget/DockWidgetSceneRendererQObject.h"
#include "PLFrontendQt/DockWidget/DockWidgetSceneRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetSceneRenderer, "PLFrontendQt", PLFrontendQt::DockWidgetScene, "Scene renderer Qt dock widget class")
	// Properties
	pl_properties
		pl_property("Title", "Scene Renderer")
	pl_properties_end
	// Constructors
	pl_constructor_2_metadata(DefaultConstructor,	QMainWindow*,	DockWidgetManager*,	"Constructor with a pointer to the Qt main window as first parameter, pointer to the dock widget manager this dock widget should be registered to as second parameter",	"")
	// Slots
	pl_slot_0_metadata(OnDestroyed,	"Called when the scene renderer assigned with this dock widget was destroyed",	"")
pl_class_metadata_end(DockWidgetSceneRenderer)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetSceneRenderer::DockWidgetSceneRenderer(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager) : DockWidgetScene(pQMainWindow, pDockWidgetManager),
	SlotOnDestroyed(this),
	m_pQTreeView(nullptr),
	m_pSceneRendererDataModel(nullptr),
	m_pDockWidgetSceneRendererQObject(new DockWidgetSceneRendererQObject(*this))
{
	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget) {
		// Create tree view and set scene graph model
		m_pQTreeView = new QTreeView();
		pQDockWidget->setWidget(m_pQTreeView);
		m_pSceneRendererDataModel = new DataModels::SceneRendererDataModel::SceneRendererDataModel(pQDockWidget);
		m_pQTreeView->setModel(m_pSceneRendererDataModel);
		m_pQTreeView->expandToDepth(0);
		m_pQTreeView->setDragEnabled(false);
		m_pQTreeView->setDragDropMode(QAbstractItemView::InternalMove);
		m_pQTreeView->setDefaultDropAction(Qt::IgnoreAction);

		// This item delegate adds additional (rudimentary) features when handling edits
		// -> See "DataModels::PLTreeItemsDelegate"-class documentation for details
		m_pQTreeView->setItemDelegate(new DataModels::PLTreeItemsDelegate);

		// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
		AddDockWidgetAndTabify(*pQMainWindow, Qt::BottomDockWidgetArea, *pQDockWidget);

		// The Qt object should receive events from the encapsulated Qt dock widget
		pQDockWidget->installEventFilter(m_pDockWidgetSceneRendererQObject);
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetSceneRenderer::~DockWidgetSceneRenderer()
{
	// Destroy the QObject instance for Qt's signal/slot mechanisms
	delete m_pDockWidgetSceneRendererQObject;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene renderer assigned with this dock widget was destroyed
*/
void DockWidgetSceneRenderer::OnDestroyed()
{
	// Argh! Mayday! We lost our scene renderer!
	// -> Now no scene renderer is currently selected
	SelectSceneRenderer(nullptr);

	// Get encapsulated Qt dock widget and hide it, it will get refreshed automatically when the encapsulated Qt dock widget is shown again
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget)
		pQDockWidget->hide();
}

/**
*  @brief
*    Selects the given scene renderer
*/
void DockWidgetSceneRenderer::SelectSceneRenderer(SceneRenderer *pSceneRenderer)
{
	// Is there a scene renderer model instance?
	if (m_pSceneRendererDataModel) {
		// Get the currently assigned scene renderer
		SceneRenderer *pCurrentSceneRenderer = m_pSceneRendererDataModel->GetSceneRenderer();

		// State change?
		if (pCurrentSceneRenderer != pSceneRenderer) {
			// Disconnect event handler
			if (pCurrentSceneRenderer)
				pCurrentSceneRenderer->EventDestroyed.Disconnect(SlotOnDestroyed);

			// Assign new scene renderer
			m_pSceneRendererDataModel->SetSceneRenderer(pSceneRenderer);

			{ // Usability: Resize the first tree view column given to the size of its contents
				// No need to backup current expanded state and restore it after we're done because we set new content above resulting in that all is collapsed when we're in here
				m_pQTreeView->expandAll();
				m_pQTreeView->resizeColumnToContents(0);
				m_pQTreeView->collapseAll();
			}

			// Connect event handler
			if (pSceneRenderer)
				pSceneRenderer->EventDestroyed.Connect(SlotOnDestroyed);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
