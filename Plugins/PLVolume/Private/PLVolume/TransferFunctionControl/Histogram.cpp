/*********************************************************\
 *  File: Histogram.cpp                                  *
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
#include <PLMath/Vector3i.h>
#include <PLGraphics/Color/Color4.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLVolume/TransferFunctionControl/Histogram.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLVolume {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates the histogram data from luminance data (= one component per element)
*/
void Histogram::Create(uint32 nNumOfBytesPerElement, uint32 nNumOfElements, const uint8 *pnData)
{
	// Check number of bytes per element (pixel/voxel), must be 1 or 2
	if (nNumOfBytesPerElement == 1 || nNumOfBytesPerElement == 2) {
		// Initialize minimum and maximum value encountered while creating the histogram
		m_fMinScalarValue = (nNumOfBytesPerElement == 1) ? 256.0f : 65536.0f;
		m_fMaxScalarValue = 0.0f;

		// Initialize the minimum and maximum value within the histogram
		m_fMinValue = -1.0f;
		m_fMaxValue = -1.0f;

		// Bin with the highest value not initialized, yet
		m_nBinWithHighestValue = -1;

		// Not normalized, yet
		m_bNormalized = false;

		// Allocate the array
		m_lstData.Resize((nNumOfBytesPerElement == 1) ? 256 : 65536, true, true);

		// Evaluate luminance data to create the histogram data from, must be valid!
		if (nNumOfElements && pnData) {
			float *pfHistogram = m_lstData.GetData();
			if (nNumOfBytesPerElement == 1) {
				// One byte per element
				const uint8 *pnDataEnd = pnData + nNumOfElements;
				for (const uint8 *pnCurrentData=pnData; pnCurrentData<pnDataEnd; pnCurrentData++) {
					// Get the current value
					const uint32 nValue = *pnCurrentData;

					// Update minimum and maximum value encountered while creating the histogram
					if (m_fMinScalarValue > nValue)
						m_fMinScalarValue = static_cast<float>(nValue);
					if (m_fMaxScalarValue < nValue)
						m_fMaxScalarValue = static_cast<float>(nValue);

					// Update the histogram
					pfHistogram[nValue] += 1.0f;
				}
			} else {
				// Two bytes per element
				const uint16 *pnDataEnd = reinterpret_cast<const uint16*>(pnData) + nNumOfElements;
				for (const uint16 *pnCurrentData=reinterpret_cast<const uint16*>(pnData); pnCurrentData<pnDataEnd; pnCurrentData++) {
					// Get the current value
					const uint32 nValue = *pnCurrentData;

					// Update minimum and maximum value encountered while creating the histogram
					if (m_fMinScalarValue > nValue)
						m_fMinScalarValue = static_cast<float>(nValue);
					if (m_fMaxScalarValue < nValue)
						m_fMaxScalarValue = static_cast<float>(nValue);

					// Update the histogram
					pfHistogram[nValue] += 1.0f;
				}
			}
		} else {
			// Error!
			m_fMinScalarValue = m_fMaxScalarValue = 0.0f;
		}
	}
}

/**
*  @brief
*    Returns the minimum value within the histogram
*/
float Histogram::GetMinValue()
{
	// Minimum value within the histogram already initialized?
	if (m_fMinValue < 0 && m_lstData.GetNumOfElements()) {
		// Initialize now
		m_fMinValue = FLT_MAX;
		m_fMaxValue = 0.0f;

		// Loop through the bins
		const float *pfDataEnd = m_lstData.GetData() + m_lstData.GetNumOfElements();
		for (const float *pfCurrentData=m_lstData.GetData(); pfCurrentData<pfDataEnd; pfCurrentData++) {
			// Get the current value
			const float fValue = *pfCurrentData;

			// Update minimum and maximum value
			if (m_fMinValue > fValue)
				m_fMinValue = fValue;
			if (m_fMaxValue < fValue)
				m_fMaxValue = fValue;
		}
	}

	// Done
	return m_fMinValue;
}

/**
*  @brief
*    Returns the bin with the highest value
*/
int Histogram::GetBinWithHighestValue()
{
	// Bin with the highest value already initialized?
	if (m_nBinWithHighestValue < 0 && m_lstData.GetNumOfElements()) {
		// Initialize now
		float fCurrentHighestValue = -1.0f;

		// Loop through the bins
		const float *pfDataEnd = m_lstData.GetData() + m_lstData.GetNumOfElements();
		uint32 nBin = 0;
		for (const float *pfCurrentData=m_lstData.GetData(); pfCurrentData<pfDataEnd; pfCurrentData++, nBin++) {
			// Get the current value
			const float fValue = *pfCurrentData;

			// Update bin with the highest value, if required
			if (fCurrentHighestValue < fValue) {
				fCurrentHighestValue = fValue;
				m_nBinWithHighestValue = nBin;
			}
		}
	}

	// Done
	return m_nBinWithHighestValue;
}

/**
*  @brief
*    Normalizes the histogram
*/
void Histogram::NormalizeLinear()
{
	// Is the histogram already normalized?
	if (!m_bNormalized) {
		// Loop through the bins and normalize the values
		const float fMin = GetMinValue();
		const float fMax = GetMaxValue();
		const float fNormalization = 1.0f/(fMax - fMin);
		const float *pfDataEnd = m_lstData.GetData() + m_lstData.GetNumOfElements();
		for (float *pfCurrentData=m_lstData.GetData(); pfCurrentData<pfDataEnd; pfCurrentData++)
			*pfCurrentData = (*pfCurrentData - fMin)*fNormalization;

		// The histogram is now normalized
		m_bNormalized = true;

		// Initialize the minimum and maximum value within the histogram
		// -> Should now be [0,1], but sure is sure
		m_fMinValue = -1.0f;
		m_fMaxValue = -1.0f;

		// Bin with the highest value not initialized, yet
		m_nBinWithHighestValue = -1;
	}
}

/**
*  @brief
*    Normalizes the histogram (linear histogram is converted into a logarithmic histogram)
*/
void Histogram::NormalizeLogarithmic()
{
	// Is the histogram already normalized?
	if (!m_bNormalized) {
		// Loop through the bins and normalize the values
		const float fLogMin = Math::Log(Math::Max(1.0f, GetMinValue()));
		const float fLogMax = Math::Log(GetMaxValue());
		const float fNormalization = 1.0f/(fLogMax - fLogMin);
		const float *pfDataEnd = m_lstData.GetData() + m_lstData.GetNumOfElements();
		for (float *pfCurrentData=m_lstData.GetData(); pfCurrentData<pfDataEnd; pfCurrentData++)
			*pfCurrentData = (Math::Log(*pfCurrentData) - fLogMin)*fNormalization;

		// The histogram is now normalized
		m_bNormalized = true;

		// Initialize the minimum and maximum value within the histogram
		// -> Should now be [0,1], but sure is sure
		m_fMinValue = -1.0f;
		m_fMaxValue = -1.0f;

		// Bin with the highest value not initialized, yet
		m_nBinWithHighestValue = -1;
	}
}

/**
*  @brief
*    Returns an normalized RGBA image representation of the histogram, bins along the x axis
*/
void Histogram::ToNormalizedImage(uint32 nHeight, Image &cDestinationImage, const Color4 &cEmptySpaceColor, const Color4 &cFilledSpaceColor)
{
	// Is the histogram normalized?
	if (m_bNormalized) {
		// Create new image
		const uint32 nWidth = GetNumOfBins();
		cDestinationImage = Image::CreateImage(DataByte, ColorRGBA, Vector3i(nWidth, nHeight, 1));

		// Get the image buffer
		ImageBuffer *pImageBuffer = cDestinationImage.GetBuffer();
		if (pImageBuffer) {
			// Color values
			const uint8 nEmptySpaceColorR  = cEmptySpaceColor.GetRInt();
			const uint8 nEmptySpaceColorG  = cEmptySpaceColor.GetGInt();
			const uint8 nEmptySpaceColorB  = cEmptySpaceColor.GetBInt();
			const uint8 nEmptySpaceColorA  = cEmptySpaceColor.GetAInt();
			const uint8 nFilledSpaceColorR = cFilledSpaceColor.GetRInt();
			const uint8 nFilledSpaceColorG = cFilledSpaceColor.GetGInt();
			const uint8 nFilledSpaceColorB = cFilledSpaceColor.GetBInt();
			const uint8 nFilledSpaceColorA = cFilledSpaceColor.GetAInt();

			// Fill the image
			const float *pfHistogramData = m_lstData.GetData();
			uint8 *pnImageData = pImageBuffer->GetData();
			for (uint32 x=0; x<nWidth; x++) {
				// Calculate the number of rows for that value
				const uint32 nNumOfRows = static_cast<uint32>(pfHistogramData[x]*nHeight);

				// Fill the row with the value
				for (uint32 y=0; y<nNumOfRows; y++) {
					uint8 *pData = &pnImageData[(x + (nHeight - y - 1)*nWidth)*4];
					*pData = nFilledSpaceColorR;
					pData++;
					*pData = nFilledSpaceColorG;
					pData++;
					*pData = nFilledSpaceColorB;
					pData++;
					*pData = nFilledSpaceColorA;
					pData++;
				}

				// Fill the rest of the empty row
				for (uint32 y=nNumOfRows; y<nHeight; y++) {
					uint8 *pData = &pnImageData[(x + (nHeight - y - 1)*nWidth)*4];
					*pData = nEmptySpaceColorR;
					pData++;
					*pData = nEmptySpaceColorG;
					pData++;
					*pData = nEmptySpaceColorB;
					pData++;
					*pData = nEmptySpaceColorA;
					pData++;
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
