/*********************************************************\
 *  File: ImagePart.inl                                  *
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
*    Constructor
*/
inline ImagePart::ImagePart() :
	m_nSemantics(0)
{
}

/**
*  @brief
*    Destructor
*/
inline ImagePart::~ImagePart()
{
	// Clear data
	Clear();
}

/**
*  @brief
*    Get semantics
*/
inline PLCore::uint32 ImagePart::GetSemantics() const
{
	// Return semantics
	return m_nSemantics;
}

/**
*  @brief
*    Set semantics
*/
inline void ImagePart::SetSemantics(PLCore::uint32 nSemantics)
{
	// Set semantics
	m_nSemantics = nSemantics;
}

/**
*  @brief
*    Check if the layer has mipmaps
*/
inline bool ImagePart::HasMipmaps() const
{
	// Check if there is more than one mipmap
	return (m_lstMipmaps.GetNumOfElements() > 1);
}

/**
*  @brief
*    Get number of mipmaps
*/
inline PLCore::uint32 ImagePart::GetNumOfMipmaps() const
{
	// Return number of mipmaps
	return m_lstMipmaps.GetNumOfElements();
}

/**
*  @brief
*    Get mipmap
*/
inline ImageBuffer *ImagePart::GetMipmap(PLCore::uint32 nIndex) const
{
	// Return mipmap
	return m_lstMipmaps[nIndex];
}

/**
*  @brief
*    Get mipmaps
*/
inline const PLCore::Container<ImageBuffer*> &ImagePart::GetMipmaps() const
{
	// Return mipmap list
	return m_lstMipmaps;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
