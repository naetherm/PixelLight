/*********************************************************\
 *  File: ImageEffects.inl                               *
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
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create image effect dynamically
*/
inline ImageEffectWrapper ImageEffects::Effect(const PLCore::String &sEffect, const PLCore::String &sParameters)
{
	return ImageEffectWrapper(sEffect, sParameters);
}

/**
*  @brief
*    Create image filter
*/
inline IEFilter ImageEffects::Filter()
{
	return IEFilter();
}

/**
*  @brief
*    Create scale image effect
*/
inline IEScale ImageEffects::Scale(const PLMath::Vector3i &vNewSize, bool bUseMipmaps)
{
	return IEScale(vNewSize, bUseMipmaps);
}

/**
*  @brief
*    Create image conversion effect
*/
inline IEConvert ImageEffects::Convert(EDataFormat nDataFormat, EColorFormat nColorFormat)
{
	return IEConvert(nDataFormat, nColorFormat);
}

/**
*  @brief
*    Create monochrome image effect
*/
inline IEMonochrome ImageEffects::Monochrome()
{
	return IEMonochrome();
}

/**
*  @brief
*    Create color key image effect
*/
inline IEColorKey ImageEffects::ColorKey(const Color3 &cColor, float fTolerance)
{
	return IEColorKey(cColor, fTolerance);
}

/**
*  @brief
*    Create flip the image around the x axis image effect
*/
inline IEFlipXAxis ImageEffects::FlipXAxis()
{
	return IEFlipXAxis();
}

/**
*  @brief
*    Create flip the image around the y axis image effect
*/
inline IEFlipYAxis ImageEffects::FlipYAxis()
{
	return IEFlipYAxis();
}

/**
*  @brief
*    Create remove palette image effect
*/
inline IERemovePalette ImageEffects::RemovePalette()
{
	return IERemovePalette();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
