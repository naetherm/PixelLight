/*********************************************************\
 *  File: DynLib.inl                                     *
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
#include "PLCore/System/DynLibImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads a dynamic library
*/
inline bool DynLib::Load(const String &sFilename)
{
	// Save filename
	m_cUrl = sFilename;

	// Call system implementation
	return m_pDynLibImpl->Load(m_cUrl);
}

/**
*  @brief
*    Returns if the dynamic library has been loaded
*/
inline bool DynLib::IsLoaded() const
{
	// Return whether the library is loaded
	return m_pDynLibImpl->IsLoaded();
}

/**
*  @brief
*    Get the path (set within 'Load()') to the dynamic library
*/
inline String DynLib::GetPath() const
{
	// Return filename
	return m_cUrl.GetUrl();
}

/**
*  @brief
*    Get the absolute path to the dynamic library
*/
inline String DynLib::GetAbsPath() const
{
	return m_pDynLibImpl->GetAbsPath();
}

/**
*  @brief
*    Returns a pointer to a symbol in the library
*/
inline void *DynLib::GetSymbol(const String &sSymbol) const
{
	// Return symbol pointer
	return m_pDynLibImpl->GetSymbol(sSymbol);
}

/**
*  @brief
*    Unloads the dynamic library
*/
inline bool DynLib::Unload()
{
	// Unload library
	return m_pDynLibImpl->Unload();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
