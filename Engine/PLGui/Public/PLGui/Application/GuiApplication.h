/*********************************************************\
 *  File: GuiApplication.h                              *
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


#ifndef __PLGUI_GUI_APPLICATION_H__
#define __PLGUI_GUI_APPLICATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Application/CoreApplication.h>
#include <PLCore/Base/Event/EventHandler.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Widget;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Gui application class
*
*  @remarks
*    An application class for a typical GUI application. Runs a
*    main loop that does the necessary message processing and
*    provides methods to manage a main application window.
*/
class GuiApplication : public PLCore::CoreApplication {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, GuiApplication, "PLGui", PLCore::CoreApplication, "Gui application class")
		#ifdef PLGUI_EXPORTS	// The following is only required when compiling PLGui
			// Constructors
			pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
			// Methods
			pl_method_0(GetMainWindow,	pl_ret_type(Widget*),				"Get the main window. Returns pointer to the main window of the application, a null pointer on error.",						"")
			pl_method_1(SetMainWindow,	pl_ret_type(void),		Widget*,	"Set the main window, pointer to the main window of the application (a null pointer is also valid) as first parameter.",	"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sGuiFilename
		*    Filename of GUI to load, can be empty
		*/
		PLGUI_API GuiApplication(const PLCore::String &sGuiFilename = "");

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~GuiApplication();

		/**
		*  @brief
		*    Get main window
		*
		*  @return
		*    Main window, can be a null pointer
		*/
		PLGUI_API Widget *GetMainWindow() const;

		/**
		*  @brief
		*    Set main window
		*
		*  @param[in] pMainWindow
		*    Pointer to the main window of the application (a null pointer is also valid)
		*/
		PLGUI_API void SetMainWindow(Widget *pMainWindow);


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
		*    - Everything that CoreApplication::OnStart() does
		*    - Call OnCreateMainWindow()
		*    - Return and go on with OnInit()
		*/
		PLGUI_API virtual bool OnStart() override;

		/**
		*  @brief
		*    De-initialization function that is called after OnDeInit()
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that CoreApplication::OnStop() does
		*    - De-initialize system GUI
		*/
		PLGUI_API virtual void OnStop() override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::CoreApplication functions   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Main function
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Run GUI main loop (processing GUI messages)
		*    - Exit loop when either the GUI or the application has been stopped
		*/
		PLGUI_API virtual void Main() override;


	//[-------------------------------------------------------]
	//[ Protected virtual GuiApplication functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when application should open it's main window
		*
		*  @remarks
		*    Use this function to create a main window and set as pointer to it using SetMainWindow().
		*    The default implementation does the following tasks:
		*    - If a GUI filename is provided, create a window by loading that file
		*    - Otherwise, create an empty top-level window
		*/
		PLGUI_API virtual void OnCreateMainWindow();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when main window was destroyed
		*/
		void OnDestroyMainWindow();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Event handlers
		PLCore::EventHandler<> EventHandlerOnDestroy;

		// Data
		Widget *m_pMainWindow;	/**< Main window of the application (can be a null pointer) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GUI_APPLICATION_H__
