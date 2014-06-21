/*********************************************************\
 *  File: GuiAndroid.h                                   *
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


#ifndef __PLGUI_GUIANDROID_H__
#define __PLGUI_GUIANDROID_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include "PLGui/Backend/Null/GuiNull.h"


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
struct AInputEvent;
typedef struct AInputEvent AInputEvent;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Android GUI implementation
*
*  @remarks
*    Mouse emulation by using the touchscreen is supported. When moving around the finger on the touchscreen,
*    the change in movement is used for the mouse axis. A short touch without any movement is handled
*    as "left mouse button clicked". As soon the mouse is moved, no "left mouse button"-events can be
*    generated anymore during the current touch. When touching without movement for half a second, the
*    emulation changes into "left mouse button hold down"-mode, following mouse movement will not change
*    this mode.
*/
class GuiAndroid : public GuiNull {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pGui
		*    Platform independent GUI
		*/
		PLGUI_API GuiAndroid(Gui *pGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~GuiAndroid();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called to process the next Android input event
		*
		*  @param[in] cAInputEvent
		*    Android input event to process
		*/
		void OnInputEvent(const struct AInputEvent &cAInputEvent);

		/**
		*  @brief
		*    Call this to process the next key input event
		*
		*  @param[in] cAKeyInputEvent
		*    Key input event to process
		*/
		void OnKeyInputEvent(const struct AInputEvent &cAKeyInputEvent);

		/**
		*  @brief
		*    Get PLGui key for Android key code
		*
		*  @param[in] int32_t
		*    Key code
		*
		*  @return
		*    Corresponding PLGui key, null if Android key code is invalid
		*/
		PLCore::uint32 GetKeyboardKey(int32_t nKeyCode);

		/**
		*  @brief
		*    Call this to process the next motion input event
		*
		*  @param[in] cAMotionInputEvent
		*    Motion input event to process
		*/
		void OnMotionInputEvent(const struct AInputEvent &cAMotionInputEvent);


	//[-------------------------------------------------------]
	//[ Private slots                                         ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<const struct AInputEvent&> SlotInputEvent;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Mouse emulation by using the touchscreen
		bool  m_bMouseMoved;				/**< Mouse moved during the current touch? */
		float m_fPreviousMousePositionX;	/**< Previous mouse x position */
		float m_fPreviousMousePositionY;	/**< Previous mouse y position */
		float m_fPreviousMousePressure;		/**< Previous mouse pressure */
		float m_fMousePositionX;			/**< Current mouse x position */
		float m_fMousePositionY;			/**< Current mouse y position */
		float m_fMousePressure;				/**< Current mouse pressure */
		bool  m_bLeftMouseButton;			/**< Is the left mouse button currently down? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GUIANDROID_H__
