/*********************************************************\
 *  File: Border.h                                       *
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


#ifndef __PLGUI_BORDER_H__
#define __PLGUI_BORDER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Containers/ContainerWidget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Container widget that displays a border around it's content
*/
class Border : public ContainerWidget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, Border, "PLGui", PLGui::ContainerWidget, "Container widget that displays a border around it's content")
		// Attributes
		pl_attribute(LineStyle,		pl_enum_type(ELineStyle),	SolidLine,					ReadWrite,	GetSet,	"Line style",	"")
		pl_attribute(BorderSize,	int,						1,							ReadWrite,	GetSet,	"Border size",	"")
		pl_attribute(BorderColor,	PLGraphics::Color4,			PLGraphics::Color4::Black,	ReadWrite,	GetSet,	"Border color",	"")
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
		*
		*  @param[in] pParent
		*    Pointer to parent widget
		*/
		PLGUI_API Border(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Border();

		/**
		*  @brief
		*    Get line style
		*
		*  @return
		*    Line style
		*/
		PLGUI_API ELineStyle GetLineStyle() const;

		/**
		*  @brief
		*    Set line style
		*
		*  @param[in] nLineStyle
		*    Line style
		*/
		PLGUI_API void SetLineStyle(ELineStyle nLineStyle);

		/**
		*  @brief
		*    Get border size
		*
		*  @return
		*    Border size
		*/
		PLGUI_API int GetBorderSize() const;

		/**
		*  @brief
		*    Set border size
		*
		*  @param[in] nSize
		*    Border size
		*/
		PLGUI_API void SetBorderSize(int nSize);

		/**
		*  @brief
		*    Get border color
		*
		*  @return
		*    Border color
		*/
		PLGUI_API const PLGraphics::Color4 &GetBorderColor() const;

		/**
		*  @brief
		*    Set border color
		*
		*  @param[in] cColor
		*    Border color
		*/
		PLGUI_API void SetBorderColor(const PLGraphics::Color4 &cColor);


	//[-------------------------------------------------------]
	//[ Public virtual Widget functions                       ]
	//[-------------------------------------------------------]
	public:
		PLGUI_API virtual void OnDraw(Graphics &cGraphics) override;
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const override;
		PLGUI_API virtual void OnAdjustContent() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	public:
		ELineStyle			m_nLineStyle;		/**< Line style */
		int					m_nBorderSize;		/**< Border size */
		PLGraphics::Color4	m_cBorderColor;		/**< Border color */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_BORDER_H__
