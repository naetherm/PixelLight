/*********************************************************\
 *  File: Application.h                                  *
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


#ifndef __PLVIEWER_APPLICATION_H__
#define __PLVIEWER_APPLICATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLEngine/Application/ScriptApplication.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLInput {
	class Control;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Application class
*/
class Application : public PLEngine::ScriptApplication {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Slots
		pl_slot_1_def(Application,	OnControl,	PLInput::Control&)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static const PLCore::String DefaultFilename;	/**< The used default resource loaded at start, if empty a file dialog will appear */


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
		*/
		Application(PLCore::Frontend &cFrontend);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Application();

		/**
		*  @brief
		*    Returns the filename of the currently loaded resource
		*
		*  @return
		*    The filename of the currently loaded resource
		*/
		PLCore::String GetResourceFilename() const;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::AbstractLifecycle functions    ]
	//[-------------------------------------------------------]
	public:
		virtual bool OnStart() override;


	//[-------------------------------------------------------]
	//[ Public virtual PLEngine::EngineApplication functions  ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadScene(const PLCore::String &sFilename) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a control event has occurred
		*
		*  @param[in] cControl
		*    Occurred control
		*/
		void OnControl(PLInput::Control &cControl);


	//[-------------------------------------------------------]
	//[ Protected virtual Application functions               ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Loads a resource which type has to be evaluated internally
		*
		*  @param[in] sFilename
		*    Filename of the resource to load
		*  @param[in] sType
		*    Loadable type ("Image", "Mesh" and so on) of the given resource, in case of an empty
		*    string the filename extension will be used to try to figure out the loadable type automatically
		*    (may not always work as expected because there are multiple loadable types with the same filename extension)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool LoadResource(const PLCore::String &sFilename, const PLCore::String &sType = "");

		/**
		*  @brief
		*    Sets the state text
		*
		*  @param[in] sText
		*    State text
		*
		*  @note
		*    - The default implementation is misusing the frontend title to communicate with the user
		*/
		virtual void SetStateText(const PLCore::String &sText);


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::CoreApplication functions   ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnInitLog() override;
		virtual void OnInit() override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::AbstractFrontend functions  ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnDrop(const PLCore::Container<PLCore::String> &lstFiles) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLEngine::EngineApplication functions ]
	//[-------------------------------------------------------]
	private:
		virtual void OnCreateScene(PLScene::SceneContainer &cContainer) override;
		virtual void OnCreateInputController() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sResourceFilename;	/**< The filename of the currently loaded resource */


};


#endif // __PLVIEWER_APPLICATION_H__
