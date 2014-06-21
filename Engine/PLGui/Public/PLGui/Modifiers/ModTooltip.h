/*********************************************************\
 *  File: ModTooltip.h                                   *
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


#ifndef __PLGUI_MODTOOLTIP_H__
#define __PLGUI_MODTOOLTIP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    Modifier that displays a tooltip for a widget
*/
class ModTooltip : public Modifier {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, ModTooltip, "PLGui", PLGui::Modifier, "Modifier that displays a tooltip for a widget")
		// Attributes
		pl_attribute(Tooltip,	PLCore::String,	"",	ReadWrite,	GetSet,	"Tooltip text",	"")
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
		PLGUI_API ModTooltip();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ModTooltip();

		/**
		*  @brief
		*    Get tooltip text
		*
		*  @return
		*    Tooltip text
		*/
		PLGUI_API PLCore::String GetTooltip() const;

		/**
		*  @brief
		*    Set tooltip text
		*
		*  @param[in] sTooltip
		*    Tooltip text
		*/
		PLGUI_API void SetTooltip(const PLCore::String &sTooltip);


	//[-------------------------------------------------------]
	//[ Protected virtual ModTooltip functions                ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a tooltip shall be displayed
		*/
		virtual void OnShowTooltip();


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
		virtual void OnMouseHover() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String m_sTooltip;	/**< Tooltip text */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MODTOOLTIP_H__
