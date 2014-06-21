/*********************************************************\
 *  File: ClipBoard.h                                    *
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


#ifndef __PLGUI_CLIPBOARD_H__
#define __PLGUI_CLIPBOARD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLGui/Gui/Data/DataObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;
class ClipBoardImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clipboard
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class ClipBoard {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Gui;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*/
		PLGUI_API ClipBoard(Gui &cGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~ClipBoard();

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object (never a null pointer)
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get implementation
		*
		*  @return
		*    Pointer to platform specific implementation
		*/
		PLGUI_API ClipBoardImpl *GetImpl() const;

		/**
		*  @brief
		*    Get data from clipboard
		*
		*  @return
		*    Data
		*/
		PLGUI_API DataObject GetData();

		/**
		*  @brief
		*    Save data to clipboard
		*
		*  @param[in] cData
		*    Data
		*/
		PLGUI_API void SetData(const DataObject &cData);

		/**
		*  @brief
		*    Get text from clipboard
		*
		*  @return
		*    String
		*/
		PLGUI_API PLCore::String GetText();

		/**
		*  @brief
		*    Save text to clipboard
		*
		*  @param[in] sText
		*    String
		*/
		PLGUI_API void SetText(const PLCore::String &sText);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Initialize clipboard (create backend)
		*/
		PLGUI_API void InitClipBoard();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui			  *m_pGui;		/**< Pointer to GUI */
		ClipBoardImpl *m_pImpl;		/**< Clipboard implementation */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_CLIPBOARD_H__
