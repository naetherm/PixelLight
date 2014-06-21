/*********************************************************\
 *  File: Surface.inl                                    *
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
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the owner renderer
*/
inline Renderer &Surface::GetRenderer() const
{
	return *m_pRenderer;
}

/**
*  @brief
*    Returns the surface type
*/
inline Surface::EType Surface::GetType() const
{
	return m_nType;
}

/**
*  @brief
*    Returns whether the surface is active or not
*/
inline bool Surface::IsActive() const
{
	return m_bActive;
}

/**
*  @brief
*    Sets whether the surface is active or not
*/
inline void Surface::SetActive(bool bActive)
{
	m_bActive = bActive;
}

/**
*  @brief
*    Returns the surface painter
*/
inline SurfacePainter *Surface::GetPainter() const
{
	return m_pPainter;
}

/**
*  @brief
*    Returns the whether the surface is flipped along the y axis
*/
inline bool Surface::IsSwapY() const
{
	return m_bSwapY;
}

/**
*  @brief
*    Sets the whether the surface is flipped along the y axis
*/
inline void Surface::SetSwapY(bool bSwapY)
{
	m_bSwapY = bSwapY;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
