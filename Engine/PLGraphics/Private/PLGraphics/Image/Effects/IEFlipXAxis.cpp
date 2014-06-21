/*********************************************************\
 *  File: IEFlipXAxis.cpp                                *
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
#include "PLGraphics/Image/Effects/IEFlipXAxis.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(IEFlipXAxis)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
IEFlipXAxis::IEFlipXAxis()
{
}

/**
*  @brief
*    Destructor
*/
IEFlipXAxis::~IEFlipXAxis()
{
}


//[-------------------------------------------------------]
//[ Public virtual ImageEffect functions                  ]
//[-------------------------------------------------------]
bool IEFlipXAxis::Apply(ImageBuffer &cImageBuffer) const
{
	// Get the number of bytes per row
	const uint32 nBytesPerRow = cImageBuffer.GetBytesPerRow();
	if (nBytesPerRow) {
		// Get the number of bytes per pixel
		const uint32 nBytesPerPixel = cImageBuffer.GetBytesPerPixel();
		if (nBytesPerPixel) {
			// Allocate memory to backup one pixel
			uint8 *pnLeftPixelBackup = new uint8[nBytesPerPixel];

			// Loop through all rows
			      uint8 *pnRow     = cImageBuffer.GetData();
			const uint8 *pnDataEnd = pnRow + cImageBuffer.GetDataSize();
			for (; pnRow<pnDataEnd; pnRow+=nBytesPerRow) {
				// Loop through pixels within the row
				uint8 *pnLeftPixel  = pnRow;
				uint8 *pnRightPixel = pnRow + nBytesPerRow - nBytesPerPixel;
				for (; pnLeftPixel<pnRightPixel; pnLeftPixel+=nBytesPerPixel, pnRightPixel-=nBytesPerPixel) {
					// Backup the content of the current left pixel
					MemoryManager::Copy(pnLeftPixelBackup, pnLeftPixel, nBytesPerPixel);

					// Copy the content of the current right pixel to the current left pixel
					MemoryManager::Copy(pnLeftPixel, pnRightPixel, nBytesPerPixel);

					// Copy the backup content of the current left pixel to the current right pixel
					MemoryManager::Copy(pnRightPixel, pnLeftPixelBackup, nBytesPerPixel);
				}
			}

			// Cleanup left pixel backup memory
			delete [] pnLeftPixelBackup;
		}

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
