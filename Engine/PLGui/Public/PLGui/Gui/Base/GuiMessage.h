/*********************************************************\
 *  File: GuiMessage.h                                   *
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


#ifndef __PLGUI_GUIMESSAGE_H__
#define __PLGUI_GUIMESSAGE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Widget;
class Graphics;
class DataObject;
class Timer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    GUI message/event
*/
class GuiMessage {


	//[-------------------------------------------------------]
	//[ Public named constructors                             ]
	//[-------------------------------------------------------]
	public:
		static PLGUI_API GuiMessage OnWakeup();
		static PLGUI_API GuiMessage OnExit();
		static PLGUI_API GuiMessage OnTimer(Timer *pTimer);
		static PLGUI_API GuiMessage OnUserMessage(Widget *pWidget, PLCore::uint32 nData, void *pData);
		static PLGUI_API GuiMessage OnInternalMessage(PLCore::uint32 nData, PLCore::uint32 nExtData);
		static PLGUI_API GuiMessage OnThemeChanged(Widget *pWidget);
		static PLGUI_API GuiMessage OnUpdateContent(Widget *pWidget);
		static PLGUI_API GuiMessage OnUpdateChildWidget(Widget *pWidget, Widget *pChildWidget);
		static PLGUI_API GuiMessage OnAddChildWidget(Widget *pWidget, Widget *pChildWidget);
		static PLGUI_API GuiMessage OnRemoveChildWidget(Widget *pWidget, Widget *pChildWidget);
		static PLGUI_API GuiMessage OnClose(Widget *pWidget);
		static PLGUI_API GuiMessage OnCreate(Widget *pWidget);
		static PLGUI_API GuiMessage OnDestroy(Widget *pWidget);
		static PLGUI_API GuiMessage OnShow(Widget *pWidget);
		static PLGUI_API GuiMessage OnHide(Widget *pWidget);
		static PLGUI_API GuiMessage OnEnable(Widget *pWidget);
		static PLGUI_API GuiMessage OnDisable(Widget *pWidget);
		static PLGUI_API GuiMessage OnGetFocus(Widget *pWidget);
		static PLGUI_API GuiMessage OnLooseFocus(Widget *pWidget);
		static PLGUI_API GuiMessage OnActivate(Widget *pWidget, bool bActivate);
		static PLGUI_API GuiMessage OnDrawBackground(Widget *pWidget, Graphics *pGraphics);
		static PLGUI_API GuiMessage OnDraw(Widget *pWidget, Graphics *pGraphics);
		static PLGUI_API GuiMessage OnMove(Widget *pWidget, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnSize(Widget *pWidget, const PLMath::Vector2i &vSize);
		static PLGUI_API GuiMessage OnWindowState(Widget *pWidget, EWindowState nWindowState);
		static PLGUI_API GuiMessage OnEnterFullscreen(Widget *pWidget);
		static PLGUI_API GuiMessage OnLeaveFullscreen(Widget *pWidget);
		static PLGUI_API GuiMessage OnPreferredSize(Widget *pWidget, const PLMath::Vector2i &vSize);
		static PLGUI_API GuiMessage OnAdjustContent(Widget *pWidget);
		static PLGUI_API GuiMessage OnMouseEnter(Widget *pWidget);
		static PLGUI_API GuiMessage OnMouseLeave(Widget *pWidget);
		static PLGUI_API GuiMessage OnMouseOver(Widget *pWidget, bool bMouseOver);
		static PLGUI_API GuiMessage OnMouseMove(Widget *pWidget, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseHover(Widget *pWidget);
		static PLGUI_API GuiMessage OnMousePosUpdate(Widget *pWidget, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonDown(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonUp(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonClick(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonDoubleClick(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseWheel(Widget *pWidget, int nDelta);
		static PLGUI_API GuiMessage OnKeyDown(Widget *pWidget, PLCore::uint32 nKey, PLCore::uint32 nModifiers);
		static PLGUI_API GuiMessage OnKeyUp(Widget *pWidget, PLCore::uint32 nKey, PLCore::uint32 nModifiers);
		static PLGUI_API GuiMessage OnHotkey(Widget *pWidget, PLCore::uint32 nHotkey);
		static PLGUI_API GuiMessage OnDrop(Widget *pWidget, DataObject *pDataObject);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pWidget
		*    Message target
		*  @param[in] nType
		*    Message type
		*/
		PLGUI_API GuiMessage(Widget *pWidget = nullptr, EMessageType nType = MessageOnUnknown);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cOther
		*    GUI message
		*/
		PLGUI_API GuiMessage(const GuiMessage &cOther);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~GuiMessage();

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cOther
		*    GUI message
		*
		*  @return
		*    'true' if equal, else 'false'
		*/
		PLGUI_API bool operator ==(const GuiMessage &cOther) const;

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cOther
		*    GUI message
		*
		*  @return
		*    Reference to this object
		*/
		PLGUI_API GuiMessage &operator =(const GuiMessage &cOther);

		/**
		*  @brief
		*    Get message representation as string
		*
		*  @return
		*    Message as string
		*/
		PLGUI_API PLCore::String ToString() const;

		/**
		*  @brief
		*    Get message target
		*
		*  @return
		*    Widget
		*/
		inline Widget *GetWidget() const {
			return m_pWidget;
		}

		/**
		*  @brief
		*    Get message type
		*
		*  @return
		*    Message type
		*/
		inline EMessageType GetType() const {
			return m_nType;
		}

		/**
		*  @brief
		*    Get message data
		*
		*  @return
		*    Message data
		*/
		inline PLCore::uint32 GetData() const {
			return m_nData;
		}

		/**
		*  @brief
		*    Get delta
		*
		*  @return
		*    Delta value
		*/
		inline int GetDelta() const {
			return m_nDelta;
		}

		/**
		*  @brief
		*    Get window state
		*
		*  @return
		*    Window state
		*/
		inline EWindowState GetWindowState() const {
			return m_nWindowState;
		}

		/**
		*  @brief
		*    Get mouse button
		*
		*  @return
		*    Mouse button
		*/
		inline EMouseButton GetMouseButton() const {
			return m_nMouseButton;
		}

		/**
		*  @brief
		*    Get key
		*
		*  @return
		*    Key
		*/
		inline PLCore::uint32 GetKey() const {
			return m_nKey;
		}

		/**
		*  @brief
		*    Get boolean state
		*
		*  @return
		*    State
		*/
		inline bool GetState() const {
			return m_bState;
		}

		/**
		*  @brief
		*    Get data pointer
		*
		*  @return
		*    Data pointer
		*/
		inline void *GetDataPointer() const {
			return m_pDataPtr;
		}

		/**
		*  @brief
		*    Get secondary data
		*
		*  @return
		*    Message data
		*/
		inline PLCore::uint32 GetExtData() const {
			return m_nExtData;
		}

		/**
		*  @brief
		*    Get data object
		*
		*  @return
		*    Data pointer
		*/
		inline DataObject *GetDataObject() const {
			return m_pDataObject;
		}

		/**
		*  @brief
		*    Get child widget
		*
		*  @return
		*    Widget
		*/
		inline Widget *GetChildWidget() const {
			return m_pChildWidget;
		}

		/**
		*  @brief
		*    Get graphics object
		*
		*  @return
		*    Graphics object
		*/
		inline Graphics *GetGraphics() const {
			return m_pGraphics;
		}

		/**
		*  @brief
		*    Get timer object
		*
		*  @return
		*    Timer object
		*/
		inline Timer *GetTimer() const {
			return m_pTimer;
		}

		/**
		*  @brief
		*    Get modifier keys
		*
		*  @return
		*    Modifiers
		*/
		inline const PLCore::uint32 GetModifiers() const {
			return m_nModifiers;
		}

		/**
		*  @brief
		*    Get position or size
		*
		*  @return
		*    Vector
		*/
		inline const PLMath::Vector2i &GetPosSize() const {
			return m_vPosSize;
		}


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Message target
		Widget				*m_pWidget;			/**< Widget */

		// Message type
		EMessageType		 m_nType;			/**< Message type */

		// First message data
		union {
			PLCore::uint32	 m_nData;			/**< Message data */
			int				 m_nDelta;			/**< Movement delta */
			EWindowState	 m_nWindowState;	/**< Window state */
			EMouseButton	 m_nMouseButton;	/**< Mouse button */
			PLCore::uint32	 m_nKey;			/**< Key code */
			bool			 m_bState;			/**< Boolean state */
		};

		// Second message data
		union {
			void			*m_pDataPtr;		/**< Message data pointer */
			PLCore::uint32	 m_nExtData;		/**< Secondary data */
			DataObject		*m_pDataObject;		/**< Data object */
			Widget			*m_pChildWidget;	/**< Child widget */
			Graphics		*m_pGraphics;		/**< Graphics object */
			Timer			*m_pTimer;			/**< Timer object */
			PLCore::uint32	 m_nModifiers;		/**< Key modifiers */
		};

		// Position or size data
		PLMath::Vector2i	 m_vPosSize;		/**< Position or size */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GUIMESSAGE_H__
