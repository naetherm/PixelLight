/*********************************************************\
 *  File: ModSnap.h                                      *
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


#ifndef __PLGUI_MODSNAP_H__
#define __PLGUI_MODSNAP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include "PLGui/Modifiers/Modifier.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Modifier that snaps a widget to window borders
*/
class ModSnap : public Modifier {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, ModSnap, "PLGui", PLGui::Modifier, "Modifier that snaps a widget to window borders")
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
		PLGUI_API ModSnap();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ModSnap();


	//[-------------------------------------------------------]
	//[ Protected virtual Modifier functions                  ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnAttach(Widget &cWidget) override;
		virtual void OnDetach(Widget &cWidget) override;


	//[-------------------------------------------------------]
	//[ Protected virtual WidgetFunctions functions           ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnMove(const PLMath::Vector2i &vPos) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Try to unsnap the widget from it's current snap-targets
		*
		*  @param[in] vPos
		*    Position
		*/
		void TryUnsnap(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Try to snap to a specific widget
		*
		*  @param[in] vPos
		*    Position
		*  @param[in] pSnapWidget
		*    Widget to snap to, can be either the parent or a sibling widget
		*  @param[in] bParent
		*    'true', if pWidget is the parent window of this widget, else 'false'
		*/
		void TrySnap(const PLMath::Vector2i &vPos, Widget *pSnapWidget, bool bParent);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Event handlers
		PLCore::EventHandler<const PLMath::Vector2i&> EventHandlerMove;		/**< Widget gets moved, new widget position as parameter */

		// State
		PLMath::Vector2i  m_vPos;			/**< Current position */
		Widget			 *m_pSnappedLeft;	/**< Widget to which we are currently snapped on our left side */
		Widget			 *m_pSnappedRight;	/**< Widget to which we are currently snapped on our right side */
		Widget			 *m_pSnappedTop;	/**< Widget to which we are currently snapped on our top side */
		Widget			 *m_pSnappedBottom;	/**< Widget to which we are currently snapped on our bottom side */
		bool			  m_bPerformedSnap;	/**< Widget has performed a snap in the last cycle */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MODSNAP_H__
