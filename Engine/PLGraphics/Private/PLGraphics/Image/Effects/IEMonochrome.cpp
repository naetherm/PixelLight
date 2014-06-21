/*********************************************************\
 *  File: IEMonochrome.cpp                               *
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
#include "PLGraphics/Image/Effects/IEMonochrome.h"


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
*    Data process template
*/
template <typename DataType>
class ProcessData {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in, out] cImageBuffer
		*    Image buffer (MUST have valid data!)
		*/
		ProcessData(ImageBuffer &cImageBuffer)
		{
			// Get the data pointer
			DataType *pData = reinterpret_cast<DataType*>(cImageBuffer.GetData());

			// Get the number of color components
			const uint32 nNumOfColorComponents = cImageBuffer.GetComponentsPerPixel();

			// Convert color format dependent
			switch (cImageBuffer.GetColorFormat()) {
				case ColorRGB:
				case ColorRGBA:
					// Loop through all pixels
					for (const DataType *pDataEnd=pData+cImageBuffer.GetDataSize(); pData<pDataEnd; pData+=nNumOfColorComponents) {
						// Convert RGB to grayscale using "Human formula"... our eyes recognize color components not uniformly
						const DataType nGrayscale = DataType(pData[0]*0.299 + pData[1]*0.587 + pData[2]*0.114);

						// Set new values
						pData[0] = nGrayscale;
						pData[1] = nGrayscale;
						pData[2] = nGrayscale;
					}
					break;

				case ColorBGR:
				case ColorBGRA:
					// Loop through all pixels
					for (const DataType *pDataEnd=pData+cImageBuffer.GetDataSize(); pData<pDataEnd; pData+=nNumOfColorComponents) {
						// Convert BGR to grayscale using "Human formula"... our eyes recognize color components not uniformly
						const DataType nGrayscale = DataType(pData[2]*0.299 + pData[1]*0.587 + pData[0]*0.114);

						// Set new values
						pData[0] = nGrayscale;
						pData[1] = nGrayscale;
						pData[2] = nGrayscale;
					}
					break;

				case ColorGrayscale:
				case ColorGrayscaleA:
					// Already monochrome
					break;

				case ColorPalette:
				default:
					// Error! Not supported!
					break;
			}
		}


};


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(IEMonochrome)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
IEMonochrome::IEMonochrome()
{
}

/**
*  @brief
*    Destructor
*/
IEMonochrome::~IEMonochrome()
{
}


//[-------------------------------------------------------]
//[ Public virtual ImageEffect functions                  ]
//[-------------------------------------------------------]
bool IEMonochrome::Apply(ImageBuffer &cImageBuffer) const
{
	// ColorPalette as color format is not supported
	if (cImageBuffer.GetColorFormat() != ColorPalette) {
		// Process data format dependent
		switch (cImageBuffer.GetDataFormat()) {
			case DataByte:
			{
				ProcessData<uint8> cProcessData(cImageBuffer);
				break;
			}

			case DataWord:
			{
				ProcessData<uint16> cProcessData(cImageBuffer);
				break;
			}

			case DataHalf:
			{
				// [TODO] DataHalf - Implement me
				break;
			}

			case DataFloat:
			{
				ProcessData<float> cProcessData(cImageBuffer);
				break;
			}

			case DataDouble:
			{
				ProcessData<double> cProcessData(cImageBuffer);
				break;
			}
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
