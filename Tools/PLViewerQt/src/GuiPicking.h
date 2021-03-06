/*********************************************************\
 *  File: GuiPicking.h                                   *
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


#ifndef __PLVIEWERQT_PICKING_H__
#define __PLVIEWERQT_PICKING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNodeHandler.h>
#include <PLEngine/Picking/MousePicking.h>
#include <PLFrontendQt/DockWidget/DockWidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QLabel;
QT_END_NAMESPACE
namespace PLScene {
	class SNMTransformGizmo;
}
class Gui;
class GuiPickingQObject;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Picking & selection application component
*
*  @note
*    - This class is also derived from "PLFrontendQt::DockWidget" to be able to spy on the dock widget communication
*/
class GuiPicking : public PLFrontendQt::DockWidget, public PLEngine::MousePicking {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GuiPickingQObject;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		pl_slot_0_def(GuiPicking,	OnDestroyed)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*/
		GuiPicking(Gui &cGui);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GuiPicking();

		/**
		*  @brief
		*    Performs the informative picking
		*
		*   @remarks
		*     The purpose of this method is to provide the user with an information what's currently under the mouse
		*     cursor. So, this method should be called continually without the need to "click on the object". This
		*     method is allowed decide whether or not to internal state.
		*
		*  @return
		*    Picked scene node, can be a null pointer
		*
		*  @see
		*    - "PerformPicking()"
		*/
		PLScene::SceneNode *PerformInformativPicking();

		/**
		*  @brief
		*    Performs picking
		*
		*  @return
		*    Picked scene node, can be a null pointer
		*
		*  @note
		*    - Also updates the Qt label shown in the status bar of the Qt main window
		*/
		PLScene::SceneNode *PerformPicking();

		/**
		*  @brief
		*    Returns the used scene container
		*
		*  @return
		*    The used scene container, can be a null pointer
		*/
		PLScene::SceneContainer *GetSceneContainer() const;

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
		*    Set the text of the Qt label shown in the status bar of the Qt main window
		*
		*  @param[in] sText
		*    Text to be set
		*/
		void SetLabelStatusBarText(const PLCore::String &sText);

		/**
		*  @brief
		*    Called when the scene node assigned with this dock widget was destroyed
		*/
		void OnDestroyed();

		/**
		*  @brief
		*    Returns whether or not any of the transform gizmo axis is currently selected
		*
		*  @return
		*    In case any of the transform gizmo axis is currently selected,
		*    the transform gizmo instance is returned, else a null pointer
		*/
		PLScene::SNMTransformGizmo *IsAnyTransformGizmoAxisSelected() const;

		/**
		*  @brief
		*    Sets whether a transform gizmo is currently in transform mode or not
		*
		*  @param[in] cTransformGizmo
		*    Transform gizmo to change the transform mode of
		*  @param[in] bTransformMode
		*    'true' if the transform gizmo is currently in transform mode, else 'false'
		*/
		void SetTransformMode(PLScene::SNMTransformGizmo &cTransformGizmo, bool bTransformMode);


	//[-------------------------------------------------------]
	//[ Private virtual PLEngine::Picking functions           ]
	//[-------------------------------------------------------]
	private:
		virtual bool OnPickingCandidate(PLScene::SceneNode &cSceneNode) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Gui										   *m_pGui;						/**< Owner GUI, always valid! */
		GuiPickingQObject						   *m_pGuiPickingQObject;		/**< QObject instance for Qt's signal/slot mechanisms, always valid */
		PLCore::uint64							    m_nLastPickingTime;			/**< Last picking time */
		PLScene::SceneNodeHandler				    m_cPickedSceneNodeHandler;	/**< Currently picked scene node (aka "mouse over") */
		PLScene::SceneNode						   *m_pSceneNode;				/**< The scene node which is currently the center of the attention */
		QLabel									   *m_pQLabelStatusBar;			/**< Qt label shown in the status bar of the Qt main window, can be a null pointer */
		PLCore::Array<PLScene::SceneNodeModifier*>  m_lstSceneNodeModifiers;	/**< List of temporary scene node modifiers we added to the selected scene node */


};


#endif // __PLVIEWERQT_PICKING_H__
