/*********************************************************\
 *  File: LoadableType.cpp                               *
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
#include "PLCore/System/System.h"
#include "PLCore/Base/Class.h"
#include "PLCore/Tools/Loader.h"
#include "PLCore/Tools/LoadableManager.h"
#include "PLCore/Tools/LoadableType.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Gets the relative version of an absolute file path
*/
String LoadableType::GetRelativeFilePath(const String &sPath) const
{
	// Check path
	if (!sPath.GetLength())
		return ""; // Nothing to do :)

	// Start with validated path
	String sValidPath = Url(sPath).GetUrl();

	// Loop through all base directories
	LoadableManager *pLoadableManager = LoadableManager::GetInstance();
	for (int i=-1; i<static_cast<int>(pLoadableManager->GetNumOfBaseDirs()); i++) {
		// Get base directory
		const String sBaseDir = (i < 0) ? System::GetInstance()->GetCurrentDir() : pLoadableManager->GetBaseDir(i);

		// Is this base directory used?
		if (sValidPath.Compare(sBaseDir, 0, sBaseDir.GetLength())) {
			// Cut off base directory
			sValidPath = sValidPath.GetSubstring(sBaseDir.GetLength()+1);

			// Haha, sometimes I'm using break, too ;-)
			break;
		}
	}

	// Return path
	return sValidPath;
}


//[-------------------------------------------------------]
//[ Formats & loaders                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns loaders by using a loadable extension
*/
void LoadableType::GetLoadersByExtension(const String &sExtension, Array<Loader*> &lstLoaders)
{
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
*    Returns a loader for loading by using a loadable file
*/
Loader *LoadableType::GetLoaderForLoadingByFile(File &cFile) const
{
	// Get the file URL
	const Url &cUrl = cFile.GetUrl();

	{ // Use file extension
		// Get file extension (e.g. "txt" if the filename was "readme.txt", "gz" if the filename was "archive.tar.gz")
		// -> There are file formats without an extension, so no extension must also be valid
		const String sExtension = cUrl.GetExtension();

		// Get a loader by using the loadable extension
		Loader *pLoader = m_mapLoaders.Get(sExtension);

		// Is there a loader and is this loader capable of loading?
		if (pLoader && pLoader->CanLoad()) {
			// Done
			return pLoader;
		} else {
			// Error: Loading of this file format is not supported!
		}
	}

	{ // Still here? Let's try the complete file extension
		// Get complete file extension (e.g. "txt" if the filename was "readme.txt", "tar.gz" if the filename was "archive.tar.gz")
		const String sCompleteExtension = cUrl.GetCompleteExtension();
		if (sCompleteExtension.GetLength()) {
			// Get a loader by using the complete loadable extension
			Loader *pLoader = m_mapLoaders.Get(sCompleteExtension);

			// Is there a loader and is this loader capable of loading?
			if (pLoader && pLoader->CanLoad()) {
				// Done
				return pLoader;
			} else {
				// Error: Loading of this file format is not supported!
			}
		} else {
			// Error: No filename extension at all
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns a loader for loading by using a loadable directory
*/
Loader *LoadableType::GetLoaderForLoadingByDirectory(Directory &cDirectory) const
{
	// Get a loader by using the loadable extension
	// -> We haven't much choice in here, just try no file extension in here
	Loader *pLoader = m_mapLoaders.Get("");

	// Is there a loader and is this loader capable of loading?
	if (pLoader && pLoader->CanLoad()) {
		// Done
		return pLoader;
	} else {
		// Error: Loading of this file format is not supported!
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
LoadableType::LoadableType() :
	m_pClass(nullptr)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Constructor
*/
LoadableType::LoadableType(const String &sName, const Class &cClass) :
	m_sName(sName),
	m_pClass(&cClass)
{
}

/**
*  @brief
*    Destructor
*/
LoadableType::~LoadableType()
{
	// Destroy all loaders managed by this loadable type
	const uint32 nNumOfElements = m_lstLoaders.GetNumOfElements();
	for (uint32 i=0; m_lstLoaders.GetNumOfElements() && i<nNumOfElements; i++) {
		// Remove the first loader, it's removed from the list automatically
		RemoveLoader(*m_lstLoaders[0]);
	}

	// Remove this type from the loadable manager
	LoadableManager::GetInstance()->m_lstTypes.Remove(this);
	LoadableManager::GetInstance()->m_mapTypes.Remove(m_sName);
}

/**
*  @brief
*    Copy operator
*/
LoadableType &LoadableType::operator =(const LoadableType &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Adds a loader to this loadable type
*/
void LoadableType::AddLoader(Loader &cLoader)
{
	// Is the given loader already managed by this loadable type?
	if (cLoader.m_pLoadableType != this) {
		// Add loader
		cLoader.m_pLoadableType = this;
		m_lstLoaders.Add(&cLoader);
		LoadableManager *pLoadableManager = LoadableManager::GetInstance();
		pLoadableManager->m_lstLoaders.Add(&cLoader);

		// Add all formats of this loader
		for (uint32 i=0; i<cLoader.GetNumOfFormats(); i++) {
			// Is there already such a format within this loadable type?
			const String  sFormat = cLoader.GetFormat(i);
			const Loader *pLoader = m_mapLoaders.Get(sFormat);
			if (pLoader) {
				// Two loaders for the same format extension of the same loadable type? That's not ok! Sadly, in fact this can happen
				// due to the tons of available file formats and there's no chance to avoid such naming conflicts...
				PL_LOG(Info, "Loader '" + cLoader.m_pClass->GetClassName() + "': Format '" + sFormat + "' is already used by loader '" + pLoader->m_pClass->GetClassName() + '\'')
			} else {
				// Is there already such a format within the loadable manager?
				pLoader = pLoadableManager->m_mapLoaders.Get(sFormat);
				if (pLoader) {
					// Write a note into the log - but no error because that's a situation that CAN occur, this just means that we can't figure
					// out the loader type by just looking at a given filename extension (but that's usually not required anyway)
					PL_LOG(Debug, "Loader '" + cLoader.m_pClass->GetClassName() + "': Format '" + sFormat + "' is already used by loader '" + pLoader->m_pClass->GetClassName() + "' (uncritical due to different loader types)")

				// Add format to the loadable manager
				} else {
					pLoadableManager->m_mapLoaders.Add(sFormat, &cLoader);
					pLoadableManager->m_lstFormats.Add(sFormat);
					pLoadableManager->m_mapTypesByExtension.Add(sFormat, this);
				}

				// Add format to this loadable type
				m_mapLoaders.Add(sFormat, &cLoader);
				m_lstFormats.Add(sFormat);
			}
		}
	}
}

/**
*  @brief
*    Removes a loader from this loadable type
*/
bool LoadableType::RemoveLoader(Loader &cLoader)
{
	// Is the given loader managed by this loadable type?
	if (cLoader.m_pLoadableType == this) {
		// Remove loader
		cLoader.m_pLoadableType = nullptr;
		m_lstLoaders.Remove(&cLoader);
		LoadableManager *pLoadableManager = LoadableManager::GetInstance();
		pLoadableManager->m_lstLoaders.Remove(&cLoader);

		// Remove all formats of this loader
		for (uint32 i=0; i<cLoader.GetNumOfFormats(); i++) {
			// Get the current format
			const String sFormat = cLoader.GetFormat(i);

			// Remove format from this loadable type
			const Loader *pLoader = m_mapLoaders.Get(sFormat);
			if (pLoader == &cLoader) {
				m_mapLoaders.Remove(sFormat);
				m_lstFormats.Remove(sFormat);
			}

			// Remove format from the loadable manager
			pLoader = pLoadableManager->m_mapLoaders.Get(sFormat);
			if (pLoader == &cLoader) {
				pLoadableManager->m_mapLoaders.Remove(sFormat);
				pLoadableManager->m_lstFormats.Remove(sFormat);
				pLoadableManager->m_mapTypesByExtension.Remove(sFormat);
			}
		}

		// Destroy the given loader instance
		delete &cLoader;

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
