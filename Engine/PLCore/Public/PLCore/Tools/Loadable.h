/*********************************************************\
 *  File: Loadable.h                                     *
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


#ifndef __PLCORE_LOADABLE_H__
#define __PLCORE_LOADABLE_H__
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
class File;
class Loader;
class Directory;
class Parameters;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract loadable (serialization) base class, derive your loadable classes from this class
*
*  @note
*    - If your loadable has special parameters for loading/saving don't forget to document it within your class!
*/
class Loadable {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the filename this loadable was given to loaded from
		*
		*  @return
		*    The filename this loadable was given to loaded from
		*/
		inline String GetFilename() const;

		/**
		*  @brief
		*    Returns the absolute filename this loadable was loaded from
		*
		*  @return
		*    The absolute filename this loadable was loaded from
		*/
		inline String GetUrl() const;


	//[-------------------------------------------------------]
	//[ Public virtual Loadable functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Loads the loadable from a file given by filename
		*
		*  @param[in] sFilename
		*    Loadable filename
		*  @param[in] sParams
		*    Optional load method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the load method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If no method name was provided, 'Load' if sParams is empty, or 'LoadParams' if sParams is not empty is used automatically for files
		*    - If no method name was provided, 'LoadDirectory' if sParams is empty, or 'LoadDirectoryParams' if sParams is not empty is used automatically for directories
		*/
		PLCORE_API virtual bool LoadByFilename(const String &sFilename, const String &sParams = "", const String &sMethod = "");

		/**
		*  @brief
		*    Loads the loadable from a file given by a reference
		*
		*  @param[in] cFile
		*    File to load from, must be opened and readable
		*  @param[in] sParams
		*    Optional load method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the load method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If no method name was provided, 'Load' if sParams is empty, or 'LoadParams' if sParams is not empty is used automatically
		*/
		PLCORE_API virtual bool LoadByFile(File &cFile, const String &sParams = "", const String &sMethod = "");

		/**
		*  @brief
		*    Loads the loadable from a directory given by a reference
		*
		*  @param[in] cDirectory
		*    Directory to load from
		*  @param[in] sParams
		*    Optional load method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the load method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If no method name was provided, 'LoadDirectory' if sParams is empty, or 'LoadDirectoryParams' if sParams is not empty is used automatically
		*/
		PLCORE_API virtual bool LoadByDirectory(Directory &cDirectory, const String &sParams = "", const String &sMethod = "");

		/**
		*  @brief
		*    Saves the loadable to a file given by filename
		*
		*  @param[in] sFilename
		*    Loadable filename
		*  @param[in] sParams
		*    Optional save method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the save method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If no method name was provided, 'Save' if sParams is empty, or 'SaveParams' if sParams is not empty is used automatically
		*    - Use "SaveByDirectory()" to save into a directory
		*/
		PLCORE_API virtual bool SaveByFilename(const String &sFilename, const String &sParams = "", const String &sMethod = "");

		/**
		*  @brief
		*    Saves the loadable to a file given by reference
		*
		*  @param[in] cFile
		*    File to save into, must be opened and writable
		*  @param[in] sParams
		*    Optional save method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the save method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If no method name was provided, 'Save' if sParams is empty, or 'SaveParams' if sParams is not empty is used automatically
		*/
		PLCORE_API virtual bool SaveByFile(File &cFile, const String &sParams = "", const String &sMethod = "");

		/**
		*  @brief
		*    Saves the loadable to a directory given by reference
		*
		*  @param[in] cDirectory
		*    Directory to save into
		*  @param[in] sParams
		*    Optional save method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the save method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If no method name was provided, 'SaveDirectory' if sParams is empty, or 'SaveDirectoryParams' if sParams is not empty is used automatically
		*/
		PLCORE_API virtual bool SaveByDirectory(Directory &cDirectory, const String &sParams = "", const String &sMethod = "");

		/**
		*  @brief
		*    Reloads the loadable
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's nothing loaded?)
		*
		*  @note
		*    - Same as pLoadable->Load(pLoadable->GetAbsFilename())
		*/
		PLCORE_API virtual bool Reload();

		/**
		*  @brief
		*    Unloads the loadable
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API virtual bool Unload();

		/**
		*  @brief
		*    Returns the loadable type name
		*
		*  @return
		*    The loadable type name
		*/
		PLCORE_API virtual String GetLoadableTypeName() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API Loadable();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Loadable();


	//[-------------------------------------------------------]
	//[ Protected virtual Loadable functions                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Calls the loadable in order to load or save
		*
		*  @param[in] cFile
		*    File to load from, MUST be opened
		*  @param[in] cLoader
		*    Loader to use
		*  @param[in] sMethod
		*    Name of the method to use
		*  @param[in] sParams
		*    Method parameters
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API virtual bool CallLoadable(File &cFile, Loader &cLoader, const String &sMethod, const String &sParams);

		/**
		*  @brief
		*    Calls the loadable in order to load or save
		*
		*  @param[in] cDirectory
		*    Directory to load from
		*  @param[in] cLoader
		*    Loader to use
		*  @param[in] sMethod
		*    Name of the method to use
		*  @param[in] sParams
		*    Method parameters
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API virtual bool CallLoadable(Directory &cDirectory, Loader &cLoader, const String &sMethod, const String &sParams);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		String m_sFilename;	/**< The filename this loadable was given to loaded from */
		String m_sUrl;		/**< The absolute filename this loadable was loaded from */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Loadable.inl"


#endif // __PLCORE_LOADABLE_H__
