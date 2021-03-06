/*********************************************************\
 *  File: Gui.h                                          *
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


#ifndef __PLVIEWERQT_GUI_H__
#define __PLVIEWERQT_GUI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qobject.h>
#include <PLCore/Base/Event/EventHandler.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMenu;
	class QLabel;
	class QAction;
	class QMainWindow;
	class QActionGroup;
	class QFileSystemWatcher;
QT_END_NAMESPACE
namespace PLScene {
	class SceneNode;
	class SceneQuery;
}
namespace PLFrontendQt {
	class DockWidget;
	class FrontendMainWindow;
}
class GuiPicking;
class ApplicationQt;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Gui class
*/
class Gui : public QObject {


	//[-------------------------------------------------------]
	//[ Qt definitions (MOC)                                  ]
	//[-------------------------------------------------------]
	Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
				// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cApplication
		*    Owner application
		*/
		Gui(ApplicationQt &cApplication);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Gui();

		/**
		*  @brief
		*    Returns the owner application
		*
		*  @return
		*    The owner application
		*/
		ApplicationQt &GetApplication() const;

		/**
		*  @brief
		*    Get frontend main window
		*
		*  @return
		*    Frontend main window, can be a null pointer
		*/
		PLFrontendQt::FrontendMainWindow *GetFrontendMainWindow() const;

		/**
		*  @brief
		*    Sets whether or not the GUI is currently enabled
		*
		*  @param[in] bEnabled
		*    'true' to enable the GUI, else 'false'
		*/
		void SetEnabled(bool bEnabled);

		/**
		*  @brief
		*    Sets the state text
		*
		*  @param[in] sText
		*    State text
		*/
		void SetStateText(const PLCore::String &sText);

		/**
		*  @brief
		*    Opens a dialog in order to give the user a choice between multiple options
		*
		*  @param[in]  sTitle
		*    Title
		*  @param[in]  sText
		*    Reason for input text
		*  @param[in] lstOptions
		*    Options, empty strings will be ignored
		*
		*  @return
		*    The chosen option or empty string on abort
		*/
		PLCore::String InputDialog(const PLCore::String &sTitle, const PLCore::String &sText, const PLCore::Array<PLCore::String> &lstOptions) const;

		/**
		*  @brief
		*    Shows a dock widget
		*
		*  @param[in] sClassName
		*    Class name of the PixelLight RTTI class encapsulating a Qt dock widget (must be derived from "PLFrontendQt::DockWidget")
		*
		*  @return
		*    The shown dock widget, can be a null pointer (you don't need to care about destroying the dock widget instance)
		*
		*  @note
		*    - If there's already a shown dock widget of the given class name, it will get the focus
		*    - If there's already a dock widget of the given class name, it will be shown
		*    - If there's currently no dock widget of the given class name, it will be instanced and shown
		*/
		PLFrontendQt::DockWidget *ShowDockWidget(const PLCore::String &sClassName);

		/**
		*  @brief
		*    Updates the GUI
		*
		*  @note
		*    - Performs work which has to be done every frame, but this work is kept to a minimum
		*/
		void Update();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initializes the main window
		*
		*  @param[in] cQMainWindow
		*    Main window to initialize
		*/
		void InitMainWindow(QMainWindow &cQMainWindow);

		/**
		*  @brief
		*    Called when a camera scene node was found
		*
		*  @param[in] cQuery
		*    Query found the scene node
		*  @param[in] cSceneNode
		*    Found scene node
		*/
		void OnCameraFound(PLScene::SceneQuery &cQuery, PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Fills the window menu recursively
		*
		*  @param[in] cQMenu
		*    Current Qt menu to fill
		*  @param[in] sBaseClass
		*    Name of the currently used RTTI base class
		*
		*  @return
		*    Number of checked items
		*/
		PLCore::uint32 FillMenuWindowRec(QMenu &cQMenu, const PLCore::String &sBaseClass);

		/**
		*  @brief
		*    Resets and fills the Qt file system watcher instance
		*/
		void ResetAndFillQFileSystemWatcher();


	//[-------------------------------------------------------]
	//[ Private Qt slots (MOC)                                ]
	//[-------------------------------------------------------]
	private slots:
		void QtSlotFileChanged(const QString &path);
		void QtSlotTriggeredNew();
		void QtSlotTriggeredLoad();
		void QtSlotTriggeredReload();
		void QtSlotTriggeredAutomaticReload();
		void QtSlotTriggeredSave();
		void QtSlotTriggeredSaveAs();
		void QtSlotTriggeredExit();
		void QtSlotAboutToShowMenuCamera();
		void QtSlotTriggeredCameraSelectCurrentCamera();
		void QtSlotSelectedCamera(QAction *);
		void QtSlotAboutToShowMenuWindow();
		void QtSlotTriggeredWindowHideAll();
		void QtSlotSelectedWindow(QAction *);
		void QtSlotAboutToShowMenuTools();
		void QtSlotTriggeredOpenLogFile();
		void QtSlotTriggeredShowColorGradientBackground();
		void QtSlotTriggeredShowBackfacesAndSilhouettes();
		void QtSlotTriggeredReloadMaterials();
		void QtSlotTriggeredReloadScene();
		void QtSlotTriggeredOpenPixelLightWebsite();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<PLScene::SceneQuery &, PLScene::SceneNode &> EventHandlerCameraFound;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ApplicationQt		*m_pApplication;						/**< Owner application, always valid */
		GuiPicking			*m_pGuiPicking;							/**< GUI picking component instance, can be a null pointer */
		QFileSystemWatcher	*m_pQFileSystemWatcher;					/**< Qt file system watcher, always valid */
		// Menu bar
		QAction				*m_pQActionNew;							/**< Automatic reload Qt action, can be a null pointer */
		QAction				*m_pQActionReload;						/**< Reload Qt action, can be a null pointer */
		QAction				*m_pQActionAutomaticReload;				/**< Automatic reload Qt action, can be a null pointer */
		QAction				*m_pQActionSave;						/**< Save Qt action, can be a null pointer */
		QAction				*m_pQActionSaveAs;						/**< Save dialog Qt action, can be a null pointer */
		QMenu				*m_pQMenuCamera;						/**< Camera Qt menu, can be a null pointer */
		QActionGroup		*m_pQActionGroupCamera;					/**< Camera Qt action group, can be a null pointer */
		QMenu				*m_pQMenuWindow;						/**< Window Qt menu, can be a null pointer */
		QActionGroup		*m_pQActionGroupWindow;					/**< Window Qt action group, can be a null pointer */
		QAction				*m_pQActionShowColorGradientBackground;	/**< Show color gradient background Qt action, can be a null pointer */
		QAction				*m_pQActionShowBackfacesAndSilhouettes;	/**< Show backfaces and silhouettes Qt action, can be a null pointer */
		QAction				*m_pQActionReloadMaterials;				/**< Reload only materials Qt action, can be a null pointer */
		QAction				*m_pQActionReloadScene;					/**< Reload only scene Qt action, can be a null pointer */
		// Status bar
		QLabel				*m_pQLabelStatusBar;					/**< Qt label shown in the status bar of the Qt main window, can be a null pointer */


};


#endif // __PLVIEWERQT_GUI_H__
