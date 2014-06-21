/*********************************************************\
 *  File: TabBarButton.h                                 *
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


#ifndef __PLGUI_TABBARBUTTON_H__
#define __PLGUI_TABBARBUTTON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Widgets/Base/AbstractButton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tab bar button
*/
class TabBarButton : public AbstractButton {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, TabBarButton, "PLGui", PLGui::AbstractButton, "Tab bar button")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class TabBar;
	friend class TabBarEntry;


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
		PLGUI_API TabBarButton(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TabBarButton();

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
		*    Get TabBarEntry
		*
		*  @return
		*    TabBarEntry
		*/
		PLGUI_API TabBarEntry *GetTabBarEntry() const;

		/**
		*  @brief
		*    Get tab-bar function
		*
		*  @return
		*    Function
		*/
		PLGUI_API ETabBarFunction GetFunction() const;


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
		*    Set TabBarEntry
		*
		*  @param[in] pEntry
		*    TabBarEntry
		*/
		PLGUI_API void SetTabBarEntry(TabBarEntry *pEntry);

		/**
		*  @brief
		*    Set tab-bar function
		*
		*  @param[in] nFunction
		*    Function
		*/
		PLGUI_API void SetFunction(ETabBarFunction nFunction);


	//[-------------------------------------------------------]
	//[ Protected virtual Widget functions                    ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnMouseEnter();
		virtual void OnMouseLeave();


	//[-------------------------------------------------------]
	//[ Private virtual AbstractButton functions              ]
	//[-------------------------------------------------------]
	private:
		virtual void OnButtonClicked();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Internal data (only to be set from TabBar itself!)
		TabBar			  *m_pTabBar;		/**< TabBar to which the entry belongs */
		TabBarEntry		  *m_pTabBarEntry;	/**< TabBarEntry to which the entry belongs */
		ETabBarFunction	   m_nFunction;		/**< TabBar function */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TABBARBUTTON_H__
