/*********************************************************\
 *  File: DockWidgetSceneMeshInformation.h               *
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


#ifndef __PLFRONTENDQT_DOCKWIDGET_SCENEMESHINFORMATION_H__
#define __PLFRONTENDQT_DOCKWIDGET_SCENEMESHINFORMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLFrontendQt/DockWidget/DockWidgetScene.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QTableView;
	class QMainWindow;
	class QStandardItemModel;
QT_END_NAMESPACE
namespace PLRenderer {
	class IndexBuffer;
	class VertexBuffer;
}
namespace PLMesh {
	class Mesh;
	class MeshLODLevel;
	class MeshMorphTarget;
}
namespace PLScene {
	class SceneNode;
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
*    Mesh information Qt dock widget class
*/
class DockWidgetSceneMeshInformation : public DockWidgetScene {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLFRONTENDQT_API)
		// Slots
		pl_slot_0_def(DockWidgetSceneMeshInformation,	OnDestroyed)
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
		DockWidgetSceneMeshInformation(QMainWindow *pQMainWindow, PLFrontendQt::DockWidgetManager *pDockWidgetManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DockWidgetSceneMeshInformation();

		/**
		*  @brief
		*    Returns the currently selected object
		*
		*  @return
		*    The currently selected object, can be a null pointer
		*/
		PLCore::Object *GetSelectedObject() const;

		/**
		*  @brief
		*    Selects the given object
		*
		*  @param[in] pObject
		*    Object to select, can be a null pointer
		*/
		void SelectObject(PLCore::Object *pObject);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the object assigned with this dock widget was destroyed
		*/
		void OnDestroyed();

		/**
		*  @brief
		*    Updates the mesh information
		*
		*  @param[in] pObject
		*    Selected object can be a null pointer
		*/
		void UpdateObject(PLCore::Object *pObject);

		/**
		*  @brief
		*    Fills the given Qt standard item model
		*
		*  @param[in] cQStandardItemModel
		*    Qt standard item model to fill
		*  @param[in] cSceneNode
		*    Currently selected scene node
		*/
		void FillStandardItemModel(QStandardItemModel &cQStandardItemModel, PLScene::SceneNode &cSceneNode) const;

		/**
		*  @brief
		*    Fills the given Qt standard item model, mesh part
		*
		*  @param[in] cQStandardItemModel
		*    Qt standard item model to fill
		*  @param[in] cMesh
		*    Currently selected mesh
		*/
		void FillStandardItemModelMesh(QStandardItemModel &cQStandardItemModel, PLMesh::Mesh &cMesh) const;

		/**
		*  @brief
		*    Fills the given Qt standard item model, mesh morph target part
		*
		*  @param[in] cQStandardItemModel
		*    Qt standard item model to fill
		*  @param[in] cMeshMorphTarget
		*    Current morph target of the selected mesh
		*/
		void FillStandardItemModelMeshMorphTarget(QStandardItemModel &cQStandardItemModel, PLMesh::MeshMorphTarget &cMeshMorphTarget) const;

		/**
		*  @brief
		*    Fills the given Qt standard item model, vertex buffer part
		*
		*  @param[in] cQStandardItemModel
		*    Qt standard item model to fill
		*  @param[in] cVertexBuffer
		*    Currently used vertex buffer
		*/
		void FillStandardItemModelVertexBuffer(QStandardItemModel &cQStandardItemModel, PLRenderer::VertexBuffer &cVertexBuffer) const;

		/**
		*  @brief
		*    Fills the given Qt standard item model, mesh LOD level part
		*
		*  @param[in] cQStandardItemModel
		*    Qt standard item model to fill
		*  @param[in] cMeshLODLevel
		*    Current mesh LOD level of the selected mesh
		*/
		void FillStandardItemModelMeshLODLevel(QStandardItemModel &cQStandardItemModel, PLMesh::MeshLODLevel &cMeshLODLevel) const;

		/**
		*  @brief
		*    Fills the given Qt standard item model, index buffer part
		*
		*  @param[in] cQStandardItemModel
		*    Qt standard item model to fill
		*  @param[in] cIndexBuffer
		*    Currently used index buffer
		*/
		void FillStandardItemModelIndexBuffer(QStandardItemModel &cQStandardItemModel, PLRenderer::IndexBuffer &cIndexBuffer) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Object *m_pObject;		/**< Object assigned with this dock widget, can be a null pointer */
		QTableView	   *m_pQTableView;	/**< Qt table view instance, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt


#endif // __PLFRONTENDQT_DOCKWIDGET_SCENEMESHINFORMATION_H__
