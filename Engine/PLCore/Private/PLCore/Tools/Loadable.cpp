/*********************************************************\
 *  File: Loadable.cpp                                   *
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
#include "PLCore/Base/Class.h"
#include "PLCore/Tools/Loader.h"
#include "PLCore/Tools/LoaderImpl.h"
#include "PLCore/Tools/LoadableType.h"
#include "PLCore/Tools/LoadableManager.h"
#include "PLCore/Tools/Loadable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public virtual Loadable functions                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads the loadable from a file given by filename
*/
bool Loadable::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Check the given filename
	if (sFilename.GetLength()) {
		// Get the registered loadable type ("Mesh", "Scene" etc.) of this loadable implementation
		const LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName(GetLoadableTypeName());
		if (pLoadableType) {
			// Open the file
			File cFile;
			if (LoadableManager::GetInstance()->OpenFile(cFile, sFilename, false)) {
				// Get loader, assigned to the current loadable type, capable of processing the file
				Loader *pLoader = pLoadableType->GetLoaderForLoadingByFile(cFile);
				if (pLoader) {
					// Because 'm_sAbsFilename' is reset before loading within the Unload()-function,
					// and 'sFilename' can be a reference to this string, we need to duplicate the string!
					const String sFilenameBackup = String(sFilename);

					// Unload the loadable
					Unload();

					// Get method name
					static const String sLoad = "Load";
					static const String sLoadParams = "LoadParams";
					String sMethodName = sMethod;
					if (!sMethodName.GetLength())
						sMethodName = sParams.GetLength() ? sLoadParams : sLoad;

					// Load the loadable
					const bool bResult = CallLoadable(cFile, *pLoader, sMethodName, sParams);
					if (bResult) {
						// Backup filename
						m_sUrl		= cFile.GetUrl().GetUrl();
						m_sFilename	= sFilenameBackup;	// The filename this loadable was given to load from
					}

					// Done
					return bResult;
				} else {
					PL_LOG(Error, "Can't load the loadable '" + GetLoadableTypeName() + "' from '" + sFilename + "' because the file format is not supported!")
				}
			} else {
				// We failed to open the file, maybe it's a directory? (e.g. DICOM is using directories for the data)

				// Open the directory
				Directory cDirectory;
				if (LoadableManager::GetInstance()->OpenDirectory(cDirectory, sFilename, false)) {
					// Get loader, assigned to the current loadable type, capable of processing the directory
					Loader *pLoader = pLoadableType->GetLoaderForLoadingByDirectory(cDirectory);
					if (pLoader) {
						// Because 'm_sAbsFilename' is reset before loading within the Unload()-function,
						// and 'sFilename' can be a reference to this string, we need to duplicate the string!
						const String sFilenameBackup = String(sFilename);

						// Unload the loadable
						Unload();

						// Get method name
						static const String sLoadDirectory = "LoadDirectory";
						static const String sLoadDirectoryParams = "LoadDirectoryParams";
						String sMethodName = sMethod;
						if (!sMethodName.GetLength())
							sMethodName = sParams.GetLength() ? sLoadDirectoryParams : sLoadDirectory;

						// Load the loadable
						const bool bResult = CallLoadable(cDirectory, *pLoader, sMethodName, sParams);
						if (bResult) {
							// Backup filename
							m_sUrl		= cDirectory.GetUrl().GetUrl();
							m_sFilename	= sFilenameBackup;	// The filename this loadable was given to load from
						}

						// Done
						return bResult;
					} else {
						PL_LOG(Error, "Can't load the loadable '" + GetLoadableTypeName() + "' from '" + sFilename + "' because the file format is not supported!")
					}
				} else {
					// We failed, again
					PL_LOG(Error, "Can't open the file '" + sFilename + "' to load in the loadable '" + GetLoadableTypeName() + "'!")
				}
			}
		} else {
			PL_LOG(Error, "Can't load '" + sFilename + "' because the loadable type '" + GetLoadableTypeName() + "' is unknown!")
		}
	} else {
		PL_LOG(Error, "Can't load the loadable '" + GetLoadableTypeName() + "' because there's no given filename!")
	}

	// Error!
	return false;
}

/**
*  @brief
*    Loads the loadable from a file given by a reference
*/
bool Loadable::LoadByFile(File &cFile, const String &sParams, const String &sMethod)
{
	// Check whether file reading is allowed
	if (cFile.IsReadable()) {
		// Get the registered loadable type ("Mesh", "Scene" etc.) of this loadable implementation
		const LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName(GetLoadableTypeName());
		if (pLoadableType) {
			// Get loader, assigned to the current loadable type, capable of processing the file
			Loader *pLoader = pLoadableType->GetLoaderForLoadingByFile(cFile);
			if (pLoader) {
				// Unload the loadable
				Unload();

				// Get method name
				static const String sLoad = "Load";
				static const String sLoadParams = "LoadParams";
				String sMethodName = sMethod;
				if (!sMethodName.GetLength())
					sMethodName = sParams.GetLength() ? sLoadParams : sLoad;

				// Load the loadable
				bool bResult = CallLoadable(cFile, *pLoader, sMethodName, sParams);
				if (bResult) {
					// Backup filename
					m_sFilename	= m_sUrl = cFile.GetUrl().GetFilename();
				}

				// Done
				return bResult;
			} else {
				PL_LOG(Error, "Can't load the loadable '" + GetLoadableTypeName() + "' from '" + cFile.GetUrl().GetNativePath() + "' because the file format is not supported!")
			}
		} else {
			PL_LOG(Error, "Can't load '" + cFile.GetUrl().GetNativePath() + "' because the loadable type '" + GetLoadableTypeName() + "' is unknown!")
		}
	} else {
		PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' because the given file is not readable!")
	}

	// Error!
	return false;
}

/**
*  @brief
*    Loads the loadable from a directory given by a reference
*/
bool Loadable::LoadByDirectory(Directory &cDirectory, const String &sParams, const String &sMethod)
{
	// Get the registered loadable type ("Mesh", "Scene" etc.) of this loadable implementation
	const LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName(GetLoadableTypeName());
	if (pLoadableType) {
		// Get loader, assigned to the current loadable type, capable of processing the directory
		Loader *pLoader = pLoadableType->GetLoaderForLoadingByDirectory(cDirectory);
		if (pLoader) {
			// Unload the loadable
			Unload();

			// Get method name
			static const String sLoadDirectory = "LoadDirectory";
			static const String sLoadDirectoryParams = "LoadDirectoryParams";
			String sMethodName = sMethod;
			if (!sMethodName.GetLength())
				sMethodName = sParams.GetLength() ? sLoadDirectoryParams : sLoadDirectory;

			// Load the loadable
			bool bResult = CallLoadable(cDirectory, *pLoader, sMethodName, sParams);
			if (bResult) {
				// Backup filename
				m_sFilename	= m_sUrl = cDirectory.GetUrl().GetFilename();
			}

			// Done
			return bResult;
		} else {
			PL_LOG(Error, "Can't load the loadable '" + GetLoadableTypeName() + "' from '" + cDirectory.GetUrl().GetNativePath() + "' because the file format is not supported!")
		}
	} else {
		PL_LOG(Error, "Can't load '" + cDirectory.GetUrl().GetNativePath() + "' because the loadable type '" + GetLoadableTypeName() + "' is unknown!")
	}

	// Error!
	return false;
}

/**
*  @brief
*    Saves the loadable to a file given by filename
*/
bool Loadable::SaveByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Check the given filename
	if (sFilename.GetLength()) {
		// Get file extension (e.g. "txt" if the filename was "readme.txt", "gz" if the filename was "archive.tar.gz")
		// -> There are file formats without an extension, so no extension must also be valid
		const Url &cUrl = sFilename;
		const String sExtension = cUrl.GetExtension();

		// Get the registered loadable type ("Mesh", "Scene" etc.) of this loadable implementation
		const LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName(GetLoadableTypeName());
		if (pLoadableType) {
			// Get loader, assigned to the current loadable type, capable of processing the file extension
			Loader *pLoader = pLoadableType->GetLoaderByExtension(sExtension);
			if (pLoader) {
				// Is this loader capable of saving?
				if (pLoader->CanSave()) {
					// Open the file
					File cFile;
					if (LoadableManager::GetInstance()->OpenFile(cFile, sFilename, true)) {
						// Get method name
						static const String sSave = "Save";
						static const String sSaveParams = "SaveParams";
						String sMethodName = sMethod;
						if (!sMethodName.GetLength())
							sMethodName = sParams.GetLength() ? sSaveParams : sSave;

						// Save
						return CallLoadable(cFile, *pLoader, sMethodName, sParams);
					} else {
						PL_LOG(Error, "Can't open the file '" + sFilename + "' to save the loadable '" + GetLoadableTypeName() + "'!")
					}
				} else {
					PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' to '" + sFilename + "' because saving of this file format is not supported!")
				}
			} else {
				PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' to '" + sFilename + "' because the file format is not supported!")
			}
		} else {
			PL_LOG(Error, "Can't save '" + sFilename + "' because the loadable type '" + GetLoadableTypeName() + "' is unknown!")
		}
	} else {
		PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' because there's no given filename!")
	}

	// Error!
	return false;
}

/**
*  @brief
*    Saves the loadable to a file given by reference
*/
bool Loadable::SaveByFile(File &cFile, const String &sParams, const String &sMethod)
{
	// Check whether file writing is allowed
	if (cFile.IsWritable()) {
		// Get the filename of the given file
		const Url &cUrl = cFile.GetUrl();

		// Check parameter
		if (cUrl.GetNativePath().GetLength()) {
			// Get file extension (e.g. "txt" if the filename was "readme.txt", "gz" if the filename was "archive.tar.gz")
			// -> There are file formats without an extension, so no extension must also be valid
			const String sExtension = cUrl.GetExtension();

			// Get the registered loadable type ("Mesh", "Scene" etc.) of this loadable implementation
			const LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName(GetLoadableTypeName());
			if (pLoadableType) {
				// Get loader, assigned to the current loadable type, capable of processing the file extension
				Loader *pLoader = pLoadableType->GetLoaderByExtension(sExtension);
				if (pLoader) {
					// Is this loader capable of saving?
					if (pLoader->CanSave()) {
						// Get method name
						static const String sSave = "Save";
						static const String sSaveParams = "SaveParams";
						String sMethodName = sMethod;
						if (!sMethodName.GetLength())
							sMethodName = sParams.GetLength() ? sSaveParams : sSave;

						// Save
						return CallLoadable(cFile, *pLoader, sMethodName, sParams);
					} else {
						PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' to '" + cUrl.GetNativePath() + "' because saving of this file format is not supported!")
					}
				} else {
					PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' to '" + cUrl.GetNativePath() + "' because the file format is not supported!")
				}
			} else {
				PL_LOG(Error, "Can't save '" + cUrl.GetNativePath() + "' because the loadable type '" + GetLoadableTypeName() + "' is unknown!")
			}
		} else {
			PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' because there's no given filename!")
		}
	} else {
		PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' because the given file is not writable!")
	}

	// Error!
	return false;
}

/**
*  @brief
*    Saves the loadable to a directory given by reference
*/
bool Loadable::SaveByDirectory(Directory &cDirectory, const String &sParams, const String &sMethod)
{
	// Get the filename of the given directory
	const Url &cUrl = cDirectory.GetUrl();

	// Check parameter
	if (cUrl.GetNativePath().GetLength()) {
		// Get the registered loadable type ("Mesh", "Scene" etc.) of this loadable implementation
		const LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName(GetLoadableTypeName());
		if (pLoadableType) {
			// Get loader, assigned to the current loadable type, capable of processing the directory
			Loader *pLoader = pLoadableType->GetLoaderByExtension("");	// We don't have an extension we can work with
			if (pLoader) {
				// Is this loader capable of saving?
				if (pLoader->CanSave()) {
					// Get method name
					static const String sSaveDirectory = "SaveDirectory";
					static const String sSaveDirectoryParams = "SaveDirectoryParams";
					String sMethodName = sMethod;
					if (!sMethodName.GetLength())
						sMethodName = sParams.GetLength() ? sSaveDirectoryParams : sSaveDirectory;

					// Save
					return CallLoadable(cDirectory, *pLoader, sMethodName, sParams);
				} else {
					PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' to '" + cUrl.GetNativePath() + "' because saving of this file format is not supported!")
				}
			} else {
				PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' to '" + cUrl.GetNativePath() + "' because the file format is not supported!")
			}
		} else {
			PL_LOG(Error, "Can't save '" + cUrl.GetNativePath() + "' because the loadable type '" + GetLoadableTypeName() + "' is unknown!")
		}
	} else {
		PL_LOG(Error, "Can't save the loadable '" + GetLoadableTypeName() + "' because there's no given filename!")
	}

	// Error!
	return false;
}

/**
*  @brief
*    Reloads the loadable
*/
bool Loadable::Reload()
{
	// Any URL set?
	if (m_sUrl.GetLength()) {
		// Backup the current URL in case that it is reset during loading -> because we pass a reference to this
		// string the loading will fail in this situation!
		String sUrl = m_sUrl;

		// Load
		return LoadByFilename(sUrl);
	}

	// Done
	return true;
}

/**
*  @brief
*    Unloads the loadable
*/
bool Loadable::Unload()
{
	// Reset the filenames
	m_sFilename	= "";
	m_sUrl		= "";

	// Done
	return true;
}

/**
*  @brief
*    Returns the loadable type name
*/
String Loadable::GetLoadableTypeName() const
{
	return "";
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Loadable::Loadable()
{
}

/**
*  @brief
*    Destructor
*/
Loadable::~Loadable()
{
	// Unload the loadable
	Unload();
}


//[-------------------------------------------------------]
//[ Protected virtual Loadable functions                  ]
//[-------------------------------------------------------]
bool Loadable::CallLoadable(File &cFile, Loader &cLoader, const String &sMethod, const String &sParams)
{
	// Get the loader implementation
	LoaderImpl *pLoaderImpl = cLoader.GetImpl();
	if (pLoaderImpl) {
		// Load
		if (sParams.GetLength()) {
			pLoaderImpl->CallMethod(sMethod, "Param0=\"" + Type<Loadable&>::ConvertToString(*this) + "\" Param1=\"" + Type<File&>::ConvertToString(cFile) + "\" " + sParams);
			return true;
		} else {
			Params<bool, Loadable&, File&> cParams(*this, cFile);
			pLoaderImpl->CallMethod(sMethod, cParams);
			return cParams.Return;
		}
	}

	// Error!
	return false;
}

bool Loadable::CallLoadable(Directory &cDirectory, Loader &cLoader, const String &sMethod, const String &sParams)
{
	// Get the loader implementation
	LoaderImpl *pLoaderImpl = cLoader.GetImpl();
	if (pLoaderImpl) {
		// Load
		if (sParams.GetLength()) {
			pLoaderImpl->CallMethod(sMethod, "Param0=\"" + Type<Loadable&>::ConvertToString(*this) + "\" Param1=\"" + Type<Directory&>::ConvertToString(cDirectory) + "\" " + sParams);
			return true;
		} else {
			Params<bool, Loadable&, Directory&> cParams(*this, cDirectory);
			pLoaderImpl->CallMethod(sMethod, cParams);
			return cParams.Return;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
