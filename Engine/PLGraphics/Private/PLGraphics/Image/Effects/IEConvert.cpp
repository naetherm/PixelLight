/*********************************************************\
 *  File: IEConvert.cpp                                  *
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
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/ImageEffects.h"
#include "PLGraphics/Image/Effects/IEConvert.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Data conversion template
*/
template <typename SourceType, typename DestinationType>
class ConvertData {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in]  cSourceImageBuffer
		*    Source image buffer (MUST have valid data!)
		*  @param[out] cDestinationImageBuffer
		*    Destination image buffer (MUST have valid data!)
		*  @param[out] AlphaValue
		*    If an alpha channel is added, this is the set alpha value for every pixel
		*/
		ConvertData(const ImageBuffer &cSourceImageBuffer, ImageBuffer &cDestinationImageBuffer, DestinationType AlphaValue)
		{
			// Get the data pointers
			const SourceType      *pSourceData      = reinterpret_cast<const SourceType*>(cSourceImageBuffer.GetData());
				  DestinationType *pDestinationData = reinterpret_cast<DestinationType*>(cDestinationImageBuffer.GetData());

			// Get the number of source and destination color components
			const uint32 nNumOfSourceColorComponents      = cSourceImageBuffer.GetComponentsPerPixel();
			const uint32 nNumOfDestinationColorComponents = cDestinationImageBuffer.GetComponentsPerPixel();

			// Get the total number of source elements (e.g. the total number of floats the image is made up of)
			const uint32 nNumOfSourceElements = cSourceImageBuffer.GetNumOfPixels()*cSourceImageBuffer.GetComponentsPerPixel();

			// Convert source color format dependent
			switch (cSourceImageBuffer.GetColorFormat()) {
				// Source
				case ColorRGB:
					// Convert destination color format dependent
					switch (cDestinationImageBuffer.GetColorFormat()) {
						//   ColorRGB     Source
						case ColorRGB: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);
							}
							break;

						//   ColorRGB      Source
						case ColorRGBA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);

								// Set alpha to AlphaValue
								pDestinationData[3] = AlphaValue;
							}
							break;

						//   ColorRGB     Source
						case ColorBGR: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);
							}
							break;

						//   ColorRGB      Source
						case ColorBGRA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// Set alpha to AlphaValue
								pDestinationData[3] = AlphaValue;
							}
							break;

						//   ColorRGB           Source
						case ColorGrayscale: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert RGB to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]*0.299 + pSourceData[1]*0.587 + pSourceData[2]*0.114);
							}
							break;

						//   ColorRGB            Source
						case ColorGrayscaleA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert RGB to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]*0.299 + pSourceData[1]*0.587 + pSourceData[2]*0.114);

								// Set alpha to AlphaValue
								pDestinationData[1] = AlphaValue;
							}
							break;

						case ColorPalette:
							// [TODO] Implement me?
							break;
					}
					break;

				// Source
				case ColorRGBA:
					// Convert destination color format dependent
					switch (cDestinationImageBuffer.GetColorFormat()) {
						//   ColorRGBA    Source
						case ColorRGB: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorRGBA     Source
						case ColorRGBA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGBA
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[3] = static_cast<DestinationType>(pSourceData[3]);
							}
							break;

						//   ColorRGBA    Source
						case ColorBGR: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorRGBA     Source
						case ColorBGRA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGBA with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[3] = static_cast<DestinationType>(pSourceData[3]);
							}
							break;

						//   ColorRGBA          Source
						case ColorGrayscale: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert RGB to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]*0.299 + pSourceData[1]*0.587 + pSourceData[2]*0.114);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorRGBA           Source
						case ColorGrayscaleA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert RGB to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]*0.299 + pSourceData[1]*0.587 + pSourceData[2]*0.114);

								// Copy over alpha
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[3]);
							}
							break;

						case ColorPalette:
							// [TODO] Implement me?
							break;
					}
					break;

				// Source
				case ColorBGR:
					// Convert destination color format dependent
					switch (cDestinationImageBuffer.GetColorFormat()) {
						//   ColorBGR     Source
						case ColorRGB: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);
							}
							break;

						//   ColorBGR      Source
						case ColorRGBA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// Set alpha to AlphaValue
								pDestinationData[3] = AlphaValue;
							}
							break;

						//   ColorBGR     Source
						case ColorBGR: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);
							}
							break;

						//   ColorBGR      Source
						case ColorBGRA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);

								// Set alpha to AlphaValue
								pDestinationData[3] = AlphaValue;
							}
							break;

						//   ColorBGR           Source
						case ColorGrayscale: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert BGR to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]*0.299 + pSourceData[1]*0.587 + pSourceData[0]*0.114);
							}
							break;

						//   ColorBGR            Source
						case ColorGrayscaleA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert BGR to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]*0.299 + pSourceData[1]*0.587 + pSourceData[0]*0.114);

								// Set alpha to AlphaValue
								pDestinationData[1] = AlphaValue;
							}
							break;

						case ColorPalette:
							// [TODO] Implement me?
							break;
					}
					break;

				// Source
				case ColorBGRA:
					// Convert destination color format dependent
					switch (cDestinationImageBuffer.GetColorFormat()) {
						//   ColorBGRA    Source
						case ColorRGB: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorBGRA     Source
						case ColorRGBA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGBA with R and B flipped
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[3] = static_cast<DestinationType>(pSourceData[3]);
							}
							break;

						//   ColorBGRA    Source
						case ColorBGR: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGB
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorBGRA     Source
						case ColorBGRA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over RGBA
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[2]);
								pDestinationData[3] = static_cast<DestinationType>(pSourceData[3]);
							}
							break;

						//   ColorBGRA          Source
						case ColorGrayscale: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert BGR to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]*0.299 + pSourceData[1]*0.587 + pSourceData[0]*0.114);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorBGRA           Source
						case ColorGrayscaleA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Convert BGR to grayscale using "Human formula"... our eyes recognize color components not uniformly
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[2]*0.299 + pSourceData[1]*0.587 + pSourceData[0]*0.114);

								// Copy over alpha
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[3]);
							}
							break;

						case ColorPalette:
							// [TODO] Implement me?
							break;
					}
					break;

				// Source
				case ColorGrayscale:
					// Convert destination color format dependent
					switch (cDestinationImageBuffer.GetColorFormat()) {
						//   ColorGrayscale  Source
						case ColorRGB: //    Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all RGB components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);
							}
							break;

						//   ColorGrayscale  Source
						case ColorRGBA: //   Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all RGB components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// Set alpha to AlphaValue
								pDestinationData[3] = AlphaValue;
							}
							break;

						//   ColorGrayscale  Source
						case ColorBGR: //    Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all BGR components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);
							}
							break;

						//   ColorGrayscale  Source
						case ColorBGRA: //   Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all BGR components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// Set alpha to AlphaValue
								pDestinationData[3] = AlphaValue;
							}
							break;

						//   ColorGrayscale     Source
						case ColorGrayscale: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over grayscale
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
							}
							break;

						//   ColorGrayscale      Source
						case ColorGrayscaleA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over grayscale
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);

								// Set alpha to AlphaValue
								pDestinationData[1] = AlphaValue;
							}
							break;

						case ColorPalette:
							// [TODO] Implement me?
							break;
					}
					break;

				// Source
				case ColorGrayscaleA:
					// Convert destination color format dependent
					switch (cDestinationImageBuffer.GetColorFormat()) {
						//   ColorGrayscaleA  Source
						case ColorRGB: //     Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all RGB components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorGrayscaleA  Source
						case ColorRGBA: //    Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all RGB components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// Copy over grayscale
								pDestinationData[3] = static_cast<DestinationType>(pSourceData[1]);
							}
							break;

						//   ColorGrayscaleA  Source
						case ColorBGR: //     Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all BGR components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorGrayscaleA  Source
						case ColorBGRA: //    Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Set all BGR components to the same grayscale value
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[2] = static_cast<DestinationType>(pSourceData[0]);

								// Copy over grayscale
								pDestinationData[3] = static_cast<DestinationType>(pSourceData[1]);
							}
							break;

						//   ColorGrayscaleA    Source
						case ColorGrayscale: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over grayscale
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);

								// ... the alpha value of the source data get's lost...
							}
							break;

						//   ColorGrayscaleA     Source
						case ColorGrayscaleA: // Destination
							// Loop through all pixels
							for (const SourceType *pSourceDataEnd=pSourceData+nNumOfSourceElements; pSourceData<pSourceDataEnd; pSourceData+=nNumOfSourceColorComponents, pDestinationData+=nNumOfDestinationColorComponents) {
								// Copy over grayscale and alpha
								pDestinationData[0] = static_cast<DestinationType>(pSourceData[0]);
								pDestinationData[1] = static_cast<DestinationType>(pSourceData[1]);
							}
							break;

						case ColorPalette:
							// [TODO] Implement me?
							break;
					}
					break;

				case ColorPalette:
					// [TODO] Implement me?
					break;
			}
		}


};


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(IEConvert)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
IEConvert::IEConvert(EDataFormat nDataFormat, EColorFormat nColorFormat) :
	m_nDataFormat(nDataFormat),
	m_nColorFormat(nColorFormat)
{
}

/**
*  @brief
*    Destructor
*/
IEConvert::~IEConvert()
{
}


//[-------------------------------------------------------]
//[ Public virtual ImageEffect functions                  ]
//[-------------------------------------------------------]
bool IEConvert::Apply(ImageBuffer &cImageBuffer) const
{
	// Anything to do in here? (the color format "ColorPalette" is not supported)
	if ((cImageBuffer.GetDataFormat() != m_nDataFormat || cImageBuffer.GetColorFormat() != m_nColorFormat) && m_nColorFormat != ColorPalette) {
		// If the image is using a palette, remove the palette!
		if (cImageBuffer.GetColorFormat() == ColorPalette) {
			// Apply effect
			ImageEffects::RemovePalette().Apply(cImageBuffer);
		}

		// Create the new image buffer
		const ImageBuffer cOldImageBuffer = cImageBuffer;
		cImageBuffer.CreateImage(m_nDataFormat, m_nColorFormat, cOldImageBuffer.GetSize());

		// Convert old data format dependent
		switch (cOldImageBuffer.GetDataFormat()) {
			// Source
			case DataByte:
				// Convert new data format dependent
				switch (cImageBuffer.GetDataFormat()) {
					//   DataByte     Source (old data format)
					case DataByte: // Destination (new data format)
					{
						ConvertData<uint8, uint8> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataByte     Source (old data format)
					case DataWord: // Destination (new data format)
					{
						ConvertData<uint8, uint16> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataByte      Source (old data format)
					case DataHalf: // Destination (new data format)
					{
						// [TODO] DataHalf - Implement me
						break;
					}

					//   DataByte      Source (old data format)
					case DataFloat: // Destination (new data format)
					{
						ConvertData<uint8, float> cConvertData(cOldImageBuffer, cImageBuffer, 1.0f);
						break;
					}

					//   DataByte       Source (old data format)
					case DataDouble: // Destination (new data format)
					{
						ConvertData<uint8, double> cConvertData(cOldImageBuffer, cImageBuffer, 1.0);
						break;
					}
				}
				break;

			// Source
			case DataWord:
				// Convert new data format dependent
				switch (cImageBuffer.GetDataFormat()) {
					//   DataWord     Source (old data format)
					case DataByte: // Destination (new data format)
					{
						ConvertData<uint16, uint8> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataWord     Source (old data format)
					case DataWord: // Destination (new data format)
					{
						ConvertData<uint16, uint16> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataWord      Source (old data format)
					case DataHalf: // Destination (new data format)
					{
						// [TODO] DataHalf - Implement me
						break;
					}

					//   DataWord      Source (old data format)
					case DataFloat: // Destination (new data format)
					{
						ConvertData<uint16, float> cConvertData(cOldImageBuffer, cImageBuffer, 1.0f);
						break;
					}

					//   DataWord       Source (old data format)
					case DataDouble: // Destination (new data format)
					{
						ConvertData<uint16, double> cConvertData(cOldImageBuffer, cImageBuffer, 1.0);
						break;
					}
				}
				break;

			// Source
			case DataHalf:
				// [TODO] DataHalf - Implement me
				break;

			// Source
			case DataFloat:
				// Convert new data format dependent
				switch (cImageBuffer.GetDataFormat()) {
					//   DataFloat    Source (old data format)
					case DataByte: // Destination (new data format)
					{
						ConvertData<float, uint8> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataFloat    Source (old data format)
					case DataWord: // Destination (new data format)
					{
						ConvertData<float, uint16> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataFloat     Source (old data format)
					case DataHalf: // Destination (new data format)
					{
						// [TODO] DataHalf - Implement me
						break;
					}

					//   DataFloat     Source (old data format)
					case DataFloat: // Destination (new data format)
					{
						ConvertData<float, float> cConvertData(cOldImageBuffer, cImageBuffer, 1.0f);
						break;
					}

					//   DataFloat      Source (old data format)
					case DataDouble: // Destination (new data format)
					{
						ConvertData<float, double> cConvertData(cOldImageBuffer, cImageBuffer, 1.0);
						break;
					}
				}
				break;

			// Source
			case DataDouble:
				// Convert new data format dependent
				switch (cImageBuffer.GetDataFormat()) {
					//   DataDouble   Source (old data format)
					case DataByte: // Destination (new data format)
					{
						ConvertData<double, uint8> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataDouble   Source (old data format)
					case DataWord: // Destination (new data format)
					{
						ConvertData<double, uint16> cConvertData(cOldImageBuffer, cImageBuffer, 255);
						break;
					}

					//   DataDouble    Source (old data format)
					case DataHalf: // Destination (new data format)
					{
						// [TODO] DataHalf - Implement me
						break;
					}

					//   DataDouble    Source (old data format)
					case DataFloat: // Destination (new data format)
					{
						ConvertData<double, float> cConvertData(cOldImageBuffer, cImageBuffer, 1.0f);
						break;
					}

					//   DataDouble     Source (old data format)
					case DataDouble: // Destination (new data format)
					{
						ConvertData<double, double> cConvertData(cOldImageBuffer, cImageBuffer, 1.0);
						break;
					}
				}
				break;
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
