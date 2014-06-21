/*********************************************************\
 *  File: MeshCreatorDisk.cpp                            *
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
#include <PLMath/Vector2.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorDisk.h"


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
pl_implement_class(MeshCreatorDisk)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorDisk::MeshCreatorDisk() :
	InnerRadius(this),
	OuterRadius(this),
	Slices(this),
	Loops(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorDisk::~MeshCreatorDisk()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorDisk::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
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
			float da = static_cast<float>(2.0f*Math::Pi/Slices);
			float dr = (OuterRadius - InnerRadius)/static_cast<float>(Loops);
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
							// Get offset
							const Vector3 &vOffset = Offset.Get();

							// Texture of a disk is a cut out of the texture unit square
							// x, y in [-outerRadius, +outerRadius]; s, t in [0, 1] (linear mapping)
							float dtc = 2.0f*OuterRadius;
							float r1  = InnerRadius;

							// Setup vertex and index buffer
							Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							uint32 nVertex = 0;
							for (uint32 nLoop=0; nLoop<Loops; nLoop++) {
								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize((Slices+1)*2);

								float r2 = r1 + dr;
								if (Order) {
									for (int nSlice=Slices; nSlice>=0; nSlice--) {
										float a  = (nSlice == static_cast<int>(Slices)) ? 0.0f : nSlice*da;
										float sa = Math::Sin(a);
										float ca = Math::Cos(a);

										// Set vertex 0
										float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + r2*sa;
										pfVertex[Vector3::Y] = vOffset.y + r2*ca;
										pfVertex[Vector3::Z] = vOffset.z;
										if (TexCoords) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
											pfVertex[Vector2::X] = 0.5f - sa*r2/dtc;
											pfVertex[Vector2::Y] = 0.5f + ca*r2/dtc;
										}
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
										pfVertex[Vector3::X] = vOffset.x + r1*sa;
										pfVertex[Vector3::Y] = vOffset.y + r1*ca;
										pfVertex[Vector3::Z] = vOffset.z;
										if (TexCoords) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
											pfVertex[Vector2::X] = 0.5f - sa*r1/dtc;
											pfVertex[Vector2::Y] = 0.5f + ca*r1/dtc;
										}
										if (Normals) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
											pfVertex[Vector3::X] =  0.0f;
											pfVertex[Vector3::Y] =  0.0f;
											pfVertex[Vector3::Z] = -1.0f;
										}

										// Set index 1
										pIndexBuffer->SetData(nVertex, nVertex);
										nVertex++;
									}
								} else {
									for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
										float a  = (nSlice == Slices) ? 0.0f : nSlice*da;
										float sa = Math::Sin(a);
										float ca = Math::Cos(a);

										// Set vertex 0
										float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + r2*sa;
										pfVertex[Vector3::Y] = vOffset.y + r2*ca;
										pfVertex[Vector3::Z] = vOffset.z;
										if (TexCoords) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
											pfVertex[Vector2::X] = 0.5f + sa*r2/dtc;
											pfVertex[Vector2::Y] = 0.5f + ca*r2/dtc;
										}
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
										pfVertex[Vector3::X] = vOffset.x + r1*sa;
										pfVertex[Vector3::Y] = vOffset.y + r1*ca;
										pfVertex[Vector3::Z] = vOffset.z;
										if (TexCoords) {
											pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
											pfVertex[Vector2::X] = 0.5f + sa*r1/dtc;
											pfVertex[Vector2::Y] = 0.5f + ca*r1/dtc;
										}
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
								}
								r1 = r2;
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
					uint32 nVertices = (Loops+1)*(Slices+1)+Slices*(Loops+1);
					pVertexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);
					pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
					pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

					// Setup vertices & geometry
					if (pVertexBuffer->Lock(Lock::WriteOnly)) {
						if (pIndexBuffer->Lock(Lock::WriteOnly)) {
							Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							uint32 nVertex = 0;

							// Get offset
							const Vector3 &vOffset = Offset.Get();

							{ // Draw loops
								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(0);
								cGeometry.SetIndexSize((Loops+1)*(Slices+1));

								// Setup vertex and index buffer
								for (uint32 nLoop=0; nLoop<=Loops; nLoop++) {
									float r = InnerRadius + nLoop*dr;
									for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
										float a = nSlice*da;

										// Set vertex
										float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + r*Math::Sin(a);
										pfVertex[Vector3::Y] = vOffset.y + r*Math::Cos(a);
										pfVertex[Vector3::Z] = vOffset.z;

										// Set index
										pIndexBuffer->SetData(nVertex, nVertex);
										nVertex++;
									}
								}
							}

							{ // Draw spokes
								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize(Slices*(Loops+1));

								// Setup vertex and index buffer
								for (uint32 nSlice=0; nSlice<Slices; nSlice++) {
									float a = nSlice*da;
									float x = Math::Sin(a);
									float y = Math::Cos(a);
									for (uint32 nLoop=0; nLoop<=Loops; nLoop++) {
										float r = InnerRadius + nLoop*dr;

										// Set vertex
										float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
										pfVertex[Vector3::X] = vOffset.x + r*x;
										pfVertex[Vector3::Y] = vOffset.y + r*y;
										pfVertex[Vector3::Z] = vOffset.z;

										// Set index
										pIndexBuffer->SetData(nVertex, nVertex);
										nVertex++;
									}
								}
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
					if (InnerRadius != 0.0f)
						nVertices += Slices+1;
					pVertexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);
					pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
					pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

					// Setup vertices & geometry
					if (pVertexBuffer->Lock(Lock::WriteOnly)) {
						if (pIndexBuffer->Lock(Lock::WriteOnly)) {
							// Get offset
							const Vector3 &vOffset = Offset.Get();

							// Create and setup the geometry
							Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							{
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(0);
								cGeometry.SetIndexSize(Slices+1);
							}

							// Setup vertex and index buffer
							uint32 nVertex = 0;
							for (float a=0; a<2.0f*Math::Pi+da; a+=da) {
								// Set vertex
								float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + OuterRadius*Math::Sin(a);
								pfVertex[Vector3::Y] = vOffset.y + OuterRadius*Math::Cos(a);
								pfVertex[Vector3::Z] = vOffset.z;

								// Set index
								pIndexBuffer->SetData(nVertex, nVertex);
								nVertex++;
							}

							// Check inner radius
							if (InnerRadius != 0.0f) {
								// Create and setup the geometry
								Geometry &cGeometry = lstGeometries.Add();
								cGeometry.SetPrimitiveType(Primitive::LineStrip);
								cGeometry.SetStartIndex(nVertex);
								cGeometry.SetIndexSize(Slices+1);

								// Setup vertex and index buffer
								for (float a=0; a<2.0f*Math::Pi+da; a+=da) {
									// Set vertex
									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + InnerRadius*Math::Sin(a);
									pfVertex[Vector3::Y] = vOffset.y + InnerRadius*Math::Cos(a);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;
								}
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
					uint32 nVertices = InnerRadius ? Slices+1 : 1;
					nVertices += OuterRadius ? Slices+1 : 1;
					pVertexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);
					pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
					pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

					// Setup vertices & geometry
					if (pVertexBuffer->Lock(Lock::WriteOnly)) {
						if (pIndexBuffer->Lock(Lock::WriteOnly)) {
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
							// Inner
							if (InnerRadius) {
								for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
									float a = nSlice*da;

									// Set vertex
									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + InnerRadius*Math::Sin(a);
									pfVertex[Vector3::Y] = vOffset.y + InnerRadius*Math::Cos(a);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;
								}
							} else {
								// Set vertex
								float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x;
								pfVertex[Vector3::Y] = vOffset.y;
								pfVertex[Vector3::Z] = vOffset.z;

								// Set index
								pIndexBuffer->SetData(nVertex, nVertex);
								nVertex++;
							}

							// Outer
							if (OuterRadius) {
								for (uint32 nSlice=0; nSlice<=Slices; nSlice++) {
									float a = nSlice*da;
									// Set vertex

									float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
									pfVertex[Vector3::X] = vOffset.x + OuterRadius*Math::Sin(a);
									pfVertex[Vector3::Y] = vOffset.y + OuterRadius*Math::Cos(a);
									pfVertex[Vector3::Z] = vOffset.z;

									// Set index
									pIndexBuffer->SetData(nVertex, nVertex);
									nVertex++;
								}
							} else {
								// Set vertex
								float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x;
								pfVertex[Vector3::Y] = vOffset.y;
								pfVertex[Vector3::Z] = vOffset.z;

								// Set index
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
