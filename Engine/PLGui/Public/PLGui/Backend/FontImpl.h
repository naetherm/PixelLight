/*********************************************************\
 *  File: FontImpl.h                                     *
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


#ifndef __PLGUI_FONTIMPL_H__
#define __PLGUI_FONTIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/RefCount.h>
#include <PLCore/String/String.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Font;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Font implementation base class
*
*  @remarks
*    This class is the base class for platform specific font implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Font' abstraction
*/
class FontImpl : public PLCore::RefCount<FontImpl> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFont
		*    Reference to platform independent font
		*/
		PLGUI_API FontImpl(Font &cFont);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~FontImpl();


	//[-------------------------------------------------------]
	//[ Public virtual FontImpl functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load font
		*
		*  @param[in] sFamily
		*    Font family name
		*  @param[in] nHeight
		*    Font height
		*  @param[in] nWeight
		*    Font weight (@see EFontWeight)
		*  @param[in] nStyle
		*    Font style
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool LoadFont(const PLCore::String &sFamily, PLCore::uint32 nHeight, PLCore::uint32 nWeight, EFontStyle nStyle) = 0;

		/**
		*  @brief
		*    Load font
		*
		*  @param[in] sFilename
		*    Font filename
		*  @param[in] nHeight
		*    Font height
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool LoadFont(const PLCore::String &sFilename, PLCore::uint32 nHeight) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Font *m_pFont;		/**< Pointer to the platform independent font object */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_FONTIMPL_H__
