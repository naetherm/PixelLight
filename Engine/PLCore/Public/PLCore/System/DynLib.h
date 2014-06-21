/*********************************************************\
 *  File: DynLib.h                                       *
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


#ifndef __PLCORE_DYNLIB_H__
#define __PLCORE_DYNLIB_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/File/Url.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class DynLibImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dynamic library (aka "shared library") class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class DynLib {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API DynLib();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - The dynamic library is unloaded automatically
		*/
		PLCORE_API ~DynLib();

		/**
		*  @brief
		*    Loads a dynamic library
		*
		*  @param[in] sPath
		*    Path to the dynamic library
		*
		*  @return
		*    'true' if the library could be loaded, else 'false'
		*
		*  @note
		*    - Dependent dynamic libraries are first searched within the same path as the given one
		*    - If the library has already been loaded, this function will fail
		*/
		inline bool Load(const String &sPath);

		/**
		*  @brief
		*    Returns if the dynamic library has been loaded
		*
		*  @return
		*    'true' if loaded, else 'false'
		*/
		inline bool IsLoaded() const;

		/**
		*  @brief
		*    Get the path (set within 'Load()') to the dynamic library
		*
		*  @return
		*    The path to the dynamic library (e.g. "file://C:/PixelLight/Runtime/x86/PLCore.dll" on Windows)
		*/
		inline String GetPath() const;

		/**
		*  @brief
		*    Get the absolute path to the dynamic library
		*
		*  @return
		*    The absolute path to the dynamic library (native path style, e.g. "C:\PixelLight\Runtime\x86\PLCore.dll" on Windows)
		*/
		inline String GetAbsPath() const;

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
		inline void *GetSymbol(const String &sSymbol) const;

		/**
		*  @brief
		*    Unloads the dynamic library
		*
		*  @return
		*    'true' if the library could be unloaded, else false
		*/
		inline bool Unload();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		DynLib(const DynLib &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		DynLib &operator =(const DynLib &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DynLibImpl *m_pDynLibImpl;	/**< Pointer to the system specific implementation (assumed to be never a null pointer!) */
		Url			m_cUrl;			/**< Url to the dynamic library */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/System/DynLib.inl"


#endif // __PLCORE_DYNLIB_H__
