/*********************************************************\
 *  File: LayoutBox.h                                    *
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


#ifndef __PLGUI_LAYOUT_LAYOUTBOX_H__
#define __PLGUI_LAYOUT_LAYOUTBOX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include "PLGui/Layout/Layout.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Box layout
*/
class LayoutBox : public Layout {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, LayoutBox, "PLGui", PLGui::Layout, "Box layout")
		// Attributes
		pl_attribute(Orientation,	pl_enum_type(EOrientation),	Horizontal,	ReadWrite,	GetSet,	"Box orientation",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLGUI_API EOrientation GetOrientation() const;
		PLGUI_API void SetOrientation(EOrientation nOrientation);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nOrientation
		*    Orientation
		*/
		PLGUI_API LayoutBox(EOrientation nOrientation = Horizontal);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~LayoutBox();


	//[-------------------------------------------------------]
	//[ Protected virtual Layout functions                    ]
	//[-------------------------------------------------------]
	protected:
		virtual void ApplyLayout() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Exported variables
		EOrientation	  m_nOrientation;	/**< Box orientation (horizontal or vertical) */

		// Internal data
		PLMath::Vector2i *m_pSizes;			/**< Data space used for calculations */
		PLCore::uint32	  m_nWidgets;		/**< Number of widgets that are in the layout */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_LAYOUT_LAYOUTBOX_H__
