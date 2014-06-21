/*********************************************************\
 *  File: LoadableType.h                                 *
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


#ifndef __PLCORE_LOADABLETYPE_H__
#define __PLCORE_LOADABLETYPE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/Array.h"
#include "PLCore/Container/HashMap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class File;
class Class;
class Loader;
class Directory;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class describing a loadable type
*
*  @remarks
*    This is used for managing search paths and loadable formats for a loadable type.
*/
class LoadableType {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LoadableManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the name of the loadable type
		*
		*  @return
		*    Name of the loadable type
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Returns the class of the loadable type
		*
		*  @return
		*    Class of the loadable type
		*/
		inline const Class &GetClass() const;

		/**
		*  @brief
		*    Gets the relative version of an absolute file path
		*
		*  @param[in] sPath
		*    Absolute file path to get the relative version from
		*
		*  @return
		*    Relative file path, empty string on error
		*
		*  @note
		*    - The function will take the FS base directories into account, the first match
		*      will be used
		*    - MS Windows example: 'C:\test\test.bmp' -> 'test.bmp'
		*/
		PLCORE_API String GetRelativeFilePath(const String &sPath) const;

		//[-------------------------------------------------------]
		//[ Formats & loaders                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of supported formats
		*
		*  @return
		*    The number of supported formats
		*/
		inline uint32 GetNumOfFormats() const;

		/**
		*  @brief
		*    Returns a supported formats
		*
		*  @param[in] nIndex
		*    Format index
		*
		*  @return
		*    The requested supported format, empty string on error
		*/
		inline String GetFormat(uint32 nIndex) const;

		/**
		*  @brief
		*    Returns the number of loaders
		*
		*  @return
		*    The number of loaders
		*/
		inline uint32 GetNumOfLoaders() const;

		/**
		*  @brief
		*    Returns a loader by using an index
		*
		*  @param[in] nIndex
		*    Loader index
		*
		*  @return
		*    The requested loader, a null pointer on error
		*/
		inline Loader *GetLoaderByIndex(uint32 nIndex) const;

		/**
		*  @brief
		*    Returns a loader by using a loadable extension
		*
		*  @param[in] sExtension
		*    Extension of loader
		*
		*  @return
		*    The requested loader (first found if there are multiple candidates), a null pointer on error (format is not supported)
		*/
		inline Loader *GetLoaderByExtension(const String &sExtension) const;

		/**
		*  @brief
		*    Returns loaders by using a loadable extension
		*
		*  @param[in]  sExtension
		*    Extension of loadable
		*  @param[out] lstLoaders
		*    Receives the list of matching loaders (list is not cleared before new entries are added), there can be multiple candidates
		*/
		PLCORE_API void GetLoadersByExtension(const String &sExtension, Array<Loader*> &lstLoaders);

		/**
		*  @brief
		*    Returns a loader for loading by using a loadable file
		*
		*  @param[in] cFile
		*    File to load from, must be opened and readable
		*
		*  @return
		*    The requested loader (first found if there are multiple candidates), a null pointer on error (format is not supported)
		*/
		PLCORE_API Loader *GetLoaderForLoadingByFile(File &cFile) const;

		/**
		*  @brief
		*    Returns a loader for loading by using a loadable directory
		*
		*  @param[in] cDirectory
		*    Directory to load from
		*
		*  @return
		*    The requested loader (first found if there are multiple candidates), a null pointer on error (format is not supported)
		*/
		PLCORE_API Loader *GetLoaderForLoadingByDirectory(Directory &cDirectory) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		LoadableType();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Name of the loadable type
		*  @param[in] cClass
		*    Class the loadable type
		*/
		LoadableType(const String &sName, const Class &cClass);

		/**
		*  @brief
		*    Destructor
		*/
		~LoadableType();

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
		LoadableType &operator =(const LoadableType &cSource);

		/**
		*  @brief
		*    Adds a loader to this loadable type
		*
		*  @param[in] cLoader
		*    Loader to add, this loadable type takes over the memory control
		*/
		void AddLoader(Loader &cLoader);

		/**
		*  @brief
		*    Removes a loader from this loadable type
		*
		*  @param[in] cLoader
		*    Loader to remove, when all went fine then the given reference is no longer valid after this method was successfully called
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool RemoveLoader(Loader &cLoader);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String						 m_sName;		/**< Name of the loadable type */
		const Class					*m_pClass;		/**< Class the loadable type, always valid! */
		Array<Loader*>				 m_lstLoaders;	/**< List of loaders */
		HashMap<String, Loader*>	 m_mapLoaders;	/**< Map of loaders (key = extension) */
		Array<String>				 m_lstFormats;	/**< List of loadable formats */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/LoadableType.inl"


#endif // __PLCORE_LOADABLETYPE_H__
