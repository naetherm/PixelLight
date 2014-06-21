/*********************************************************\
 *  File: MeshLoaderPL.cpp                               *
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


// [TODO] Use String in this class


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/File/File.h>
#include <PLCore/Container/Stack.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Animation/AnimationEvent.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLMesh/Mesh.h"
#include "PLMesh/MeshOctree.h"
#include "PLMesh/AnchorPoint.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MorphTargetAni.h"
#include "PLMesh/SkeletonManager.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Loader/MeshLoaderPL.h"


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
pl_implement_class(MeshLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool MeshLoaderPL::Load(Mesh &cMesh, File &cFile)
{
	return LoadParams(cMesh, cFile, true);
}

bool MeshLoaderPL::LoadParams(Mesh &cMesh, File &cFile, bool bStatic)
{
	// Read first chunk
	bool bResult = true; // No error by default
	MeshFile::Chunk sChunk = ReadChunk(cFile);
	switch (sChunk.nType) {
		// MESHFILE chunk has been found
		case MeshFile::CHUNK_MESHFILE:
			bResult = ReadMeshFile(cMesh, cFile, sChunk, bStatic);
			break;

		// Unknown chunk
		default:
			// Ignore the unknown chunk :)
//			bResult = false; // Error!
			break;
	}

	// Create final octree
	for (uint32 i=0; i<cMesh.GetNumOfLODLevels(); i++) {
		MeshLODLevel &cLODLevel = *cMesh.GetLODLevel(i);
		if (cLODLevel.GetOctree()) {
			cLODLevel.CreateOctree(cLODLevel.GetOctree()->GetSubdivide(),
								   cLODLevel.GetOctree()->GetMinGeometries());
		}
	}

	// Done
	return bResult;
}

bool MeshLoaderPL::Save(Mesh &cMesh, File &cFile)
{
	// Write file
	Stack<MeshFile::Chunk> cChunkStack;
	return WriteMeshFile(cMesh, cFile, cChunkStack);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshLoaderPL::MeshLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
MeshLoaderPL::~MeshLoaderPL()
{
}


//[-------------------------------------------------------]
//[ File loading                                          ]
//[-------------------------------------------------------]
bool MeshLoaderPL::ReadMeshFile(Mesh &cMesh, File &cFile, MeshFile::Chunk &cChunk, bool bStatic) const
{
	// Read header
	MeshFile::MeshFileHeader sMeshFileHeader;
	if (cFile.Read(&sMeshFileHeader, 1, sizeof(sMeshFileHeader)) &&
		sMeshFileHeader.nMagic == MeshFile::MAGIC &&
		sMeshFileHeader.nVersion == MeshFile::VERSION) {
		// Read chunks
		bool bResult = true; // No error by default
		MeshFile::Chunk sChunk = ReadChunk(cFile);
		while (bResult && sChunk.nType && sChunk.nSize) {
			// Interpret chunk
			switch (sChunk.nType) {
				case MeshFile::CHUNK_MATERIALS:
					bResult = ReadMaterials(cMesh, cFile);
					break;

				case MeshFile::CHUNK_MESH:
					bResult = ReadMesh(cMesh, cFile, bStatic);
					break;

				case MeshFile::CHUNK_SKELETON:
					bResult = ReadSkeleton(cMesh, cFile);
					break;

				case MeshFile::CHUNK_ANCHORPOINTS:
					bResult = ReadAnchorPoints(cMesh, cFile);
					break;

				case MeshFile::CHUNK_ANIMATIONS:
					bResult = ReadAnimations(cFile);
					break;

				case MeshFile::CHUNK_MORPHTARGETANIMATION:
					bResult = ReadMorphTargetAnimation(cMesh, cFile);
					break;

				case MeshFile::CHUNK_MESHBOUNDINGBOX:
					bResult = ReadMeshBoundingBox(cMesh, cFile);
					break;

				default:
					// Skip unknown chunk
					cFile.Seek(sChunk.nSize - sizeof(MeshFile::Chunk), File::SeekCurrent);
					break;
			}

			// Read next chunk
			sChunk = ReadChunk(cFile);
		}

		// Done
		return bResult;
	}

	// Error!
	return false;
}

bool MeshLoaderPL::ReadMaterials(Mesh &cMesh, File &cFile) const
{
	// Read materials header
	MeshFile::Materials sMaterials;
	if (!cFile.Read(&sMaterials, 1, sizeof(sMaterials)))
		return false; // Error!

	// Get the material manager
	MaterialManager &cMaterialManager = cMesh.GetRenderer()->GetRendererContext().GetMaterialManager();

	// Read materials
	char szMaterial[256];
	for (uint32 i=0; i<sMaterials.nMaterials; i++) {
		if (!cFile.Read(szMaterial, 1, 256))
			return false; // Error!
		Material *pMaterial = cMaterialManager.LoadResource(szMaterial);
		cMesh.AddMaterial(pMaterial ? pMaterial : cMaterialManager.Create(szMaterial));
	}

	// Done
	return true;
}

bool MeshLoaderPL::ReadMesh(Mesh &cMesh, File &cFile, bool bStatic) const
{
	// Read mesh header
	MeshFile::Mesh sMesh;
	if (!cFile.Read(&sMesh, 1, sizeof(sMesh)))
		return false; // Error!

	// Read LOD levels
	for (uint32 i=0; i<sMesh.nLODLevels; i++) {
		// Read LOD level
		MeshFile::Chunk sChunk = ReadChunk(cFile);
		if (sChunk.nType != MeshFile::CHUNK_LODLEVEL)
			return false; // Error!
		MeshLODLevel *pLODLevel = cMesh.AddLODLevel();
		if (!pLODLevel || !ReadLODLevel(cFile, *pLODLevel, bStatic))
			return false; // Error!
	}

	// Read morph targets
	for (uint32 i=0; i<sMesh.nMorphTargets; i++) {
		// Read morph target
		MeshFile::Chunk sChunk = ReadChunk(cFile);
		if (sChunk.nType != MeshFile::CHUNK_MORPHTARGET)
			return false; // Error!
		MeshMorphTarget *pMeshMorphTarget = cMesh.AddMorphTarget();
		if (!pMeshMorphTarget || !ReadMorphTarget(cFile, *pMeshMorphTarget, bStatic))
			return false; // Error!
	}

	// Read weighs
	cMesh.GetWeights().Resize(sMesh.nWeights);
	for (uint32 i=0; i<sMesh.nWeights; i++) {
		MeshFile::Chunk sChunk = ReadChunk(cFile);
		if (sChunk.nType != MeshFile::CHUNK_WEIGHT)
			return false; // Error!
		if (!ReadWeight(cFile, cMesh.GetWeights()[i]))
			return false; // Error!
	}

	// Read vertex weights
	Array<VertexWeights> &lstVW = cMesh.GetVertexWeights();
	lstVW.Resize(sMesh.nVertexWeights);
	for (uint32 i=0; i<lstVW.GetNumOfElements(); i++) {
		MeshFile::Chunk sChunk = ReadChunk(cFile);
		if (sChunk.nType != MeshFile::CHUNK_VERTEXWEIGHTS)
			return false; // Error!
		if (!ReadVertexWeights(cFile, lstVW[i]))
			return false; // Error!
	}

	// Done
	return true;
}

bool MeshLoaderPL::ReadLODLevel(File &cFile, MeshLODLevel &cLODLevel, bool bStatic) const
{
	MeshFile::LODLevel sLODLevel;
	if (!cFile.Read(&sLODLevel, 1, sizeof(sLODLevel)))
		return false; // Error!
	cLODLevel.SetLODDistance(sLODLevel.fDistance);

	// [TODO] Remove octree from mesh class :)
	// Create temp octree for storing the two settings
//	if (sLODLevel.nOctreeSubdivide != 0 && sLODLevel.nOctreeMinGeometries != 0)
//		cLODLevel.CreateOctree(sLODLevel.nOctreeSubdivide, sLODLevel.nOctreeMinGeometries);

	// Read index buffer
	bool bSkip = false;
	MeshFile::Chunk sChunk = ReadChunk(cFile);
	if (sChunk.nType == MeshFile::CHUNK_INDEXBUFFER) {
		cLODLevel.CreateIndexBuffer();
		IndexBuffer *pIndexBuffer = cLODLevel.GetIndexBuffer();
		if (!pIndexBuffer || !ReadIndexBuffer(cFile, *pIndexBuffer, bStatic))
			return false; // Error!
	} else {
		bSkip = true;
	}

	// Read geometries
	if (sLODLevel.nGeometries > 0)
		cLODLevel.CreateGeometries();
	Array<Geometry> *plstGeometries = cLODLevel.GetGeometries();
	plstGeometries->Resize(sLODLevel.nGeometries);
	for (uint32 i=0; i<sLODLevel.nGeometries; i++) {
		// Read next chunk
		if (!bSkip || i > 0)
			sChunk = ReadChunk(cFile);

		// Read geometry
		if (sChunk.nType != MeshFile::CHUNK_GEOMETRY)
			return false; // Error!
		Geometry &cGeometry = plstGeometries->Get(i);
		if ((&cGeometry == &Array<Geometry>::Null) || !ReadGeometry(cFile, cGeometry))
			return false; // Error!
	}

	// Done
	return true;
}

bool MeshLoaderPL::ReadIndexBuffer(File &cFile, IndexBuffer &cIndexBuffer, bool bStatic) const
{
	// Read index buffer
	MeshFile::IndexBuffer sIndexBuffer;
	if (!cFile.Read(&sIndexBuffer, 1, sizeof(sIndexBuffer)))
		return false; // Error!
	cIndexBuffer.SetElementType(static_cast<IndexBuffer::EType>(sIndexBuffer.nElementType));
	cIndexBuffer.Allocate(sIndexBuffer.nElements, bStatic ? Usage::Static : Usage::Dynamic);

	// Read data
	bool bResult = true; // No error by default
	void *pData = cIndexBuffer.Lock(Lock::WriteOnly);
	if (pData) {
		if (!cFile.Read(pData, 1, sIndexBuffer.nSize))
			bResult = false; // Error!

		// Unlock the index buffer
		cIndexBuffer.Unlock();
	} else {
		// Error!
		bResult = false;
	}

	// Done
	return bResult;
}

bool MeshLoaderPL::ReadGeometry(File &cFile, Geometry &cGeometry) const
{
	// Read geometry header
	MeshFile::Geometry sGeometry;
	if (!cFile.Read(&sGeometry, 1, sizeof(sGeometry)))
		return false; // Error!
	cGeometry.SetName		  (sGeometry.szName);
	cGeometry.SetFlags		  (sGeometry.nFlags);
	cGeometry.SetActive		  (sGeometry.bActive);
	cGeometry.SetPrimitiveType(static_cast<Primitive::Enum>(sGeometry.nPrimitiveType));
	cGeometry.SetMaterial	  (sGeometry.nMaterial);
	cGeometry.SetStartIndex   (sGeometry.nStartIndex);
	cGeometry.SetIndexSize	  (sGeometry.nIndexSize);

	// Done
	return true;
}

bool MeshLoaderPL::ReadWeight(File &cFile, Weight &cWeight) const
{
	// Read weight header
	MeshFile::Weight sWeight;
	if (!cFile.Read(&sWeight, 1, sizeof(sWeight)))
		return false; // Error!
	cWeight.SetJoint(sWeight.nJoint);
	cWeight.SetBias (sWeight.fBias);

	// Done
	return true;
}

bool MeshLoaderPL::ReadVertexWeights(File &cFile, VertexWeights &cVertexWeights) const
{
	// Read vertex weights header
	MeshFile::VertexWeights sVertexWeights;
	if (!cFile.Read(&sVertexWeights, 1, sizeof(sVertexWeights)))
		return false; // Error!

	// Read vertex weights
	Array<uint32> &lstVertexWeights = cVertexWeights.GetWeights();
	lstVertexWeights.Resize(sVertexWeights.nWeights);
	if (lstVertexWeights.GetNumOfElements()) {
		if (!cFile.Read(lstVertexWeights.GetData(), 1, sizeof(uint32)*lstVertexWeights.GetNumOfElements()))
			return false; // Error!
	}

	// Done
	return true;
}

bool MeshLoaderPL::ReadMorphTarget(File &cFile, MeshMorphTarget &cMorphTarget, bool bStatic) const
{
	// Read morph target header
	MeshFile::MorphTarget sMorphTarget;
	if (!cFile.Read(&sMorphTarget, 1, sizeof(sMorphTarget)))
		return false; // Error!
	cMorphTarget.SetName(sMorphTarget.szName);
	cMorphTarget.SetRelative(sMorphTarget.bRelative);

	// Read vertex ID's
	Array<uint32> &lstVertexIDs = cMorphTarget.GetVertexIDs();
	lstVertexIDs.Resize(sMorphTarget.nVertexIDs);
	if (lstVertexIDs.GetNumOfElements()) {
		if (!cFile.Read(lstVertexIDs.GetData(), 1, sizeof(uint32)*lstVertexIDs.GetNumOfElements()))
			return false; // Error!
	}

	// Read vertex buffers
	for (uint32 i=0; i<sMorphTarget.nVertexBuffers; i++) {
		// Read vertex buffer
		MeshFile::Chunk sChunk = ReadChunk(cFile);
		if (sChunk.nType != MeshFile::CHUNK_VERTEXBUFFER)
			return false; // Error!
		if (!ReadVertexBuffer(cFile, *cMorphTarget.GetVertexBuffer(), i, bStatic))
			return false; // Error!
	}

	// Done
	return true;
}

bool MeshLoaderPL::ReadVertexBuffer(File &cFile, VertexBuffer &cVertexBuffer, uint32 nIndex, bool bStatic) const
{
	// Read vertex buffer header
	MeshFile::VertexBuffer sVertexBuffer;
	if (!cFile.Read(&sVertexBuffer, 1, sizeof(sVertexBuffer)))
		return false; // Error!

	// Read vertex attributes
	for (uint32 i=0; i<sVertexBuffer.nVertexAttributes; i++) {
		// Read vertex attribute
		MeshFile::Chunk sChunk = ReadChunk(cFile);
		if (sChunk.nType != MeshFile::CHUNK_VERTEXATTRIBUTE)
			return false; // Error!
		if (!ReadVertexAttribute(cFile, cVertexBuffer))
			return false; // Error!
	}

	// Allocate the vertex buffer
	if (nIndex)
		cVertexBuffer.Allocate(sVertexBuffer.nVertices, Usage::Software);
	else
		cVertexBuffer.Allocate(sVertexBuffer.nVertices, bStatic ? Usage::Static : Usage::Dynamic);

	// Read data
	bool bResult = true; // No error by default
	void *pData = cVertexBuffer.Lock(Lock::WriteOnly);
	if (pData) {
		if (!cFile.Read(pData, 1, sVertexBuffer.nSize))
			bResult = false; // Error!

		// Unlock the vertex buffer
		cVertexBuffer.Unlock();
	} else {
		// Error!
		bResult = false;
	}

	// Done
	return bResult;
}

bool MeshLoaderPL::ReadVertexAttribute(File &cFile, VertexBuffer &cVertexBuffer) const
{
	// Read vertex attribute
	MeshFile::VertexAttribute sVertexAttribute;
	if (!cFile.Read(&sVertexAttribute, 1, sizeof(sVertexAttribute)))
		return false; // Error!
	cVertexBuffer.AddVertexAttribute(
		static_cast<VertexBuffer::ESemantic>(sVertexAttribute.nSemantic),
		sVertexAttribute.nChannel,
		static_cast<VertexBuffer::EType>(sVertexAttribute.nType)
	);

	// Done
	return true;
}

bool MeshLoaderPL::ReadSkeleton(Mesh &cMesh, File &cFile) const
{
	// Read skeleton header
	MeshFile::Skeleton sSkeleton;
	if (!cFile.Read(&sSkeleton, 1, sizeof(sSkeleton)))
		return false; // Error!

	// Get the skeleton manager
	SkeletonManager &cSkeletonManager = cMesh.GetSkeletonManager();

	// Create skeleton
	Skeleton *pSkeleton = cSkeletonManager.Create(sSkeleton.szName);
	if (pSkeleton) {
		// Add this skeleton to the skeleton list of the mesh
		SkeletonHandler *pSkeletonHandler = new SkeletonHandler();
		pSkeletonHandler->SetResource(pSkeleton);
		cMesh.GetSkeletonHandlers().Add(pSkeletonHandler);

		// Animated joint states
		uint32 nAnimatedComponents = 0; // Total number of animated components per frame
		Array<Skeleton::AniJoint> &lstJoints = pSkeleton->GetJoints();
		lstJoints.Resize(sSkeleton.nJoints);

		// Read joints
		for (uint32 i=0; i<sSkeleton.nJoints; i++) {
			MeshFile::Joint sJoint;
			if (!cFile.Read(&sJoint, 1, sizeof(sJoint)))
				return false; // Error!
			Joint *pJoint = pSkeleton->Create(sJoint.szName);
			pJoint->SetID(i);
			pJoint->SetParent(sJoint.nParent);
			Skeleton::AniJoint &cAniJoint = lstJoints[i];
			cAniJoint.nAnimatedComponents = sJoint.nAnimatedComponents;
			if (sJoint.nAnimatedComponents & Skeleton::AX)
				nAnimatedComponents++;
			if (sJoint.nAnimatedComponents & Skeleton::AY)
				nAnimatedComponents++;
			if (sJoint.nAnimatedComponents & Skeleton::AZ)
				nAnimatedComponents++;
			if (sJoint.nAnimatedComponents & Skeleton::AYaw)
				nAnimatedComponents++;
			if (sJoint.nAnimatedComponents & Skeleton::APitch)
				nAnimatedComponents++;
			if (sJoint.nAnimatedComponents & Skeleton::ARoll)
				nAnimatedComponents++;
			if (sJoint.nAnimatedComponents & Skeleton::AW)
				nAnimatedComponents++;
		}

		// Read base frame
		for (uint32 i=0; i<sSkeleton.nJoints; i++) {
			MeshFile::JointState cJointState;
			if (!cFile.Read(&cJointState, 1, sizeof(MeshFile::JointState)))
				return false; // Error!
			Joint *pJoint = pSkeleton->GetByIndex(i);
			pJoint->SetTranslation(Vector3(cJointState.fTranslation[0],
				cJointState.fTranslation[1], cJointState.fTranslation[2]));
			pJoint->SetRotation(Quaternion(cJointState.fRotation[0],
				cJointState.fRotation[1], cJointState.fRotation[2], cJointState.fRotation[3]));
			pJoint->SetTranslationJointSpace(Vector3(cJointState.fTranslationJointSpace[0],
				cJointState.fTranslationJointSpace[1], cJointState.fTranslationJointSpace[2]));
			pJoint->SetRotationJointSpace(Quaternion(cJointState.fRotationJointSpace[0],
				cJointState.fRotationJointSpace[1], cJointState.fRotationJointSpace[2], cJointState.fRotationJointSpace[3]));
		}

		// Read frame keys
		Array<Skeleton::FrameKeys> &lstFrameKeys = pSkeleton->GetFrameKeys();
		lstFrameKeys.Resize(sSkeleton.nFrames);
		for (uint32 i=0; i<lstFrameKeys.GetNumOfElements(); i++) {
			Array<float> &lstFrameKeysT = lstFrameKeys[i].lstFrameKeys;
			lstFrameKeysT.Resize(nAnimatedComponents);
			if (lstFrameKeysT.GetNumOfElements()) {
				if (!cFile.Read(lstFrameKeysT.GetData(), 1, sizeof(float)*lstFrameKeysT.GetNumOfElements()))
					return false; // Error!
			}
		}
		pSkeleton->SetEndFrame(sSkeleton.nFrames ? sSkeleton.nFrames-1 : 0);
		pSkeleton->SetFlags(AnimationInfo::Loop);

		// Update skeleton
		pSkeleton->UpdateJointInformation();

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

bool MeshLoaderPL::ReadAnchorPoints(Mesh &cMesh, File &cFile) const
{
	// Read anchor points header
	MeshFile::AnchorPoints sAnchorPoints;
	if (!cFile.Read(&sAnchorPoints, 1, sizeof(sAnchorPoints)))
		return false; // Error!

	// Read anchor points
	for (uint32 i=0; i<sAnchorPoints.nAnchorPoints; i++) {
		MeshFile::AnchorPoint sAnchorPoint;
		if (!cFile.Read(&sAnchorPoint, 1, sizeof(sAnchorPoint)))
			return false; // Error!
		// Added into a manager automatically...
		new AnchorPoint(sAnchorPoint.szName, sAnchorPoint.bType, sAnchorPoint.nID, &cMesh.GetAnchorPointManager());
	}

	// Done
	return true;
}

bool MeshLoaderPL::ReadAnimations(File &cFile) const
{
	// Read animations header
	MeshFile::Animations sAnimations;
	if (!cFile.Read(&sAnimations, 1, sizeof(sAnimations)))
		return false; // Error!

	// Read animations
	for (uint32 i=0; i<sAnimations.nAnimations; i++) {
		// Read animation
		MeshFile::Animation sAnimation;
		if (!cFile.Read(&sAnimation, 1, sizeof(sAnimation)))
			return false; // Error!
		AnimationInfo *pAni = new AnimationInfo();
		// [TODO] Update this
//		pAni->SetName(sAnimation.szName);
		pAni->SetType(sAnimation.nType);
		pAni->SetStartFrame(sAnimation.nStart);
		pAni->SetEndFrame(sAnimation.nEnd);
		pAni->SetSpeed(sAnimation.fSpeed);
		pAni->SetFlags(sAnimation.nFlags);
		// Read animation frames
		for (uint32 j=0; j<pAni->GetNumOfFrames(); j++) {
			MeshFile::AnimationFrame sAnimationFrame;
			if (!cFile.Read(&sAnimationFrame, 1, sizeof(sAnimationFrame)))
				return false; // Error!
			pAni->GetFrameInfo(j)->SetSpeed(sAnimationFrame.m_fSpeed);
		}
		// Read animation events
		for (int j=0; j<sAnimation.nEvents; j++) {
			MeshFile::AnimationEvent sAnimationEvent;
			if (!cFile.Read(&sAnimationEvent, 1, sizeof(sAnimationEvent)))
				return false; // Error!
			// Added into a manager automatically...
			new AnimationEvent(sAnimationEvent.m_nID, sAnimationEvent.m_nFrame, &pAni->GetEventManager());
		}
		delete pAni;
	}

	// Done
	return true;
}

bool MeshLoaderPL::ReadMorphTargetAnimation(Mesh &cMesh, File &cFile) const
{
	// Read morph target animation header
	MeshFile::MorphTargetAnimation sMorphTargetAnimation;
	if (!cFile.Read(&sMorphTargetAnimation, 1, sizeof(sMorphTargetAnimation)))
		return false; // Error!

	// Add morph target animation
	MorphTargetAni *pAni = new MorphTargetAni(sMorphTargetAnimation.szName, &cMesh.GetMorphTargetAnimationManager());

	// Read morph targets
	pAni->GetMorphTargets().Resize(sMorphTargetAnimation.nMorphTargets);
	char szName[64];
	for (uint32 i=0; i<sMorphTargetAnimation.nMorphTargets; i++) {
		if (!cFile.Read(szName, 1, sizeof(char)*64))
			return false; // Error!
		pAni->GetMorphTargets()[i].sName = szName;
	}

	// Read frame keys
	Array<MorphTargetAni::FrameKeys> &lstFrameKeys = pAni->GetFrameKeys();
	lstFrameKeys.Resize(sMorphTargetAnimation.nFrames);
	for (uint32 i=0; i<lstFrameKeys.GetNumOfElements(); i++) {
		Array<float> &lstFrameKeysT = lstFrameKeys[i].lstFrameKeys;
		lstFrameKeysT.Resize(sMorphTargetAnimation.nMorphTargets);
		if (lstFrameKeysT.GetNumOfElements()) {
			if (!cFile.Read(lstFrameKeysT.GetData(), 1, sizeof(float)*lstFrameKeysT.GetNumOfElements()))
				return false; // Error!
		}
	}
	pAni->SetEndFrame(sMorphTargetAnimation.nFrames ? sMorphTargetAnimation.nFrames-1 : 0);
	pAni->SetFlags(AnimationInfo::Loop);
	pAni->SetType(1);

	// Done
	return true;
}

bool MeshLoaderPL::ReadMeshBoundingBox(Mesh &cMesh, File &cFile) const
{
	// Read mesh bounding box
	MeshFile::BoundingBox sBoundingBox;
	if (!cFile.Read(&sBoundingBox, 1, sizeof(sBoundingBox)))
		return false; // Error!

	// Set the mesh bounding box
	cMesh.SetBoundingBox(Vector3(sBoundingBox.fMin), Vector3(sBoundingBox.fMax));

	// Done
	return true;
}

MeshFile::Chunk MeshLoaderPL::ReadChunk(File &cFile) const
{
	// Read chunk
	MeshFile::Chunk sChunk = {0, 0};
	if (!cFile.Read(&sChunk, 1, sizeof(MeshFile::Chunk))) {
		// Error
		sChunk.nType = 0;
		sChunk.nSize = 0;
	}

	// Return chunk
	return sChunk;
}


//[-------------------------------------------------------]
//[ File saving                                           ]
//[-------------------------------------------------------]
bool MeshLoaderPL::WriteMeshFile(Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_MESHFILE)) {
		// Write mesh file header
		MeshFile::MeshFileHeader cMeshFileHeader;
		cMeshFileHeader.nMagic   = MeshFile::MAGIC;
		cMeshFileHeader.nVersion = MeshFile::VERSION;
		if (cFile.Write(&cMeshFileHeader, 1, sizeof(cMeshFileHeader))) {
			// Write mesh data
			if (WriteMaterials(cMesh, cFile, cChunkStack)) {
				if (WriteMesh(cMesh, cFile, cChunkStack)) {
					// Write skeleton data
					Array<SkeletonHandler*> &lstSkeletonHandlers = cMesh.GetSkeletonHandlers();
					for (uint32 i=0; i<lstSkeletonHandlers.GetNumOfElements(); i++) {
						Skeleton *pSkeleton = lstSkeletonHandlers[i]->GetResource();
						if (!pSkeleton || !WriteSkeleton(cFile, cChunkStack, *pSkeleton))
							return false; // Error!
					}

					// Misc
					if (WriteAnchorPoints(cMesh, cFile, cChunkStack)) {
					// [TODO] Update this
					//	if (!WriteAnimations(cMesh, cFile, cChunkStack))	 return false; // Error!

						// Write morph target animations
						for (uint32 i=0; i<cMesh.GetMorphTargetAnimationManager().GetNumOfElements(); i++)
							WriteMorphTargetAnimation(cMesh, cFile, cChunkStack, i);

						// End chunk
						return EndChunk(cFile, cChunkStack);
					}
				}
			}
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteMaterials(const Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_MATERIALS)) {
		// Write materials header
		MeshFile::Materials cMaterials;
		cMaterials.nMaterials = cMesh.GetNumOfMaterials();
		if (cFile.Write(&cMaterials, 1, sizeof(cMaterials))) {
			// Write materials
			char szMaterial[256];
			for (uint32 i=0; i<cMesh.GetNumOfMaterials(); i++) {
				String sName   = cMesh.GetMaterial(i)->GetName();
				uint32 nLength = sName.GetLength();
				if (nLength > 255)
					nLength = 255;
				MemoryManager::Copy(szMaterial, sName.GetASCII(), nLength);
				szMaterial[nLength] = '\0';
				if (!cFile.Write(szMaterial, 1, 256))
					return false; // Error!
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteMesh(Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_MESH)) {
		// Write mesh header
		MeshFile::Mesh cMeshHeader;
		cMeshHeader.nLODLevels     = cMesh.GetNumOfLODLevels();
		cMeshHeader.nMorphTargets  = cMesh.GetNumOfMorphTargets();
		cMeshHeader.nWeights       = cMesh.GetWeights().GetNumOfElements();
		cMeshHeader.nVertexWeights = cMesh.GetVertexWeights().GetNumOfElements();
		if (cFile.Write(&cMeshHeader, 1, sizeof(cMeshHeader))) {
			// Write LOD levels
			for (uint32 i=0; i<cMesh.GetNumOfLODLevels(); i++) {
				if (!WriteLODLevel(cFile, cChunkStack, *cMesh.GetLODLevel(i)))
					return false; // Error!
			}

			// Write morph targets
			for (uint32 i=0; i<cMesh.GetNumOfMorphTargets(); i++) {
				if (!WriteMorphTarget(cFile, cChunkStack, *cMesh.GetMorphTarget(i)))
					return false; // Error!
			}

			// Write weights
			for (uint32 i=0; i<cMeshHeader.nWeights; i++) {
				if (!WriteWeight(cFile, cChunkStack, cMesh.GetWeights()[i]))
					return false; // Error!
			}

			// Write vertex weights
			for (uint32 i=0; i<cMeshHeader.nVertexWeights; i++) {
				if (!WriteVertexWeights(cFile, cChunkStack, cMesh.GetVertexWeights()[i]))
					return false; // Error!
			}

			// Write mesh bounding box
			if (WriteMeshBoundingBox(cMesh, cFile, cChunkStack))
				return EndChunk(cFile, cChunkStack); // End chunk
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteLODLevel(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, MeshLODLevel &cLODLevel) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_LODLEVEL)) {
		// Write LOD level header
		MeshFile::LODLevel sLODLevel;
		sLODLevel.fDistance   = cLODLevel.GetLODDistance();
		sLODLevel.nGeometries = cLODLevel.GetGeometries()->GetNumOfElements();
		if (cLODLevel.GetOctree()) {
			sLODLevel.nOctreeSubdivide     = cLODLevel.GetOctree()->GetSubdivide();
			sLODLevel.nOctreeMinGeometries = cLODLevel.GetOctree()->GetMinGeometries();
		} else {
			sLODLevel.nOctreeSubdivide     = 0;
			sLODLevel.nOctreeMinGeometries = 0;
		}
		if (cFile.Write(&sLODLevel, 1, sizeof(sLODLevel))) {
			// Write index buffer
			IndexBuffer *pIndexBuffer = cLODLevel.GetIndexBuffer();
			if (!pIndexBuffer || !WriteIndexBuffer(cFile, cChunkStack, *pIndexBuffer))
				return false; // Error!

			// Write geometries
			for (uint32 i=0; i<sLODLevel.nGeometries; i++) {
				if (!WriteGeometry(cFile, cChunkStack, cLODLevel.GetGeometries()->Get(i)))
					return false; // Error!
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteIndexBuffer(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, IndexBuffer &cIndexBuffer) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_INDEXBUFFER)) {
		// Write index buffer header
		MeshFile::IndexBuffer sIndexBuffer;
		sIndexBuffer.nElementType = cIndexBuffer.GetElementType();
		sIndexBuffer.nElements	  = cIndexBuffer.GetNumOfElements();
		sIndexBuffer.nSize		  = cIndexBuffer.GetSize();
		if (cFile.Write(&sIndexBuffer, 1, sizeof(sIndexBuffer))) {
			// Write index buffer
			bool bResult = true; // No error by default
			void *pData = cIndexBuffer.Lock(Lock::ReadOnly);
			if (pData) {
				if (!cFile.Write(pData, 1, cIndexBuffer.GetSize()))
					bResult = false; // Error!

				// Unlock the index buffer
				cIndexBuffer.Unlock();
			} else {
				// Error!
				bResult = false;
			}

			// End chunk
			if (!EndChunk(cFile, cChunkStack))
				bResult = false; // Error!

			// Done
			return bResult;
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteGeometry(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, const Geometry &cGeometry) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_GEOMETRY)) {
		// Write geometry
		MeshFile::Geometry sGeometry;
		String sName   = cGeometry.GetName();
		uint32 nLength = sName.GetLength();
		if (nLength > 63)
			nLength = 63;
		MemoryManager::Copy(sGeometry.szName, sName.GetASCII(), nLength);
		sGeometry.szName[nLength] = '\0';
		sGeometry.nFlags		 = cGeometry.GetFlags();
		sGeometry.bActive		 = cGeometry.IsActive();
		sGeometry.nPrimitiveType = cGeometry.GetPrimitiveType();
		sGeometry.nMaterial		 = cGeometry.GetMaterial();
		sGeometry.nStartIndex	 = cGeometry.GetStartIndex();
		sGeometry.nIndexSize	 = cGeometry.GetIndexSize();
		if (cFile.Write(&sGeometry, 1, sizeof(sGeometry)))
			return EndChunk(cFile, cChunkStack); // End chunk
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteWeight(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, const Weight &cWeight) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_WEIGHT)) {
		// Write weight
		MeshFile::Weight sWeight;
		sWeight.nJoint = cWeight.GetJoint();
		sWeight.fBias  = cWeight.GetBias();
		if (cFile.Write(&sWeight, 1, sizeof(sWeight)))
			return EndChunk(cFile, cChunkStack); // End chunk
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteVertexWeights(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, VertexWeights &cVertexWeights) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_VERTEXWEIGHTS)) {
		// Write vertex weights
		MeshFile::VertexWeights sVertexWeights;
		sVertexWeights.nWeights = cVertexWeights.GetWeights().GetNumOfElements();
		if (cFile.Write(&sVertexWeights, 1, sizeof(sVertexWeights))) {
			// Write vertex weights
			Array<uint32> &cWeights = cVertexWeights.GetWeights();
			if (sVertexWeights.nWeights) {
				if (!cFile.Write(cWeights.GetData(), 1, sizeof(uint32)*sVertexWeights.nWeights))
					return false; // Error!
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteMorphTarget(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, MeshMorphTarget &cMorphTarget) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_MORPHTARGET)) {
		// Write morph target header
		MeshFile::MorphTarget sMorphTarget;
		String sName   = cMorphTarget.GetName();
		uint32 nLength = sName.GetLength();
		if (nLength > 63)
			nLength = 63;
		MemoryManager::Copy(sMorphTarget.szName, sName.GetASCII(), nLength);
		sMorphTarget.szName[nLength] = '\0';
		sMorphTarget.bRelative      = cMorphTarget.IsRelative();
		sMorphTarget.nVertexIDs     = cMorphTarget.GetVertexIDs().GetNumOfElements();
		sMorphTarget.nVertexBuffers = 1;
		if (cFile.Write(&sMorphTarget, 1, sizeof(sMorphTarget))) {
			// Write vertex ID's
			Array<uint32> &lstVertexIDs = cMorphTarget.GetVertexIDs();
			if (lstVertexIDs.GetNumOfElements()) {
				if (!cFile.Write(lstVertexIDs.GetData(), 1, sizeof(uint32)*lstVertexIDs.GetNumOfElements()))
					return false; // Error!
			}

			// Write vertex buffers
			for (uint32 i=0; i<1; i++) {
				if (!WriteVertexBuffer(cFile, cChunkStack, *cMorphTarget.GetVertexBuffer()))
					return false; // Error!
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteVertexBuffer(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, VertexBuffer &cVertexBuffer) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_VERTEXBUFFER)) {
		// Write vertex buffer header
		MeshFile::VertexBuffer sVertexBuffer;
		sVertexBuffer.nVertexAttributes = cVertexBuffer.GetNumOfVertexAttributes();
		sVertexBuffer.nVertices			= cVertexBuffer.GetNumOfElements();
		sVertexBuffer.nSize				= cVertexBuffer.GetSize();
		if (cFile.Write(&sVertexBuffer, 1, sizeof(sVertexBuffer))) {
			// Write vertex attributes
			for (uint32 i=0; i<cVertexBuffer.GetNumOfVertexAttributes(); i++) {
				if (!WriteVertexAttribute(cFile, cChunkStack, *cVertexBuffer.GetVertexAttribute(i)))
					return false; // Error!
			}

			// Write vertex buffer
			bool bResult = true; // No error by default
			void *pData = cVertexBuffer.Lock(Lock::ReadOnly);
			if (pData) {
				if (!cFile.Write(pData, 1, cVertexBuffer.GetSize()))
					bResult = false; // Error!

				// Unlock the vertex buffer
				cVertexBuffer.Unlock();
			} else {
				// Error!
				bResult = false;
			}

			// End chunk
			if (!EndChunk(cFile, cChunkStack))
				bResult = false; // Error!

			// Done
			return bResult;
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteVertexAttribute(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, const VertexBuffer::Attribute &cVertexAttribute) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_VERTEXATTRIBUTE)) {
		// Write vertex attribute
		MeshFile::VertexAttribute sVertexAttribute;
		sVertexAttribute.nSemantic = cVertexAttribute.nSemantic;
		sVertexAttribute.nChannel  = cVertexAttribute.nChannel;
		sVertexAttribute.nType     = cVertexAttribute.nType;
		if (cFile.Write(&sVertexAttribute, 1, sizeof(sVertexAttribute)))
			return EndChunk(cFile, cChunkStack); // End chunk
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteSkeleton(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, Skeleton &cSkeleton) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_SKELETON)) {
		// Write skeleton header
		MeshFile::Skeleton sSkeleton;
		{ // Setup skeleton header
			String sName   = cSkeleton.GetName();
			uint32 nLength = sName.GetLength();
			if (nLength > 63)
				nLength = 63;
			MemoryManager::Copy(sSkeleton.szName, sName.GetASCII(), nLength);
			sSkeleton.szName[nLength] = '\0';
			sSkeleton.nJoints = cSkeleton.GetNumOfElements();
			sSkeleton.nFrames = cSkeleton.GetFrameKeys().GetNumOfElements();
		}
		if (cFile.Write(&sSkeleton, 1, sizeof(sSkeleton))) {
			// Write joints
			for (uint32 i=0; i<sSkeleton.nJoints; i++) {
				const Joint *pJoint = cSkeleton.GetByIndex(i);
				MeshFile::Joint sJoint;
				String sName   = pJoint->GetName();
				uint32 nLength = sName.GetLength();
				if (nLength > 63)
					nLength = 63;
				MemoryManager::Copy(sJoint.szName, sName.GetASCII(), nLength);
				sJoint.szName[nLength] = '\0';
				sJoint.nParent             = pJoint->GetParent();
				sJoint.nAnimatedComponents = cSkeleton.GetJoints()[i].nAnimatedComponents;
				if (!cFile.Write(&sJoint, 1, sizeof(sJoint)))
					return false; // Error!
			}

			// Write base frame
			for (uint32 i=0; i<sSkeleton.nJoints; i++) {
				MeshFile::JointState cJointState;
				const Joint *pJoint = cSkeleton.GetByIndex(i);
				{ // Current
					const Vector3 &vV = pJoint->GetTranslation();
					cJointState.fTranslation[0] = vV.x;
					cJointState.fTranslation[1] = vV.y;
					cJointState.fTranslation[2] = vV.z;
					const Quaternion &qQ = pJoint->GetRotation();
					cJointState.fRotation[0] = qQ.w;
					cJointState.fRotation[1] = qQ.x;
					cJointState.fRotation[2] = qQ.y;
					cJointState.fRotation[3] = qQ.z;
				}
				{ // Joint space
					const Vector3 &vV = pJoint->GetTranslationJointSpace();
					cJointState.fTranslationJointSpace[0] = vV.x;
					cJointState.fTranslationJointSpace[1] = vV.y;
					cJointState.fTranslationJointSpace[2] = vV.z;
					const Quaternion &qQ = pJoint->GetRotationJointSpace();
					cJointState.fRotationJointSpace[0] = qQ.w;
					cJointState.fRotationJointSpace[1] = qQ.x;
					cJointState.fRotationJointSpace[2] = qQ.y;
					cJointState.fRotationJointSpace[3] = qQ.z;
				}
				if (!cFile.Write(&cJointState, 1, sizeof(cJointState)))
					return false; // Error!
			}

			// Write frame keys
			const Array<AnimationBase::FrameKeys> &lstFrameKeys = cSkeleton.GetFrameKeys();
			for (uint32 i=0; i<lstFrameKeys.GetNumOfElements(); i++) {
				Array<float> &lstFrameKeysT = lstFrameKeys[i].lstFrameKeys;
				if (lstFrameKeysT.GetNumOfElements()) {
					if (!cFile.Write(lstFrameKeysT.GetData(), 1, sizeof(float)*lstFrameKeysT.GetNumOfElements()))
						return false; // Error!
				}
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteAnchorPoints(Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_ANCHORPOINTS)) {
		// Write anchor points header
		MeshFile::AnchorPoints cAnchorPoints;
		cAnchorPoints.nAnchorPoints = cMesh.GetAnchorPointManager().GetNumOfElements();
		if (cFile.Write(&cAnchorPoints, 1, sizeof(cAnchorPoints))) {
			// Write anchor points
			for (uint32 i=0; i<cAnchorPoints.nAnchorPoints; i++) {
				const AnchorPoint *pAnchorPoint = cMesh.GetAnchorPointManager().GetByIndex(i);
				MeshFile::AnchorPoint sAnchorPoint;
				String sName   = pAnchorPoint->GetName();
				uint32 nLength = sName.GetLength();
				if (nLength > 63)
					nLength = 63;
				MemoryManager::Copy(sAnchorPoint.szName, sName.GetASCII(), nLength);
				sAnchorPoint.szName[nLength] = '\0';
				sAnchorPoint.bType = pAnchorPoint->GetType();
				sAnchorPoint.nID   = pAnchorPoint->GetID();
				if (!cFile.Write(&sAnchorPoint, 1, sizeof(sAnchorPoint)))
					return false; // Error!
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}

// [TODO] Update this
/*
bool MeshLoaderPL::WriteAnimations(const Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_ANIMATIONS)) {
		// Write animations header
		MeshFile::Animations cAnimations;
		cAnimations.nAnimations = cMesh.GetAnimationPool().GetNumOfElements();
		if (cFile.Write(&cAnimations, 1, sizeof(cAnimations))) {
			// Write animations
			for (uint32 i=0; i<cAnimations.nAnimations; i++) {
				const AnimationInfo *pAni = (AnimationInfo*)cMesh.GetAnimationPool().Get(i);
				// Write animation
				MeshFile::Animation sAnimation;
				strcpy(sAnimation.szName, pAni->GetName());
				sAnimation.nType   = pAni->GetType();
				sAnimation.nStart  = pAni->GetStartFrame();
				sAnimation.nEnd    = pAni->GetEndFrame();
				sAnimation.fSpeed  = pAni->GetSpeed();
				sAnimation.nFlags  = pAni->GetFlags();
				sAnimation.nEvents = pAni->GetEventPool().GetNumOfElements();
				if (!cFile.Write(&sAnimation, 1, sizeof(sAnimation)))
					return false; // Error!

				// Write animation frames
				for (uint32 j=0; j<pAni->GetNumOfFrames(); j++) {
					MeshFile::AnimationFrame sAnimationFrame;
					sAnimationFrame.m_fSpeed = pAni->GetFrameInfo(j)->GetSpeed();
					if (!cFile.Write(&sAnimationFrame, 1, sizeof(sAnimationFrame)))
						return false; // Error!
				}

				// Write animation events
				for (uint32 j=0; j<sAnimation.nEvents; j++) {
					MeshFile::AnimationEvent sAnimationEvent;
					sAnimationEvent.m_nID    = ((AnimationEvent*)pAni->GetEventPool().Get(j))->GetID();
					sAnimationEvent.m_nFrame = ((AnimationEvent*)pAni->GetEventPool().Get(j))->GetFrame();
					if (!cFile.Write(&sAnimationEvent, 1, sizeof(sAnimationEvent)))
						return false; // Error!
				}
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}
*/

bool MeshLoaderPL::WriteMorphTargetAnimation(Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack, uint32 nAnimation) const
{
	// Get the animation
	MorphTargetAni *pAni = cMesh.GetMorphTargetAnimationManager().GetByIndex(nAnimation);
	if (pAni) {
		// Start chunk
		if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_MORPHTARGETANIMATION)) {
			// Write morph target animation header
			MeshFile::MorphTargetAnimation sMorphTargetAnimation;
			{ // Setup morph target animation header
				String sName   = pAni->GetName();
				uint32 nLength = sName.GetLength();
				if (nLength > 63)
					nLength = 63;
				MemoryManager::Copy(sMorphTargetAnimation.szName, sName.GetASCII(), nLength);
				sMorphTargetAnimation.szName[nLength] = '\0';
				sMorphTargetAnimation.nMorphTargets = pAni->GetMorphTargets().GetNumOfElements();
				sMorphTargetAnimation.nFrames		= pAni->GetFrameKeys().GetNumOfElements();
			}
			if (cFile.Write(&sMorphTargetAnimation, 1, sizeof(sMorphTargetAnimation))) {
				// Write morph targets
				char szName[64];
				for (uint32 i=0; i<sMorphTargetAnimation.nMorphTargets; i++) {
					String sName   = pAni->GetMorphTargets()[i].sName;
					uint32 nLength = sName.GetLength();
					if (nLength > 63)
						nLength = 63;
					MemoryManager::Copy(szName, sName.GetASCII(), nLength);
					szName[nLength] = '\0';
					if (!cFile.Write(szName, 1, sizeof(char)*64))
						return false; // Error!
				}

				// Write frame keys
				const Array<MorphTargetAni::FrameKeys> &lstFrameKeys = pAni->GetFrameKeys();
				for (uint32 i=0; i<lstFrameKeys.GetNumOfElements(); i++) {
					Array<float> &lstFrameKeysT = lstFrameKeys[i].lstFrameKeys;
					if (lstFrameKeysT.GetNumOfElements()) {
						if (!cFile.Write(lstFrameKeysT.GetData(), 1, sizeof(float)*lstFrameKeysT.GetNumOfElements()))
							return false; // Error!
					}
				}

				// End chunk
				return EndChunk(cFile, cChunkStack);
			}
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPL::WriteMeshBoundingBox(const Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_MESHBOUNDINGBOX)) {
		// Get the mesh bounding box
		Vector3 vMin, vMax;
		cMesh.GetBoundingBox(vMin, vMax);

		// Write mesh bounding box
		MeshFile::BoundingBox sBoundingBox;
		sBoundingBox.fMin[0] = vMin.x;
		sBoundingBox.fMin[1] = vMin.y;
		sBoundingBox.fMin[2] = vMin.z;
		sBoundingBox.fMax[0] = vMax.x;
		sBoundingBox.fMax[1] = vMax.y;
		sBoundingBox.fMax[2] = vMax.z;
		if (cFile.Write(&sBoundingBox, 1, sizeof(sBoundingBox)))
			return EndChunk(cFile, cChunkStack); // End chunk
	}

	// Error!
	return false;
}

bool MeshLoaderPL::BeginChunk(File &cFile, Stack<MeshFile::Chunk> &cChunkStack, uint32 nType) const
{
	// Create a new chunk
	MeshFile::Chunk sChunk;
	sChunk.nType = nType;
	sChunk.nSize = cFile.Tell();

	// Write empty chunk
	MeshFile::Chunk sEmptyChunk = {0, 0};
	if (!cFile.Write(&sEmptyChunk, 1, sizeof(sEmptyChunk)))
		return false; // Error!

	// Push on stack
	cChunkStack.Push(sChunk);

	// Done
	return true;
}

bool MeshLoaderPL::EndChunk(File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Pop chunk from stack
	if (cChunkStack.GetNumOfElements() <= 0)
		return false; // Error!
	MeshFile::Chunk sChunk;
	cChunkStack.Pop(&sChunk);

	// Save current file position and calculate chunk size
	uint32 nChunkStart = sChunk.nSize;
	int32  nChunkEnd   = cFile.Tell();
	sChunk.nSize = nChunkEnd - nChunkStart;

	// Write chunk header
	if (cFile.Seek(nChunkStart)) {
		if (cFile.Write(&sChunk, 1, sizeof(sChunk))) {
			if (cFile.Seek(nChunkEnd))
				return true;  // Done
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
