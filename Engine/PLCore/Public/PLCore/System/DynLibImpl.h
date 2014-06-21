/*********************************************************\
 *  File: DynLibImpl.h                                   *
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


#ifndef __PLCORE_DYNLIB_IMPL_H__
#define __PLCORE_DYNLIB_IMPL_H__
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
class Url;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific 'DynLib' implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'DynLib' abstraction
*/
class DynLibImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class DynLib;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		DynLibImpl();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DynLibImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual DynLibImpl functions                ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Returns if the dynamic library has been loaded
		*
		*  @return
		*    'true' if loaded, else 'false'
		*/
		virtual bool IsLoaded() const = 0;

		/**
		*  @brief
		*    Loads a dynamic library
		*
		*  @param[in] cUrl
		*    Path to the dynamic library
		*
		*  @return
		*    'true' if the library could be loaded, else false
		*/
		virtual bool Load(const Url &cUrl) = 0;

		/**
		*  @brief
		*    Get the absolute path to the dynamic library
		*
		*  @return
		*    The absolute path to the dynamic library (native path style)
		*/
		virtual String GetAbsPath() const = 0;

		/**
		*  @brief
		*    Unloads the dynamic library
		*
		*  @return
		*    'true' if the library could be unloaded, else false
		*/
		virtual bool Unload() = 0;

		/**
		*  @brief
		*    Returns a pointer to a symbol in the library
		*
		*  @param[in] sSymbol
		*    Name of the symbol to retrieve
		*
		*  @return
		*    Pointer to the symbol, or a null pointer on error
		*
		*  @note
		*    - The pointer to the symbol only stays valid as long as this dynamic library instance is not unloaded
		*/
		virtual void *GetSymbol(const String &sSymbol) const = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_DYNLIB_IMPL_H__
