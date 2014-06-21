/*********************************************************\
 *  File: ChunkLoaderPL.cpp                              *
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
#include "PLCore/Log/Log.h"
#include "PLCore/File/File.h"
#include "PLCore/Tools/Chunk.h"
#include "PLCore/Tools/ChunkLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ChunkLoaderPL)


//[-------------------------------------------------------]
//[ Constants                                             ]
//[-------------------------------------------------------]
const uint32 ChunkLoaderPL::MAGIC	= 0x57754632;
const uint32 ChunkLoaderPL::VERSION	= 0;


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ChunkLoaderPL::Load(Chunk &cChunk, File &cFile)
{
	// Read the magic number
	uint32 nMagic;
	cFile.Read(&nMagic, sizeof(uint32), 1);
	if (nMagic == MAGIC) {
		// Read the format version
		uint32 nVersion;
		cFile.Read(&nVersion, sizeof(uint32), 1);

		// Unknown
		if (nVersion > 0) {
			PL_LOG(Error, UnknownFormatVersion)

		// 0 (current)
		} else if (nVersion == 0) {
			return LoadV1(cChunk, cFile);
		}
	} else {
		PL_LOG(Error, "Invalid magic number")
	}

	// Error!
	return false;
}

bool ChunkLoaderPL::Save(const Chunk &cChunk, File &cFile)
{
	// Write the header
	cFile.Write(&MAGIC,   sizeof(uint32), 1);
	cFile.Write(&VERSION, sizeof(uint32), 1);

	// Write the total size of the following chunk (header + data) in bytes
	//                   Semantic         Data type        Components per element   Number of elements
	const uint32 nSize = sizeof(uint32) + sizeof(uint32) + sizeof(uint32) +         sizeof(uint32) +     cChunk.GetTotalNumOfBytes();
	cFile.Write(&nSize, sizeof(uint32), 1);

	// Write semantic
	const uint32 nSemantic = cChunk.GetSemantic();
	cFile.Write(&nSemantic, sizeof(uint32), 1);

	// Write element type
	const uint32 nElementType = cChunk.GetElementType();
	cFile.Write(&nElementType, sizeof(uint32), 1);

	// Write number of components per element
	const uint32 nNumOfComponentsPerElement = cChunk.GetNumOfComponentsPerElement();
	cFile.Write(&nNumOfComponentsPerElement, sizeof(uint32), 1);

	// Write number of elements
	const uint32 nNumOfElements = cChunk.GetNumOfElements();
	cFile.Write(&nNumOfElements, sizeof(uint32), 1);

	// Write chunk data
	cFile.Write(cChunk.GetData(), cChunk.GetTotalNumOfBytes(), 1);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ChunkLoaderPL::ChunkLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
ChunkLoaderPL::~ChunkLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool ChunkLoaderPL::LoadV1(Chunk &cChunk, File &cFile) const
{
	// Read the total size of the following chunk (header + data) in bytes
	uint32 nSize = 0;
	cFile.Read(&nSize, sizeof(uint32), 1);

	// Read semantic
	uint32 nSemantic = 0;
	cFile.Read(&nSemantic, sizeof(uint32), 1);
	cChunk.SetSemantic(static_cast<Chunk::ESemantic>(nSemantic));

	// Read element type
	uint32 nElementType = 0;
	cFile.Read(&nElementType, sizeof(uint32), 1);

	// Validate the element type
	if (nElementType >= Chunk::Int8 && nElementType <= Chunk::Double) {
		// Read number of components per element
		uint32 nNumOfComponentsPerElement = 0;
		cFile.Read(&nNumOfComponentsPerElement, sizeof(uint32), 1);

		// Read number of elements
		uint32 nNumOfElements = 0;
		cFile.Read(&nNumOfElements, sizeof(uint32), 1);

		// Allocate the chunk data
		if (cChunk.Allocate(static_cast<Chunk::EElementType>(nElementType), nNumOfComponentsPerElement, nNumOfElements)) {
			// Get the header size
			//                                Semantic         Data type        Components per element   Number of elements
			static const uint32 nHeaderSize = sizeof(uint32) + sizeof(uint32) + sizeof(uint32) +         sizeof(uint32);

			// Validate the size of the chunk
			if ((nSize-nHeaderSize) == cChunk.GetTotalNumOfBytes()) {
				// Read chunk data
				cFile.Read(cChunk.GetData(), cChunk.GetTotalNumOfBytes(), 1);

				// Done
				return true;
			} else {
				PL_LOG(Error, "Invalid chunk size")
			}
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
