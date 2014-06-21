/*********************************************************\
 *  File: LoadableType.inl                               *
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
*    Returns the name of the loadable type
*/
inline String LoadableType::GetName() const
{
	// Return description
	return m_sName;
}

/**
*  @brief
*    Returns the class of the loadable type
*/
inline const Class &LoadableType::GetClass() const
{
	return *m_pClass;
}


//[-------------------------------------------------------]
//[ Formats & loaders                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of supported formats
*/
inline uint32 LoadableType::GetNumOfFormats() const
{
	return m_lstFormats.GetNumOfElements();
}

/**
*  @brief
*    Returns a supported formats
*/
inline String LoadableType::GetFormat(uint32 nIndex) const
{
	return m_lstFormats.Get(nIndex);
}

/**
*  @brief
*    Returns the number of loaders
*/
inline uint32 LoadableType::GetNumOfLoaders() const
{
	return m_lstLoaders.GetNumOfElements();
}

/**
*  @brief
*    Returns a loader by using an index
*/
inline Loader *LoadableType::GetLoaderByIndex(uint32 nIndex) const
{
	return m_lstLoaders.Get(nIndex);
}

/**
*  @brief
*    Returns a loader by using a loadable extension
*/
inline Loader *LoadableType::GetLoaderByExtension(const String &sExtension) const
{
	return m_mapLoaders.Get(sExtension);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
