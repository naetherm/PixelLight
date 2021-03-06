/*********************************************************\
 *  File: ApplicationQt.h                                *
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


#ifndef __PLVIEWERQT_APPLICATIONQT_H__
#define __PLVIEWERQT_APPLICATIONQT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Application.h>	// Reused from "PLViewer"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
	class DockWidgetManager;
}
class Gui;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PLViewer application class with an added lightweight Qt GUI-layer
*/
class ApplicationQt : public Application {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Gui;
	friend class PLViewerQtConfig;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
	pl_class_def_end


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
		ApplicationQt(PLCore::Frontend &cFrontend);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ApplicationQt();

		/**
		*  @brief
		*    Returns the dock widget manager instance the application is using
		*
		*  @return
		*    The dock widget manager instance the application is using, can be a null pointer (do not destroy the returned instance)
		*/
		PLFrontendQt::DockWidgetManager *GetDockWidgetManager() const;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::AbstractLifecycle functions    ]
	//[-------------------------------------------------------]
	public:
		virtual bool OnStart() override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::AbstractFrontend functions  ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnUpdate() override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::CoreApplication functions     ]
	//[-------------------------------------------------------]
	private:
		virtual void OnInit() override;
		virtual void OnDeInit() override;


	//[-------------------------------------------------------]
	//[ Public virtual EngineApplication functions            ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadScene(const PLCore::String &sFilename) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLEngine::EngineApplication functions ]
	//[-------------------------------------------------------]
	private:
		virtual void OnLoadProgress(float fLoadProgress) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Application functions               ]
	//[-------------------------------------------------------]
	protected:
		virtual bool LoadResource(const PLCore::String &sFilename, const PLCore::String &sType = "") override;
		virtual void SetStateText(const PLCore::String &sText) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update color gradient background according to the configuration
		*/
		void UpdateColorGradientBackground();

		/**
		*  @brief
		*    Update backfaces and silhouettes visualization according to the configuration
		*/
		void UpdateBackfacesAndSilhouettes();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Gui  *m_pGui;			/**< GUI, can be a null pointer */
		float m_fLoadProgress;	/**< Current load progress (0.0-1.0) */


};


#endif // __PLVIEWERQT_APPLICATIONQT_H__
