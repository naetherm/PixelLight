/*********************************************************\
 *  File: LoadableManager.cpp                            *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Log/Log.h"
#include "PLCore/File/File.h"
#include "PLCore/File/Directory.h"
#include "PLCore/File/FileSearch.h"
#include "PLCore/System/System.h"
#include "PLCore/Base/Class.h"
#include "PLCore/Base/ClassManager.h"
#include "PLCore/Tools/Loader.h"
#include "PLCore/Tools/LoadableType.h"
#include "PLCore/Tools/LoadableManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<LoadableManager>;


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
LoadableManager *LoadableManager::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<LoadableManager>::GetInstance();
}

bool LoadableManager::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<LoadableManager>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns loadable types by using a loadable extension
*/
void LoadableManager::GetTypesByExtension(const String &sExtension, Array<LoadableType*> &lstTypes)
{
	RegisterClasses();

	// Loop through all registered loadable types
	for (uint32 i=0; i<m_lstTypes.GetNumOfElements(); i++) {
		// Get the current loadable type
		LoadableType *pLoadableType = m_lstTypes[i];

		// Check whether or not there's at least one loader capable of dealing with the given extension
		if (pLoadableType->GetLoaderByExtension(sExtension))
			lstTypes.Add(pLoadableType);
	}
}

/**
*  @brief
*    Returns loaders by using a loadable extension
*/
void LoadableManager::GetLoadersByExtension(const String &sExtension, Array<Loader*> &lstLoaders)
{
	RegisterClasses();

	// Loop through all registered loaders
	for (uint32 i=0; i<m_lstLoaders.GetNumOfElements(); i++) {
		// Get the current loader
		Loader *pLoader = m_lstLoaders[i];

		// Check whether or not the loader is capable of dealing with the given extension
		if (pLoader->IsFormatSupported(sExtension))
			lstLoaders.Add(pLoader);
	}
}

/**
*  @brief
*    Returns whether loading is supported for the given format
*/
bool LoadableManager::IsFormatLoadSupported(const String &sExtension, const String &sType)
{
	RegisterClasses();
	if (sType.GetLength()) {
		const LoadableType *pLoadableType = GetTypeByName(sType);
		if (pLoadableType) {
			const Loader *pLoader = pLoadableType->GetLoaderByExtension(sExtension);
			if (pLoader)
				return pLoader->CanLoad();
		}
	} else {
		const Loader *pLoader = GetLoaderByExtension(sExtension);
		if (pLoader)
			return pLoader->CanLoad();
	}

	// Loading is not supported for the given format
	return false;
}

/**
*  @brief
*    Returns whether saving is supported for the given format
*/
bool LoadableManager::IsFormatSaveSupported(const String &sExtension, const String &sType)
{
	RegisterClasses();
	if (sType.GetLength()) {
		const LoadableType *pLoadableType = GetTypeByName(sType);
		if (pLoadableType) {
			const Loader *pLoader = pLoadableType->GetLoaderByExtension(sExtension);
			if (pLoader)
				return pLoader->CanSave();
		}
	} else {
		const Loader *pLoader = GetLoaderByExtension(sExtension);
		if (pLoader)
			return pLoader->CanSave();
	}

	// Saving is not supported for the given format
	return false;
}

/**
*  @brief
*    Checks whether the given path is a base directory
*/
bool LoadableManager::IsBaseDir(const String &sPath) const
{
	// Correct path
	String sValidUrl = Url(sPath).GetUrl();
	if (!Url(sValidUrl).IsDirectory())
		sValidUrl += '/';

	// Loop through all base directories
	for (uint32 i=0; i<m_lstBaseDirs.GetNumOfElements(); i++) {
		if (m_lstBaseDirs[i] == sValidUrl)
			return true; // This is a base directory
	}

	// Done, the given path is no base directory
	return false;
}

/**
*  @brief
*    Adds a base directory
*/
bool LoadableManager::AddBaseDir(const String &sPath)
{
	// Correct path
	String sValidUrl = Url(sPath).GetUrl();
	if (!Url(sValidUrl).IsDirectory())
		sValidUrl += '/';

	// Check whether the given base directory is already registered
	for (uint32 i=0; i<m_lstBaseDirs.GetNumOfElements(); i++) {
		if (m_lstBaseDirs[i] == sValidUrl)
			return true; // Done
	}

	// Add directory
	m_lstBaseDirs.Add(sValidUrl);
	return true; // Done
}

/**
*  @brief
*    Set the priority of base directories
*/
bool LoadableManager::SetBaseDirPriority(const String &sFirstPath, const String &sSecondPath)
{
	// Check parameters
	if (sFirstPath == sSecondPath)
		return true; // Done

	// Correct paths
	String sValidFirst  = Url(sFirstPath).GetUrl();
	if (!Url(sValidFirst).IsDirectory())
		sValidFirst += '/';
	String sValidSecond = Url(sSecondPath).GetUrl();
	if (!Url(sValidSecond).IsDirectory())
		sValidSecond += '/';

	// Get the first path
	for (uint32 nFirst=0; nFirst<m_lstBaseDirs.GetNumOfElements(); nFirst++) {
		const String &sFirstDirectory = m_lstBaseDirs[nFirst];
		if (sFirstDirectory == sValidFirst) {
			// Get the second path
			for (uint32 nSecond=0; nSecond<m_lstBaseDirs.GetNumOfElements(); nSecond++) {
				const String &sSecondDirectory = m_lstBaseDirs[nSecond];
				if (sSecondDirectory == sValidSecond) {
					// Already in order?
					if (nFirst > nSecond) {
						// Remove the first one
						m_lstBaseDirs.RemoveAtIndex(nFirst);

						// The first one replaces the second one
						m_lstBaseDirs.ReplaceAtIndex(nSecond, sValidFirst);

						// Add the second one directly after the first one
						m_lstBaseDirs.AddAtIndex(sValidSecond, nSecond + 1);
					}

					// Done
					return true;
				}
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Remove a base directory
*/
bool LoadableManager::RemoveBaseDir(const String &sPath)
{
	// Correct path
	String sValidUrl = Url(sPath).GetUrl();
	if (!Url(sValidUrl).IsDirectory())
		sValidUrl += '/';

	// Search for base directory
	ConstIterator<String> cIterator = m_lstBaseDirs.GetConstIterator();
	while (cIterator.HasNext()) {
		// Check directory name
		const String &sDir = cIterator.Next();
		if (sValidUrl == sDir) {
			// Remove directory
			m_lstBaseDirs.Remove(sDir);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Remove a base directory
*/
bool LoadableManager::RemoveBaseDir(uint32 nNum)
{
	// Get directory
	String &sDir = m_lstBaseDirs[nNum];
	if (&sDir != &Array<String>::Null) {
		// Remove directory
		m_lstBaseDirs.Remove(sDir);

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Gets the relative version of an absolute filename
*/
String LoadableManager::GetRelativeFilename(const String &sFilename)
{
	// Get file extension
	// -> There are file formats without an extension, so no extension must also be valid
	RegisterClasses();
	const String sExtension = Url(sFilename).GetExtension();

	// Get the loadable type
	const LoadableType *pLoadableType = m_mapTypesByExtension.Get(sExtension);
	if (pLoadableType)
		return pLoadableType->GetRelativeFilePath(sFilename);

	// Error!
	return "";
}

/**
*  @brief
*    Scan a directory for data packages and add them as base paths to the loadable manager
*/
bool LoadableManager::ScanPackages(const String &sPath, const String &sExtension)
{
	// Check parameters
	if (sPath.GetLength() && sExtension.GetLength()) {
		// Add FS base directories
		Directory cDir(sPath);
		if (cDir.IsDirectory()) {
			FileSearch cSearch(cDir);
			while (cSearch.HasNextFile()) {
				// Get next file
				String sNextFilename = cSearch.GetNextFile();
				String sNextPath     = cDir.GetUrl().GetUrl() + '/' + sNextFilename;

				// Add all found files to the base-directories (meaning these must be packages ...)
				if (File(sNextPath).IsFile())
					AddBaseDir(sNextPath + '/');
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Opens a file by using base directories
*/
bool LoadableManager::OpenFile(File &cFile, const String &sFilename, bool bCreate, String::EFormat nStringFormat) const
{
	// Because absolute filenames can be accessed fastest by the file system, we first give
	// the file system an absolute filename which is hopefully the correct one... if
	// not, we must search the file which is quite slow...
	const Url cUrl(sFilename);
	if (cUrl.IsAbsolute()) {
		// The given filename is already absolute! :)
		cFile.Assign(cUrl);
	} else {
		// Are there any base directories?
		const uint32 nNumOfBaseDirs = m_lstBaseDirs.GetNumOfElements();
		if (nNumOfBaseDirs) {
			// Reset file
			cFile.Assign("");

			// Loop through all base directories
			bool bFileFound = false;
			for (uint32 nBaseDir=0; nBaseDir<nNumOfBaseDirs && !bFileFound; nBaseDir++) {
				// Get the base directory
				const String sBaseDir = m_lstBaseDirs[nBaseDir];

				// Try to open the file directly (resolve "./" because we always want to work with absolute paths so the user can figure out the absolute path later on)
				if (sBaseDir == "./") {
					// Use current directory
					cFile.Assign(System::GetInstance()->GetCurrentDir() + '/' + sFilename);
				} else {
					// Use given base directory
					cFile.Assign(sBaseDir + sFilename);
				}

				// File found?
				bFileFound = cFile.IsFile();

				// Create the file?
				if (bCreate && !bFileFound)
					bFileFound = cFile.Create();
			}
		} else {
			// Try to open the file directly
			cFile.Assign(cUrl);
		}
	}

	// Check if the file has been found
	return cFile.Open(bCreate ? (File::FileWrite | File::FileCreate) : File::FileRead, nStringFormat);
}

/**
*  @brief
*    Opens a directory by using base directories
*/
bool LoadableManager::OpenDirectory(Directory &cDirectory, const String &sFilename, bool bCreate) const
{
	// Because absolute filenames can be accessed fastest by the file system, we first give
	// the file system an absolute filename which is hopefully the correct one... if
	// not, we must search the directory which is quite slow...
	const Url cUrl(sFilename);
	if (cUrl.IsAbsolute()) {
		// The given filename is already absolute! :)
		cDirectory.Assign(cUrl);
	} else {
		// Are there any base directories?
		const uint32 nNumOfBaseDirs = m_lstBaseDirs.GetNumOfElements();
		if (nNumOfBaseDirs) {
			// Reset directory
			cDirectory.Assign("");

			// Loop through all base directories
			bool bDirectoryFound = false;
			for (uint32 nBaseDir=0; nBaseDir<nNumOfBaseDirs && !bDirectoryFound; nBaseDir++) {
				// Get the base directory
				const String sBaseDir = m_lstBaseDirs[nBaseDir];

				// Try to open the directory directly (resolve "./" because we always want to work with absolute paths so the user can figure out the absolute path later on)
				if (sBaseDir == "./") {
					// Use current directory
					cDirectory.Assign(System::GetInstance()->GetCurrentDir() + '/' + sFilename);
				} else {
					// Use given base directory
					cDirectory.Assign(sBaseDir + sFilename);
				}

				// Directory found?
				bDirectoryFound = cDirectory.IsFile();

				// Create the directory?
				if (bCreate && !bDirectoryFound)
					bDirectoryFound = cDirectory.CreateRecursive();
			}
		} else {
			// Try to open the directory directly
			cDirectory.Assign(cUrl);
		}
	}

	// Check if the directory has been found
	if (cDirectory.IsDirectory()) {
		// Done
		return true;
	} else {
		// Create the directory right now?
		return bCreate ? cDirectory.CreateRecursive() : false;
	}
}

/**
*  @brief
*    Loads in a string by using a file
*/
String LoadableManager::LoadStringFromFile(const String &sFilename, String::EFormat nStringFormat) const
{
	// Open the file
	File cFile;
	if (OpenFile(cFile, sFilename, false, nStringFormat))
		return cFile.GetContentAsString();

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
LoadableManager::LoadableManager() :
	SlotClassLoaded(&LoadableManager::OnClassLoaded, this),
	SlotClassUnloaded(&LoadableManager::OnClassUnloaded, this)
{
	// The loadable manager MUST be informed if new classes are registered in order to register new loadable types!
	ClassManager::GetInstance()->EventClassLoaded.Connect(SlotClassLoaded);
	ClassManager::GetInstance()->EventClassUnloaded.Connect(SlotClassUnloaded);

	// Register all loaders
	List<const Class*> lstClasses;
	ClassManager::GetInstance()->GetClasses(lstClasses, "PLCore::LoaderImpl", Recursive, NoBase, NoAbstract);
	Iterator<const Class*> cIterator = lstClasses.GetIterator();
	while (cIterator.HasNext())
		m_lstNewClasses.Add(cIterator.Next());
}

/**
*  @brief
*    Destructor
*/
LoadableManager::~LoadableManager()
{
	// Destroy all types, usually when we're in here there should only be types and loaders left which are defined within PLCore itself
	const uint32 nNumOfElements = m_lstTypes.GetNumOfElements();
	for (uint32 i=0; m_lstTypes.GetNumOfElements() && i<nNumOfElements; i++) {
		// Remove the first type, it's removed from the list automatically
		delete m_lstTypes[0];
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy operator
*/
LoadableManager &LoadableManager::operator =(const LoadableManager &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Register a class
*/
void LoadableManager::OnClassLoaded(const Class *pClass)
{
	// Just put the new class on the queue - because we can't assume that this class is or can be initialized
	// already, we can't even expect "IsDerivedFrom" to work, so, just REALLY note this class in here!
	m_lstNewClasses.Add(pClass);
}

/**
*  @brief
*    Unregister a class
*/
void LoadableManager::OnClassUnloaded(const Class *pClass)
{
	// Destroy loadable type instance if required
	for (uint32 nType=0; nType<m_lstTypes.GetNumOfElements(); nType++) {
		// Get the current loadable type instance
		LoadableType *pLoadableType = m_lstTypes[nType];

		// Is this an instance of the unloaded class?
		if (&pLoadableType->GetClass() == pClass) {
			// Destroy instance
			m_lstTypes.RemoveAtIndex(nType);
			delete pLoadableType;

			// Get us out of here (there's just one instance per loadable type or loader class)
			nType = m_lstTypes.GetNumOfElements();
		} else {
			// Destroy loader instance if required
			Array<Loader*> &lstLoaders = pLoadableType->m_lstLoaders;
			for (uint32 nLoader=0; nLoader<lstLoaders.GetNumOfElements(); nLoader++) {
				// Get the current loader instance
				Loader *pLoader = lstLoaders[nLoader];

				// Is this an instance of the unloaded class?
				if (&pLoader->GetClass() == pClass) {
					// Destroy instance
					pLoadableType->RemoveLoader(*pLoader);

					// Get us out of here (there's just one instance per loadable type or loader class)
					nLoader = lstLoaders.GetNumOfElements();
					nType = m_lstTypes.GetNumOfElements();
				}
			}
		}
	}
}

/**
*  @brief
*    Registers queued classes
*/
void LoadableManager::RegisterClasses()
{
	// Is there anything on the register queue?
	if (m_lstNewClasses.GetNumOfElements()) {
		Iterator<const Class*> cIterator = m_lstNewClasses.GetIterator();
		while (cIterator.HasNext()) {
			// Get the class
			const Class *pClass = cIterator.Next();

			// Check base class, in here, we're only interested in loader implementation classes
			static const String sClassString = "PLCore::LoaderImpl";
			if (pClass->IsDerivedFrom(sClassString) && pClass->HasConstructor()) {
				// Get the loadable type ("Mesh", "Scene" etc.)
				const String sType = pClass->GetProperties().Get("Type");
				if (sType.GetLength()) {
					// Try to get an instance of the loadable type
					LoadableType *pLoadableType = m_mapTypes.Get(sType);
					if (!pLoadableType) {
						// Currently, there's no such loadable type, add one

						// Find the base class (the one directly derived from "PLCore::LoaderImpl"), all loader implementations of this type are derived from
						const Class *pLoaderTypeClass = pClass->GetBaseClass();
						const Class *pBaseLoaderClass = ClassManager::GetInstance()->GetClass(sClassString);
						while (pLoaderTypeClass && pLoaderTypeClass->GetBaseClass() != pBaseLoaderClass)
							pLoaderTypeClass = pLoaderTypeClass->GetBaseClass();
						if (pLoaderTypeClass) {
							// Get the loadable type of the loadable type class
							const String sBaseType = pLoaderTypeClass->GetProperties().Get("Type");
							if (sBaseType.GetLength()) {
								// Security check: The type of the loader class and it's loader base class must match
								if (sType == sBaseType) {
									// Create an instance of the loadable type
									pLoadableType = new LoadableType(sBaseType, *pLoaderTypeClass);
									m_lstTypes.Add(pLoadableType);
									m_mapTypes.Add(sBaseType, pLoadableType);
								} else {
									// Error!
									PL_LOG(Error, "Failed to register the loader class '" + pClass->GetClassName() + "': Loader base class 'Type'-property and loader class 'Type'-property mismatch")
								}
							} else {
								// Error!
								PL_LOG(Error, "Failed to register the loader class '" + pClass->GetClassName() + "': Loader base class has no 'Type'-property")
							}
						} else {
							// Error!
							PL_LOG(Error, "Failed to register the loader class '" + pClass->GetClassName() + "': Unable to find the loader base class")
						}
					}

					// Loadable type instance found?
					if (pLoadableType) {
						// Security check: All loader classes of a certain type must be derived from one and the same loader type class
						if (pClass->IsDerivedFrom(pLoadableType->GetClass())) {
							// Finally, create loader instance and add loader to this loadable type
							pLoadableType->AddLoader(*(new Loader(*pClass)));
						} else {
							// Error!
							PL_LOG(Error, "Failed to register the loader class '" + pClass->GetClassName() + "': Loader base class doesn't match the one specified by the 'Type'-property")
						}
					}
				} else {
					// Error!
					PL_LOG(Error, "Failed to register the loader class '" + pClass->GetClassName() + "': Class has no 'Type'-property")
				}
			}
		}
		m_lstNewClasses.Clear();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
