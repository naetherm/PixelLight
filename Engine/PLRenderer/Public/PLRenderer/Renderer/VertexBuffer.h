/*********************************************************\
 *  File: VertexBuffer.h                                 *
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


#ifndef __PLRENDERER_VERTEXBUFFER_H__
#define __PLRENDERER_VERTEXBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLGraphics/Color/Color4.h>
#include "PLRenderer/Renderer/Buffer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector3;
}
namespace PLRenderer {
	class IndexBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Renderer vertex buffer (VBO) resource
*
*  @note
*    - If for instance fog coordinates (Capabilities::bVertexBufferFogCoord) are not supported by the
*      GPU, this data is ignored for rendering but can still be used inside your vertex buffer
*/
class VertexBuffer : public Buffer {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Vertex attribute semantic
		*
		*  @note
		*    - Defines input vertex data to the pipeline, on legacy hardware, this should be defined in this order (performance)
		*/
		enum ESemantic {
			Position     =  0,	/**< Position data (1 channel) */
			BlendWeight  =  1,	/**< Blend weight data (1 channel) */
			Normal       =  2,	/**< Normal data (1 channel, only Float3/Half3 allowed due to restrictions of legacy APIs!) */
			Color        =  3,	/**< Color data (2 channels, only RGBA allowed, second channel requires Capabilities::bVertexBufferSecondaryColor) */
			FogCoord     =  4,	/**< Fog coordinate data (1 channel, only Float1 allowed, requires FixedFunctions::Capabilities::bVertexBufferFogCoord) */
			PointSize    =  5,	/**< Point sprite size data (1 channel, only Float1 allowed! Known as "PSize", but this name may conflict with OS definitions) */
			BlendIndices =  6,	/**< Blend indices data (1 channel) */
			TexCoord     =  7,	/**< Texture coordinate data (n channels) */
			Tangent      =  8,	/**< Tangent data (1 channel, only Float3/Half3 allowed due to restrictions of legacy APIs!) */
			Binormal     =  9	/**< Binormal (also referred to as bitangent) data (1 channel, only Float3/Half3 allowed due to restrictions of legacy APIs!) */
		};
		static const PLCore::uint8 NumOfSemantics      = 10;	/**< Number of vertex attribute semantics */
		static const PLCore::uint8 MaxPipelineChannels = 16;	/**< Maximum possible number of channels */

		/**
		*  @brief
		*    Vertex attribute types
		*/
		enum EType {
			RGBA   = 0,	/**< Color (legacy API dependent storage which is no longer required when using modern shader based API's, do always use GetColor() and SetColor()!) */
			Float1 = 1,	/**< Float 1 (one component per element, 32 bit floating point per component) */
			Float2 = 2,	/**< Float 2 (two components per element, 32 bit floating point per component) */
			Float3 = 3,	/**< Float 3 (three components per element, 32 bit floating point per component) */
			Float4 = 4,	/**< Float 4 (four components per element, 32 bit floating point per component) */
			Short2 = 5,	/**< Short 2 (two components per element, 16 bit integer per component) */
			Short4 = 6,	/**< Short 4 (four components per element, 16 bit integer per component) */
			Half1  = 7,	/**< Half 1 (one component per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it) */
			Half2  = 8,	/**< Half 2 (two components per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it) */
			Half3  = 9,	/**< Half 3 (three components per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it) */
			Half4  = 10	/**< Half 4 (four components per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it) */
		};


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Defines input vertex attribute to the pipeline
		*/
		struct Attribute {
			ESemantic	   nSemantic;		/**< Any member of the vertex attribute semantic enumeration type */
			PLCore::uint32 nChannel;		/**< Pipeline channel (see ESemantic) */
			EType		   nType;			/**< Any member of the EType enumeration type */
			PLCore::uint32 nOffset;			/**< Offset (in bytes) of the vertex attribute */
			// API dependent
			PLCore::uint32 nSizeAPI;		/**< Size (in bytes) of the vertex attribute */
			PLCore::uint32 nTypeAPI;		/**< API dependent vertex type */
			PLCore::uint32 nComponentsAPI;	/**< Number of vertex type components */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~VertexBuffer();

		/**
		*  @brief
		*    Returns the number of vertex attributes
		*
		*  @return
		*    Number of vertex attributes
		*/
		inline PLCore::uint32 GetNumOfVertexAttributes() const;

		/**
		*  @brief
		*    Clears the vertex attributes
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This is only possible if the vertex buffer is not allocated (see Allocate())
		*    - The virtual buffer function Clear() only clears the buffer itself,
		*      ClearVertexAttributes() clears the vertex attributes definition
		*/
		PLRENDERER_API bool ClearVertexAttributes();

		/**
		*  @brief
		*    Adds vertex attribute
		*
		*  @param[in] nSemantic
		*    Any member of the vertex attribute semantic enumeration type
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic, maximum see MaxPipelineChannels)
		*  @param[in] nType
		*    Any member of the EType enumeration type
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe this vertex attribute is already there or the requested type is not supported)
		*
		*  @note
		*    - You shouldn't change the vertex attribute after the vertex buffer was
		*      allocated using the Allocate()-function! (performance!)
		*    - It the buffer is still locked, it's forced to be unlocked immediately if all went fine
		*/
		PLRENDERER_API bool AddVertexAttribute(ESemantic nSemantic, PLCore::uint32 nChannel, EType nType);

		/**
		*  @brief
		*    Returns a vertex attribute
		*
		*  @param[in] nIndex
		*    Index of the vertex attribute which should be returned
		*
		*  @return
		*    The requested vertex attribute, a null pointer on error
		*/
		inline const Attribute *GetVertexAttribute(PLCore::uint32 nIndex = 0) const;

		/**
		*  @brief
		*    Returns the first found vertex attribute with the requested semantic
		*
		*  @param[in] nSemantic
		*    Vertex attribute semantic
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic, maximum see MaxPipelineChannels)
		*
		*  @return
		*    The first found vertex attribute with the requested semantic, a null pointer on error
		*/
		PLRENDERER_API const Attribute *GetVertexAttribute(ESemantic nSemantic, PLCore::uint32 nChannel = 0) const;

		/**
		*  @brief
		*    Returns the vertex size (in bytes)
		*
		*  @return
		*    Vertex size (in bytes)
		*/
		inline PLCore::uint32 GetVertexSize() const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this object
		*/
		PLRENDERER_API VertexBuffer &operator =(const VertexBuffer &cSource);

		/**
		*  @brief
		*    Copies data from a given buffer into this vertex buffer
		*
		*  @param[in] pData
		*    Data to copy into this vertex buffer, must be valid and must have at least "GetSize()" bytes
		*
		*  @remarks
		*    This ease of use method locks the vertex buffer, copies the
		*    given data into it and unlocks the vertex buffer when done.
		*/
		inline void CopyDataFrom(const void *pData);

		/**
		*  @brief
		*    Copies data from this vertex buffer into a given buffer
		*
		*  @param[out] pData
		*    Buffer to copy into, must be valid and must have at least "GetSize()" bytes
		*
		*  @remarks
		*    This ease of use method locks the vertex buffer, copies the vertex buffer
		*    data into the given buffer and unlocks the vertex buffer when done.
		*/
		inline void CopyDataTo(void *pData);

		//[-------------------------------------------------------]
		//[ Tool functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Fills the data of a vertex buffer attribute into four given generic floating point components
		*
		*  @param[in]  nIndex
		*    Vertex index
		*  @param[in]  nSemantic
		*    Any member of the vertex attribute semantic enumeration type
		*  @param[in]  nChannel
		*    Pipeline channel (see ESemantic)
		*  @param[out] fX
		*    On success, receives the first component, set to null on error
		*  @param[out] fY
		*    On success, receives the second component, set to null on error or when the component does not exist
		*  @param[out] fZ
		*    On success, receives the third component, set to null on error or when the component does not exist
		*  @param[out] fW
		*    On success, receives the fourth component, set to null on error or when the component does not exist
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Performance warning: This is only a comfort method and there's a notable overhead
		*      due to the required multiple checks, access the data directly if you need maximum possible performance
		*    - Unavailable components are ignored, e.g. when setting "Float1" the given "fY", "fZ" and "fW" will be ignored
		*    - See GetData() in Buffer
		*    - This function will give you the correct offset of the requested
		*      vertex attribute
		*    - When manipulating color data you should use GetColor() and SetColor()!
		*/
		PLRENDERER_API bool GetFloat(PLCore::uint32 nIndex, PLCore::uint32 nSemantic, PLCore::uint32 nChannel, float &fX, float &fY, float &fZ, float &fW);

		/**
		*  @brief
		*    Sets the data of a vertex buffer attribute by using four given generic floating point components
		*
		*  @param[in] nIndex
		*    Vertex index
		*  @param[in] nSemantic
		*    Any member of the vertex attribute semantic enumeration type
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic)
		*  @param[in] fX
		*    First component
		*  @param[in] fY
		*    Second component
		*  @param[in] fZ
		*    Third component
		*  @param[in] fW
		*    Fourth component
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Performance warning: This is only a comfort method and there's a notable overhead
		*      due to the required multiple checks, access the data directly if you need maximum possible performance
		*    - Unavailable components are ignored, e.g. when setting "Float1" the given "fY", "fZ" and "fW" will be ignored
		*    - See GetData() in Buffer
		*    - This function will give you the correct offset of the requested
		*      vertex attribute
		*    - When manipulating color data you should use GetColor() and SetColor()!
		*/
		PLRENDERER_API bool SetFloat(PLCore::uint32 nIndex, PLCore::uint32 nSemantic, PLCore::uint32 nChannel, float fX, float fY = 0.0f, float fZ = 0.0f, float fW = 0.0f);

		/**
		*  @brief
		*    Returns the vertex buffer bounding box
		*
		*  @param[out] vMinPos
		*    Will receive the minimum bounding box position
		*  @param[out] vMaxPos
		*    Will receive the maximum bounding box position
		*  @param[in]  pIndexBuffer
		*    Optional index buffer to take only a set of vertices into account, set to a null pointer if not used
		*/
		PLRENDERER_API void CalculateBoundingBox(PLMath::Vector3 &vMinPos, PLMath::Vector3 &vMaxPos, PLRenderer::IndexBuffer *pIndexBuffer = nullptr);

		/**
		*  @brief
		*    Returns the vertex buffer bounding sphere
		*
		*  @param[out] vPos
		*    Will receive the bounding sphere position
		*  @param[out] fRadius
		*    Will receive the bounding sphere radius
		*  @param[in]  pIndexBuffer
		*    Optional index buffer to take only a set of vertices into account, set to a null pointer if not used
		*/
		PLRENDERER_API void CalculateBoundingSphere(PLMath::Vector3 &vPos, float &fRadius, PLRenderer::IndexBuffer *pIndexBuffer = nullptr);


	//[-------------------------------------------------------]
	//[ Public virtual VertexBuffer functions                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the data of a vertex buffer attribute
		*
		*  @param[in] nIndex
		*    Vertex index
		*  @param[in] nSemantic
		*    Any member of the vertex attribute semantic enumeration type
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic)
		*
		*  @return
		*    The data of the locked vertex buffer, a null pointer on error
		*
		*  @note
		*    - See GetData() in Buffer
		*    - This function will give you the correct offset of the requested
		*      vertex attribute
		*    - When manipulating color data you should use GetColor() and SetColor()!
		*/
		virtual void *GetData(PLCore::uint32 nIndex, PLCore::uint32 nSemantic, PLCore::uint32 nChannel = 0) = 0;

		/**
		*  @brief
		*    Returns the RGBA color of a vertex
		*
		*  @param[in] nIndex
		*    Vertex index
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic)
		*
		*  @return
		*    The RGBA color of a vertex
		*
		*  @note
		*    - Because the different APIs will store the colors in different formats
		*      you should always use this offered color functions which are API
		*      independent!
		*/
		virtual PLGraphics::Color4 GetColor(PLCore::uint32 nIndex, PLCore::uint32 nChannel = 0) = 0;

		/**
		*  @brief
		*    Set the RGBA color of a vertex
		*
		*  @param[in] nIndex
		*    Vertex index
		*  @param[in] cColor
		*    Color to set
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetColor()
		*/
		virtual bool SetColor(PLCore::uint32 nIndex, const PLGraphics::Color4 &cColor, PLCore::uint32 nChannel = 0) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*/
		PLRENDERER_API VertexBuffer(Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::Array<Attribute*> m_lstVertexAttributes;	/**< The vertex attributes */
		PLCore::uint32			  m_nVertexSize;			/**< Size (in bytes) of the vertex buffer */


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
		VertexBuffer(const VertexBuffer &cSource);


	//[-------------------------------------------------------]
	//[ Private virtual VertexBuffer functions                ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Is called after a vertex attribute was added to setup some API dependent stuff
		*
		*  @param[in, out] cAttribute
		*    The added vertex attribute, a returned "Attribute::nSizeAPI" of 0 has to be handled like an error
		*
		*  @note
		*    - This function will setup the API dependent vertex attribute data
		*    - It will also update the current vertex size
		*/
		virtual void VertexAttributeAdded(Attribute &cAttribute) = 0;


	//[-------------------------------------------------------]
	//[ Public virtual Buffer functions                       ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual void *GetData() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/VertexBuffer.inl"


#endif // __PLRENDERER_VERTEXBUFFER_H__
