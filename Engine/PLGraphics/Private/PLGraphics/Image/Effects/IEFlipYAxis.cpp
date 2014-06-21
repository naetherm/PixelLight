/*********************************************************\
 *  File: IEFlipYAxis.cpp                                *
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
#include "PLGraphics/Image/Effects/IEFlipYAxis.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(IEFlipYAxis)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
IEFlipYAxis::IEFlipYAxis()
{
}

/**
*  @brief
*    Destructor
*/
IEFlipYAxis::~IEFlipYAxis()
{
}


//[-------------------------------------------------------]
//[ Public virtual ImageEffect functions                  ]
//[-------------------------------------------------------]
bool IEFlipYAxis::Apply(ImageBuffer &cImageBuffer) const
{
	// Get the number of bytes per row
	const uint32 nBytesPerRow = cImageBuffer.GetBytesPerRow();
	if (nBytesPerRow) {
		// Allocate memory to backup one row
		uint8 *pnUpperRowBackup = new uint8[nBytesPerRow];

		// Loop through rows
		uint8 *pnUpperRow = cImageBuffer.GetData();
		uint8 *pnLowerRow = pnUpperRow + cImageBuffer.GetDataSize() - nBytesPerRow;
		for (; pnUpperRow<pnLowerRow; pnUpperRow+=nBytesPerRow, pnLowerRow-=nBytesPerRow) {
			// Backup the content of the current upper row
			MemoryManager::Copy(pnUpperRowBackup, pnUpperRow, nBytesPerRow);

			// Copy the content of the current lower row to the current upper row
			MemoryManager::Copy(pnUpperRow, pnLowerRow, nBytesPerRow);

			// Copy the backup content of the current upper row to the current lower row
			MemoryManager::Copy(pnLowerRow, pnUpperRowBackup, nBytesPerRow);
		}

		// Cleanup upper row backup memory
		delete [] pnUpperRowBackup;

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
