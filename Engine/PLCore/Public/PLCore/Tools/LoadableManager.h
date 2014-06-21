/*********************************************************\
 *  File: LoadableManager.h                              *
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


#ifndef __PLCORE_LOADABLEMANAGER_H__
#define __PLCORE_LOADABLEMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Loadable.h"
#include "PLCore/Core/Singleton.h"
#include "PLCore/Container/Array.h"
#include "PLCore/Container/HashMap.h"
#include "PLCore/Base/Event/EventHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Class;
class Loader;
class Directory;
class LoadableType;
class LoadableManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loadable manager
*
*  @remarks
*    A loadable class is for instance an image that can be loaded & saved in multiple ways/formats.
*    Such a loadable is registered within this manager as a 'loadable type'. Each loadable type can
*    have multiple loaders which load/save the loadable within a concrete way/format.
*
*    Example:
*      - Image
*        - ImageLoaderEXR (*.exr)
*      - Mesh
*        - MeshLoaderPL (*.mesh)
*        - MeshLoader3ds (*.3ds)
*        - MeshLoaderObj (*.obj)
*    -> By asking the loadable manager which 'file formats/extensions' are supported for the 'Mesh' type,
*       it will return in this example 'mesh, 3ds. obj'. One loader can also support multiple formats.
*
*  @note
*    - An empty string is also a valid base directory and represents the current system directory
*      (see System::GetCurrentDir())
*/
class LoadableManager : public Singleton<LoadableManager> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LoadableType;
	friend class Singleton<LoadableManager>;


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		static PLCORE_API LoadableManager *GetInstance();
		static PLCORE_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Types                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of loadable types
		*
		*  @return
		*    The number of loadable types
		*/
		inline uint32 GetNumOfTypes();

		/**
		*  @brief
		*    Returns a loadable type by using an index
		*
		*  @param[in] nIndex
		*    Loadable type index
		*
		*  @return
		*    The requested loadable type, a null pointer on error
		*/
		inline LoadableType *GetTypeByIndex(uint32 nIndex);

		/**
		*  @brief
		*    Returns a loadable type by using it's name
		*
		*  @param[in] sName
		*    Loadable type name
		*
		*  @return
		*    The requested loadable type, a null pointer on error
		*/
		inline LoadableType *GetTypeByName(const String &sName);

		/**
		*  @brief
		*    Returns a loadable type by using a loadable extension
		*
		*  @param[in] sExtension
		*    Extension of loadable
		*
		*  @return
		*    The requested loadable type (first found if there are multiple candidates), a null pointer on error
		*/
		inline LoadableType *GetTypeByExtension(const String &sExtension);

		/**
		*  @brief
		*    Returns loadable types by using a loadable extension
		*
		*  @param[in]  sExtension
		*    Extension of loadable
		*  @param[out] lstTypes
		*    Receives the list of matching loadable types (list is not cleared before new entries are added), there can be multiple candidates
		*/
		PLCORE_API void GetTypesByExtension(const String &sExtension, Array<LoadableType*> &lstTypes);

		//[-------------------------------------------------------]
		//[ Loaders                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of loaders
		*
		*  @return
		*    The number of loaders
		*/
		inline uint32 GetNumOfLoaders();

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
		inline Loader *GetLoaderByIndex(uint32 nIndex);

		/**
		*  @brief
		*    Returns a loader by using a loadable extension
		*
		*  @param[in] sExtension
		*    Extension of loadable
		*
		*  @return
		*    The requested loader (first found if there are multiple candidates), a null pointer on error
		*/
		inline Loader *GetLoaderByExtension(const String &sExtension);

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

		//[-------------------------------------------------------]
		//[ Formats                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of supported formats
		*
		*  @return
		*    The number of supported formats
		*/
		inline uint32 GetNumOfFormats();

		/**
		*  @brief
		*    Returns a supported format
		*
		*  @param[in] nIndex
		*    Format index
		*
		*  @return
		*    The requested supported format, empty string on error
		*/
		inline String GetFormat(uint32 nIndex);

		/**
		*  @brief
		*    Returns whether loading is supported for the given format
		*
		*  @param[in] sExtension
		*    Extension of the format
		*  @param[in] sType
		*    Required loadable type, if empty string ignore loadable type
		*
		*  @return
		*    'true' if loading is supported for the given format, else 'false'
		*/
		PLCORE_API bool IsFormatLoadSupported(const String &sExtension, const String &sType = "");

		/**
		*  @brief
		*    Returns whether saving is supported for the given format
		*
		*  @param[in] sExtension
		*    Extension of the format
		*  @param[in] sType
		*    Required loadable type, if empty string ignore loadable type
		*
		*  @return
		*    'true' if saving is supported for the given format, else 'false'
		*/
		PLCORE_API bool IsFormatSaveSupported(const String &sExtension, const String &sType = "");

		//[-------------------------------------------------------]
		//[ Base directories                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of base directories
		*
		*  @return
		*    Number of base directories
		*/
		inline uint32 GetNumOfBaseDirs() const;

		/**
		*  @brief
		*    Returns one of the base directories
		*
		*  @param[in] nNum
		*    Number of the base directory to get
		*
		*  @return
		*    Path of the base directory or empty string
		*/
		inline String GetBaseDir(uint32 nNum) const;

		/**
		*  @brief
		*    Checks whether the given path is a base directory
		*
		*  @param[in] sPath
		*    Path to the base directory
		*
		*  @return
		*    'true', if the given path is a base directory, else 'false'
		*/
		PLCORE_API bool IsBaseDir(const String &sPath) const;

		/**
		*  @brief
		*    Adds a base directory
		*
		*  @param[in] sPath
		*    Path to the base directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		PLCORE_API bool AddBaseDir(const String &sPath);

		/**
		*  @brief
		*    Set the priority of base directories
		*
		*  @param[in] sFirstPath
		*    Path to the first base directory
		*  @param[in] sSecondPath
		*    Path to the second base directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*
		*  @remarks
		*    This function changes the order of the two base dirs
		*    so that sSecondPath comes right after sFirstPath.
		*/
		PLCORE_API bool SetBaseDirPriority(const String &sFirstPath, const String &sSecondPath);

		/**
		*  @brief
		*    Remove a base directory
		*
		*  @param[in] sPath
		*    Base directory to remove
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		PLCORE_API bool RemoveBaseDir(const String &sPath);

		/**
		*  @brief
		*    Remove a base directory
		*
		*  @param[in] nNum
		*    Number of the base directory to remove
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		PLCORE_API bool RemoveBaseDir(uint32 nNum);

		/**
		*  @brief
		*    Removes all base directories
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		inline bool ClearBaseDirs();

		/**
		*  @brief
		*    Gets the relative version of an absolute filename
		*
		*  @param[in] sFilename
		*    Absolute filename to get the relative version from
		*
		*  @return
		*    Relative filename, empty string on error (maybe unknown loadable type?)
		*
		*  @note
		*    - This function only checks whether there's a loadable type for the given file type,
		*      if so, the request is given to this loadable type
		*
		*  @see
		*    - LoadableType::GetRelativeFilePath()
		*/
		PLCORE_API String GetRelativeFilename(const String &sFilename);

		/**
		*  @brief
		*    Scan a directory for data packages and add them as base paths to the loadable manager
		*
		*  @param[in] sPath
		*    Directory to search in
		*  @param[in] sExtension
		*    Extension of the archives to add (e.g. 'zip' or 'pak')
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool ScanPackages(const String &sPath, const String &sExtension = "*.zip");

		/**
		*  @brief
		*    Opens a file by using base directories
		*
		*  @param[in] cFile
		*    File object
		*  @param[in] sFilename
		*    Filename of the file to open
		*  @param[in] bCreate
		*    Create the file if it doesn't exist?
		*  @param[in] nStringFormat
		*    String encoding format to use when dealing with string functions (not supported by all file implementations)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - It's not recommended to use Unicode by because internally wchar_t is used and this data type has not
		*      the same size on every platform (use ASCII or UTF8 instead)
		*/
		PLCORE_API bool OpenFile(File &cFile, const String &sFilename, bool bCreate = false, String::EFormat nStringFormat = String::ASCII) const;

		/**
		*  @brief
		*    Opens a directory by using base directories
		*
		*  @param[in] cDirectory
		*    Directory object
		*  @param[in] sFilename
		*    Filename of the directory to open
		*  @param[in] bCreate
		*    Create the directory if it doesn't exist?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool OpenDirectory(Directory &cDirectory, const String &sFilename, bool bCreate = false) const;

		/**
		*  @brief
		*    Loads in a string by using a file
		*
		*  @param[in] sFilename
		*    Name of the file to read the string from
		*  @param[in] nStringFormat
		*    String encoding format to use when dealing with string functions (not supported by all file implementations)
		*
		*  @return
		*    The read string, empty string on error or if the file is just empty
		*
		*  @note
		*    - The file is opened by using base directories
		*    - It's not recommended to use Unicode by because internally wchar_t is used and this data type has not
		*      the same size on every platform (use ASCII or UTF8 instead)
		*/
		PLCORE_API String LoadStringFromFile(const String &sFilename, String::EFormat nStringFormat = String::ASCII) const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API LoadableManager();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~LoadableManager();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
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
		LoadableManager &operator =(const LoadableManager &cSource);

		/**
		*  @brief
		*    Register a class
		*
		*  @param[in] pClass
		*    Class to register, must be valid!
		*
		*  @note
		*    - If the class is not derived from 'Loader' it is ignored
		*    - The given class must be a new one, so this function did not check whether the
		*      class is already registered
		*/
		void OnClassLoaded(const Class *pClass);

		/**
		*  @brief
		*    Unregister a class
		*
		*  @param[in] pClass
		*    Class to unregister, must be valid!
		*/
		void OnClassUnloaded(const Class *pClass);

		/**
		*  @brief
		*    Registers queued classes
		*/
		PLCORE_API void RegisterClasses();


	//[-------------------------------------------------------]
	//[ Private slots                                         ]
	//[-------------------------------------------------------]
	private:
		EventHandler<const Class*> SlotClassLoaded;
		EventHandler<const Class*> SlotClassUnloaded;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// General
		Array<LoadableType*>			m_lstTypes;				/**< List of loadable types */
		HashMap<String, LoadableType*>	m_mapTypes;				/**< Map of loadable types */
		Array<String>					m_lstBaseDirs;			/**< List of base directories */
		Array<const Class*>				m_lstNewClasses;		/**< New classes to register as soon as required */
		// Filled by LoadableType
		Array<Loader*>					m_lstLoaders;			/**< List of loaders */
		HashMap<String, Loader*>		m_mapLoaders;			/**< Map of loaders (key = extension) */
		Array<String>					m_lstFormats;			/**< List of loadable formats */
		HashMap<String, LoadableType*>	m_mapTypesByExtension;	/**< Map of loadable types (key = extension) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/LoadableManager.inl"


#endif // __PLCORE_LOADABLEMANAGER_H__
