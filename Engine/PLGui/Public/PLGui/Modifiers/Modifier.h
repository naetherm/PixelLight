/*********************************************************\
 *  File: Modifier.h                                     *
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


#ifndef __PLGUI_MODIFIER_H__
#define __PLGUI_MODIFIER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLGui/Gui/Base/WidgetFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Widget;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Modifier base class
*/
class Modifier : public PLCore::Object, public WidgetFunctions {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, Modifier, "PLGui", PLCore::Object, "Modifier base class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Widget;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API Modifier();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Modifier();

		/**
		*  @brief
		*    Return the widget to which the modifier belongs
		*
		*  @return
		*    Widget that contains the modifier
		*/
		PLGUI_API Widget *GetWidget() const;

		/**
		*  @brief
		*    Get name of modifier
		*
		*  @return
		*    Name
		*/
		PLGUI_API PLCore::String GetName() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Set name of modifier
		*
		*  @param[in] sName
		*    Name
		*/
		PLGUI_API void SetName(const PLCore::String &sName);

		/**
		*  @brief
		*    Attach the modifier to a widget
		*
		*  @param[in] cWidget
		*    Widget to which the modifier is attached
		*/
		PLGUI_API void Attach(Widget &cWidget);

		/**
		*  @brief
		*    Detach the modifier
		*/
		PLGUI_API void Detach();


	//[-------------------------------------------------------]
	//[ Protected virtual Modifier functions                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the modifier is attached to a widget
		*
		*  @param[in] cWidget
		*    Widget to which the modifier is attached
		*/
		virtual void OnAttach(Widget &cWidget);

		/**
		*  @brief
		*    Called when the modifier is detached from a widget
		*
		*  @param[in] cWidget
		*    Widget from which the modifier is detached
		*/
		virtual void OnDetach(Widget &cWidget);


	//[-------------------------------------------------------]
	//[ Protected virtual WidgetFunctions functions           ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnMessage(const GuiMessage &cMessage);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Widget		   *m_pWidget;	/**< Widget to which the modifier belongs */
		PLCore::String  m_sName;	/**< Name of modifier */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MODIFIER_H__
