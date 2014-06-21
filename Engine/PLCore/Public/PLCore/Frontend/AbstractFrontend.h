/*********************************************************\
 *  File: AbstractFrontend.h                             *
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


#ifndef __PLCORE_ABSTRACTFRONTEND_H__
#define __PLCORE_ABSTRACTFRONTEND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class ValueType> class Container;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract frontend class
*
*  @remarks
*    Please note that the frontend system is not designed to be a replacement for a decent GUI
*    system. In here, only primitive and commonly used GUI related feature are offered with a
*    limited feature set. For more complex stuff one has to use a real GUI system.
*/
class AbstractFrontend {


	//[-------------------------------------------------------]
	//[ Public virtual AbstractFrontend functions             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called when the window size has been changed
		*/
		virtual void OnSize() = 0;

		/**
		*  @brief
		*    Called when the fullscreen mode was changed
		*
		*  @remarks
		*    This method just says "something related to fullscreen mode has been changed". Whether we
		*    changed from window mode into fullscreen mode or changed e.g. the resolution used in
		*    fullscreen mode is not really interesting in here.
		*/
		virtual void OnFullscreenMode() = 0;

		/**
		*  @brief
		*    Called to let the frontend draw into it's window
		*/
		virtual void OnDraw() = 0;

		/**
		*  @brief
		*    Called to let the frontend update it's states
		*
		*  @remarks
		*    You can use this method to do work you have to perform on a regular basis. It's
		*    recommended to keep the work done within the implementation as compact as possible.
		*    Don't use this function to perform 'polling'-everything, use events or if required
		*    for example timers or threads instead.
		*/
		virtual void OnUpdate() = 0;

		/**
		*  @brief
		*    Called when string data has been dropped onto the frontend window
		*
		*  @param[in] lstFiles
		*    List of file names
		*/
		virtual void OnDrop(const Container<String> &lstFiles) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		AbstractFrontend();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AbstractFrontend();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_ABSTRACTFRONTEND_H__
