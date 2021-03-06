/*********************************************************\
 *  File: DockWidgetSceneGraph.h                         *
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


#ifndef __PLFRONTENDQT_DOCKWIDGET_SCENEGRAPH_H__
#define __PLFRONTENDQT_DOCKWIDGET_SCENEGRAPH_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLFrontendQt/DockWidget/DockWidgetScene.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QTreeView;
	class QModelIndex;
	class QMainWindow;
QT_END_NAMESPACE
namespace PLScene {
	class SceneNode;
	class SceneContainer;
}
namespace PLFrontendQt {
	namespace DataModels {
		class SceneGraphTreeModel;
		class TreeSortAndFilterProxyModel;
	}
	class DockWidgetSceneGraphQObject;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene graph Qt dock widget class
*/
class DockWidgetSceneGraph : public DockWidgetScene {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class DockWidgetSceneGraphQObject;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLFRONTENDQT_API)
		// Slots
		pl_slot_0_def(DockWidgetSceneGraph,	OnDestroy)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pQMainWindow
		*    Pointer to Qt main window, can be a null pointer (in this case you're responsible for destroying this instance)
		*  @param[in] pDockWidgetManager
		*    Optional pointer to the dock widget manager this dock widget should be registered to, can be a null pointer
		*/
		PLFRONTENDQT_API DockWidgetSceneGraph(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDQT_API virtual ~DockWidgetSceneGraph();

		/**
		*  @brief
		*    Returns the used scene container
		*
		*  @return
		*    The used scene container, can be a null pointer
		*/
		PLFRONTENDQT_API PLScene::SceneContainer *GetSceneContainer() const;

		/**
		*  @brief
		*    Sets the scene container to use
		*
		*  @param[in] pSceneContainer
		*    Scene container to use, can be a null pointer
		*/
		PLFRONTENDQT_API void SetSceneContainer(PLScene::SceneContainer *pSceneContainer);

		/**
		*  @brief
		*    Returns the currently selected object
		*
		*  @return
		*    The currently selected object, can be a null pointer
		*/
		PLFRONTENDQT_API PLCore::Object *GetSelectedObject() const;

		/**
		*  @brief
		*    Selects the given object (post-broadcast)
		*
		*  @param[in] pObject
		*    Object to select, can be a null pointer
		*/
		PLFRONTENDQT_API void PostSelectObject(PLCore::Object *pObject);

		/**
		*  @brief
		*    An object was added
		*
		*  @param[in] cObject
		*    Added object
		*  @param[in] nPosition
		*    Index position specifying the location within the object list where the object should be added, <0 for at the end
		*/
		PLFRONTENDQT_API void AddedObject(PLCore::Object &cObject, int nPosition);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets the used scene container and object
		*/
		void SetSceneContainerAndObject();

		/**
		*  @brief
		*    Called when the scene container assigned with this dock widget was destroyed
		*/
		void OnDestroy();

		/**
		*  @brief
		*    Returns an object by using a given Qt model index
		*
		*  @param[in] cQModelIndex
		*    Qt model index to return the RTTI class instance from
		*
		*  @return
		*    The object, can be a null pointer
		*/
		PLCore::Object *GetObjectByQModelIndex(const QModelIndex &cQModelIndex) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QTreeView								*m_pQTreeView;						/**< Qt tree view instance, can be a null pointer */
		DataModels::SceneGraphTreeModel 		*m_pSceneGraphTreeModel;			/**< Scene graph tree model, can be a null pointer */
		PLScene::SceneContainer					*m_pSceneContainer;					/**< Scene container assigned with this dock widget, can be a null pointer */
		DockWidgetSceneGraphQObject				*m_pDockWidgetSceneGraphQObject;	/**< QObject instance for Qt's signal/slot mechanisms, always valid */
		DataModels::TreeSortAndFilterProxyModel	*m_pSortAndFilterModel;				/**< sort and filter model, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt


#endif // __PLFRONTENDQT_DOCKWIDGET_SCENEGRAPH_H__
