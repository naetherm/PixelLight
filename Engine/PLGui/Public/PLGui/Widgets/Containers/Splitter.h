/*********************************************************\
 *  File: Splitter.h                                     *
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


#ifndef __PLGUI_SPLITTER_H__
#define __PLGUI_SPLITTER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Widgets/Widget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Container widget that splits an area into two sides
*/
class Splitter : public Widget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, Splitter, "PLGui", PLGui::Widget, "Container widget that splits an area into two sides")
		// Attributes
		pl_attribute(Orientation,	pl_enum_type(EOrientation),	Horizontal,	ReadWrite,	GetSet,	"Orientation",							"")
		pl_attribute(Sizable,		bool,						true,		ReadWrite,	GetSet,	"Is the widget sizable?",				"")
		pl_attribute(SizeLeft,		SizeHint,					SizeHint(),	ReadWrite,	GetSet,	"Size of left (or top) container",		"")
		pl_attribute(SizeRight,		SizeHint,					SizeHint(),	ReadWrite,	GetSet,	"Size of right (or bottom) container",	"")
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
		PLGUI_API Splitter(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Splitter();

		/**
		*  @brief
		*    Get orientation
		*
		*  @return
		*    Orientation
		*/
		PLGUI_API EOrientation GetOrientation() const;

		/**
		*  @brief
		*    Set orientation
		*
		*  @param[in] nOrientation
		*    Orientation
		*/
		PLGUI_API void SetOrientation(EOrientation nOrientation);

		/**
		*  @brief
		*    Check if size can be altered by the user
		*
		*  @return
		*    'true' if sizable, else 'false'
		*/
		PLGUI_API bool GetSizable() const;

		/**
		*  @brief
		*    Set if size can be altered by the user
		*
		*  @param[in] bSizable
		*    'true' if sizable, else 'false'
		*/
		PLGUI_API void SetSizable(bool bSizable);

		/**
		*  @brief
		*    Get size of left (or top) side
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetSizeLeft() const;

		/**
		*  @brief
		*    Set size of left (or top) side
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetSizeLeft(const SizeHint &cSize);

		/**
		*  @brief
		*    Get size of right (or bottom) side
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetSizeRight() const;

		/**
		*  @brief
		*    Set size of right (or bottom) side
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetSizeRight(const SizeHint &cSize);

		/**
		*  @brief
		*    Get content widget of a given side
		*
		*  @param[in] nSide
		*    Side
		*
		*  @return
		*    Content widget
		*
		*  @remarks
		*    A splitter has exactly two content widgets.
		*    The first is retrieved with SideLeft or SideTop, the second with SideRight or SideBottom.
		*/
		PLGUI_API Widget *GetSide(ESide nSide) const;

		/**
		*  @brief
		*    Get content widget
		*
		*  @return
		*    Content widget
		*
		*  @remarks
		*    A splitter has actually two content widgets, this function returns the first one.
		*    See GetSide() for retrieving both content widgets.
		*/
		PLGUI_API virtual Widget *GetContentWidget() const;


	//[-------------------------------------------------------]
	//[ Public virtual Widget functions                       ]
	//[-------------------------------------------------------]
	public:
		PLGUI_API virtual void OnDraw(Graphics &cGraphics) override;
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const override;
		PLGUI_API virtual void OnAdjustContent() override;
		PLGUI_API virtual void OnMouseLeave() override;
		PLGUI_API virtual void OnMouseMove(const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonUp(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Change size of content widgets
		*
		*  @param[in] nDelta
		*    Movement delta (in pixels)
		*/
		PLGUI_API void ChangeSize(int nDelta);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Options
		EOrientation	  m_nOrientation;		/**< Orientation */
		bool			  m_bSizable;			/**< Is the widget sizable? */
		SizeHint		  m_cSizeLeft;			/**< Size of left (or top) container */
		SizeHint		  m_cSizeRight;			/**< Size of right (or bottom) container */

		// Internal data
		Widget			 *m_pContentLeft;		/**< Inner widget for the left/top side */
		Widget			 *m_pContentRight;		/**< Inner widget for the right/bottom side */
		PLMath::Vector2i  m_vMousePos;			/**< Current mouse position */
		bool			  m_bSizing;			/**< Are we currently changing the size? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_SPLITTER_H__
