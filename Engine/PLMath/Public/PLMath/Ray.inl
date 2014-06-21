/*********************************************************\
 *  File: Ray.inl                                        *
 *      Ray inline implementation
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
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting all position and direction components to 0
*/
inline Ray::Ray()
{
}

/**
*  @brief
*    Copy constructor
*/
inline Ray::Ray(const Ray &cSource) :
	m_vPos(cSource.m_vPos),
	m_vDir(cSource.m_vDir)
{
}

/**
*  @brief
*    Destructor
*/
inline Ray::~Ray()
{
}

/**
*  @brief
*    Copy operator
*/
inline Ray &Ray::operator =(const Ray &cSource)
{
	m_vPos = cSource.m_vPos;
	m_vDir = cSource.m_vDir;

	// Return this
	return *this;
}

/**
*  @brief
*    Set the current ray using a line
*/
inline void Ray::Set(const Vector3 &vLineStartPos, const Vector3 &vLineEndPos)
{
	m_vPos = vLineStartPos;
	m_vDir = vLineEndPos-vLineStartPos;
}

/**
*  @brief
*    Returns the current position
*/
inline const Vector3 &Ray::GetPos() const
{
	return m_vPos;
}

/**
*  @brief
*    Set the current position
*/
inline void Ray::SetPos(float fX, float fY, float fZ)
{
	m_vPos.SetXYZ(fX, fY, fZ);
}

/**
*  @brief
*    Set the current position
*/
inline void Ray::SetPos(const Vector3 &vPos)
{
	m_vPos = vPos;
}

/**
*  @brief
*    Returns the ray direction
*/
inline const Vector3 &Ray::GetDir() const
{
	return m_vDir;
}

/**
*  @brief
*    Set the ray direction
*/
inline void Ray::SetDir(float fX, float fY, float fZ)
{
	m_vDir.SetXYZ(fX, fY, fZ);
}

/**
*  @brief
*    Set the ray direction
*/
inline void Ray::SetDir(const Vector3 &vDir)
{
	m_vDir = vDir;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
