/*********************************************************\
 *  File: BodyMesh.cpp                                   *
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
#include <PLCore/System/System.h>
#include <PLCore/File/File.h>
#include <PLCore/File/Directory.h>
#include <PLMath/Half.h>
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLMesh/MeshManager.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLMesh/MeshMorphTarget.h>
#include "PLPhysicsNewton/BodyImpl.h"
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/BodyMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
BodyMesh::~BodyMesh()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BodyMesh::BodyMesh(PLPhysics::World &cWorld, MeshManager &cMeshManager, const String &sMesh, const Vector3 &vMeshScale, bool bOptimize) :
	PLPhysics::BodyMesh(cWorld, static_cast<World&>(cWorld).CreateBodyImpl(), sMesh, vMeshScale, bOptimize)
{
	// Deactivate the physics simulation if required
	const bool bSimulationActive = cWorld.IsSimulationActive();
	if (bSimulationActive)
		cWorld.SetSimulationActive(false);

	// Get the Newton physics world
	Newton::NewtonWorld *pNewtonWorld = static_cast<World&>(cWorld).GetNewtonWorld();

	// First at all: IS there already a collision mesh with this name and scale?
	const String sMeshScale = m_sMesh + String::Format("_%g_%g_%g", vMeshScale.x, vMeshScale.y, vMeshScale.z);
	Newton::NewtonCollision *pCollision = static_cast<World&>(cWorld).GetCollisionMeshMap().Get(sMeshScale);

	// If not, create it NOW
	if (!pCollision) {
		{ // Try to use a cached version
			File *pFile = OpenFile(cWorld, sMeshScale, false);
			if (pFile) {
				pCollision = NewtonCreateCollisionFromSerialization(pNewtonWorld, World::PhysicsDeserialize, pFile);
				pFile->Close();
				delete pFile;
				pFile = nullptr;
			}
		}

		// Do we need to build the tree collision from scratch?
		if (!pCollision) {
			// Get mesh data
			MeshHandler *pMeshHandler = new MeshHandler();
			pMeshHandler->SetMesh(cMeshManager.LoadMesh(m_sMesh));
			const Mesh *pMesh = pMeshHandler->GetResource();
			if (pMesh && pMesh->GetMorphTarget()) {
				// Get LOD level
				const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
				if (pLODLevel) {
					// Setup tree collision
					VertexBuffer *pVertexBuffer = pMesh->GetMorphTarget()->GetVertexBuffer();
					if (pVertexBuffer && pVertexBuffer->Lock(Lock::ReadOnly)) {
						// Get and check vertex position attribute
						const VertexBuffer::Attribute *pPositionAttribute = pVertexBuffer->GetVertexAttribute(VertexBuffer::Position);
						if (pPositionAttribute && (pPositionAttribute->nType == VertexBuffer::Float3 || pPositionAttribute->nType == VertexBuffer::Float4 ||
							pPositionAttribute->nType == VertexBuffer::Half3 || pPositionAttribute->nType == VertexBuffer::Half4)) {
							const bool bHalf = (pPositionAttribute->nType == VertexBuffer::Half3 || pPositionAttribute->nType == VertexBuffer::Half4);

							// Get index buffer
							IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
							if (pIndexBuffer && pIndexBuffer->Lock(Lock::ReadOnly)) {
								// Create collision primitive
								pCollision = NewtonCreateTreeCollision(pNewtonWorld, 0);
								if (pCollision) {
									// Start collision tree
									NewtonTreeCollisionBeginBuild(pCollision);

									// Process every geometry object
									float fVertex[9];
									uint32 nFaces = 0;
									const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
									for (uint32 i=0; i<lstGeometries.GetNumOfElements(); i++) {
										// Get geometry object
										const Geometry &cGeometry = lstGeometries[i];

										// Get the material this geometry is using and check whether or not it's two sided
										bool bTwoSided = false;
										const Material *pMaterial = pMeshHandler->GetMaterial(cGeometry.GetMaterial());
										if (pMaterial) {
											// Two sided material?
											static const String sTwoSided = "TwoSided";
											const Parameter *pParameter = pMaterial->GetParameter(sTwoSided);
											if (pParameter)
												bTwoSided = (pParameter->GetValue1f() == 1.0f);
										}

										// Two sided triangle? If so, we need to add the other side, too
										if (bTwoSided) {
											// Process every triangle
											for (uint32 j=0; j<cGeometry.GetNumOfTriangles(); j++) {
												// Get triangle
												uint32 nVertex[3];
												pLODLevel->GetTriangle(i, j, nVertex[0], nVertex[1], nVertex[2]);

												{ // Add first side face
													// Get triangle vertices
													if (bHalf) {
														for (int nV=0; nV<3; nV++) {
															const uint16 *pnVertex = static_cast<const uint16*>(pVertexBuffer->GetData(nVertex[nV], VertexBuffer::Position));
															fVertex[nV*3+0] = Half::ToFloat(pnVertex[0])*m_vMeshScale.x;
															fVertex[nV*3+1] = Half::ToFloat(pnVertex[1])*m_vMeshScale.y;
															fVertex[nV*3+2] = Half::ToFloat(pnVertex[2])*m_vMeshScale.z;
														}
													} else {
														for (int nV=0; nV<3; nV++) {
															const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(nVertex[nV], VertexBuffer::Position));
															fVertex[nV*3+0] = pfVertex[0]*m_vMeshScale.x;
															fVertex[nV*3+1] = pfVertex[1]*m_vMeshScale.y;
															fVertex[nV*3+2] = pfVertex[2]*m_vMeshScale.z;
														}
													}

													// Add face
													NewtonTreeCollisionAddFace(pCollision, 3, fVertex, sizeof(float)*3, 1);
													nFaces++;
												}

												/* // [TODO] Currently not working correctly and may produce a hang under Linux
												   // Check http://www.newtondynamics.com/forum/viewtopic.php?f=9&t=4922&start=465
												   // [TODO] "half"-support
												{ // Add second side face
													// Get triangle vertices
													for (int nV=0; nV<3; nV++) {
														const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(nVertex[2-nV], VertexBuffer::Position));
														fVertex[nV*3+0] = pfVertex[0]*m_vMeshScale.x;
														fVertex[nV*3+1] = pfVertex[1]*m_vMeshScale.y;
														fVertex[nV*3+2] = pfVertex[2]*m_vMeshScale.z;
													}

													// Add face
													NewtonTreeCollisionAddFace(pCollision, 3, fVertex, sizeof(float)*3, 1);
													nFaces++;
												}
												*/
											}
										} else {
											// Process every triangle
											if (bHalf) {
												for (uint32 j=0; j<cGeometry.GetNumOfTriangles(); j++) {
													// Get triangle
													uint32 nVertex[3];
													pLODLevel->GetTriangle(i, j, nVertex[0], nVertex[1], nVertex[2]);

													// Get triangle vertices
													for (int nV=0; nV<3; nV++) {
														const uint16 *pnVertex = static_cast<const uint16*>(pVertexBuffer->GetData(nVertex[nV], VertexBuffer::Position));
														fVertex[nV*3+0] = Half::ToFloat(pnVertex[0])*m_vMeshScale.x;
														fVertex[nV*3+1] = Half::ToFloat(pnVertex[1])*m_vMeshScale.y;
														fVertex[nV*3+2] = Half::ToFloat(pnVertex[2])*m_vMeshScale.z;
													}

													// Add face
													NewtonTreeCollisionAddFace(pCollision, 3, fVertex, sizeof(float)*3, 1);
													nFaces++;
												}
											} else {
												for (uint32 j=0; j<cGeometry.GetNumOfTriangles(); j++) {
													// Get triangle
													uint32 nVertex[3];
													pLODLevel->GetTriangle(i, j, nVertex[0], nVertex[1], nVertex[2]);

													// Get triangle vertices
													for (int nV=0; nV<3; nV++) {
														const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(nVertex[nV], VertexBuffer::Position));
														fVertex[nV*3+0] = pfVertex[0]*m_vMeshScale.x;
														fVertex[nV*3+1] = pfVertex[1]*m_vMeshScale.y;
														fVertex[nV*3+2] = pfVertex[2]*m_vMeshScale.z;
													}

													// Add face
													NewtonTreeCollisionAddFace(pCollision, 3, fVertex, sizeof(float)*3, 1);
													nFaces++;
												}
											}
										}
									}

									// Finish tree collision (may take some time!)
									NewtonTreeCollisionEndBuild(pCollision, m_bOptimize);

									// Try to cache the result so we don't need to do this again next time
									File *pFile = OpenFile(cWorld, sMeshScale, true);
									if (pFile) {
										NewtonCollisionSerialize(pNewtonWorld, pCollision, World::PhysicsSerialize, pFile);
										pFile->Close();
										delete pFile;
										pFile = nullptr;
									}
								}

								// Unlock the index buffer
								pIndexBuffer->Unlock();
							}

							// Unlock the vertex buffer
							pVertexBuffer->Unlock();
						}
					}
				}
			}

			// Cleanup
			delete pMeshHandler;
		}

		// Add to collision mesh map
		if (pCollision)
			static_cast<World&>(cWorld).GetCollisionMeshMap().Add(sMeshScale, pCollision);
	}

	// Create the rigid body
	if (pCollision) {
		// [TODO] Remove this as soon as there's an up-to-date Linux version of Newton Game Dynamics available!
		#if (NEWTON_MAJOR_VERSION == 2) && (NEWTON_MINOR_VERSION >= 28)
			Newton::NewtonBody *pNewtonBody = NewtonCreateBody(pNewtonWorld, pCollision, Matrix4x4::Identity);
		#else
			Newton::NewtonBody *pNewtonBody = NewtonCreateBody(pNewtonWorld, pCollision);
		#endif

		// Initialize the Newton physics body
		static_cast<BodyImpl&>(GetBodyImpl()).InitializeNewtonBody(*this, *pNewtonBody, 0.0f);
	}

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		cWorld.SetSimulationActive(bSimulationActive);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Opens a file
*/
File *BodyMesh::OpenFile(PLPhysics::World &cWorld, const String &sFilename, bool bCreate) const
{
	// Get the Newton physics cache directory
	String sCacheDirectory = cWorld.GetAttribute("CacheDirectory")->GetString();
	if (sCacheDirectory.GetLength()) {
		// Replace any dots or slashes within the given filename through '#'
		String sCacheFilename = sFilename;
		sCacheFilename.Replace('.',  '#');
		sCacheFilename.Replace('/',  '#');
		sCacheFilename.Replace('\\', '#');

		// If the filename is absolute, we need to make it relative
		if (Url(sCacheFilename).IsAbsolute()) {
			const String &sDirectory = System::GetInstance()->GetCurrentDir();
			if (sCacheFilename.IsSubstring(sDirectory))
				sCacheFilename = sCacheFilename.GetSubstring(sDirectory.GetLength());
			else
				return nullptr; // Error!
		}

		// We REALLY need a slash at the end of the cache directory name
		const char nEnding = sCacheDirectory[sCacheDirectory.GetLength()-1];
		if (nEnding != '/' && nEnding != '\\')
			sCacheDirectory += '/';

		// Add the Newton physics cache directory
		const String sFinalCacheFilename = sCacheDirectory + sCacheFilename + ".tc";

		// Open the file object
		File *pFile = new File(sFinalCacheFilename);

		// Abort, if it is a directory
		if (pFile->Exists() && pFile->IsDirectory()) {
			// Error!
			delete pFile;
			return nullptr;
		}

		// Create the file?
		if (!pFile->Exists() && bCreate) {
			if (!pFile->Create()) {
				String sPath = pFile->GetUrl().CutFilename();
				sPath.Delete(sPath.GetLength()-1); // Remove the slash at the end
				if (Directory(sPath).CreateRecursive())
					pFile->Create();
			}
		}

		// Open the file itself
		if (pFile->Open(bCreate ? File::FileWrite : File::FileRead))
			return pFile; // Done
		else
			delete pFile;
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
