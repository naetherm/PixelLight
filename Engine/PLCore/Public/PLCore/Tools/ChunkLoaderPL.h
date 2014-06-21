/*********************************************************\
 *  File: ChunkLoaderPL.h                                *
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


#ifndef __PLCORE_CHUNKLOADER_PL_H__
#define __PLCORE_CHUNKLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/ChunkLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Chunk;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Chunk loader implementation for the PixelLight binary (Little-Endian) chunk file format
*
*  @remarks
*    PixelLight mesh format description:
*    - uint32: Magic number
*    - uint32: Version
*      - uint32: The total size of the following chunk (header + data) in bytes
*      - uint32: Semantic
*      - uint32: Element type
*      - uint32: Components per element
*      - uint32: Number of elements
*      -         Data
*/
class ChunkLoaderPL : public ChunkLoader {


	//[-------------------------------------------------------]
	//[ Public constants                                      ]
	//[-------------------------------------------------------]
	public:
		static PLCORE_API const uint32 MAGIC;	/**< Magic number of the file format */
		static PLCORE_API const uint32 VERSION;	/**< File format version */


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, ChunkLoaderPL, "PLCore", PLCore::ChunkLoader, "Chunk loader implementation for the PixelLight binary (Little-Endian) chunk file format")
		// Properties
		pl_properties
			pl_property("Formats",	"chunk,CHUNK")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,	pl_ret_type(bool),	Chunk&,			File&,	"Load method",	"")
		pl_method_2(Save,	pl_ret_type(bool),	const Chunk&,	File&,	"Save method",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API bool Load(Chunk &cChunk, File &cFile);
		PLCORE_API bool Save(const Chunk &cChunk, File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API ChunkLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ChunkLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cChunk
		*    Chunk to load into
		*  @param[in] cFile
		*    Chunk file to read the data from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(Chunk &cChunk, File &cFile) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CHUNKLOADER_PL_H__
