/*********************************************************\
 *  File: Chunk.h                                        *
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


#ifndef __PLCORE_CHUNK_H__
#define __PLCORE_CHUNK_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Loadable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Chunk of data - one may also call it BLOB (binary large object)
*
*  @remarks
*    This class encapsulates a chunk of data in a quite primitive way. The semantic of the chunk data can be provided, but
*    it's no requirement. A chunk consists of a bunch of elements, were each element consists of at least one component and
*    all components of all elements have the same data type. A chunk can for example be used to store keyframed positions for
*    animation purposes. 
*/
class Chunk : public Loadable {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Semantic
		*/
		enum ESemantic {
			Unknown  = 0,	/**< Unknown */
			Position = 1,	/**< Position */
			Rotation = 2,	/**< Rotation */
			Scale    = 3	/**< Scale */
		};

		/**
		*  @brief
		*    Element type
		*/
		enum EElementType {
			Int8   = 0,	/**< Int8 */
			Int16  = 1,	/**< Int16 */
			Int32  = 2,	/**< Int32 */
			UInt8  = 3,	/**< UInt8 */
			UInt16 = 4,	/**< UInt16 */
			UInt32 = 5,	/**< UInt32 */
			Float  = 6,	/**< Float */
			Double = 7	/**< Double */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API Chunk();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Chunk();

		/**
		*  @brief
		*    Returns the semantic
		*
		*  @return
		*    The semantic
		*/
		inline ESemantic GetSemantic() const;

		/**
		*  @brief
		*    Sets the semantic
		*
		*  @param[in] nSemantic
		*    The semantic
		*/
		inline void SetSemantic(ESemantic nSemantic);

		/**
		*  @brief
		*    Allocates the chunk
		*
		*  @param[in] nElementType
		*    The element type
		*  @param[in] nNumOfComponentsPerElement
		*    The number of components per element
		*  @param[in] nNumOfElements
		*    The number of elements
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Allocate(EElementType nElementType, uint32 nNumOfComponentsPerElement, uint32 nNumOfElements);

		/**
		*  @brief
		*    Returns the chunk data
		*
		*  @return
		*    The chunk data, can be a null pointer - do NOT delete this memory!
		*/
		inline uint8 *GetData();

		/**
		*  @brief
		*    Returns the chunk data
		*
		*  @return
		*    The chunk data, can be a null pointer - do NOT delete this memory!
		*/
		inline const uint8 *GetData() const;

		/**
		*  @brief
		*    Returns the chunk element component data
		*
		*  @param[in] nElement
		*    Element index to return the component data from (0 - GetNumOfElements()-1), must be valid!
		*  @param[in] nComponent
		*    Index of the element component to data from (0 - GetNumOfComponentsPerElement()-1), must be valid!
		*
		*  @return
		*    The chunk element component data, can be a null pointer - do NOT delete this memory!
		*/
		inline uint8 *GetElementComponentData(uint32 nElement, uint32 nComponent);

		/**
		*  @brief
		*    Returns the chunk element component data
		*
		*  @param[in] nElement
		*    Element index to return the component data from (0 - GetNumOfElements()-1), must be valid!
		*  @param[in] nComponent
		*    Index of the element component to data from (0 - GetNumOfComponentsPerElement()-1), must be valid!
		*
		*  @return
		*    The chunk element component data, can be a null pointer - do NOT delete this memory!
		*/
		inline const uint8 *GetElementComponentData(uint32 nElement, uint32 nComponent) const;

		/**
		*  @brief
		*    Clears the chunk
		*/
		PLCORE_API void Clear();

		/**
		*  @brief
		*    Returns the element type
		*
		*  @return
		*    The element type
		*/
		inline EElementType GetElementType() const;

		/**
		*  @brief
		*    Returns the number of components per element
		*
		*  @return
		*    The number of components per element
		*/
		inline uint32 GetNumOfComponentsPerElement() const;

		/**
		*  @brief
		*    Returns the number of elements
		*
		*  @return
		*    The number of elements
		*/
		inline uint32 GetNumOfElements() const;

		/**
		*  @brief
		*    Returns the number of bytes per component of an element
		*
		*  @return
		*    The number of bytes per component of an element
		*/
		PLCORE_API uint32 GetNumOfBytesPerElementComponent() const;

		/**
		*  @brief
		*    Returns the number of bytes per element
		*
		*  @return
		*    The number of bytes per element
		*/
		inline uint32 GetNumOfBytesPerElement() const;

		/**
		*  @brief
		*    Returns the total number of bytes of the chunk data
		*
		*  @return
		*    The total number of bytes of the chunk data
		*/
		inline uint32 GetTotalNumOfBytes() const;

		/**
		*  @brief
		*    Returns the total number of components (all elements summed up) within the chunk
		*
		*  @return
		*    The total number of components within the chunk
		*/
		inline uint32 GetTotalNumOfComponents() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Chunk(const Chunk &cSource);

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
		Chunk &operator =(const Chunk &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ESemantic	  m_nSemantic;					/**< Semantic */
		EElementType  m_nElementType;				/**< Element type */
		uint32		  m_nNumOfComponentsPerElement;	/**< Number of components per element */
		uint32		  m_nNumOfElements;				/**< Number of elements */
		uint8		 *m_pData;						/**< The data, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual Loadable functions                     ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual bool Unload() override;
		PLCORE_API virtual String GetLoadableTypeName() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Chunk.inl"


#endif // __PLCORE_CHUNK_H__
