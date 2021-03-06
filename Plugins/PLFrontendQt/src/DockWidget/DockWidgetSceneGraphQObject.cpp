/*********************************************************\
 *  File: DockWidgetSceneGraphQObject.cpp                *
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
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include "PLFrontendQt/DataModels/SceneGraphTreeModel.h"
#include "PLFrontendQt/DataModels/TreeSortAndFilterProxyModel.h"
#include "PLFrontendQt/DockWidget/DockWidgetManager.h"
#include "PLFrontendQt/DockWidget/DockWidgetSceneGraph.h"
#include "PLFrontendQt/DockWidget/DockWidgetSceneGraphQObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetSceneGraphQObject::DockWidgetSceneGraphQObject(DockWidgetSceneGraph &cDockWidgetSceneGraph) :
	m_pDockWidgetSceneGraph(&cDockWidgetSceneGraph)
{
}

/**
*  @brief
*    Destructor
*/
DockWidgetSceneGraphQObject::~DockWidgetSceneGraphQObject()
{
}

/**
*  @brief
*    Updates the scene graph tree view
*/
void DockWidgetSceneGraphQObject::UpdateTreeView(SceneGraphMenu::EAction nAction, Object *pCreatedObject)
{
	// Check dock widget instances
	if (m_pDockWidgetSceneGraph->m_pQTreeView && m_pDockWidgetSceneGraph->m_pSceneGraphTreeModel) {
		// Check the reason for the update
		switch (nAction) {
			// Something was added or cloned
			case SceneGraphMenu::ActionAdded:
			case SceneGraphMenu::ActionCloned:
				// In case something was added, the provided pointer must be valid
				if (pCreatedObject) {
					// Scene node
					if (pCreatedObject->IsInstanceOf("PLScene::SceneNode")) {
						// Get scene node instance
						SceneNode *pSceneNode = static_cast<SceneNode*>(pCreatedObject);

						// Ignore automatically generated stuff
						if (!(pSceneNode->GetFlags() & SceneNode::Automatic)) {
							// Get the scene container
							SceneContainer *pSceneContainer = pSceneNode->GetContainer();
							if (pSceneContainer) {
								// Get the index of this scene node inside the scene node list of the scene container
								const int nIndex = (nAction == SceneGraphMenu::ActionCloned) ? pSceneContainer->GetIndex(*pSceneNode) : -1;

								// Add the new scene node modifier to the tree view
								m_pDockWidgetSceneGraph->m_pSceneGraphTreeModel->AddSceneNode(pSceneContainer, pSceneNode, nIndex);
							}
						}

					// Scene node modifier
					} else if (pCreatedObject->IsInstanceOf("PLScene::SceneNodeModifier")) {
						// Get scene node modifier instance
						SceneNodeModifier *pSceneNodeModifier = static_cast<SceneNodeModifier*>(pCreatedObject);

						// Ignore automatically generated stuff
						if (!(pSceneNodeModifier->GetFlags() & SceneNodeModifier::Automatic)) {
							// Get the index of this scene modifier inside the scene node modifier list of the owner scene node
							const int nIndex = (nAction == SceneGraphMenu::ActionCloned) ? pSceneNodeModifier->GetSceneNodeIndex() : -1;

							// Add the new scene node modifier to the tree view
							m_pDockWidgetSceneGraph->m_pSceneGraphTreeModel->AddSceneNodeModifier(&pSceneNodeModifier->GetSceneNode(), pSceneNodeModifier, nIndex);
						}
					}

					// Usability: Automatically select the new object
					{ // -> Perform a dock widget manager broadcast (do not exclude this emitting dock widget)
						DockWidgetManager *pDockWidgetManager = m_pDockWidgetSceneGraph->GetDockWidgetManager();
						if (pDockWidgetManager)
							pDockWidgetManager->CallDockWidgetsMethod("SelectObject", Params<void, Object*>(pCreatedObject));
					}
				}
				break;

			// Something was deleted
			case SceneGraphMenu::ActionDeleted:
				m_pDockWidgetSceneGraph->m_pQTreeView->selectionModel()->clearSelection();
				break;
		}
	}
}


//[-------------------------------------------------------]
//[ Private Qt slots (MOC)                                ]
//[-------------------------------------------------------]
void DockWidgetSceneGraphQObject::QtSlotTreeViewDoubleClicked(const QModelIndex &cQModelIndex)
{
	// Use the given Qt model index to figure out the currently selected RTTI class instance
	Object *pObject = m_pDockWidgetSceneGraph->GetObjectByQModelIndex(cQModelIndex);

	// Perform a dock widget manager broadcast (excludes this emitting dock widget)
	m_pDockWidgetSceneGraph->CallDockWidgetsMethod("SelectObject", Params<void, Object*>(pObject));
}

void DockWidgetSceneGraphQObject::QtSlotCustomContextMenuRequested(const QPoint &cQPoint)
{
	// Show the scene graph context menu
	if (m_pDockWidgetSceneGraph->m_pQTreeView) {
		// We could use
		//   "Object *pObject = m_pDockWidgetSceneGraph->GetSelectedObject();"
		// in here, but it looks odd to be able to add a context menu for
		// an item while the mouse is far, far away from the item itself
		// -> So, we're asking Qt for a model index below the given point
		const QModelIndex cQModelIndex = m_pDockWidgetSceneGraph->m_pQTreeView->indexAt(cQPoint);

		// Use the resulting Qt model index to figure out the currently selected RTTI class instance
		Object *pObject = cQModelIndex.isValid() ? m_pDockWidgetSceneGraph->GetObjectByQModelIndex(cQModelIndex) : nullptr;

		// Show the scene graph context menu
		if (pObject) {
			SceneGraphMenu cSceneGraphMenu(*pObject);
			QAction *pAction = cSceneGraphMenu.exec(m_pDockWidgetSceneGraph->m_pQTreeView->mapToGlobal(cQPoint));
			if (pAction) {
				// Evaluate the performed action and update the tree view if required
				const QVariant cUserData = pAction->data();
				UpdateTreeView(cUserData.isValid() ? static_cast<SceneGraphMenu::EAction>(pAction->data().value<int>()) : SceneGraphMenu::ActionUnknown, cSceneGraphMenu.GetCreatedObject());
			}
		}
	}
}

void DockWidgetSceneGraphQObject::QtSlotFilterTypeChanged(int filterId)
{
	m_pDockWidgetSceneGraph->m_pSortAndFilterModel->setFilterRole((filterId == ByClassName) ? DataModels::SceneGraphTreeModel::ClassNameRole : static_cast<int>(Qt::DisplayRole));
}

void DockWidgetSceneGraphQObject::QtSlotFilterChanged(const QString &text)
{
	m_pDockWidgetSceneGraph->m_pSortAndFilterModel->setFilterWildcard(text);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
