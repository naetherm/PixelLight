/*********************************************************\
 *  File: Frontend.h                                     *
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


#ifndef __PLFRONTENDPLGUI_FRONTEND_H__
#define __PLFRONTENDPLGUI_FRONTEND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLCore/Frontend/FrontendImpl.h>
#include <PLMath/Vector2i.h>
#include "PLFrontendPLGui/PLFrontendPLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class Widget;
	class Graphics;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PLGui frontend implementation class
*
*  @remarks
*    An frontend class for a typical GUI application. Runs a
*    main loop that does the necessary message processing and
*    provides methods to manage a main application window.
*/
class Frontend : public PLCore::FrontendImpl {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLFRONTENDPLGUI_RTTI_EXPORT, Frontend, "PLFrontendPLGui", PLCore::FrontendImpl, "PLGui frontend implementation class")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLFRONTENDPLGUI_API Frontend();

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~Frontend();

		/**
		*  @brief
		*    Get main window
		*
		*  @return
		*    Main window, can be a null pointer
		*/
		PLFRONTENDPLGUI_API PLGui::Widget *GetMainWindow() const;

		/**
		*  @brief
		*    Set main window
		*
		*  @param[in] pMainWindow
		*    Pointer to the main window of the application (a null pointer is also valid)
		*/
		PLFRONTENDPLGUI_API void SetMainWindow(PLGui::Widget *pMainWindow);


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::AbstractLifecycle functions ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnCreate() override;	// Automatically called from within "Frontend::Run()"


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::FrontendImpl functions      ]
	//[-------------------------------------------------------]
	protected:
		virtual int Run(const PLCore::String &sExecutableFilename, const PLCore::Array<PLCore::String> &lstArguments) override;
		virtual PLCore::handle GetNativeWindowHandle() const override;
		virtual void Redraw() override;
		virtual void Ping() override;
		virtual PLCore::String GetTitle() const override;
		virtual void SetTitle(const PLCore::String &sTitle) override;
		virtual int GetX() const override;
		virtual int GetY() const override;
		virtual PLCore::uint32 GetWidth() const override;
		virtual PLCore::uint32 GetHeight() const override;
		virtual void GetWindowPositionSize(int &nX, int &nY, PLCore::uint32 &nWidth, PLCore::uint32 &nHeight) const override;
		virtual void SetWindowPositionSize(int nX, int nY, PLCore::uint32 nWidth, PLCore::uint32 nHeight) override;
		virtual bool GetToggleFullscreenMode() const override;
		virtual void SetToggleFullscreenMode(bool bToggleFullscreenMode) override;
		virtual bool GetFullscreenAltTab() const override;
		virtual void SetFullscreenAltTab(bool bAllowed) override;
		virtual bool IsFullscreen() const override;
		virtual void SetFullscreen(bool bFullscreen) override;
		virtual void RefreshFullscreen() override;
		virtual bool IsMouseOver() const override;
		virtual int GetMousePositionX() const override;
		virtual int GetMousePositionY() const override;
		virtual bool IsMouseVisible() const override;
		virtual void SetMouseVisible(bool bVisible) override;
		virtual void SetTrapMouse(bool bTrap) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Frontend functions                  ]
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
		*
		*  @note
		*    - "OnCreateMainWindow()": The created main window automatically activates itself and therefore is catching the focus
		*    - "OnCreateMainWindow()": Adds a "PLGui::ModClose" modifier with "ExitApplication=true" to the created main window,
		*      therefore the application will shut down as soon as clicking on 'x' within the window title
		*/
		PLFRONTENDPLGUI_API virtual void OnCreateMainWindow();


	//[-------------------------------------------------------]
	//[ Protected event handlers                              ]
	//[-------------------------------------------------------]
	protected:
		PLCore::EventHandler<>								 EventHandlerDestroyMainWindow;
		PLCore::EventHandler<bool>							 EventHandlerActivateMainWindow;
		PLCore::EventHandler<PLGui::Graphics&>				 EventHandlerDrawMainWindow;
		PLCore::EventHandler<const PLMath::Vector2i&>		 EventHandlerSizeMainWindow;
		PLCore::EventHandler<PLCore::uint32, PLCore::uint32> EventHandlerKeyDownMainWindow;
		PLCore::EventHandler<const PLGui::DataObject&>		 EventHandlerDropMainWindow;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		bool			  m_bInitialized;					/**< Initialization done? */
		PLGui::Widget	 *m_pMainWindow;					/**< Main window of the application (can be a null pointer) */
		PLCore::uint32	  m_nHotkeyIDAltTab;				/**< Alt-tab hotkey ID */
		bool			  m_bToggleFullscreenMode;			/**< Is it allowed to toggle the fullscreen mode using hotkeys? */
		bool			  m_bFullscreenAltTab;				/**< Is it allowed to use Alt-Tab within fullscreen mode? */
		bool			  m_bIsFullscreen;					/**< 'true' if the window is in fullscreen mode, else 'false' */
		bool			  m_bMainWindowPositionSizeBackup;	/**< Is there a main window position & size backup? */
		PLMath::Vector2i  m_vMainWindowPositionBackup;		/**< Main window position backup */
		PLMath::Vector2i  m_vMainWindowSizeBackup;			/**< Main window size backup */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Updates the Alt-Tab handling
		*/
		void UpdateAltTab();

		/**
		*  @brief
		*    Called when main window was destroyed
		*/
		void OnDestroyMainWindow();

		/**
		*  @brief
		*    Called when main window was (de-)activated
		*
		*  @param[in] bActivate
		*    'true' if window is activated, else 'false'
		*/
		void OnActivateMainWindow(bool bActivate);

		/**
		*  @brief
		*    Called when main window was drawn
		*
		*  @param[in] cGraphics
		*    Graphics object used for painting
		*/
		void OnDrawMainWindow(PLGui::Graphics &cGraphics);

		/**
		*  @brief
		*    Called when the main window gets resized
		*
		*  @param[in] vSize
		*    Size
		*/
		void OnSizeMainWindow(const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Called when a key is pressed down
		*
		*  @param[in] nKey
		*    Pressed key
		*  @param[in] nModifiers
		*    Modifier keys pressed
		*/
		void OnKeyDownMainWindow(PLCore::uint32 nKey, PLCore::uint32 nModifiers);

		/**
		*  @brief
		*    Called when data has been dropped onto the main window
		*
		*  @param[in] cData
		*    Data
		*/
		void OnDropMainWindow(const PLGui::DataObject &cData);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui


#endif // __PLFRONTENDPLGUI_FRONTEND_H__
