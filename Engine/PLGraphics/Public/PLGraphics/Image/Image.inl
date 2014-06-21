/*********************************************************\
 *  File: Image.inl                                      *
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
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if this image is a multi-part image (e.g. a CubeMap)
*/
inline bool Image::IsMultiPart() const
{
	// Check if there is more than one part
	return (m_lstParts.GetNumOfElements() > 1);
}

/**
*  @brief
*    Get number of image parts
*/
inline PLCore::uint32 Image::GetNumOfParts() const
{
	// Return number of parts
	return m_lstParts.GetNumOfElements();
}

/**
*  @brief
*    Get image part
*/
inline ImagePart *Image::GetPart(PLCore::uint32 nIndex) const
{
	// Return part
	return m_lstParts[nIndex];
}

/**
*  @brief
*    Get image part by semantics
*/
inline ImagePart *Image::GetPartBySemantics(PLCore::uint32 nSemantics) const
{
	// Return part
	return m_mapParts.Get(nSemantics);
}

/**
*  @brief
*    Get image parts
*/
inline const PLCore::Container<ImagePart*> &Image::GetParts() const
{
	// Return part list
	return m_lstParts;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
