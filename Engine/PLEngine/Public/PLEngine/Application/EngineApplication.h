/*********************************************************\
 *  File: EngineApplication.h                            *
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


#ifndef __PLENGINE_ENGINEAPPLICATION_H__
#define __PLENGINE_ENGINEAPPLICATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/TextureBuffer.h>
#include <PLScene/Scene/SceneNodeHandler.h>
#include <PLScene/Application/SceneApplication.h>
#include "PLEngine/Tools/Screenshot.h"
#include "PLEngine/Tools/SceneRendererTool.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLInput {
	class Controller;
	class VirtualController;
}
namespace PLScene {
	class SNCamera;
	class SceneQuery;
}
namespace PLEngine {
	class ConsoleCommand;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Basic scene application class
*
*  @remarks
*    An application class that provides a standard scene graph for usual 3D applications and offers functionality
*    to load in whole scenes at once as well as load screen handling and screenshot capturing.
*/
class EngineApplication : public PLScene::SceneApplication {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, EngineApplication, "PLEngine", PLScene::SceneApplication, "Basic scene application class")
		#ifdef PLENGINE_EXPORTS	// The following is only required when compiling PLEngine
			// Constructors
			pl_constructor_1(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
			// Methods
			pl_method_0(GetScene,				pl_ret_type(PLScene::SceneContainer*),										"Returns the scene container (the 'concrete scene'), can be a null pointer.",																								"")
			pl_method_1(SetScene,				pl_ret_type(void),							PLScene::SceneContainer*,		"Sets the scene container (the 'concrete scene'). New scene container as first parameter (can be a null pointer).",															"")
			pl_method_0(ClearScene,				pl_ret_type(void),															"Clears the scene, after calling this method the scene is empty.",																											"")
			pl_method_1(LoadScene,				pl_ret_type(bool),							const PLCore::String&,			"Loads a scene. Filename of the scene to load as first argument. Returns 'true' if all went fine, else 'false'. This method will completely replace the current scene.",	"")
			pl_method_0(GetCamera,				pl_ret_type(PLScene::SNCamera*),											"Get the scene camera, can be a null pointer.",																																"")
			pl_method_1(SetCamera,				pl_ret_type(void),							PLScene::SNCamera*,				"Sets the scene camera. New scene camera as first parameter (can be a null pointer).",																						"")
			pl_method_0(GetInputController,		pl_ret_type(PLInput::VirtualController*),									"Get the virtual input controller (can be a null pointer).",																												"")
			pl_method_1(SetInputController,		pl_ret_type(void),							PLInput::VirtualController*,	"Set the virtual input controller. Virtual input controller (can be a null pointer) as first parameter.",																	"")
			pl_method_0(GetSceneRendererTool,	pl_ret_type(SceneRendererTool&),											"Returns the scene renderer tool.",																																			"")
			pl_method_0(GetScreenshotTool,		pl_ret_type(Screenshot&),													"Returns the screenshot tool.",																																				"")
		#endif
		// Signals
		pl_signal_0(SignalCameraSet,			"A new camera has been set",					"")
		pl_signal_0(SignalSceneLoadingFinished,	"Scene loading has been finished successfully",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		PL_API static const PLCore::String DefaultSceneRenderer;	/**< The used default (and very basic) scene renderer */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontend
		*    Frontend this application instance is running in
		*  @param[in] sSceneFilename
		*    Filename of the scene to load
		*/
		PL_API EngineApplication(PLCore::Frontend &cFrontend, const PLCore::String &sSceneFilename = "");

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~EngineApplication();

		/**
		*  @brief
		*    Returns the scene container (the 'concrete scene')
		*
		*  @return
		*    Scene container, can be a null pointer
		*/
		PL_API PLScene::SceneContainer *GetScene() const;

		/**
		*  @brief
		*    Sets the scene container (the 'concrete scene')
		*
		*  @param[in] pContainer
		*    New scene container, can be a null pointer
		*/
		PL_API void SetScene(PLScene::SceneContainer *pContainer);

		/**
		*  @brief
		*    Clears the scene, after calling this method the scene is empty
		*/
		PL_API void ClearScene();

		/**
		*  @brief
		*    Get scene camera
		*
		*  @return
		*    Scene camera, can be a null pointer
		*/
		PL_API PLScene::SNCamera *GetCamera() const;

		/**
		*  @brief
		*    Get virtual input controller
		*
		*  @return
		*    Virtual input controller (can be a null pointer)
		*/
		PL_API PLInput::VirtualController *GetInputController() const;

		/**
		*  @brief
		*    Set virtual input controller
		*
		*  @param[in] pInputController
		*    Virtual input controller (can be a null pointer)
		*/
		PL_API void SetInputController(PLInput::VirtualController *pInputController);

		/**
		*  @brief
		*    Get scene renderer tool
		*
		*  @return
		*    Scene renderer tool instance
		*
		*  @remarks
		*    Use "GetSceneRendererTool()" for a simplified interface to the scene renderer. By writing for example
		*    "GetSceneRendererTool().SetPassAttribute("BackgroundBitmap", "Material", "Data/Textures/Background.dds");"
		*    one can usually (on standard scene renderer configurations) set directly a background bitmap.
		*
		*    This component is initialized within the application framework initialization function "OnStart()" that is called prior to "Main()".
		*    As a result, using the returned component instance prior to the application-specific initialization routine "OnInit()" will not
		*    work.
		*/
		PL_API SceneRendererTool &GetSceneRendererTool();

		/**
		*  @brief
		*    Get scene renderer tool
		*
		*  @return
		*    Scene renderer tool instance
		*
		*  @see
		*    - Non-constant GetSceneRendererTool()
		*/
		PL_API const SceneRendererTool &GetSceneRendererTool() const;

		/**
		*  @brief
		*    Get screenshot tool
		*
		*  @return
		*    Screenshot tool instance
		*
		*  @remarks
		*    This component is initialized within the application framework initialization function "OnStart()" that is called prior to "Main()".
		*    As a result, using the returned component instance prior to the application-specific initialization routine "OnInit()" will not
		*    work.
		*/
		PL_API Screenshot &GetScreenshotTool();

		//[-------------------------------------------------------]
		//[ Edit functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether or not edit mode is enabled
		*
		*  @return
		*    'true' if edit mode is enabled, else 'false'
		*
		*  @remarks
		*    This class introduces some generic edit features which are enabled by default. For public release
		*    versions you may disable the edit mode so users can't for example use edit features to chat.
		*/
		PL_API bool IsEditModeEnabled() const;

		/**
		*  @brief
		*    Sets whether or not edit mode is enabled
		*
		*  @param[in] bEnabled
		*    'true' if edit mode is enabled, else 'false'
		*
		*  @remarks
		*    Also the scene nodes 'SNEngineInformation0' and 'SNConsole0' from the root scene are enabled/disabled.
		*    By default, edit mode is enabled.
		*
		*  @see
		*    - IsEditModeEnabled()
		*/
		PL_API void SetEditModeEnabled(bool bEnabled = true);

		//[-------------------------------------------------------]
		//[ Console functions                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Quit the engine
		*
		*  @param[in] cCommand
		*    Calling command
		*/
		PL_API void ConsoleCommandQuit(ConsoleCommand &cCommand);


	//[-------------------------------------------------------]
	//[ Public virtual EngineApplication functions            ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets the scene camera
		*
		*  @param[in] pCamera
		*    New scene camera, can be a null pointer
		*
		*  @note
		*    - Deactivates automatically the current set camera and activates the new camera
		*    - Sets this camera also within the 'SPScene' surface painter instance
		*    - Emits the "SignalCameraSet"-signal
		*/
		PL_API virtual void SetCamera(PLScene::SNCamera *pCamera);

		/**
		*  @brief
		*    Loads a scene
		*
		*  @param[in] sFilename
		*    Filename of the scene to load
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    This function will use the current set scene container (the 'concrete scene') to load
		*    in given scene data. It will temporarily add a new base path so all scene data can be found
		*    even if the scene to load is not within the application directory.
		*    Supported 'SNKeyValue' information:
		*      Key                       Value
		*      'SceneRenderer'           'Class name of the scene renderer to use'
		*      'SceneRendererVariables'  '<Variable>=<Value>'
		*      'ClearColor'              '<red> <green> <blue> <alpha>' (all floating point values from 0-1)
		*      'StartCamera'             '<name of the start camera scene node>' (name is relative to the loaded scene container)
		*
		*  @note
		*    - If currently the edit dialog is opened, it will be closed automatically to avoid update problems
		*    - Emits the "SignalSceneLoadingFinished"-signal when the scene loading has been finished successfully
		*/
		PL_API virtual bool LoadScene(const PLCore::String &sFilename);


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::AbstractLifecycle functions ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Initialization function that is called prior to OnInit()
		*
		*  @return
		*    'true' if all went fine, else 'false' which will stop the application
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that PLRenderer::RendererApplication::OnStart() does (we reimplement the PLScene::SceneApplication::OnStart() order)
		*    - Initialize input system
		*    - Call OnCreateInputController()
		*    - Initialize scene renderer tool
		*    - Initialize screenshot tool
		*    - Create scene context
		*    - Call OnCreateRootScene()
		*    - Return and go on with OnInit()
		*/
		PL_API virtual bool OnStart() override;

		/**
		*  @brief
		*    Called when the object has the focus (keep the implementation lightweight)
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that PLScene::SceneApplication::OnResume() does
		*    - Activate the input controller
		*/
		PL_API virtual void OnResume() override;

		/**
		*  @brief
		*    Called when the object has no longer the focus (keep the implementation lightweight)
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that PLScene::SceneApplication::OnPause() does
		*    - Deactivate the input controller
		*/
		PL_API virtual void OnPause() override;

		/**
		*  @brief
		*    De-initialization function that is called after OnDeInit()
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Destroy input controller
		*    - Everything that PLScene::SceneApplication::OnStop() does
		*/
		PL_API virtual void OnStop() override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::AbstractFrontend functions  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called to let the frontend update it's states
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that PLScene::SceneApplication::OnUpdate() does
		*    - Update input manager
		*/
		PL_API virtual void OnUpdate() override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneApplication functions ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void OnCreateRootScene() override;


	//[-------------------------------------------------------]
	//[ Protected virtual EngineApplication functions         ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Function that is called to create the application's scene container
		*
		*  @param[in] cContainer
		*    Scene container where the 'concrete scene' should be created in
		*
		*  @note
		*    - Part of the application framework initialization function "OnStart()"
		*    - The default implementation creates an controllable camera and a simple mesh scene node
		*    - Called from within "PLScene::SceneApplication::OnCreateRootScene()"
		*/
		PL_API virtual void OnCreateScene(PLScene::SceneContainer &cContainer);

		/**
		*  @brief
		*    Function that is called to initialize the application's virtual input controller
		*
		*  @note
		*    - Part of the application framework initialization function "OnStart()"
		*    - In the default implementation, an instance of VirtualStandardController is created
		*/
		PL_API virtual void OnCreateInputController();

		/**
		*  @brief
		*    Function that is called when an input controller has been found
		*
		*  @param[in] pInputController
		*    Found input controller, always valid
		*  @param[in] sInputSemantic
		*    Purpose of this input controller
		*
		*  @remarks
		*    Use this virtual method for instance to connect the input controller to real input devices.
		*
		*  @note
		*    - Connected to the "PLInput::InputManager::EventInputControllerFound"-event
		*    - The default implementation tries to connect all controls automatically with the virtual standard controller
		*/
		PL_API virtual void OnInputControllerFound(PLInput::Controller *pInputController, PLCore::String sInputSemantic);

		/**
		*  @brief
		*    Called on load progress
		*
		*  @param[in] fLoadProgress
		*    Load progress (0.0-1.0)
		*
		*  @note
		*    - Calls "GetFrontend().RedrawAndPing()", but only if there's a load screen
		*/
		PL_API virtual void OnLoadProgress(float fLoadProgress);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLScene::SceneNodeHandler				   m_cSceneContainerHandler;	/**< Scene node handler for the scene container */
		PLScene::SceneNodeHandler				   m_cCameraHandler;			/**< Scene node handler for the camera */
		PLCore::String							   m_sDefaultSceneRenderer;		/**< Default scene renderer */
		PLCore::String							   m_sSceneRendererVariables;	/**< Scene renderer variables */
		PLCore::String							   m_sClearColor;				/**< Clear color */
		PLCore::String							   m_sStartCamera;				/**< Name of the given start camera */
		PLScene::SceneNode						  *m_pFirstFoundCamera;			/**< First found camera, can be a null pointer */
		bool									   m_bHasLoadScreen;			/**< Is there a load screen? */
		PLInput::VirtualController				  *m_pInputController;			/**< Virtual input controller, can be a null pointer */
		SceneRendererTool						   m_cSceneRendererTool;		/**< Scene renderer tool */
		Screenshot								   m_cScreenshot;				/**< Screenshot tool */
		bool									   m_bEditModeEnabled;			/**< Edit mode enabled? */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a scene node was found
		*
		*  @param[in] cQuery
		*    Query found the scene node
		*  @param[in] cSceneNode
		*    Found scene node
		*/
		void OnSceneNode(PLScene::SceneQuery &cQuery, PLScene::SceneNode &cSceneNode);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<PLScene::SceneQuery &, PLScene::SceneNode &>	EventHandlerSceneNode;
		PLCore::EventHandler<float>											EventHandlerLoadProgress;
		PLCore::EventHandler<PLInput::Controller*, PLCore::String>			EventHandlerInputControllerFound;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_ENGINEAPPLICATION_H__
