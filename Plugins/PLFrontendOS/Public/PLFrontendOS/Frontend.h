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


#ifndef __PLFRONTENDOS_FRONTEND_H__
#define __PLFRONTENDOS_FRONTEND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Frontend/FrontendImpl.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendOS {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class OSWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OS frontend implementation class
*/
class Frontend : public PLCore::FrontendImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class OSWindowWindows;
	friend class OSWindowMacOSX;
	friend class OSWindowLinux;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, Frontend, "PLFrontendOS", PLCore::FrontendImpl, "OS frontend implementation class")
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
		Frontend();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Frontend();


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::FrontendImpl functions        ]
	//[-------------------------------------------------------]
	private:
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
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		OSWindow *m_pOSWindow;				/**< The OS specific window implementation, can be a null pointer */
		bool	  m_bQuit;					/**< Quit frontend? */
		bool	  m_bToggleFullscreenMode;	/**< Is it allowed to toggle the fullscreen mode using hotkeys? */
		bool	  m_bFullscreenAltTab;		/**< Is it allowed to use Alt-Tab within fullscreen mode? */
		bool	  m_bIsFullscreen;			/**< 'true' if the window is in fullscreen mode, else 'false' */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendOS


#endif // __PLFRONTENDOS_FRONTEND_H__
