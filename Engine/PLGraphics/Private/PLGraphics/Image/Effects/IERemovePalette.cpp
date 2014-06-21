/*********************************************************\
 *  File: IERemovePalette.cpp                            *
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
#include "PLGraphics/Color/Color3.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/ImagePalette.h"
#include "PLGraphics/Image/Effects/IERemovePalette.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(IERemovePalette)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
IERemovePalette::IERemovePalette()
{
}

/**
*  @brief
*    Destructor
*/
IERemovePalette::~IERemovePalette()
{
}


//[-------------------------------------------------------]
//[ Public virtual ImageEffect functions                  ]
//[-------------------------------------------------------]
bool IERemovePalette::Apply(ImageBuffer &cImageBuffer) const
{
	// Must be a palette color format
	if (cImageBuffer.GetColorFormat() == ColorPalette) {
		// Get the palette
		const ImagePalette *pImagePalette = cImageBuffer.GetPalette();
		if (pImagePalette) {
			// Create the new image buffer
			ImageBuffer cOldImageBuffer = cImageBuffer;
			cImageBuffer.CreateImage(cOldImageBuffer.GetDataFormat(), ColorRGB, cOldImageBuffer.GetSize());

			// Remove palette data format dependent
			switch (cImageBuffer.GetDataFormat()) {
				case DataByte:
				{
					// Get the data pointers
						  uint8 *pNewData = cImageBuffer   .GetData();
					const uint8 *pOldData = cOldImageBuffer.GetData();

					// Fill the new buffer
					for (const uint8 *pNewDataEnd=cImageBuffer.GetData()+cImageBuffer.GetDataSize(); pNewData<pNewDataEnd; ) {
						// Get the color from the palette
						const Color3 cColor = pImagePalette->GetColor(*pOldData);

						// Next color index, please
						pOldData++;

						// Write down the red color component into the new buffer and go to the next color component
						*pNewData = cColor.GetRInt();
						pNewData++;

						// Write down the green color component into the new buffer and go to the next color component
						*pNewData = cColor.GetGInt();
						pNewData++;

						// Write down the blue color component into the new buffer and go to the next color component
						*pNewData = cColor.GetBInt();
						pNewData++;

					}
					break;
				}

				case DataWord:
				{
					// Get the data pointers
						  uint16 *pNewData = reinterpret_cast<uint16*>(cImageBuffer.GetData());
					const uint16 *pOldData = reinterpret_cast<const uint16*>(cOldImageBuffer.GetData());

					// Fill the new buffer
					for (const uint8 *pNewDataEnd=cImageBuffer.GetData()+cImageBuffer.GetDataSize(); reinterpret_cast<uint8*>(pNewData)<pNewDataEnd; ) {
						// Get the color from the palette
						const Color3 cColor = pImagePalette->GetColor(*pOldData);

						// Next color index, please
						pOldData++;

						// Write down the red color component into the new buffer and go to the next color component
						*pNewData = cColor.GetRInt();
						pNewData++;

						// Write down the green color component into the new buffer and go to the next color component
						*pNewData = cColor.GetGInt();
						pNewData++;

						// Write down the blue color component into the new buffer and go to the next color component
						*pNewData = cColor.GetBInt();
						pNewData++;

					}
					break;
				}

				case DataHalf:
					// [TODO] Is this possible within a palette?
					break;

				case DataFloat:
					// [TODO] Is this possible within a palette?
					break;

				case DataDouble:
					// [TODO] Is this possible within a palette?
					break;
			}
		}
	}

	// Not implemented
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
