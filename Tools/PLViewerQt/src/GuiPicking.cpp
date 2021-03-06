/*********************************************************\
 *  File: GuiPicking.cpp                                 *
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
#include <PLCore/PLCore.h>
PL_WARNING_PUSH
	PL_WARNING_DISABLE(4127)	// "warning C4127: conditional expression is constant"
	#include <QtGui/qlabel.h>
	#include <QtGui/qstatusbar.h>
PL_WARNING_POP
#include <PLCore/Tools/Timing.h>
#include <PLScene/Scene/SceneNodeModifiers/SNMTransformGizmo.h>
#include <PLEngine/Picking/PickingResult.h>
#include <PLFrontendQt/QtStringAdapter.h>
#include <PLFrontendQt/FrontendMainWindow.h>
#include <PLFrontendQt/DockWidget/DockWidgetManager.h>
#include "Gui.h"
#include "ApplicationQt.h"
#include "GuiPickingQObject.h"
#include "GuiPicking.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
using namespace PLEngine;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(GuiPicking, "", PLFrontendQt::DockWidget, "Picking & selection application component")
	// Methods
	pl_method_0_metadata(GetSceneContainer,	pl_ret_type(PLScene::SceneContainer*),						"Returns the used scene container, can be a null pointer.",			"")
	pl_method_0_metadata(GetSelectedObject,	pl_ret_type(PLCore::Object*),								"Returns the currently selected object, can be a null pointer.",	"")
	pl_method_1_metadata(SelectObject,		pl_ret_type(void),						PLCore::Object*,	"Selects the given object. Object to select as first parameter.",	"")
	// Slots
	pl_slot_0_metadata(OnDestroyed,	"Called when the scene node assigned with this dock widget was destroyed",	"")
pl_class_metadata_end(GuiPicking)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GuiPicking::GuiPicking(Gui &cGui) : DockWidget(nullptr, cGui.GetFrontendMainWindow() ? &cGui.GetFrontendMainWindow()->GetDockWidgetManager() : nullptr), MousePicking(cGui.GetApplication().GetFrontend()),
	SlotOnDestroyed(this),
	m_pGui(&cGui),
	m_pGuiPickingQObject(new GuiPickingQObject(*this)),
	m_nLastPickingTime(Timing::GetInstance()->GetPastTime()),
	m_pSceneNode(nullptr),
	m_pQLabelStatusBar(new QLabel())
{
	// Get the Qt main window
	PLFrontendQt::FrontendMainWindow *pFrontendMainWindow = cGui.GetFrontendMainWindow();
	if (pFrontendMainWindow) {
		// Add to Qt main window status bar
		pFrontendMainWindow->statusBar()->addWidget(m_pQLabelStatusBar);
	}
}

/**
*  @brief
*    Destructor
*/
GuiPicking::~GuiPicking()
{
	// Destroy the QObject instance for Qt's signal/slot mechanisms
	delete m_pGuiPickingQObject;

	// Now no scene node is currently selected
	SelectObject(nullptr);

	// Destroy Qt label shown in the status bar of the Qt main window
	delete m_pQLabelStatusBar;
}

/**
*  @brief
*    Performs the informative picking
*/
SceneNode *GuiPicking::PerformInformativPicking()
{
	// Get the current time data
	const uint64 nPastTime = Timing::GetInstance()->GetPastTime();

	// Perform mouse picking - do not make this test each frame because it may cost some performance
	if ((nPastTime-m_nLastPickingTime) > 100) {
		m_nLastPickingTime = nPastTime;

		// Perform picking
		return PerformPicking();
	}

	// Use the previous result as currently picked scene node
	return m_cPickedSceneNodeHandler.GetElement();
}

/**
*  @brief
*    Performs picking
*/
SceneNode *GuiPicking::PerformPicking()
{
	// Set the camera used for picking
	SetCamera(m_pGui->GetApplication().GetCamera());

	// Perform picking - "PerformMousePicking()" is using the current mouse position inside the main window
	// and the currently used camera in order to find the scene node under the mouse cursor
	PickingResult cPickingResult;
	SceneNode *pPickedSceneNode = nullptr;
	if (PerformMousePicking(cPickingResult))
		pPickedSceneNode = cPickingResult.GetSceneNode();

	// Picked changed?
	if (pPickedSceneNode != m_cPickedSceneNodeHandler.GetElement()) {
		// Backup the currently picked scene node
		m_cPickedSceneNodeHandler.SetElement(pPickedSceneNode);

		// Update the Qt label shown in the status bar of the Qt main window
		SetLabelStatusBarText(pPickedSceneNode ? pPickedSceneNode->GetAbsoluteName() : "");
	}

	// Done
	return pPickedSceneNode;
}

/**
*  @brief
*    Returns the used scene container
*/
SceneContainer *GuiPicking::GetSceneContainer() const
{
	return m_pGui->GetApplication().GetScene();
}

/**
*  @brief
*    Returns the currently selected object
*/
Object *GuiPicking::GetSelectedObject() const
{
	return m_pSceneNode;
}

/**
*  @brief
*    Selects the given object
*/
void GuiPicking::SelectObject(Object *pObject)
{
	// We only know "PLScene::SceneNode" and "PLScene::SceneNodeModifier"
	SceneNode *pSceneNode = nullptr;
	if (pObject) {
		// Is it an "PLScene::SceneNode"-instance?
		if (pObject->IsInstanceOf("PLScene::SceneNode")) {
			pSceneNode = static_cast<SceneNode*>(pObject);

		// Is it an "PLScene::SceneNodeModifier"-instance?
		} else if (pObject->IsInstanceOf("PLScene::SceneNodeModifier")) {
			// Select the owner scene node of this scene node modifier
			pSceneNode = &static_cast<SceneNodeModifier*>(pObject)->GetSceneNode();
		}
	}

	// State change?
	if (m_pSceneNode != pSceneNode) {
		// Disable debug mode of the previous scene node, if there's one
		if (m_pSceneNode) {
			m_pSceneNode->SetDebugFlags(m_pSceneNode->GetDebugFlags() & ~SceneNode::DebugEnabled);

			// In case we added transform gizmos, remove them right now
			for (uint32 i=0; i<m_lstSceneNodeModifiers.GetNumOfElements(); i++)
				m_pSceneNode->RemoveModifierByReference(*m_lstSceneNodeModifiers[i]);
			m_lstSceneNodeModifiers.Clear();
		}

		// Disconnect event handler
		if (m_pSceneNode)
			m_pSceneNode->SignalDestroyed.Disconnect(SlotOnDestroyed);

		// Backup the given scene node pointer
		m_pSceneNode = pSceneNode;

		// Connect event handler
		if (m_pSceneNode)
			m_pSceneNode->SignalDestroyed.Connect(SlotOnDestroyed);

		// Enable debug mode of the current scene node, if there's one
		if (m_pSceneNode) {
			m_pSceneNode->SetDebugFlags(m_pSceneNode->GetDebugFlags() | SceneNode::DebugEnabled);

			// Add transform gizmos
			// -> "m_lstSceneNodeModifiers.GetNumOfElements()" is just a security check and should not be required, but safe is safe
			if (!m_lstSceneNodeModifiers.GetNumOfElements()) {
				// Add position and rotation transform gizmos
				// -> Both can be combined visually without issues
				m_lstSceneNodeModifiers.Add(m_pSceneNode->AddModifier("PLEngine::SNMTransformGizmoPositionController", "Flags=\"Automatic\""));
				m_lstSceneNodeModifiers.Add(m_pSceneNode->AddModifier("PLEngine::SNMTransformGizmoRotationController", "Flags=\"Automatic\""));

				// Activating position and scale transform gizmos at the same time is confusing
				// -> Being able to visually manipulate scale is not that important in here
			//	m_lstSceneNodeModifiers.Add(m_pSceneNode->AddModifier("PLEngine::SNMTransformGizmoScaleController", "Flags=\"Automatic\""));
			}
		}
	}

	// Usability:
	// -> Show the "PLFrontendQt::DockWidgetObject" dock widget so we can view & edit RTTI attributes at once
	// -> Show the "PLFrontendQt::DockWidgetSceneGraph" dock widget so we can view & edit the scene elements at once
	if (m_pSceneNode) {
		m_pGui->ShowDockWidget("PLFrontendQt::DockWidgetObject");
		m_pGui->ShowDockWidget("PLFrontendQt::DockWidgetSceneGraph");
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Set the text of the Qt label shown in the status bar of the Qt main window
*/
void GuiPicking::SetLabelStatusBarText(const String &sText)
{
	// Update the Qt label shown in the status bar of the Qt main window
	m_pQLabelStatusBar->setText(sText.GetLength() ? PLFrontendQt::QtStringAdapter::PLToQt("    Scene node: \"" + sText + "\" (double left click to select)") : "");
}

/**
*  @brief
*    Called when the scene node assigned with this dock widget was destroyed
*/
void GuiPicking::OnDestroyed()
{
	// Argh! Mayday! We lost our scene node!
	// -> Now no scene node is currently selected
	m_pSceneNode = nullptr;
	m_pGuiPickingQObject->m_pTransformGizmo = nullptr;
	m_lstSceneNodeModifiers.Clear();
	SelectObject(nullptr);
}

/**
*  @brief
*    Returns whether or not any of the transform gizmo axis is currently selected
*/
SNMTransformGizmo *GuiPicking::IsAnyTransformGizmoAxisSelected() const
{
	// Loop through all scene node modifiers
	for (uint32 i=0; i<m_lstSceneNodeModifiers.GetNumOfElements(); i++) {
		// Get the current scene node modifier
		SceneNodeModifier *pSceneNodeModifer = m_lstSceneNodeModifiers[i];
		if (pSceneNodeModifer->IsInstanceOf("PLScene::SNMTransformGizmo")) {
			// Is an axis of this transform gizmo currently selected?
			SNMTransformGizmo *pSNMTransformGizmo = static_cast<SNMTransformGizmo*>(pSceneNodeModifer);
			if (pSNMTransformGizmo->GetSelected())
				return pSNMTransformGizmo;	// A transform gizmo axis is currently selected
		}
	}

	// No transform gizmo axis is currently selected
	return nullptr;
}

/**
*  @brief
*    Sets whether a transform gizmo is currently in transform mode or not
*/
void GuiPicking::SetTransformMode(SNMTransformGizmo &cTransformGizmo, bool bTransformMode)
{
	// State change?
	if (cTransformGizmo.IsTransformMode() != bTransformMode) {
		{ // Enable/disable the camera
			SceneNode *pSceneNode = reinterpret_cast<SceneNode*>(m_pGui->GetApplication().GetCamera());
			if (pSceneNode)
				pSceneNode->SetActive(!bTransformMode);
		}

		// Loop through all scene node modifiers and disable the transform mode
		for (uint32 i=0; i<m_lstSceneNodeModifiers.GetNumOfElements(); i++) {
			// Get the current scene node modifier
			SceneNodeModifier *pSceneNodeModifer = m_lstSceneNodeModifiers[i];
			if (pSceneNodeModifer->IsInstanceOf("PLScene::SNMTransformGizmo")) {
				static_cast<SNMTransformGizmo*>(pSceneNodeModifer)->SetTransformMode(false);
				pSceneNodeModifer->SetActive(true);
			}
		}

		// Enable the transform mode of the given transform gizmo
		if (bTransformMode) {
			cTransformGizmo.SetTransformMode(true);

			// Hide all other transform gizmos
			for (uint32 i=0; i<m_lstSceneNodeModifiers.GetNumOfElements(); i++) {
				// Get the current scene node modifier
				SceneNodeModifier *pSceneNodeModifer = m_lstSceneNodeModifiers[i];
				if (pSceneNodeModifer != &cTransformGizmo && pSceneNodeModifer->IsInstanceOf("PLScene::SNMTransformGizmo"))
					pSceneNodeModifer->SetActive(false);
			}
		} else {
			// When stopping a transform, unselect all axis
			// -> The scene node the transform gizmo scene node modifier is attached to may have
			//    been moved outside the visible area
			// -> To handle this case we now have to perform a new axis selection
			cTransformGizmo.SetSelected(0);
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::Picking functions           ]
//[-------------------------------------------------------]
bool GuiPicking::OnPickingCandidate(SceneNode &cSceneNode)
{
	// Is this scene node visible? If so, we allow picking of it...
	return cSceneNode.IsVisible();
}
