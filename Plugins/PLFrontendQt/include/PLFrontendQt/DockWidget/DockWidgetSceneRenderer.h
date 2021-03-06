/*********************************************************\
 *  File: DockWidgetSceneRenderer.h                      *
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


#ifndef __PLFRONTENDQT_DOCKWIDGET_SCENERENDERER_H__
#define __PLFRONTENDQT_DOCKWIDGET_SCENERENDERER_H__
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
	class QMainWindow;
QT_END_NAMESPACE
namespace PLScene {
	class SceneRenderer;
}
namespace PLFrontendQt {
	namespace DataModels {
		namespace SceneRendererDataModel {
			class SceneRendererDataModel;
		}
	}
	class DockWidgetSceneRendererQObject;
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
*    Scene renderer Qt dock widget class
*
*  @note
*    - Similar to "PLFrontend::DockWidgetObject", but specialized in scene renderer
*    - By default, the "PLEngine::EngineApplication::GetSceneRendererTool()::GetSceneRenderer()" scene renderer is
*      used, but only if "PLCore::CoreApplication::GetApplication()" is an "PLEngine::EngineApplication"-instance
*/
class DockWidgetSceneRenderer : public DockWidgetScene {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class DockWidgetSceneRendererQObject;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLFRONTENDQT_API)
		// Slots
		pl_slot_0_def(DockWidgetSceneRenderer,	OnDestroyed)
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
		PLFRONTENDQT_API DockWidgetSceneRenderer(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDQT_API virtual ~DockWidgetSceneRenderer();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene renderer assigned with this dock widget was destroyed
		*/
		void OnDestroyed();

		/**
		*  @brief
		*    Selects the given scene renderer
		*
		*  @param[in] pSceneRenderer
		*    Scene renderer to select, can be a null pointer
		*/
		void SelectSceneRenderer(PLScene::SceneRenderer *pSceneRenderer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QTreeView												   *m_pQTreeView;						/**< Qt tree view instance, can be a null pointer */
		DataModels::SceneRendererDataModel::SceneRendererDataModel *m_pSceneRendererDataModel;			/**< Scene renderer data model, can be a null pointer */
		DockWidgetSceneRendererQObject							   *m_pDockWidgetSceneRendererQObject;	/**< QObject instance for Qt's signal/slot mechanisms, always valid */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt


#endif // __PLFRONTENDQT_DOCKWIDGET_SCENERENDERER_H__
