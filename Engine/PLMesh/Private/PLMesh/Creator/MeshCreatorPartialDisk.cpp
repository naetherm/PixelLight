/*********************************************************\
 *  File: MeshCreatorPartialDisk.cpp                     *
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


// [TODO] Calculate texture coordinates for the 'Fill' flag


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorPartialDisk.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(MeshCreatorPartialDisk)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorPartialDisk::MeshCreatorPartialDisk() :
	InnerRadius(this),
	OuterRadius(this),
	Slices(this),
	Loops(this),
	StartAngle(this),
	SweepAngle(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorPartialDisk::~MeshCreatorPartialDisk()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorPartialDisk::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
{
	// Call base implementation
	MeshCreator::Create(cMesh, nLODLevel, bStatic);

	// Get morph target
	MeshMorphTarget *pMorphTarget = cMesh.GetMorphTarget(0);
	if (pMorphTarget && pMorphTarget->GetVertexBuffer()) {
		// Setup vertex buffer
		VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (DrawStyle == Fill) {
			if (TexCoords)
				pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
			if (Normals)
				pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal,   0, VertexBuffer::Float3);
		}

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();

			// Draw
			switch (DrawStyle) {
				case Fill:
				{
					// Allocate vertex and index buffer
					uint32 nVertices = Loops*(Slices+1)*2;
					pVertexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);
					pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
					pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

					// Setup vertices & geometry
					if (pVertexBuffer->Lock(Lock::WriteOnly)) {
						if (pIndexBuffer->Lock(Lock::WriteOnly)) {
							float fDeltaRadius = (OuterRadius - InnerRadius)/Loops;
							float fDeltaAngle  = static_cast<float>(Math::DegToRad*(SweepAngle/Slices));
							float fRadius      = InnerRadius;

							// Get offset
							const Vector3 &vOffset = Offset.Get();

							// Setup vertex and index buffer
							Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							uint32 nVertex = 0;
							for (uint32 nLoop=0; nLoop<Loops; nLoop++) {
								float fAngle = static_cast<float>(Math::DegToRad*StartAngle);

								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize((Slices+1)*2);

								for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
									if (Order) {
										// Set vertex 0
										float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + fRadius*Math::Sin(fAngle);
										pfVertex[Vector3::Y] = vOffset.y + fRadius*Math::Cos(fAngle);
										pfVertex[Vector3::Z] = vOffset.z;
										if (Normals) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
											pfVertex[Vector3::X] =  0.0f;
											pfVertex[Vector3::Y] =  0.0f;
											pfVertex[Vector3::Z] = -1.0f;
										}

										// Set index 0
										pIndexBuffer->SetData(nVertex, nVertex);
										nVertex++;

										// Set vertex 1
										pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + (fRadius + fDeltaRadius)*Math::Sin(fAngle);
										pfVertex[Vector3::Y] = vOffset.y + (fRadius + fDeltaRadius)*Math::Cos(fAngle);
										pfVertex[Vector3::Z] = vOffset.z;
										if (Normals) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
											pfVertex[Vector3::X] =  0.0f;
											pfVertex[Vector3::Y] =  0.0f;
											pfVertex[Vector3::Z] = -1.0f;
										}

										// Set index 1
										pIndexBuffer->SetData(nVertex, nVertex);
										nVertex++;
									} else {
										// Set vertex 0
										float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + (fRadius + fDeltaRadius)*Math::Sin(fAngle);
										pfVertex[Vector3::Y] = vOffset.y + (fRadius + fDeltaRadius)*Math::Cos(fAngle);
										pfVertex[Vector3::Z] = vOffset.z;
										if (Normals) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
											pfVertex[Vector3::X] = 0.0f;
											pfVertex[Vector3::Y] = 0.0f;
											pfVertex[Vector3::Z] = 1.0f;
										}

										// Set index 0
										pIndexBuffer->SetData(nVertex, nVertex);
										nVertex++;

										// Set vertex 1
										pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + fRadius*Math::Sin(fAngle);
										pfVertex[Vector3::Y] = vOffset.y + fRadius*Math::Cos(fAngle);
										pfVertex[Vector3::Z] = vOffset.z;
										if (Normals) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
											pfVertex[Vector3::X] = 0.0f;
											pfVertex[Vector3::Y] = 0.0f;
											pfVertex[Vector3::Z] = 1.0f;
										}

										// Set index 1
										pIndexBuffer->SetData(nVertex, nVertex);
										nVertex++;
									}

									// Increase fAngle
									fAngle += fDeltaAngle;
								}

								// Increase fRadius
								fRadius += fDeltaRadius;
							}

							// Unlock the index buffer
							pIndexBuffer->Unlock();
						}

						// Unlock the vertex buffer
						pVertexBuffer->Unlock();
					}
					break;
				}

				case Line:
				{
					// Allocate vertex and index buffer
					uint32 nVertices = (Loops+1)*(Slices+1)*2;
					pVertexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);
					pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
					pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

					// Setup vertices & geometry
					if (pVertexBuffer->Lock(Lock::WriteOnly)) {
						if (pIndexBuffer->Lock(Lock::WriteOnly)) {
							float fDeltaRadius = (OuterRadius - InnerRadius)/static_cast<float>(Loops);
							float fDeltaAngle  = static_cast<float>(Math::DegToRad*(SweepAngle/static_cast<float>(Slices)));
							float fRadius      = InnerRadius;

							// Get offset
							const Vector3 &vOffset = Offset.Get();

							// Setup vertex and index buffer
							Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							uint32 nVertex = 0;

						// Draw rings
							for (uint32 nLoop=0; nLoop<=Loops; nLoop++) {
								float fAngle = static_cast<float>(Math::DegToRad*StartAngle);

								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize(Slices+1);

								for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
									// Set vertex 0
									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + fRadius*Math::Sin(fAngle);
									pfVertex[Vector3::Y] = vOffset.y + fRadius*Math::Cos(fAngle);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index 0
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;

									// Increase fAngle
									fAngle += fDeltaAngle;
								}

								// Increase fRadius
								fRadius += fDeltaRadius;
							}

						// Draw spokes
							float fAngle = static_cast<float>(Math::DegToRad*StartAngle);
							for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
								fRadius = InnerRadius;

								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize(Loops+1);

								for (uint32 nLoop=0; nLoop<=Loops; nLoop++) {
									// Set vertex 0
									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + fRadius*Math::Sin(fAngle);
									pfVertex[Vector3::Y] = vOffset.y + fRadius*Math::Cos(fAngle);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index 0
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;

									// Increase fRadius
									fRadius += fDeltaRadius;
								}

								// Increase fAngle
								fAngle += fDeltaAngle;
							}

							// Unlock the index buffer
							pIndexBuffer->Unlock();
						}

						// Unlock the vertex buffer
						pVertexBuffer->Unlock();
					}
					break;
				}

				case Silhouette:
				{
					// Allocate vertex and index buffer
					uint32 nVertices = Slices+1;
					if (InnerRadius > 0.0f)
						nVertices += Slices+1;
					if (SweepAngle < 360.0f)
						nVertices += 4;
					pVertexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);
					pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
					pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

					// Setup vertices & geometry
					if (pVertexBuffer->Lock(Lock::WriteOnly)) {
						if (pIndexBuffer->Lock(Lock::WriteOnly)) {
							float fDeltaAngle = static_cast<float>(Math::DegToRad*SweepAngle/static_cast<float>(Slices));

							// Get offset
							const Vector3 &vOffset = Offset.Get();

							// Setup vertex and index buffer
							Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							uint32 nVertex = 0;

						// Draw outer ring
							{ // Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize(Slices+1);
								// Setup vertex and index buffer
								float fAngle = static_cast<float>(Math::DegToRad*StartAngle);
								for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
									// Set vertex 0
									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + OuterRadius*Math::Sin(fAngle);
									pfVertex[Vector3::Y] = vOffset.y + OuterRadius*Math::Cos(fAngle);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index 0
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;

									// Increase fAngle
									fAngle += fDeltaAngle;
								}
							}

						// Draw inner ring
							if (InnerRadius > 0.0f) {
								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize(Slices+1);

								// Setup vertex and index buffer
								float fAngle = static_cast<float>(Math::DegToRad*StartAngle);
								for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
									// Set vertex 0
									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + InnerRadius*Math::Sin(fAngle);
									pfVertex[Vector3::Y] = vOffset.y + InnerRadius*Math::Cos(fAngle);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index 0
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;

									// Increase fAngle
									fAngle += fDeltaAngle;
								}
							}

						// Draw spokes
							if (SweepAngle < 360.0f) {
								// Sin and Cos for degree angles
								float fStopAngle = StartAngle + SweepAngle;

								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineList);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize(4);

								// Setup vertex and index buffer
								// Set vertex 0
								float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + InnerRadius*Math::Sin(static_cast<float>(StartAngle*Math::DegToRad));
								pfVertex[Vector3::Y] = vOffset.y + InnerRadius*Math::Cos(static_cast<float>(StartAngle*Math::DegToRad));
								pfVertex[Vector3::Z] = vOffset.z;
								// Set index 0
								pIndexBuffer->SetData(nVertex, nVertex);
								nVertex++;
								// Set vertex 1
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + OuterRadius*Math::Sin(static_cast<float>(StartAngle*Math::DegToRad));
								pfVertex[Vector3::Y] = vOffset.y + OuterRadius*Math::Cos(static_cast<float>(StartAngle*Math::DegToRad));
								pfVertex[Vector3::Z] = vOffset.z;
								// Set index 1
								pIndexBuffer->SetData(nVertex, nVertex);
								nVertex++;
								// Set vertex 2
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + InnerRadius*Math::Sin(static_cast<float>(fStopAngle*Math::DegToRad));
								pfVertex[Vector3::Y] = vOffset.y + InnerRadius*Math::Cos(static_cast<float>(fStopAngle*Math::DegToRad));
								pfVertex[Vector3::Z] = vOffset.z;
								// Set index 2
								pIndexBuffer->SetData(nVertex, nVertex);
								nVertex++;
								// Set vertex 3
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + OuterRadius*Math::Sin(static_cast<float>(fStopAngle*Math::DegToRad));
								pfVertex[Vector3::Y] = vOffset.y + OuterRadius*Math::Cos(static_cast<float>(fStopAngle*Math::DegToRad));
								pfVertex[Vector3::Z] = vOffset.z;
								// Set index 3
								pIndexBuffer->SetData(nVertex, nVertex);
								nVertex++;
							}

							// Unlock the index buffer
							pIndexBuffer->Unlock();
						}

						// Unlock the vertex buffer
						pVertexBuffer->Unlock();
					}
					break;
				}

				case Point:
				{
					// Allocate vertex and index buffer
					uint32 nVertices = (Slices+1)*(Loops+1);
					pVertexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);
					pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
					pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

					// Setup vertices & geometry
					if (pVertexBuffer->Lock(Lock::WriteOnly)) {
						if (pIndexBuffer->Lock(Lock::WriteOnly)) {
							float fDeltaRadius = (OuterRadius - InnerRadius)/Loops;
							float fDeltaAngle  = static_cast<float>(Math::DegToRad*SweepAngle/Slices);

							// Get offset
							const Vector3 &vOffset = Offset.Get();

							// Create and setup the geometry
							Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							Geometry &cGeometry = lstGeometries.Add();
							cGeometry.SetPrimitiveType(Primitive::PointList);
							cGeometry.SetStartIndex(0);
							cGeometry.SetIndexSize(nVertices);

							// Setup vertex and index buffer
							uint32 nVertex = 0;
							float fRadius = InnerRadius;
							for (uint32 nLoop=0; nLoop<=Loops; nLoop++) {
								float fAngle = static_cast<float>(Math::DegToRad*StartAngle);
								for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
									// Set vertex
									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + fRadius*Math::Sin(fAngle);
									pfVertex[Vector3::Y] = vOffset.y + fRadius*Math::Cos(fAngle);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;

									// Increase fAngle
									fAngle += fDeltaAngle;
								}
								// Increase fRadius
								fRadius += fDeltaRadius;
							}

							// Unlock the index buffer
							pIndexBuffer->Unlock();
						}

						// Unlock the vertex buffer
						pVertexBuffer->Unlock();
					}
					break;
				}
			}
		}
	}

	// Return the created mesh
	return &cMesh;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
