/*********************************************************\
 *  File: TabBarEntry.h                                  *
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


#ifndef __PLGUI_TABBARENTRY_H__
#define __PLGUI_TABBARENTRY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Widgets/Widget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class TabBar;
class TabBarButton;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tab bar entry
*/
class TabBarEntry : public Widget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, TabBarEntry, "PLGui", PLGui::Widget, "Tab bar entry")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class TabBar;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Pointer to parent widget
		*/
		PLGUI_API TabBarEntry(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TabBarEntry();

		/**
		*  @brief
		*    Get TabBar
		*
		*  @return
		*    TabBar
		*/
		PLGUI_API TabBar *GetTabBar() const;

		/**
		*  @brief
		*    Get text
		*
		*  @return
		*    Text
		*/
		PLGUI_API PLCore::String GetText() const;

		/**
		*  @brief
		*    Set text
		*
		*  @param[in] sText
		*    Text
		*/
		PLGUI_API void SetText(const PLCore::String &sText);

		/**
		*  @brief
		*    Get icon
		*
		*  @return
		*    Image
		*/
		PLGUI_API const Image &GetIcon() const;

		/**
		*  @brief
		*    Set icon
		*
		*  @param[in] cIcon
		*    Image
		*/
		PLGUI_API void SetIcon(const Image &cIcon);

		/**
		*  @brief
		*    Get associated widget
		*
		*  @return
		*    Widget (can be a null pointer)
		*
		*  @remarks
		*    see SetWidget()
		*/
		PLGUI_API Widget *GetWidget() const;

		/**
		*  @brief
		*    Set associated widget
		*
		*  @param[in] pWidget
		*    Widget (can be a null pointer)
		*
		*  @remarks
		*    You can use this function to associate a widget with a tab entry.
		*    Note however that this is only a pure data slot and is not used by the
		*    TabBar at all, so the TabBar doesn't automatically show or hide widgets
		*    based on the currently selected tab. If you want functionality like that,
		*    you can use TabContainer which has implemented that already and provides
		*    a window container with an associated TabBar to select windows.
		*/
		PLGUI_API void SetWidget(Widget *pWidget);

		/**
		*  @brief
		*    Get associated data
		*
		*  @return
		*    Data pointer
		*
		*  @remarks
		*    see SetData()
		*/
		PLGUI_API void *GetUserData() const;

		/**
		*  @brief
		*    Set associated data
		*
		*  @param[in] pData
		*    Data pointer
		*
		*  @remarks
		*    You can use this function to associate arbitrary data with a tab entry.
		*    This is a pure user data that is not used by the TabBar itself.
		*/
		PLGUI_API void SetUserData(void *pData);

		/**
		*  @brief
		*    Get tab index
		*
		*  @return
		*    Index of tab
		*/
		PLGUI_API int GetIndex() const;

		/**
		*  @brief
		*    Check if tab can be closed by the user
		*
		*  @return
		*    'true' if tab is closable, else 'false'
		*/
		PLGUI_API bool IsClosable() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Set TabBar
		*
		*  @param[in] pTabBar
		*    TabBar
		*/
		PLGUI_API void SetTabBar(TabBar *pTabBar);

		/**
		*  @brief
		*    Set tab index
		*
		*  @param[in] nIndex
		*    Index of tab
		*/
		PLGUI_API void SetIndex(int nIndex);

		/**
		*  @brief
		*    Set if tab can be closed by the user
		*
		*  @param[in] bClosable
		*    'true' if tab is closable, else 'false'
		*/
		PLGUI_API void SetClosable(bool bClosable);


	//[-------------------------------------------------------]
	//[ Protected virtual Widget functions                    ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const;
		PLGUI_API virtual void OnAdjustContent();
		PLGUI_API virtual void OnMouseEnter();
		PLGUI_API virtual void OnMouseLeave();
		PLGUI_API virtual void OnMouseButtonClick(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Tab data
		PLCore::String     m_sText;			/**< Text displayed in tab */
		Image			   m_cIcon;			/**< Tab icon */
		Widget			  *m_pWidget;		/**< Widget associated with the tab (data only!) */
		void			  *m_pData;			/**< User data */

		// Internal data (only to be set from TabBar itself!)
		TabBar			  *m_pTabBar;		/**< TabBar to which the entry belongs */
		int				   m_nIndex;		/**< Tab index */
		bool			   m_bClosable;		/**< Can the tab be closed by the user? */

		// Child widgets
		TabBarButton	  *m_pButtonClose;	/**< Button to close the tab */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TABBARENTRY_H__
