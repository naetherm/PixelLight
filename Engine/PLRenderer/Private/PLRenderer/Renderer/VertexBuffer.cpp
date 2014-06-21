/*********************************************************\
 *  File: VertexBuffer.cpp                               *
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
#include <PLMath/Half.h>
#include <PLMath/Vector3.h>
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/FixedFunctions.h"
#include "PLRenderer/Renderer/IndexBuffer.h"
#include "PLRenderer/Renderer/VertexBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
VertexBuffer::~VertexBuffer()
{
	// Check if this is the renderers current fixed functions vertex buffer
	FixedFunctions *pFixedFunctions = GetRenderer().GetFixedFunctions();
	if (pFixedFunctions && pFixedFunctions->GetVertexBuffer() == this)
		pFixedFunctions->SetVertexBuffer();

	// Delete vertex attributes
	for (uint32 i=0; i<m_lstVertexAttributes.GetNumOfElements(); i++)
		delete m_lstVertexAttributes[i];
	m_lstVertexAttributes.Clear();
}

/**
*  @brief
*    Clears the vertex attributes
*/
bool VertexBuffer::ClearVertexAttributes()
{
	// Is the vertex buffer already allocated?
	if (IsAllocated())
		return false; // Error!

	// Delete vertex attributes
	for (uint32 i=0; i<m_lstVertexAttributes.GetNumOfElements(); i++)
		delete m_lstVertexAttributes[i];
	m_lstVertexAttributes.Clear();

	// Reset size (in bytes) of the vertex buffer
	m_nVertexSize = 0;

	// Done
	return true;
}

/**
*  @brief
*    Adds vertex attribute
*/
bool VertexBuffer::AddVertexAttribute(ESemantic nSemantic, uint32 nChannel, EType nType)
{
	// Check whether this semantic & channel is already used
	for (uint32 i=0; i<m_lstVertexAttributes.GetNumOfElements(); i++) {
		if (m_lstVertexAttributes[i]->nSemantic == nSemantic &&
			m_lstVertexAttributes[i]->nChannel  == nChannel)
			return false; // Error, the vertex attribute is already defined!
	}

	// Check normal, tangent and binormal (only Float3/Half3 allowed due to restrictions of legacy APIs!)
	if ((nSemantic == Normal || nSemantic == Tangent || nSemantic == Binormal) && nType != Float3 && nType != Half3)
		return false; // Error!

	// Check color
	if (nSemantic == Color && nType != RGBA)
		return false; // Error!

	// Check whether the channel is correct
	switch (nSemantic) {
		case Position:
		case Normal:
			if (nChannel > 1)
				return false; // Error!
			break;

		case Color:
			if (nChannel > 2)
				return false; // Error!
			break;

		case BlendWeight:
		case FogCoord:
		case PointSize:
		case BlendIndices:
		case TexCoord:
		case Tangent:
		case Binormal:
		default:
			// Nothing to do in here
			break;
	}

	// Allocate vertex attribute
	Attribute *pAttribute = new Attribute;
	// Setup values
	pAttribute->nSemantic	   = nSemantic;
	pAttribute->nChannel	   = nChannel;
	pAttribute->nType		   = nType;
	pAttribute->nOffset		   = m_nVertexSize;
	// This values will be setup through the API dependent function
	pAttribute->nSizeAPI	   = 0;
	pAttribute->nTypeAPI	   = 0;
	pAttribute->nComponentsAPI = 0;

	// Call API dependent vertex attribute added function
	VertexAttributeAdded(*pAttribute);
	if (!pAttribute->nSizeAPI) {
		// Failed to setup the vertex attribute (maybe unknown type?)

		// Deallocate vertex attribute
		delete pAttribute;

		// Error!
		return false;
	}

	// Add vertex attribute
	m_lstVertexAttributes.Add(pAttribute);

	// Update vertex size
	uint32 nVertexSize = m_nVertexSize;
	m_nVertexSize += pAttribute->nSizeAPI;

	// Is the vertex buffer already allocated?
	if (!IsAllocated())
		return true; // Yeah, that was pretty easy! :)

	// Uff, now we have to update the buffer!
	void *pData = Lock(Lock::ReadOnly);
	if (pData) {
		// Backup old buffer
		uint8 *pOldData = new uint8[GetSize()];
		MemoryManager::Copy(pOldData, pData, GetSize());

		// Allocate new buffer
		uint32      nElements = GetNumOfElements();
		Usage::Enum nUsage    = GetUsage();
		bool        bManaged  = IsManaged();
		Clear();  // Now 'pData' becomes invalid and we don't have to call Unlock() at the end
		Allocate(nElements, nUsage, bManaged);

		// Copy the old data into the new buffer
		if (Lock(Lock::WriteOnly)) {
			for (uint32 nElement=0; nElement<GetNumOfElements(); nElement++) {
				for (uint32 i=0; i<m_lstVertexAttributes.GetNumOfElements()-1; i++) {
					const Attribute *pCurrentAttribute = GetVertexAttribute(i);
					MemoryManager::Copy(GetData(nElement, pCurrentAttribute->nSemantic, pCurrentAttribute->nChannel),
										pOldData+nElement*nVertexSize+pCurrentAttribute->nOffset,
										pCurrentAttribute->nSizeAPI);
				}
			}

			// Unlock this buffer
			Unlock();
		}

		// Finally free the old buffer backup
		delete [] pOldData;
	}

	// Done
	return true;
}

/**
*  @brief
*    Returns the first found vertex attribute with the requested semantic
*/
const VertexBuffer::Attribute *VertexBuffer::GetVertexAttribute(ESemantic nSemantic, uint32 nChannel) const
{
	// Loop through all vertex attributes
	for (uint32 i=0; i<m_lstVertexAttributes.GetNumOfElements(); i++) {
		// Get the current vertex attribute
		const Attribute *pAttribute = m_lstVertexAttributes[i];

		// Semantic and channel match?
		if (pAttribute->nSemantic == nSemantic && pAttribute->nChannel == nChannel)
			return pAttribute; // Vertex attribute found!
	}

	// Error, no vertex attribute with the requested semantic found!
	return nullptr;
}

/**
*  @brief
*    Copy operator
*/
VertexBuffer &VertexBuffer::operator =(const VertexBuffer &cSource)
{
	// Lock the source buffer
	if (const_cast<VertexBuffer&>(cSource).Lock(Lock::ReadOnly)) {
		// Clear old vertex buffer
		Clear();
		ClearVertexAttributes();

		// Copy vertex attributes
		for (uint32 i=0; i<cSource.GetNumOfVertexAttributes(); i++) {
			const Attribute *pAttribute = cSource.GetVertexAttribute(i);
			if (pAttribute)
				AddVertexAttribute(pAttribute->nSemantic, pAttribute->nChannel, pAttribute->nType);
		}

		// Allocate this buffer
		Allocate(cSource.GetNumOfElements(), cSource.GetUsage(), cSource.IsManaged());

		// Lock this (target) buffer
		if (Lock(Lock::WriteOnly)) {
			// Copy the buffer data
			MemoryManager::Copy(GetData(), const_cast<VertexBuffer&>(cSource).GetData(), cSource.GetSize());

			// Unlock this (target) buffer
			Unlock();
		}

		// Unlock the source buffer
		const_cast<VertexBuffer&>(cSource).Unlock();
	}

	// Done
	return *this;
}

/**
*  @brief
*    Fills the data of a vertex buffer attribute into four given generic floating point components
*/
bool VertexBuffer::GetFloat(uint32 nIndex, uint32 nSemantic, uint32 nChannel, float &fX, float &fY, float &fZ, float &fW)
{
	// Get the first found vertex attribute with the requested semantic
	const Attribute *pAttribute = GetVertexAttribute(static_cast<ESemantic>(nSemantic), nChannel);
	if (pAttribute) {
		// Check attribute type
		switch (pAttribute->nType) {
			// Color (legacy API dependent storage which is no longer required when using modern shader based API's, do always use GetColor() and SetColor()!)
			case RGBA:
			{
				const Color4 cColor = GetColor(nIndex, nChannel);
				fX = cColor.r;
				fY = cColor.g;
				fZ = cColor.b;
				fW = cColor.a;

				// Done
				return true;
			}

			// Float 1 (one component per element, 32 bit floating point per component)
			case Float1:
			{
				const float *pfVertex = static_cast<const float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					fX = pfVertex[0];
					fY = 0.0f;
					fZ = 0.0f;
					fW = 0.0f;

					// Done
					return true;
				}
				break;
			}

			// Float 2 (two components per element, 32 bit floating point per component)
			case Float2:
			{
				const float *pfVertex = static_cast<const float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					fX = pfVertex[0];
					fY = pfVertex[1];
					fZ = 0.0f;
					fW = 0.0f;

					// Done
					return true;
				}
				break;
			}

			// Float 3 (three components per element, 32 bit floating point per component)
			case Float3:
			{
				const float *pfVertex = static_cast<const float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					fX = pfVertex[0];
					fY = pfVertex[1];
					fZ = pfVertex[2];
					fW = 0.0f;

					// Done
					return true;
				}
				break;
			}

			// Float 4 (four components per element, 32 bit floating point per component)
			case Float4:
			{
				const float *pfVertex = static_cast<const float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					fX = pfVertex[0];
					fY = pfVertex[1];
					fZ = pfVertex[2];
					fW = pfVertex[3];

					// Done
					return true;
				}
				break;
			}

			// Short 2 (two components per element, 16 bit integer per component)
			case Short2:
			{
				const uint16 *pnVertex = static_cast<const uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					fX = static_cast<float>(pnVertex[0]);
					fY = static_cast<float>(pnVertex[1]);

					// Done
					return true;
				}
				break;
			}

			// Short 4 (four components per element, 16 bit integer per component)
			case Short4:
			{
				const uint16 *pnVertex = static_cast<const uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					fX = static_cast<float>(pnVertex[0]);
					fY = static_cast<float>(pnVertex[1]);
					fZ = static_cast<float>(pnVertex[2]);
					fW = static_cast<float>(pnVertex[3]);

					// Done
					return true;
				}
				break;
			}

			// Half 1 (one component per element, 16 bit floating point per component, may not be supported by each API)
			case Half1:
			{
				const uint16 *pnVertex = static_cast<const uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					fX = Half::ToFloat(pnVertex[0]);

					// Done
					return true;
				}
				break;
			}

			// Half 2 (two components per element, 16 bit floating point per component, may not be supported by each API)
			case Half2:
			{
				const uint16 *pnVertex = static_cast<const uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					fX = Half::ToFloat(pnVertex[0]);
					fY = Half::ToFloat(pnVertex[1]);

					// Done
					return true;
				}
				break;
			}

			// Half 3 (three components per element, 16 bit floating point per component, may not be supported by each API)
			case Half3:
			{
				const uint16 *pnVertex = static_cast<const uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					fX = Half::ToFloat(pnVertex[0]);
					fY = Half::ToFloat(pnVertex[1]);
					fZ = Half::ToFloat(pnVertex[2]);

					// Done
					return true;
				}
				break;
			}

			// Half 4 (four components per element, 16 bit floating point per component, may not be supported by each API)
			case Half4:
			{
				const uint16 *pnVertex = static_cast<const uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					fX = Half::ToFloat(pnVertex[0]);
					fY = Half::ToFloat(pnVertex[1]);
					fZ = Half::ToFloat(pnVertex[2]);
					fW = Half::ToFloat(pnVertex[3]);

					// Done
					return true;
				}
				break;
			}
		}
	}

	// Error!
	fX = 0.0f;
	fY = 0.0f;
	fZ = 0.0f;
	fW = 0.0f;
	return false;
}

/**
*  @brief
*    Sets the data of a vertex buffer attribute by using four given generic floating point components
*/
bool VertexBuffer::SetFloat(uint32 nIndex, uint32 nSemantic, uint32 nChannel, float fX, float fY, float fZ, float fW)
{
	// Get the first found vertex attribute with the requested semantic
	const Attribute *pAttribute = GetVertexAttribute(static_cast<ESemantic>(nSemantic), nChannel);
	if (pAttribute) {
		// Check attribute type
		switch (pAttribute->nType) {
			// Color (legacy API dependent storage which is no longer required when using modern shader based API's, do always use GetColor() and SetColor()!)
			case RGBA:
				return SetColor(nIndex, Color4(fX, fY, fZ, fW), nChannel);

			// Float 1 (one component per element, 32 bit floating point per component)
			case Float1:
			{
				float *pfVertex = static_cast<float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					pfVertex[0] = fX;

					// Done
					return true;
				}
				break;
			}

			// Float 2 (two components per element, 32 bit floating point per component)
			case Float2:
			{
				float *pfVertex = static_cast<float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					pfVertex[0] = fX;
					pfVertex[1] = fY;

					// Done
					return true;
				}
				break;
			}

			// Float 3 (three components per element, 32 bit floating point per component)
			case Float3:
			{
				float *pfVertex = static_cast<float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					pfVertex[0] = fX;
					pfVertex[1] = fY;
					pfVertex[2] = fZ;

					// Done
					return true;
				}
				break;
			}

			// Float 4 (four components per element, 32 bit floating point per component)
			case Float4:
			{
				float *pfVertex = static_cast<float*>(GetData(nIndex, nSemantic, nChannel));
				if (pfVertex) {
					pfVertex[0] = fX;
					pfVertex[1] = fY;
					pfVertex[2] = fZ;
					pfVertex[3] = fW;

					// Done
					return true;
				}
				break;
			}

			// Short 2 (two components per element, 16 bit integer per component)
			case Short2:
			{
				uint16 *pnVertex = static_cast<uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					pnVertex[0] = static_cast<uint16>(fX);
					pnVertex[1] = static_cast<uint16>(fY);

					// Done
					return true;
				}
				break;
			}

			// Short 4 (four components per element, 16 bit integer per component)
			case Short4:
			{
				uint16 *pnVertex = static_cast<uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					pnVertex[0] = static_cast<uint16>(fX);
					pnVertex[1] = static_cast<uint16>(fY);
					pnVertex[2] = static_cast<uint16>(fZ);
					pnVertex[3] = static_cast<uint16>(fW);

					// Done
					return true;
				}
				break;
			}

			// Half 1 (one component per element, 16 bit floating point per component, may not be supported by each API)
			case Half1:
			{
				uint16 *pnVertex = static_cast<uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					pnVertex[0] = Half::FromFloat(fX);

					// Done
					return true;
				}
				break;
			}

			// Half 2 (two components per element, 16 bit floating point per component, may not be supported by each API)
			case Half2:
			{
				uint16 *pnVertex = static_cast<uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					pnVertex[0] = Half::FromFloat(fX);
					pnVertex[1] = Half::FromFloat(fY);

					// Done
					return true;
				}
				break;
			}

			// Half 3 (three components per element, 16 bit floating point per component, may not be supported by each API)
			case Half3:
			{
				uint16 *pnVertex = static_cast<uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					pnVertex[0] = Half::FromFloat(fX);
					pnVertex[1] = Half::FromFloat(fY);
					pnVertex[2] = Half::FromFloat(fZ);

					// Done
					return true;
				}
				break;
			}

			// Half 4 (four components per element, 16 bit floating point per component, may not be supported by each API)
			case Half4:
			{
				uint16 *pnVertex = static_cast<uint16*>(GetData(nIndex, nSemantic, nChannel));
				if (pnVertex) {
					pnVertex[0] = Half::FromFloat(fX);
					pnVertex[1] = Half::FromFloat(fY);
					pnVertex[2] = Half::FromFloat(fZ);
					pnVertex[3] = Half::FromFloat(fW);

					// Done
					return true;
				}
				break;
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the vertex buffer bounding box
*/
void VertexBuffer::CalculateBoundingBox(Vector3 &vMinPos, Vector3 &vMaxPos, PLRenderer::IndexBuffer *pIndexBuffer)
{
	// Initialize the parameters
	vMinPos = Vector3::Zero;
	vMaxPos = Vector3::Zero;

	// Index buffer given?
	if (pIndexBuffer) {
		// Lock this vertex buffer
		if (Lock(Lock::ReadOnly)) {
			// Lock the given index buffer
			if (pIndexBuffer->Lock(Lock::ReadOnly)) {
				// We have to take the indices type into account...
				switch (pIndexBuffer->GetElementType()) {
					case IndexBuffer::UInt:
					{
						const uint32 *pIndices = static_cast<const uint32*>(pIndexBuffer->GetData());
						if (pIndexBuffer->GetNumOfElements()) {
							const float *pfPos = static_cast<const float*>(GetData(pIndices[0], Position));
							if (pfPos) {
								vMinPos = vMaxPos = pfPos;

								// Loop through all indices
								for (uint32 i=1; i<pIndexBuffer->GetNumOfElements(); i++) {
									pfPos = static_cast<const float*>(GetData(pIndices[i], Position));
									// Min
									if (vMinPos.x > pfPos[0])
										vMinPos.x = pfPos[0];
									if (vMinPos.y > pfPos[1])
										vMinPos.y = pfPos[1];
									if (vMinPos.z > pfPos[2])
										vMinPos.z = pfPos[2];
									// Max
									if (vMaxPos.x < pfPos[0])
										vMaxPos.x = pfPos[0];
									if (vMaxPos.y < pfPos[1])
										vMaxPos.y = pfPos[1];
									if (vMaxPos.z < pfPos[2])
										vMaxPos.z = pfPos[2];
								}
							}
						}
						break;
					}

					case IndexBuffer::UShort:
					{
						const uint16 *pIndices = static_cast<const uint16*>(pIndexBuffer->GetData());
						if (pIndexBuffer->GetNumOfElements()) {
							const float *pfPos = static_cast<const float*>(GetData(pIndices[0], Position));
							if (pfPos) {
								vMinPos = vMaxPos = pfPos;

								// Loop through all indices
								for (uint32 i=1; i<pIndexBuffer->GetNumOfElements(); i++) {
									pfPos = static_cast<const float*>(GetData(pIndices[i], Position));
									// Min
									if (vMinPos.x > pfPos[Vector3::X])
										vMinPos.x = pfPos[Vector3::X];
									if (vMinPos.y > pfPos[Vector3::Y])
										vMinPos.y = pfPos[Vector3::Y];
									if (vMinPos.z > pfPos[Vector3::Z])
										vMinPos.z = pfPos[Vector3::Z];
									// Max
									if (vMaxPos.x < pfPos[Vector3::X])
										vMaxPos.x = pfPos[Vector3::X];
									if (vMaxPos.y < pfPos[Vector3::Y])
										vMaxPos.y = pfPos[Vector3::Y];
									if (vMaxPos.z < pfPos[Vector3::Z])
										vMaxPos.z = pfPos[Vector3::Z];
								}
							}
						}
						break;
					}

					case IndexBuffer::UByte:
					{
						const uint8 *pIndices = static_cast<const uint8*>(pIndexBuffer->GetData());
						if (pIndexBuffer->GetNumOfElements()) {
							const float *pfPos = static_cast<const float*>(GetData(pIndices[0], Position));
							if (pfPos) {
								vMinPos = vMaxPos = pfPos;

								// Loop through all indices
								for (uint32 i=1; i<pIndexBuffer->GetNumOfElements(); i++) {
									pfPos = static_cast<const float*>(GetData(pIndices[i], Position));
									// Min
									if (vMinPos.x > pfPos[Vector3::X])
										vMinPos.x = pfPos[Vector3::X];
									if (vMinPos.y > pfPos[Vector3::Y])
										vMinPos.y = pfPos[Vector3::Y];
									if (vMinPos.z > pfPos[Vector3::Z])
										vMinPos.z = pfPos[Vector3::Z];
									// Max
									if (vMaxPos.x < pfPos[Vector3::X])
										vMaxPos.x = pfPos[Vector3::X];
									if (vMaxPos.y < pfPos[Vector3::Y])
										vMaxPos.y = pfPos[Vector3::Y];
									if (vMaxPos.z < pfPos[Vector3::Z])
										vMaxPos.z = pfPos[Vector3::Z];
								}
							}
						}
						break;
					}
				}

				// Unlock the given index buffer
				pIndexBuffer->Unlock();
			}

			// Unlock this vertex buffer
			Unlock();
		}
	} else {
		// Lock this vertex buffer
		if (Lock(Lock::ReadOnly)) {
			const float *pfPos = static_cast<const float*>(GetData(0, Position));
			if (pfPos) {
				vMinPos = vMaxPos = pfPos;

				// Loop through all indices
				for (uint32 i=1; i<m_nElements; i++) {
					pfPos = static_cast<const float*>(GetData(i, Position));
					// Min
					if (vMinPos.x > pfPos[Vector3::X])
						vMinPos.x = pfPos[Vector3::X];
					if (vMinPos.y > pfPos[Vector3::Y])
						vMinPos.y = pfPos[Vector3::Y];
					if (vMinPos.z > pfPos[Vector3::Z])
						vMinPos.z = pfPos[Vector3::Z];
					// Max
					if (vMaxPos.x < pfPos[Vector3::X])
						vMaxPos.x = pfPos[Vector3::X];
					if (vMaxPos.y < pfPos[Vector3::Y])
						vMaxPos.y = pfPos[Vector3::Y];
					if (vMaxPos.z < pfPos[Vector3::Z])
						vMaxPos.z = pfPos[Vector3::Z];
				}
			}

			// Unlock this vertex buffer
			Unlock();
		}
	}
}

/**
*  @brief
*    Returns the vertex buffer bounding sphere
*/
void VertexBuffer::CalculateBoundingSphere(Vector3 &vPos, float &fRadius, PLRenderer::IndexBuffer *pIndexBuffer)
{
	// Initialize the parameters
	vPos    = Vector3::Zero;
	fRadius = 0.0f;

	// Index buffer given?
	if (pIndexBuffer) {
		// Lock this vertex buffer
		if (Lock(Lock::ReadOnly)) {
			// Lock the given index buffer
			if (pIndexBuffer->Lock(Lock::ReadOnly)) {
				// We have to take the indices type into account...
				switch (pIndexBuffer->GetElementType()) {
					case IndexBuffer::UInt:
					{
						const uint32 *pIndices = static_cast<const uint32*>(pIndexBuffer->GetData());
						if (pIndexBuffer->GetNumOfElements()) {
							const float *pfPos = static_cast<const float*>(GetData(pIndices[0], Position));
							if (pfPos) {
								// Average points to get approximate center
								for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++) {
									pfPos = static_cast<const float*>(GetData(pIndices[i], Position));
									vPos.x += pfPos[0];
									vPos.y += pfPos[1];
									vPos.z += pfPos[2];
								}
								vPos /= static_cast<float>(pIndexBuffer->GetNumOfElements());

								// Find maximum distance from center (sphere radius)
								Vector3 vV;
								for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++) {
									vV      = static_cast<const float*>(GetData(pIndices[i], Position));
									fRadius = Math::Max(fRadius, (vV-vPos).GetSquaredLength());
								}
								fRadius = Math::Sqrt(fRadius);
							}
						}
						break;
					}

					case IndexBuffer::UShort:
					{
						const uint16 *pIndices = static_cast<const uint16*>(pIndexBuffer->GetData());
						if (pIndexBuffer->GetNumOfElements()) {
							const float *pfPos = static_cast<const float*>(GetData(pIndices[0], Position));
							if (pfPos) {
								// Average points to get approximate center
								for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++) {
									pfPos = static_cast<const float*>(GetData(pIndices[i], Position));
									vPos.x += pfPos[0];
									vPos.y += pfPos[1];
									vPos.z += pfPos[2];
								}
								vPos /= static_cast<float>(pIndexBuffer->GetNumOfElements());

								// Find maximum distance from center (sphere radius)
								Vector3 vV;
								for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++) {
									vV      = static_cast<const float*>(GetData(pIndices[i], Position));
									fRadius = Math::Max(fRadius, (vV-vPos).GetSquaredLength());
								}
								fRadius = Math::Sqrt(fRadius);
							}
						}
						break;
					}

					case IndexBuffer::UByte:
					{
						const uint8 *pIndices = static_cast<const uint8*>(pIndexBuffer->GetData());
						if (pIndexBuffer->GetNumOfElements()) {
							const float *pfPos = static_cast<const float*>(GetData(pIndices[0], Position));
							if (pfPos) {
								// Average points to get approximate center
								for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++) {
									pfPos = static_cast<const float*>(GetData(pIndices[i], Position));
									vPos.x += pfPos[0];
									vPos.y += pfPos[1];
									vPos.z += pfPos[2];
								}
								vPos /= static_cast<float>(pIndexBuffer->GetNumOfElements());

								// Find maximum distance from center (sphere radius)
								Vector3 vV;
								for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++) {
									vV      = static_cast<const float*>(GetData(pIndices[i], Position));
									fRadius = Math::Max(fRadius, (vV-vPos).GetSquaredLength());
								}
								fRadius = Math::Sqrt(fRadius);
							}
						}
						break;
					}
				}

				// Unlock the given index buffer
				pIndexBuffer->Unlock();
			}

			// Unlock this vertex buffer
			Unlock();
		}
	} else {
		// Lock this vertex buffer
		if (Lock(Lock::ReadOnly)) {
			const float *pfPos = static_cast<const float*>(GetData(0, Position));
			if (pfPos) {
				// Average points to get approximate center
				for (uint32 i=0; i<GetNumOfElements(); i++) {
					pfPos = static_cast<const float*>(GetData(i, Position));
					vPos.x += pfPos[0];
					vPos.y += pfPos[1];
					vPos.z += pfPos[2];
				}
				vPos /= static_cast<float>(GetNumOfElements());

				// Find maximum distance from center (sphere radius)
				Vector3 vV;
				for (uint32 i=0; i<GetNumOfElements(); i++) {
					vV      = static_cast<const float*>(GetData(i, Position));
					fRadius = Math::Max(fRadius, (vV-vPos).GetSquaredLength());
				}
				fRadius = Math::Sqrt(fRadius);
			}

			// Unlock this vertex buffer
			Unlock();
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VertexBuffer::VertexBuffer(Renderer &cRenderer) : Buffer(cRenderer, TypeVertexBuffer),
	m_nVertexSize(0)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
VertexBuffer::VertexBuffer(const VertexBuffer &cSource) : Buffer(cSource.GetRenderer(), TypeVertexBuffer),
	m_nVertexSize(cSource.GetVertexSize())
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Public virtual Buffer functions                       ]
//[-------------------------------------------------------]
void *VertexBuffer::GetData()
{
	// We need this dummy default implementation because this function is used within this file!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
