/*********************************************************\
 *  File: TransferFunction.cpp                           *
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
#include <PLMath/Math.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/ResourceHandler.h>
#include "PLVolume/TransferFunction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLVolume {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TransferFunction::TransferFunction()
{
	for (uint32 i=0; i<NumOfTextureBufferTypes; i++) {
		m_pTextureBufferHandler[i] = new ResourceHandler();
		m_bTextureBufferDirty[i] = true;
	}
}

/**
*  @brief
*    Destructor
*/
TransferFunction::~TransferFunction()
{
	for (uint32 i=0; i<NumOfTextureBufferTypes; i++) {
		// Destroy the renderer texture buffer
		if (m_pTextureBufferHandler[i]->GetResource())
			delete m_pTextureBufferHandler[i]->GetResource();

		// Destroy the texture buffer handler
		delete m_pTextureBufferHandler[i];
	}
}

/**
*  @brief
*    Returns the number of bins
*/
uint32 TransferFunction::GetNumOfBins() const
{
	// [TODO] Flexible number of bins
	return 256;
}

// "Pre-Integration" as described within the book "Real-Time Volume Graphics", section "4.5 Pre-Integrated Transfer Functions" (page 92) and section "7.2 Single-Pass GPU Ray Casting for Uniform Grids" (page 167).
// Listing 4.5. C code for computing a 256 × 256 pre-integration look-up texture
// from an 8-bit transfer function using integral functions.
TextureBuffer *createPreintegrationTable(Renderer &cRenderer, uint8* Table) {
	double tauc=0.,r=0.,g=0.,b=0.,a=0.; int rcol, gcol, bcol, acol;
	double rInt[256],gInt[256],bInt[256],aInt[256];
	uint8 lookupImg[256*256*4]; int smin,smax;
	double factor; int lookupindex = 0;
	rInt[0] = 0.;gInt[0] = 0.;bInt[0] = 0.;aInt[0] = 0.;
	// compute integral functions
	for (int i=1;i<256;i++) {
		tauc = (Table[(i-1)*4+3]+Table[i*4+3])/2.;
		r = r + (Table[(i-1)*4+0]+Table[i*4+0])/2.*tauc/255.;
		g = g + (Table[(i-1)*4+1]+Table[i*4+1])/2.*tauc/255.;
		b = b + (Table[(i-1)*4+2]+Table[i*4+2])/2.*tauc/255.;
		a = a + tauc;
		rInt[i] = r;gInt[i] = g;bInt[i] = b;aInt[i] = a; }
	// compute look-up table from integral functions
	for (int sb=0;sb<256;sb++)
		for (int sf=0;sf<256;sf++) {
			if (sb < sf) { smin = sb;smax = sf; }
			else { smin = sf;smax = sb; }
			if (smax != smin) {
				factor = 1. / (double)(smax - smin);
				rcol = (rInt[smax] - rInt[smin]) * factor;
				gcol = (gInt[smax] - gInt[smin]) * factor;
				bcol = (bInt[smax] - bInt[smin]) * factor;
				acol = 256.*
				(1.-exp(-(aInt[smax]-aInt[smin])*factor/255.));
			} else {
				factor = 1. / 255.;
				rcol = Table[smin*4+0]*Table[smin*4+3]*factor;
				gcol = Table[smin*4+1]*Table[smin*4+3]*factor;
				bcol = Table[smin*4+2]*Table[smin*4+3]*factor;
				acol = (1.-exp(-Table[smin*4+3]*1./255.))*256.; }
			lookupImg[lookupindex++] = Math::ClampToInterval(rcol,0,255);
			lookupImg[lookupindex++] = Math::ClampToInterval(gcol,0,255);
			lookupImg[lookupindex++] = Math::ClampToInterval(bcol,0,255);
			lookupImg[lookupindex++] = Math::ClampToInterval(acol,0,255); }
	// create texture
	Image cImage = Image::CreateImageAndShareData(DataByte, ColorRGBA, Vector3i(256, 256, 1), CompressionNone, lookupImg);
	return reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer2D(cImage, TextureBuffer::Unknown, 0));
}

/**
*  @brief
*    Returns the renderer texture buffer holding the transfer function
*/
TextureBuffer *TransferFunction::GetTextureBuffer(Renderer &cRenderer, ETextureBufferType nType)
{
	// Is the cached texture buffer considered to be dirty?
	if (m_bTextureBufferDirty[nType]) {
		// [TODO] Add optimization: Do only create a new texture buffer in case there's no way around it
		if (m_pTextureBufferHandler[nType]->GetResource())
			delete m_pTextureBufferHandler[nType]->GetResource();

		// Get the image instance, this call will update the image data if required
		Image &cSourceImage = GetImage();

		// Create a new texture buffer?
		if (!m_pTextureBufferHandler[nType]->GetResource() && cSourceImage.GetNumOfParts()) {
			switch (nType) {
				// Texture buffer without any special applied features
				case StandardTextureBuffer:
					// Create texture buffer
					m_pTextureBufferHandler[nType]->SetResource(reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer1D(cSourceImage, TextureBuffer::Unknown, 0)));
					break;

				// Texture buffer with pre-multiplied color by their corresponding opacity value in order to avoid color bleeding due to interpolation
				case PreMultipliedTextureBuffer:
				{
					// Get the number of bins
					const uint32 nNumOfBins = GetNumOfBins();

					// Pre-multiply color by their corresponding opacity value in order to avoid color bleeding due to interpolation
					Image cPreMultipliedImage = Image::CreateImage(DataFloat, ColorRGBA, Vector3i(nNumOfBins, 1, 1));
					const uint8 *pnSourceData = cSourceImage.GetBuffer()->GetData();
					float *pfDestinationData = reinterpret_cast<float*>(cPreMultipliedImage.GetBuffer()->GetData());
					for (uint32 i=0; i<nNumOfBins; i++, pnSourceData+=4) {
						// Get alpha
						const float fAlpha = static_cast<float>(pnSourceData[3])/255.0f;

						// Pre-multiply red
						*pfDestinationData = static_cast<float>(pnSourceData[0])/255.0f*fAlpha;
						pfDestinationData++;

						// Pre-multiply green
						*pfDestinationData = static_cast<float>(pnSourceData[1])/255.0f*fAlpha;
						pfDestinationData++;

						// Pre-multiply blue
						*pfDestinationData = static_cast<float>(pnSourceData[2])/255.0f*fAlpha;
						pfDestinationData++;

						// Copy alpha
						*pfDestinationData = fAlpha;
						pfDestinationData++;
					}

					// Create texture buffer
					m_pTextureBufferHandler[nType]->SetResource(reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer1D(cPreMultipliedImage, TextureBuffer::Unknown, 0)));
					break;
				}

				case PreIntegratedTextureBuffer:
				{
						/*
					for (int i=0; i<256; i++) {
						nTable[i*4 + 0] = i;
						nTable[i*4 + 1] = i;
						nTable[i*4 + 2] = i;
		*/
					uint8 nTable[256*4];
					const uint8 *pData = cSourceImage.GetBuffer()->GetData();
					for (int i=0; i<256; i++, pData+=4) {
						nTable[i*4 + 0] = pData[0];
						nTable[i*4 + 1] = pData[1];
						nTable[i*4 + 2] = pData[2];
						nTable[i*4 + 3] = pData[3];


						/*
						// Simple threshold for alpha
						// [TODO] Move this option outside
						float fAlpha = static_cast<float>(pData[3])/255.0f;
		//				float fAlpha = static_cast<float>(i)/255.0f;
						fAlpha = (fAlpha > 0.06f) ? fAlpha : 0.0f;

						// Set alpha
						nTable[i*4 + 3] = fAlpha*255.0f;
						*/
					}

					// Create texture buffer
					m_pTextureBufferHandler[nType]->SetResource(reinterpret_cast<TextureBuffer*>(createPreintegrationTable(cRenderer, nTable)));
					break;
				}

				case NumOfTextureBufferTypes:
				default:
					// Invalid cases, nothing to do in here
					break;
			}
		}

		// Cached texture buffer is no longer considered to be dirty
		m_bTextureBufferDirty[nType] = false;
	}

	// Return the texture buffer
	return static_cast<TextureBuffer*>(m_pTextureBufferHandler[nType]->GetResource());
}


//[-------------------------------------------------------]
//[ Public virtual TransferFunction functions             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the image holding the transfer function
*/
Image &TransferFunction::GetImage()
{
	// Cached texture buffer is now considered to be dirty
	for (uint32 i=0; i<NumOfTextureBufferTypes; i++)
		m_bTextureBufferDirty[i] = true;

	// Return the image holding the transfer function
	return m_cImage;
}

/**
*  @brief
*    Returns the image holding the transfer function
*/
const Image &TransferFunction::GetConstImage()
{
	// Return the image holding the transfer function
	return m_cImage;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool TransferFunction::Unload()
{
	// Unload the image holding the transfer function
	m_cImage.Unload();

	// Destroy the renderer texture buffer
	for (uint32 i=0; i<NumOfTextureBufferTypes; i++) {
		if (m_pTextureBufferHandler[i]->GetResource()) {
			delete m_pTextureBufferHandler[i]->GetResource();
			m_bTextureBufferDirty[i] = true;
		}
	}

	// Call base implementation
	return Loadable::Unload();
}

String TransferFunction::GetLoadableTypeName() const
{
	static const String sString = "TransferFunction";
	return sString;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
TransferFunction::TransferFunction(const TransferFunction &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
TransferFunction &TransferFunction::operator =(const TransferFunction &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
