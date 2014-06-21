/*********************************************************\
 *  File: MeshAnimationManagerSoftware.cpp               *
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
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Animation/Animation.h>
#include "PLMesh/Skeleton.h"
#include "PLMesh/MeshHandler.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/SkeletonHandler.h"
#include "PLMesh/MeshAnimationManagerSoftware.h"


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
pl_implement_class(MeshAnimationManagerSoftware)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshAnimationManagerSoftware::MeshAnimationManagerSoftware()
{
}

/**
*  @brief
*    Destructor
*/
MeshAnimationManagerSoftware::~MeshAnimationManagerSoftware()
{
}


//[-------------------------------------------------------]
//[ Public virtual MeshAnimationManager functions         ]
//[-------------------------------------------------------]
bool MeshAnimationManagerSoftware::Apply(MeshHandler &cMeshHandler) const
{
	// Call base implementation which calculates the current skeleton handler joint states
	if (!MeshAnimationManager::Apply(cMeshHandler))
		return false; // Error!

	// Get mesh
	Mesh *pMesh = cMeshHandler.GetResource();
	if (!pMesh)
		return false; // Error, mesh handler has no mesh!

	// Lock vertex buffers
	MeshMorphTarget *pMorphTarget = pMesh->GetMorphTarget(0);
	if (!pMorphTarget)
		return false; // Error!
	VertexBuffer *pVB = pMorphTarget->GetVertexBuffer();
	if (!pVB)
		return false; // Error!
	VertexBuffer *pVBT = cMeshHandler.GetVertexBuffer();
	if (!pVBT)
		return false; // Error!
	if (pVB == pVBT)
		return false; // Can't manipulate original mesh vertex buffer!
	if (!pVB->Lock(Lock::ReadOnly))
		return false; // Error!
	if (!pVBT->Lock(Lock::ReadWrite)) {
		// Unlock the original vertex buffer
		pVB->Unlock();

		// Error!
		return false;
	}

	{ // Set base morph target
		// Get data
		float *pfVertices   = static_cast<float*>(pVB->GetData(0, VertexBuffer::Position));
		uint32 nVertexSize  = pVB->GetVertexSize();
		float *pfVerticesT  = static_cast<float*>(pVBT->GetData(0, VertexBuffer::Position));
		uint32 nVertexTSize = pVBT->GetVertexSize();

		// Loop through all vertices
		for (uint32 nVertex=0; nVertex<pVB->GetNumOfElements(); nVertex++) {
			// Set data
			pfVerticesT[0] = pfVertices[0];
			pfVerticesT[1] = pfVertices[1];
			pfVerticesT[2] = pfVertices[2];

			// Next data, please
			pfVertices = reinterpret_cast<float*>(reinterpret_cast<char*>(pfVertices)+nVertexSize);
			pfVerticesT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfVerticesT)+nVertexTSize);
		}
	}

	// Loop through all morph targets and add deltas, do NEVER add the first morph target
	// because thats the base! :)
	for (uint32 nMorphTarget=1; nMorphTarget<pMesh->GetNumOfMorphTargets(); nMorphTarget++) {
			  pMorphTarget = pMesh->GetMorphTarget(nMorphTarget);
		float fWeight      = cMeshHandler.GetMorphTargetWeights()[nMorphTarget];
		if (pMorphTarget->IsRelative() && fWeight) {
			VertexBuffer *pVBMT = pMorphTarget->GetVertexBuffer();
			if (pVBMT && pVBMT->Lock(Lock::ReadOnly)) {
				Array<uint32> &lstVertexIDs = pMorphTarget->GetVertexIDs();
				uint32         nVertexIDs   = lstVertexIDs.GetNumOfElements();
				if (nVertexIDs) {
					// Get data
					float *pfVertices  = static_cast<float*>(pVBMT->GetData(0, VertexBuffer::Position));
					uint32 nVertexSize = pVBMT->GetVertexSize();

					// Use vertex ID's to add certain deltas
					for (uint32 nVertex=0; nVertex<nVertexIDs; nVertex++) {
						// Get vertex to set
						float *pfVerticesT = static_cast<float*>(pVBT->GetData(lstVertexIDs[nVertex], VertexBuffer::Position));

						// Set data
						pfVerticesT[Vector3::X] += pfVertices[Vector3::X]*fWeight;
						pfVerticesT[Vector3::Y] += pfVertices[Vector3::Y]*fWeight;
						pfVerticesT[Vector3::Z] += pfVertices[Vector3::Z]*fWeight;

						// Next data, please
						pfVertices = reinterpret_cast<float*>(reinterpret_cast<char*>(pfVertices)+nVertexSize);
					}
				} else {
					// Get data
					float *pfVertices  = static_cast<float*>(pVBMT->GetData(0, VertexBuffer::Position));
					uint32 nVertexSize = pVBMT->GetVertexSize();
					float *pfVerticesT = static_cast<float*>(pVBT->GetData(0, VertexBuffer::Position));
					uint32 nVertexTSize = pVBT->GetVertexSize();

					// Go through ALL vertices and add deltas
					for (uint32 nVertex=0; nVertex<pVB->GetNumOfElements(); nVertex++) {
						// Set data
						pfVerticesT[Vector3::X] += pfVertices[Vector3::X]*fWeight;
						pfVerticesT[Vector3::Y] += pfVertices[Vector3::Y]*fWeight;
						pfVerticesT[Vector3::Z] += pfVertices[Vector3::Z]*fWeight;

						// Next data, please
						pfVertices = reinterpret_cast<float*>(reinterpret_cast<char*>(pfVertices)+nVertexSize);
						pfVerticesT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfVerticesT)+nVertexTSize);
					}
				}
				pVBMT->Unlock();
			}
		}
	}

	// Get skeleton handler
	const SkeletonHandler *pSH = cMeshHandler.GetSkeletonHandler();
	if (!pSH) {
		// Unlock the vertex buffers
		pVB->Unlock();
		pVBT->Unlock();

		// Done - no skeleton handler, no skinning :(
		return true;
	}

	// Apply skeleton to vertex buffer
	const Array<VertexWeights> &lstVertexWeights = pMesh->GetVertexWeights();
	if (!lstVertexWeights.GetNumOfElements()) {
		// Unlock the vertex buffers
		pVB->Unlock();
		pVBT->Unlock();

		// Error!
		return false;
	}
	const Array<Weight>		  &lstWeights		= pMesh->GetWeights();
	const Array<JointHandler> &lstJointHandlers = pSH->GetJointHandlers();

	// Get data
	float       *pfPosition   = static_cast<float*>      (pVBT->GetData(0, VertexBuffer::Position));
	const float *pfNormal     = static_cast<const float*>(pVB-> GetData(0, VertexBuffer::Normal));
	float       *pfNormalT    = static_cast<float*>      (pVBT->GetData(0, VertexBuffer::Normal));
	const float *pfTangent    = static_cast<const float*>(pVB-> GetData(0, VertexBuffer::Tangent));
	float       *pfTangentT   = static_cast<float*>      (pVBT->GetData(0, VertexBuffer::Tangent));
	const float *pfBinormal   = static_cast<const float*>(pVB-> GetData(0, VertexBuffer::Binormal));
	float       *pfBinormalT  = static_cast<float*>      (pVBT->GetData(0, VertexBuffer::Binormal));
	const uint32 nVertexSize  = pVB->GetVertexSize();
	const uint32 nVertexTSize = pVBT->GetVertexSize();

	// Loop through all vertices
	const VertexWeights *pVertexWeights = &lstVertexWeights[0];
	const uint32 nNumOfVertices = pVBT->GetNumOfElements();

/* Here's the unoptimized version, but because this can become quite fast to the 
   bottleneck, we have to optimize it!
    Vector3 vPos, vV;
	for (uint32 nVertex=0; nVertex<nNumOfVertices; nVertex++) {
		const Array<uint32> &lstVWeights = pVertexWeights->GetWeights();

	// Position
		// Get current vertex position
		vV.x = pfPosition[0];
		vV.y = pfPosition[1];
		vV.z = pfPosition[2];

		// Add weights
		vPos = 0.0f;
		const uint32 nWeights = lstVWeights.GetNumOfElements();
		for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
			const Weight       &cWeight = *lstWeights[*lstVWeights[nWeight]];
			const JointHandler &cJH     = *lstJointHandlers[cWeight.GetJoint()];
			const Matrix3x3    &mTrans  = cJH.GetMatrixTransform();
			vPos += ((mTrans*vV)+cJH.GetTranslationJointSpace())*cWeight.GetBias();
		}

		// Set new vertex position
		pfPosition[Vector3::X] = vPos.x;
		pfPosition[Vector3::Y] = vPos.y;
		pfPosition[Vector3::Z] = vPos.z;
		// Next position, please
		pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);

	// Normal
		if (pfNormal) {
			// Get current vertex normal
			vV.x = pfNormal[0];
			vV.y = pfNormal[1];
			vV.z = pfNormal[2];

			// Add weights
			vPos = 0.0f;
			for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
				const Weight       &cWeight = *lstWeights[*lstVWeights[nWeight]];
				const JointHandler &cJH     = *lstJointHandlers[cWeight.GetJoint()];
				const Matrix3x3 &mTrans     = cJH.GetMatrixTransform();
				vPos += (mTrans*vV)*cWeight.GetBias();
			}

			// Set new vertex normal
			pfNormalT[Vector3::X] = vPos.x;
			pfNormalT[Vector3::Y] = vPos.y;
			pfNormalT[Vector3::Z] = vPos.z;
			// Next normal, please
			pfNormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfNormal)+nVertexSize);
			pfNormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfNormalT)+nVertexTSize);
		}

	// Tangent
		if (pfTangent) {
			// Get current vertex tangent
			vV.x = pfTangent[0];
			vV.y = pfTangent[1];
			vV.z = pfTangent[2];

			// Add weights
			vPos = 0.0f;
			for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
				const Weight       &cWeight = *lstWeights[*lstVWeights[nWeight]];
				const JointHandler &cJH     = *lstJointHandlers[cWeight.GetJoint()];
				const Matrix3x3    &mTrans  = cJH.GetMatrixTransform();
				vPos += (mTrans*vV)*cWeight.GetBias();
			}

			// Set new vertex tangent
			pfTangentT[Vector3::X] = vPos.x;
			pfTangentT[Vector3::Y] = vPos.y;
			pfTangentT[Vector3::Z] = vPos.z;
			// Next tangent, please
			pfTangent = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfTangent)+nVertexSize);
			pfTangentT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTangentT)+nVertexTSize);
		}

	// Binormal
		if (pfBinormal) {
			// Get current vertex binormal
			vV.x = pfBinormal[0];
			vV.y = pfBinormal[1];
			vV.z = pfBinormal[2];

			// Add weights
			vPos = 0.0f;
			for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
				const Weight       &cWeight = *lstWeights[*lstVWeights[nWeight]];
				const JointHandler &cJH     = *lstJointHandlers[cWeight.GetJoint()];
				const Matrix3x3    &mTrans  = cJH.GetMatrixTransform();
				vPos += (mTrans*vV)*cWeight.GetBias();
			}

			// Set new vertex binormal
			pfBinormalT[Vector3::X] = vPos.x;
			pfBinormalT[Vector3::Y] = vPos.y;
			pfBinormalT[Vector3::Z] = vPos.z;
			// Next binormal, please
			pfBinormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfBinormal)+nVertexSize);
			pfBinormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfBinormalT)+nVertexTSize);
		}

		// Next vertex weights
		pVertexWeights++;
	}
*/

	// Optimized version for different combinations
	if (!pfNormal && !pfTangent && !pfBinormal) {
		for (uint32 nVertex=0; nVertex<nNumOfVertices; nVertex++) {
			const Array<uint32> &lstVWeights = pVertexWeights->GetWeights();
			const uint32         nWeights    = lstVWeights.GetNumOfElements();
			if (nWeights) {
				// Get current vertex position
				const float fPositionX = pfPosition[0];
				const float fPositionY = pfPosition[1];
				const float fPositionZ = pfPosition[2];

			// First weight
				{
					const Weight       &cWeight = lstWeights[lstVWeights[0]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();
					// Position
					// vPositionT = ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
					pfPosition[Vector3::X] = ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
					pfPosition[Vector3::Y] = ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
					pfPosition[Vector3::Z] = ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
				}

			// Add other weights
				for (uint32 nWeight=1; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();

					// Position
					{ // vPositionT += ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
						pfPosition[Vector3::X] += ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
						pfPosition[Vector3::Y] += ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
						pfPosition[Vector3::Z] += ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					}
				}
			}

			// Next position, please
			pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);

			// Next vertex weights
			pVertexWeights++;
		}
	} else if (pfNormal && !pfTangent && !pfBinormal) {
		for (uint32 nVertex=0; nVertex<nNumOfVertices; nVertex++) {
			const Array<uint32> &lstVWeights = pVertexWeights->GetWeights();
			const uint32         nWeights    = lstVWeights.GetNumOfElements();
			if (nWeights) {
				// Get current vertex position
				const float fPositionX = pfPosition[0];
				const float fPositionY = pfPosition[1];
				const float fPositionZ = pfPosition[2];
				// Get current vertex normal
				const float fNormalX = pfNormal[0];
				const float fNormalY = pfNormal[1];
				const float fNormalZ = pfNormal[2];

			// First weight
				{
					const Weight       &cWeight = lstWeights[lstVWeights[0]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();
					// vPositionT = ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
					pfPosition[Vector3::X] = ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
					pfPosition[Vector3::Y] = ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
					pfPosition[Vector3::Z] = ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					// vNormalT = (mMat*vNormal)*fBias;
					pfNormalT[Vector3::X] = (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
					pfNormalT[Vector3::Y] = (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
					pfNormalT[Vector3::Z] = (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
				}

			// Add other weights
				for (uint32 nWeight=1; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();

					// Position
					{ // vPositionT += ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
						pfPosition[Vector3::X] += ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
						pfPosition[Vector3::Y] += ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
						pfPosition[Vector3::Z] += ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					}
					// Normal
					{ // vNormalT += (mMat*vNormal)*fBias;
						pfNormalT[Vector3::X] += (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
						pfNormalT[Vector3::Y] += (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
						pfNormalT[Vector3::Z] += (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
					}
				}
			}

			// Next position, please
			pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);

			// Next normal, please
			pfNormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfNormal)+nVertexSize);
			pfNormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfNormalT)+nVertexTSize);

			// Next vertex weights
			pVertexWeights++;
		}
	} else if (pfNormal && pfTangent && pfBinormal) {
		for (uint32 nVertex=0; nVertex<nNumOfVertices; nVertex++) {
			const Array<uint32> &lstVWeights = pVertexWeights->GetWeights();
			const uint32         nWeights    = lstVWeights.GetNumOfElements();
			if (nWeights) {
				// Get current vertex position
				const float fPositionX = pfPosition[0];
				const float fPositionY = pfPosition[1];
				const float fPositionZ = pfPosition[2];
				// Get current vertex normal
				const float fNormalX = pfNormal[0];
				const float fNormalY = pfNormal[1];
				const float fNormalZ = pfNormal[2];
				// Get current vertex tangent
				const float fTangentX = pfTangent[0];
				const float fTangentY = pfTangent[1];
				const float fTangentZ = pfTangent[2];
				// Get current vertex binormal
				const float fBinormalX = pfBinormal[0];
				const float fBinormalY = pfBinormal[1];
				const float fBinormalZ = pfBinormal[2];

			// First weight
				{
					const Weight       &cWeight = lstWeights[lstVWeights[0]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();
					// vPositionT = ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
					pfPosition[Vector3::X] = ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
					pfPosition[Vector3::Y] = ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
					pfPosition[Vector3::Z] = ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					// vNormalT = (mMat*vNormal)*fBias;
					pfNormalT[Vector3::X] = (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
					pfNormalT[Vector3::Y] = (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
					pfNormalT[Vector3::Z] = (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
					// vTangentT = (mMat*vTangent)*fBias;
					pfTangentT[Vector3::X] = (mMat.xx*fTangentX + mMat.xy*fTangentY + mMat.xz*fTangentZ) * fBias;
					pfTangentT[Vector3::Y] = (mMat.yx*fTangentX + mMat.yy*fTangentY + mMat.yz*fTangentZ) * fBias;
					pfTangentT[Vector3::Z] = (mMat.zx*fTangentX + mMat.zy*fTangentY + mMat.zz*fTangentZ) * fBias;
					// vBinormalT = (mMat*vBinormal)*fBias;
					pfBinormalT[Vector3::X] = (mMat.xx*fBinormalX + mMat.xy*fBinormalY + mMat.xz*fBinormalZ) * fBias;
					pfBinormalT[Vector3::Y] = (mMat.yx*fBinormalX + mMat.yy*fBinormalY + mMat.yz*fBinormalZ) * fBias;
					pfBinormalT[Vector3::Z] = (mMat.zx*fBinormalX + mMat.zy*fBinormalY + mMat.zz*fBinormalZ) * fBias;
				}

			// Add other weights
				for (uint32 nWeight=1; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float        fBias    = cWeight.GetBias();

					// Position
					{ // vPositionT += ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
						pfPosition[Vector3::X] += ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
						pfPosition[Vector3::Y] += ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
						pfPosition[Vector3::Z] += ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					}
					// Normal
					{ // vNormalT += (mMat*vNormal)*fBias;
						pfNormalT[Vector3::X] += (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
						pfNormalT[Vector3::Y] += (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
						pfNormalT[Vector3::Z] += (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
					}
					{ // vTangentT += (mMat*vTangent)*fBias;
						pfTangentT[Vector3::X] += (mMat.xx*fTangentX + mMat.xy*fTangentY + mMat.xz*fTangentZ) * fBias;
						pfTangentT[Vector3::Y] += (mMat.yx*fTangentX + mMat.yy*fTangentY + mMat.yz*fTangentZ) * fBias;
						pfTangentT[Vector3::Z] += (mMat.zx*fTangentX + mMat.zy*fTangentY + mMat.zz*fTangentZ) * fBias;
					}
					{ // vBinormalT += (mMat*vBinormal)*fBias;
						pfBinormalT[Vector3::X] += (mMat.xx*fBinormalX + mMat.xy*fBinormalY + mMat.xz*fBinormalZ) * fBias;
						pfBinormalT[Vector3::Y] += (mMat.yx*fBinormalX + mMat.yy*fBinormalY + mMat.yz*fBinormalZ) * fBias;
						pfBinormalT[Vector3::Z] += (mMat.zx*fBinormalX + mMat.zy*fBinormalY + mMat.zz*fBinormalZ) * fBias;
					}
				}
			}

			// Next position, please
			pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);

			// Next normal, please
			pfNormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfNormal)+nVertexSize);
			pfNormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfNormalT)+nVertexTSize);

			// Next tangent, please
			pfTangent = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfTangent)+nVertexSize);
			pfTangentT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTangentT)+nVertexTSize);

			// Next binormal, please
			pfBinormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfBinormal)+nVertexSize);
			pfBinormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfBinormalT)+nVertexTSize);

			// Next vertex weights
			pVertexWeights++;
		}
	} else if (pfNormal && pfTangent && !pfBinormal) {
		for (uint32 nVertex=0; nVertex<nNumOfVertices; nVertex++) {
			const Array<uint32> &lstVWeights = pVertexWeights->GetWeights();
			const uint32         nWeights    = lstVWeights.GetNumOfElements();
			if (nWeights) {
				// Get current vertex position
				const float fPositionX = pfPosition[0];
				const float fPositionY = pfPosition[1];
				const float fPositionZ = pfPosition[2];
				// Get current vertex normal
				const float fNormalX = pfNormal[0];
				const float fNormalY = pfNormal[1];
				const float fNormalZ = pfNormal[2];
				// Get current vertex tangent
				const float fTangentX = pfTangent[0];
				const float fTangentY = pfTangent[1];
				const float fTangentZ = pfTangent[2];

			// First weight
				{
					const Weight       &cWeight = lstWeights[lstVWeights[0]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();
					// vPositionT = ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
					pfPosition[Vector3::X] = ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
					pfPosition[Vector3::Y] = ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
					pfPosition[Vector3::Z] = ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					// vNormalT = (mMat*vNormal)*fBias;
					pfNormalT[Vector3::X] = (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
					pfNormalT[Vector3::Y] = (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
					pfNormalT[Vector3::Z] = (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
					// vTangentT = (mMat*vTangent)*fBias;
					pfTangentT[Vector3::X] = (mMat.xx*fTangentX + mMat.xy*fTangentY + mMat.xz*fTangentZ) * fBias;
					pfTangentT[Vector3::Y] = (mMat.yx*fTangentX + mMat.yy*fTangentY + mMat.yz*fTangentZ) * fBias;
					pfTangentT[Vector3::Z] = (mMat.zx*fTangentX + mMat.zy*fTangentY + mMat.zz*fTangentZ) * fBias;
				}

			// Add other weights
				for (uint32 nWeight=1; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();

					// Position
					{ // vPositionT += ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
						pfPosition[Vector3::X] += ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
						pfPosition[Vector3::Y] += ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
						pfPosition[Vector3::Z] += ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					}
					// Normal
					{ // vNormalT += (mMat*vNormal)*fBias;
						pfNormalT[Vector3::X] += (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
						pfNormalT[Vector3::Y] += (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
						pfNormalT[Vector3::Z] += (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
					}
					{ // vTangentT += (mMat*vTangent)*fBias;
						pfTangentT[Vector3::X] += (mMat.xx*fTangentX + mMat.xy*fTangentY + mMat.xz*fTangentZ) * fBias;
						pfTangentT[Vector3::Y] += (mMat.yx*fTangentX + mMat.yy*fTangentY + mMat.yz*fTangentZ) * fBias;
						pfTangentT[Vector3::Z] += (mMat.zx*fTangentX + mMat.zy*fTangentY + mMat.zz*fTangentZ) * fBias;
					}
				}
			}

			// Next position, please
			pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);

			// Next normal, please
			pfNormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfNormal)+nVertexSize);
			pfNormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfNormalT)+nVertexTSize);

			// Next tangent, please
			pfTangent = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfTangent)+nVertexSize);
			pfTangentT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTangentT)+nVertexTSize);

			// Next vertex weights
			pVertexWeights++;
		}
	} else if (pfNormal && !pfTangent && pfBinormal) {
		for (uint32 nVertex=0; nVertex<nNumOfVertices; nVertex++) {
			const Array<uint32> &lstVWeights = pVertexWeights->GetWeights();
			const uint32         nWeights    = lstVWeights.GetNumOfElements();
			if (nWeights) {
				// Get current vertex position
				const float fPositionX = pfPosition[0];
				const float fPositionY = pfPosition[1];
				const float fPositionZ = pfPosition[2];
				// Get current vertex normal
				const float fNormalX = pfNormal[0];
				const float fNormalY = pfNormal[1];
				const float fNormalZ = pfNormal[2];
				// Get current vertex binormal
				const float fBinormalX = pfBinormal[0];
				const float fBinormalY = pfBinormal[1];
				const float fBinormalZ = pfBinormal[2];

			// First weight
				{
					const Weight       &cWeight = lstWeights[lstVWeights[0]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();
					// vPositionT = ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
					pfPosition[Vector3::X] = ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
					pfPosition[Vector3::Y] = ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
					pfPosition[Vector3::Z] = ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					// vNormalT = (mMat*vNormal)*fBias;
					pfNormalT[Vector3::X] = (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
					pfNormalT[Vector3::Y] = (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
					pfNormalT[Vector3::Z] = (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
					// vBinormalT = (mMat*vBinormal)*fBias;
					pfBinormalT[Vector3::X] = (mMat.xx*fBinormalX + mMat.xy*fBinormalY + mMat.xz*fBinormalZ) * fBias;
					pfBinormalT[Vector3::Y] = (mMat.yx*fBinormalX + mMat.yy*fBinormalY + mMat.yz*fBinormalZ) * fBias;
					pfBinormalT[Vector3::Z] = (mMat.zx*fBinormalX + mMat.zy*fBinormalY + mMat.zz*fBinormalZ) * fBias;
				}

			// Add other weights
				for (uint32 nWeight=1; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Vector3      &vJH     = cJH.GetTranslationJointSpace();
					const Matrix3x3    &mMat    = cJH.GetMatrixTransform();
					const float         fBias   = cWeight.GetBias();

					// Position
					{ // vPositionT += ((mTrans*vPosition)+cJH.GetTranslationJointSpace())*fBias;
						pfPosition[Vector3::X] += ((mMat.xx*fPositionX + mMat.xy*fPositionY + mMat.xz*fPositionZ) + vJH.x) * fBias;
						pfPosition[Vector3::Y] += ((mMat.yx*fPositionX + mMat.yy*fPositionY + mMat.yz*fPositionZ) + vJH.y) * fBias;
						pfPosition[Vector3::Z] += ((mMat.zx*fPositionX + mMat.zy*fPositionY + mMat.zz*fPositionZ) + vJH.z) * fBias;
					}
					// Normal
					{ // vNormalT += (mMat*vNormal)*fBias;
						pfNormalT[Vector3::X] += (mMat.xx*fNormalX + mMat.xy*fNormalY + mMat.xz*fNormalZ) * fBias;
						pfNormalT[Vector3::Y] += (mMat.yx*fNormalX + mMat.yy*fNormalY + mMat.yz*fNormalZ) * fBias;
						pfNormalT[Vector3::Z] += (mMat.zx*fNormalX + mMat.zy*fNormalY + mMat.zz*fNormalZ) * fBias;
					}
					{ // vBinormalT += (mMat*vBinormal)*fBias;
						pfBinormalT[Vector3::X] += (mMat.xx*fBinormalX + mMat.xy*fBinormalY + mMat.xz*fBinormalZ) * fBias;
						pfBinormalT[Vector3::Y] += (mMat.yx*fBinormalX + mMat.yy*fBinormalY + mMat.yz*fBinormalZ) * fBias;
						pfBinormalT[Vector3::Z] += (mMat.zx*fBinormalX + mMat.zy*fBinormalY + mMat.zz*fBinormalZ) * fBias;
					}
				}
			}

			// Next position, please
			pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);

			// Next normal, please
			pfNormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfNormal)+nVertexSize);
			pfNormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfNormalT)+nVertexTSize);

			// Next binormal, please
			pfBinormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfBinormal)+nVertexSize);
			pfBinormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfBinormalT)+nVertexTSize);

			// Next vertex weights
			pVertexWeights++;
		}
	} else { // Fallback for unusual combinations...
		Vector3 vPos, vV;
		for (uint32 nVertex=0; nVertex<nNumOfVertices; nVertex++) {
			const Array<uint32> &lstVWeights = pVertexWeights->GetWeights();

		// Position
			// Get current vertex position
			vV.x = pfPosition[0];
			vV.y = pfPosition[1];
			vV.z = pfPosition[2];

			// Add weights
			vPos = Vector3::Zero;
			const uint32 nWeights = lstVWeights.GetNumOfElements();
			for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
				const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
				const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
				const Matrix3x3    &mTrans  = cJH.GetMatrixTransform();
				vPos += ((mTrans*vV)+cJH.GetTranslationJointSpace())*cWeight.GetBias();
			}

			// Set new vertex position
			pfPosition[Vector3::X] = vPos.x;
			pfPosition[Vector3::Y] = vPos.y;
			pfPosition[Vector3::Z] = vPos.z;
			// Next position, please
			pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);

		// Normal
			if (pfNormal) {
				// Get current vertex normal
				vV.x = pfNormal[0];
				vV.y = pfNormal[1];
				vV.z = pfNormal[2];

				// Add weights
				vPos = Vector3::Zero;
				for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Matrix3x3    &mTrans  = cJH.GetMatrixTransform();
					vPos += (mTrans*vV)*cWeight.GetBias();
				}

				// Set new vertex normal
				pfNormalT[Vector3::X] = vPos.x;
				pfNormalT[Vector3::Y] = vPos.y;
				pfNormalT[Vector3::Z] = vPos.z;
				// Next normal, please
				pfNormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfNormal)+nVertexSize);
				pfNormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfNormalT)+nVertexTSize);
			}

		// Tangent
			if (pfTangent) {
				// Get current vertex tangent
				vV.x = pfTangent[0];
				vV.y = pfTangent[1];
				vV.z = pfTangent[2];

				// Add weights
				vPos = Vector3::Zero;
				for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Matrix3x3    &mTrans  = cJH.GetMatrixTransform();
					vPos += (mTrans*vV)*cWeight.GetBias();
				}

				// Set new vertex tangent
				pfTangentT[Vector3::X] = vPos.x;
				pfTangentT[Vector3::Y] = vPos.y;
				pfTangentT[Vector3::Z] = vPos.z;
				// Next tangent, please
				pfTangent = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfTangent)+nVertexSize);
				pfTangentT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTangentT)+nVertexTSize);
			}

		// Binormal
			if (pfBinormal) {
				// Get current vertex binormal
				vV.x = pfBinormal[0];
				vV.y = pfBinormal[1];
				vV.z = pfBinormal[2];

				// Add weights
				vPos = Vector3::Zero;
				for (uint32 nWeight=0; nWeight<nWeights; nWeight++) {
					const Weight       &cWeight = lstWeights[lstVWeights[nWeight]];
					const JointHandler &cJH     = lstJointHandlers[cWeight.GetJoint()];
					const Matrix3x3    &mTrans  = cJH.GetMatrixTransform();
					vPos += (mTrans*vV)*cWeight.GetBias();
				}

				// Set new vertex binormal
				pfBinormalT[Vector3::X] = vPos.x;
				pfBinormalT[Vector3::Y] = vPos.y;
				pfBinormalT[Vector3::Z] = vPos.z;
				// Next binormal, please
				pfBinormal = reinterpret_cast<const float*>(reinterpret_cast<const char*>(pfBinormal)+nVertexSize);
				pfBinormalT = reinterpret_cast<float*>(reinterpret_cast<char*>(pfBinormalT)+nVertexTSize);
			}

			// Next vertex weights
			pVertexWeights++;
		}
	}

	// Unlock the vertex buffers
	pVB->Unlock();
	pVBT->Unlock();

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
