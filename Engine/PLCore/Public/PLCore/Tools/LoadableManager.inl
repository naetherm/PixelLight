/*********************************************************\
 *  File: LoadableManager.inl                            *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of loadable types
*/
inline uint32 LoadableManager::GetNumOfTypes()
{
	RegisterClasses();
	return m_lstTypes.GetNumOfElements();
}

/**
*  @brief
*    Returns a loadable type by using an index
*/
inline LoadableType *LoadableManager::GetTypeByIndex(uint32 nIndex)
{
	RegisterClasses();
	return m_lstTypes.Get(nIndex);
}

/**
*  @brief
*    Returns a loadable type by using it's name
*/
inline LoadableType *LoadableManager::GetTypeByName(const String &sName)
{
	RegisterClasses();
	return m_mapTypes.Get(sName);
}

/**
*  @brief
*    Returns a loadable type by using a loadable extension
*/
inline LoadableType *LoadableManager::GetTypeByExtension(const String &sExtension)
{
	RegisterClasses();
	return m_mapTypesByExtension.Get(sExtension);
}

/**
*  @brief
*    Returns the number of loaders
*/
inline uint32 LoadableManager::GetNumOfLoaders()
{
	RegisterClasses();
	return m_lstLoaders.GetNumOfElements();
}

/**
*  @brief
*    Returns a loader by using an index
*/
inline Loader *LoadableManager::GetLoaderByIndex(uint32 nIndex)
{
	RegisterClasses();
	return m_lstLoaders.Get(nIndex);
}

/**
*  @brief
*    Returns a loader by using a extension
*/
inline Loader *LoadableManager::GetLoaderByExtension(const String &sExtension)
{
	RegisterClasses();
	return m_mapLoaders.Get(sExtension);
}

/**
*  @brief
*    Returns the number of supported formats
*/
inline uint32 LoadableManager::GetNumOfFormats()
{
	RegisterClasses();
	return m_lstFormats.GetNumOfElements();
}

/**
*  @brief
*    Returns a supported format
*/
inline String LoadableManager::GetFormat(uint32 nIndex)
{
	RegisterClasses();
	return m_lstFormats.Get(nIndex);
}

/**
*  @brief
*    Returns the number of base directories
*/
inline uint32 LoadableManager::GetNumOfBaseDirs() const
{
	return m_lstBaseDirs.GetNumOfElements();
}

/**
*  @brief
*    Returns one of the base directories
*/
inline String LoadableManager::GetBaseDir(uint32 nNum) const
{
	// Get directory
	return m_lstBaseDirs[nNum];
}

/**
*  @brief
*    Removes all base directories
*/
inline bool LoadableManager::ClearBaseDirs()
{
	// Delete all base dirs
	m_lstBaseDirs.Clear();

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
