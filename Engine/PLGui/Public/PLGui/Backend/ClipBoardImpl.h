/*********************************************************\
 *  File: ClipBoardImpl.h                                *
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


#ifndef __PLGUI_CLIPBOARDIMPL_H__
#define __PLGUI_CLIPBOARDIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Gui/Data/DataObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ClipBoard;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clipboard implementation base class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'ClipBoard' abstraction
*/
class ClipBoardImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ClipBoard;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cClipBoard
		*    Reference to platform independent clipboard
		*/
		PLGUI_API ClipBoardImpl(ClipBoard &cClipBoard);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ClipBoardImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual ClipBoardImpl functions             ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Get data from clipboard
		*
		*  @return
		*    Data
		*/
		virtual DataObject GetData() = 0;

		/**
		*  @brief
		*    Save data to clipboard
		*
		*  @param[in] cData
		*    Data
		*/
		virtual void SetData(const DataObject &cData) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		ClipBoard *m_pClipBoard;	/**< Pointer to the platform independent clipboard object */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_CLIPBOARDIMPL_H__
