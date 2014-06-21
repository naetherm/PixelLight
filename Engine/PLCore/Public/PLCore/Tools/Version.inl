/*********************************************************\
 *  File: Version.inl                                    *
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
*    Destructor
*/
inline Version::~Version()
{
}

/**
*  @brief
*    Get project name
*/
inline String Version::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Set project name
*/
inline void Version::SetName(const String &sName)
{
	m_sName = sName;
}

/**
*  @brief
*    Get codename
*/
inline String Version::GetCodename() const
{
	return m_sCodename;
}

/**
*  @brief
*    Set codename
*/
inline void Version::SetCodename(const String &sCodename)
{
	m_sCodename = sCodename;
}

/**
*  @brief
*    Get major version number
*/
inline uint16 Version::GetMajor() const
{
	return m_nMajor;
}

/**
*  @brief
*    Set major version number
*/
inline void Version::SetMajor(uint16 nMajor)
{
	m_nMajor = nMajor;
}

/**
*  @brief
*    Get minor version number
*/
inline uint16 Version::GetMinor() const
{
	return m_nMinor;
}

/**
*  @brief
*    Set minor version number
*/
inline void Version::SetMinor(uint16 nMinor)
{
	m_nMinor = nMinor;
}

/**
*  @brief
*    Get patch number
*/
inline uint16 Version::GetPatch() const
{
	return m_nPatch;
}

/**
*  @brief
*    Set patch number
*/
inline void Version::SetPatch(uint16 nPatch)
{
	m_nPatch = nPatch;
}

/**
*  @brief
*    Get release name
*/
inline String Version::GetRelease() const
{
	return m_sName;
}

/**
*  @brief
*    Set release name
*/
inline void Version::SetRelease(const String &sRelease)
{
	m_sRelease = sRelease;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
