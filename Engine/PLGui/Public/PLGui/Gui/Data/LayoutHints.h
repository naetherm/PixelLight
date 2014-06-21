/*********************************************************\
 *  File: LayoutHints.h                                  *
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


#ifndef __PLGUI_LAYOUTHINTS_H__
#define __PLGUI_LAYOUTHINTS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include <PLMath/Vector2i.h>
#include "PLGui/Gui/Data/SizeHint.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Layout information for a widget
*
*  @note
*    - Margin defines how much empty space (pixel) should be around an element
*    - Padding defines the how much empty space (pixel) should be within the borders of a container
*    - Spacing defines the how much empty space (pixel) should be between elements by default
*/
class LayoutHints {


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<>	EventUpdate;	/**< Layout hints have been changed */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API LayoutHints();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~LayoutHints();

		/**
		*  @brief
		*    Get desired size (X)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetSizeX() const;

		/**
		*  @brief
		*    Set desired size (X)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetSizeX(const SizeHint &cSize);

		/**
		*  @brief
		*    Get desired size (Y)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetSizeY() const;

		/**
		*  @brief
		*    Set desired size (Y)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetSizeY(const SizeHint &cSize);

		/**
		*  @brief
		*    Get minimum size (X)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMinSizeX() const;

		/**
		*  @brief
		*    Set minimum size (X)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMinSizeX(const SizeHint &cSize);

		/**
		*  @brief
		*    Get minimum size (Y)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMinSizeY() const;

		/**
		*  @brief
		*    Set minimum size (Y)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMinSizeY(const SizeHint &cSize);

		/**
		*  @brief
		*    Get maximum size (X)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMaxSizeX() const;

		/**
		*  @brief
		*    Set maximum size (X)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMaxSizeX(const SizeHint &cSize);

		/**
		*  @brief
		*    Get maximum size (Y)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMaxSizeY() const;

		/**
		*  @brief
		*    Set maximum size (Y)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMaxSizeY(const SizeHint &cSize);

		/**
		*  @brief
		*    Get margin (left)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMarginLeft() const;

		/**
		*  @brief
		*    Set margin (left)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMarginLeft(const SizeHint &cSize);

		/**
		*  @brief
		*    Get margin (right)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMarginRight() const;

		/**
		*  @brief
		*    Set margin (right)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMarginRight(const SizeHint &cSize);

		/**
		*  @brief
		*    Get margin (top)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMarginTop() const;

		/**
		*  @brief
		*    Set margin (top)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMarginTop(const SizeHint &cSize);

		/**
		*  @brief
		*    Get margin (bottom)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetMarginBottom() const;

		/**
		*  @brief
		*    Set margin (bottom)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetMarginBottom(const SizeHint &cSize);

		/**
		*  @brief
		*    Set margin (all)
		*
		*  @param[in] cSize
		*    Size hint
		*
		*  @remarks
		*    This function sets all margins (left, right, top and bottom) to the same value
		*/
		PLGUI_API void SetMargin(const SizeHint &cSize);

		/**
		*  @brief
		*    Get padding (left)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetPaddingLeft() const;

		/**
		*  @brief
		*    Set padding (left)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetPaddingLeft(const SizeHint &cSize);

		/**
		*  @brief
		*    Get padding (right)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetPaddingRight() const;

		/**
		*  @brief
		*    Set padding (right)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetPaddingRight(const SizeHint &cSize);

		/**
		*  @brief
		*    Get padding (top)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetPaddingTop() const;

		/**
		*  @brief
		*    Set padding (top)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetPaddingTop(const SizeHint &cSize);

		/**
		*  @brief
		*    Get padding (bottom)
		*
		*  @return
		*    Size hint
		*/
		PLGUI_API const SizeHint &GetPaddingBottom() const;

		/**
		*  @brief
		*    Set padding (bottom)
		*
		*  @param[in] cSize
		*    Size hint
		*/
		PLGUI_API void SetPaddingBottom(const SizeHint &cSize);

		/**
		*  @brief
		*    Set padding (all)
		*
		*  @param[in] cSize
		*    Size hint
		*
		*  @remarks
		*    This function sets all paddings (left, right, top and bottom) to the same value
		*/
		PLGUI_API void SetPadding(const SizeHint &cSize);

		/**
		*  @brief
		*    Get alignment of child elements (horizontal)
		*
		*  @return
		*    Alignment
		*/
		PLGUI_API EAlign GetAlign() const;

		/**
		*  @brief
		*    Set alignment of child elements (horizontal)
		*
		*  @param[in] nAlign
		*    Alignment
		*/
		PLGUI_API void SetAlign(EAlign nAlign);

		/**
		*  @brief
		*    Get alignment of child elements (vertical)
		*
		*  @return
		*    Alignment
		*/
		PLGUI_API EVAlign GetVAlign() const;

		/**
		*  @brief
		*    Set alignment of child elements (vertical)
		*
		*  @param[in] nAlign
		*    Alignment
		*/
		PLGUI_API void SetVAlign(EVAlign nAlign);

		/**
		*  @brief
		*    Get spacing between child elements
		*
		*  @return
		*    Spacing
		*/
		PLGUI_API PLCore::uint32 GetSpacing() const;

		/**
		*  @brief
		*    Set spacing between child elements
		*
		*  @param[in] nSpacing
		*    Spacing, default is 10 pixel
		*/
		PLGUI_API void SetSpacing(PLCore::uint32 nSpacing);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Update layout hints
		*/
		PLGUI_API void Update();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Size
		SizeHint		m_cSizeX;			/**< Desired size (X) */
		SizeHint		m_cSizeY;			/**< Desired size (Y) */
		SizeHint		m_cMinSizeX;		/**< Minimum size (X) */
		SizeHint		m_cMinSizeY;		/**< Minimum size (Y) */
		SizeHint		m_cMaxSizeX;		/**< Maximum size (X) */
		SizeHint		m_cMaxSizeY;		/**< Maximum size (Y) */
		SizeHint		m_cMarginLeft;		/**< Margin (left) */
		SizeHint		m_cMarginRight;		/**< Margin (right) */
		SizeHint		m_cMarginTop;		/**< Margin (top) */
		SizeHint		m_cMarginBottom;	/**< Margin (bottom) */
		SizeHint		m_cPaddingLeft;		/**< Padding (left) */
		SizeHint		m_cPaddingRight;	/**< Padding (right) */
		SizeHint		m_cPaddingTop;		/**< Padding (top) */
		SizeHint		m_cPaddingBottom;	/**< Padding (bottom) */
		EAlign			m_nAlign;			/**< Horizontal alignment of child elements */
		EVAlign			m_nVAlign;			/**< Vertical alignment of child elements */
		PLCore::uint32	m_nSpacing;			/**< Spacing between child elements */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_LAYOUTHINTS_H__
