/*********************************************************\
 *  File: Histogram.h                                    *
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


#ifndef __PLVOLUME_HISTOGRAM_H__
#define __PLVOLUME_HISTOGRAM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Image;
	class Color4;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Histogram
*
*  @remarks
*    By looking at a histogram, one can see how often a certain value is present in the
*    data set the histogram was created from. In the literature, those values are usually
*    called "bins" or "isovalues" or "data values". A 8 bit data set has e.g. 256 bins, a
*    12 bit data set 4096 bins and a 16 bit data set 65536. In volume rendering, the original
*    dataset has usually 12 bit, during rendering often 8 bit to fit into efficient GPU texture
*    formats while of course sacrificing precision.
*/
class Histogram {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Histogram();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline Histogram(const Histogram &cSource);

		/**
		*  @brief
		*    Constructor, creates the histogram data from luminance data (= one component per element)
		*
		*  @param[in] nNumOfBytesPerElement
		*    Number of bytes per element (pixel/voxel), must be 1 or 2
		*  @param[in] nNumOfElements
		*    Number elements (pixel/voxel)
		*  @param[in] pnData
		*    Luminance data to create the histogram data from, must be valid! (only accessed from within this method)
		*/
		inline Histogram(PLCore::uint32 nNumOfBytesPerElement, PLCore::uint32 nNumOfElements, const PLCore::uint8 *pnData);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Histogram();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		inline Histogram &operator =(const Histogram &cSource);

		/**
		*  @brief
		*    Creates the histogram data from luminance data (= one component per element)
		*
		*  @param[in] nNumOfBytesPerElement
		*    Number of bytes per element (pixel/voxel), must be 1 or 2
		*  @param[in] nNumOfElements
		*    Number elements (pixel/voxel)
		*  @param[in] pnData
		*    Luminance data to create the histogram data from, must be valid! (only accessed from within this method)
		*/
		PLVOLUME_API void Create(PLCore::uint32 nNumOfBytesPerElement, PLCore::uint32 nNumOfElements, const PLCore::uint8 *pnData);

		/**
		*  @brief
		*    Returns the number of bins
		*
		*  @return
		*    The number of bins
		*/
		inline PLCore::uint32 GetNumOfBins() const;

		/**
		*  @brief
		*    Returns the minimum value encountered while creating the histogram
		*
		*  @return
		*    The minimum value encountered while creating the histogram
		*/
		inline float GetMinScalarValue() const;

		/**
		*  @brief
		*    Returns the maximum value encountered while creating the histogram
		*
		*  @return
		*    The maximum value encountered while creating the histogram
		*/
		inline float GetMaxScalarValue() const;

		/**
		*  @brief
		*    Returns the minimum value within the histogram
		*
		*  @return
		*    The minimum value within the histogram
		*/
		PLVOLUME_API float GetMinValue();

		/**
		*  @brief
		*    Returns the maximum value within the histogram
		*
		*  @return
		*    The maximum value within the histogram
		*/
		inline float GetMaxValue();

		/**
		*  @brief
		*    Returns the bin with the highest value
		*
		*  @return
		*    The bin with the highest value, <0 on error
		*/
		PLVOLUME_API int GetBinWithHighestValue();

		/**
		*  @brief
		*    List containing the histogram data
		*
		*  @return
		*    List containing the histogram data
		*/
		inline const PLCore::Array<float> &GetData() const;

		/**
		*  @brief
		*    Normalizes the histogram
		*/
		PLVOLUME_API void NormalizeLinear();

		/**
		*  @brief
		*    Normalizes the histogram (linear histogram is converted into a logarithmic histogram)
		*/
		PLVOLUME_API void NormalizeLogarithmic();

		//[-------------------------------------------------------]
		//[ To image                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns an normalized RGBA image representation of the histogram, bins along the x axis
		*
		*  @param[in]  nHeight
		*    Image height, >= 1
		*  @param[out] cDestinationImage
		*    Receives the image representation of the histogram (no need to allocated the image, this is done automatically, just pass in any image instance), RGBA byte
		*  @param[in]  cEmptySpaceColor
		*    Empty space color
		*  @param[in]  cFilledSpaceColor
		*    Filled space color
		*
		*  @note
		*   - Histogram must already be normalized, use e.g. "NormalizeLinear()" or "NormalizeLogarithmic()" before calling this method
		*
		*  @note
		*    - For debugging and visualization
		*/
		PLVOLUME_API void ToNormalizedImage(PLCore::uint32 nHeight, PLGraphics::Image &cDestinationImage, const PLGraphics::Color4 &cEmptySpaceColor, const PLGraphics::Color4 &cFilledSpaceColor);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<float> m_lstData;					/**< Histogram data */
		float				 m_fMinScalarValue;			/**< The minimum value encountered while creating the histogram */
		float				 m_fMaxScalarValue;			/**< The maximum value encountered while creating the histogram */
		float				 m_fMinValue;				/**< The minimum value within the histogram, <0 if not yet initialized */
		float				 m_fMaxValue;				/**< The maximum value within the histogram, <0 if not yet initialized */
		int					 m_nBinWithHighestValue;	/**< The bin with the highest value, <0 if not yet initialized */
		bool				 m_bNormalized;				/**< Is the histogram normalized? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLVolume/TransferFunctionControl/Histogram.inl"


#endif // __PLVOLUME_HISTOGRAM_H__
