/*********************************************************\
 *  File: SNCamera.cpp                                   *
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
#include <PLMath/Rectangle.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Compositing/SceneRendererHandler.h"
#include "PLScene/Compositing/SceneRendererManager.h"
#include "PLScene/Scene/SNCamera.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNCamera)


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
SNCamera *SNCamera::m_pCamera = nullptr;


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
float SNCamera::GetFOV() const
{
	return m_fFOV;
}

void SNCamera::SetFOV(float fValue)
{
	if (m_fFOV != fValue) {
		m_fFOV = fValue;

		// We have to recalculate some stuff
		m_nInternalCameraFlags |= RecalculateProjectionMatrix;
		m_nInternalCameraFlags |= RecalculateFrustum;
		m_nInternalCameraFlags |= RecalculateFrustumVertices;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}

float SNCamera::GetAspect() const
{
	return m_fAspect;
}

void SNCamera::SetAspect(float fValue)
{
	if (m_fAspect != fValue) {
		m_fAspect = fValue;

		// We have to recalculate some stuff
		m_nInternalCameraFlags |= RecalculateProjectionMatrix;
		m_nInternalCameraFlags |= RecalculateFrustum;
		m_nInternalCameraFlags |= RecalculateFrustumVertices;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}

float SNCamera::GetZNear() const
{
	return m_fZNear;
}

void SNCamera::SetZNear(float fValue)
{
	if (m_fZNear != fValue) {
		m_fZNear = fValue;

		// We have to recalculate some stuff
		m_nInternalCameraFlags |= RecalculateProjectionMatrix;
		m_nInternalCameraFlags |= RecalculateFrustum;
		m_nInternalCameraFlags |= RecalculateFrustumVertices;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}

float SNCamera::GetZFar() const
{
	return m_fZFar;
}

void SNCamera::SetZFar(float fValue)
{
	if (m_fZFar != fValue) {
		m_fZFar = fValue;

		// We have to recalculate some stuff
		m_nInternalCameraFlags |= RecalculateProjectionMatrix;
		m_nInternalCameraFlags |= RecalculateFrustum;
		m_nInternalCameraFlags |= RecalculateFrustumVertices;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}

String SNCamera::GetSceneRendererFilename() const
{
	return m_sSceneRendererFilename;
}

void SNCamera::SetSceneRendererFilename(const String &sValue)
{
	// Set new scene renderer
	if (m_sSceneRendererFilename != sValue) {
		m_sSceneRendererFilename = sValue;
		m_pSceneRendererHandler->SetResource(nullptr);
	}
}

void SNCamera::SetFlags(uint32 nValue)
{
	if (GetFlags() != nValue) {
		// Call base implementation
		SceneNode::SetFlags(nValue);

		// We have to recalculate some stuff
		m_nInternalCameraFlags |= RecalculateProjectionMatrix;
		m_nInternalCameraFlags |= RecalculateFrustum;
		m_nInternalCameraFlags |= RecalculateFrustumVertices;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the current set camera
*/
SNCamera *SNCamera::GetCamera()
{
	return m_pCamera;
}

/**
*  @brief
*    Sets the current set camera
*/
void SNCamera::SetCamera(SNCamera *pCamera, Renderer *pRenderer)
{
	m_pCamera = pCamera;
	if (m_pCamera && pRenderer)
		pCamera->SetCamera(*pRenderer);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNCamera::SNCamera() :
	FOV(this),
	Aspect(this),
	ZNear(this),
	ZFar(this),
	SceneRendererFilename(this),
	Flags(this),
	DebugFlags(this),
	EventHandlerPositionRotation(&SNCamera::OnPositionRotation, this),
	m_fFOV(45.0f),
	m_fAspect(1.0f),
	m_fZNear(0.1f),
	m_fZFar(1000.0f),
	m_nInternalCameraFlags(RecalculateProjectionMatrix | RecalculateViewMatrix | RecalculateFrustum  | RecalculateFrustumVertices),
	m_bAutoUpdate(true),
	m_nViewportWidth(0),
	m_nViewportHeight(0),
	m_pSceneRendererHandler(new SceneRendererHandler())
{
	// Set the internal flag
	m_nInternalFlags |= ClassCamera;

	// Connect event handlers
	GetTransform().EventPosition.Connect(EventHandlerPositionRotation);
	GetTransform().EventRotation.Connect(EventHandlerPositionRotation);
}

/**
*  @brief
*    Destructor
*/
SNCamera::~SNCamera()
{
	// Is this the current camera?
	if (m_pCamera == this)
		m_pCamera = nullptr;

	delete m_pSceneRendererHandler;
}

/**
*  @brief
*    Returns the used scene renderer
*/
SceneRenderer *SNCamera::GetSceneRenderer() const
{
	// Get/load the scene renderer
	SceneRenderer *pSceneRenderer = m_pSceneRendererHandler->GetResource();
	if (!pSceneRenderer && m_sSceneRendererFilename.GetLength()) {
		// Get the scene context
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext) {
			pSceneRenderer = pSceneContext->GetSceneRendererManager().LoadResource(m_sSceneRendererFilename);
			m_pSceneRendererHandler->SetResource(pSceneRenderer);
		}
	}

	// Return the scene renderer
	return pSceneRenderer;
}

/**
*  @brief
*    Sets the camera
*/
bool SNCamera::SetCamera(Renderer &cRenderer, bool bSetProjection, bool bSetView)
{
	// Fixed functions
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		// Set projection matrix
		if (bSetProjection)
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, GetProjectionMatrix(cRenderer.GetViewport()));

		// Set view matrix
		if (bSetView)
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, GetViewMatrix());
	}

	// Set camera
	m_pCamera = this;

	// Done
	return true;
}

/**
*  @brief
*    Returns whether the camera projection and view matrix and the frustum are updated automatically
*/
bool SNCamera::GetAutoUpdate() const
{
	return m_bAutoUpdate;
}

/**
*  @brief
*    Sets whether the camera projection and view matrix and the frustum are updated automatically
*/
void SNCamera::SetAutoUpdate(bool bAutoUpdate)
{
	m_bAutoUpdate = bAutoUpdate;
}

/**
*  @brief
*    Returns the projection matrix
*/
Matrix4x4 &SNCamera::GetProjectionMatrix(const Rectangle &cViewport)
{
	// Get the viewport width and height
	const uint32 nViewportWidth  = static_cast<uint32>(cViewport.GetWidth());
	const uint32 nViewportHeight = static_cast<uint32>(cViewport.GetHeight());

	// Calculate projection matrix if required
	if (m_bAutoUpdate && ((m_nInternalCameraFlags & RecalculateProjectionMatrix) ||
		m_nViewportWidth != nViewportWidth || m_nViewportHeight != nViewportHeight)) {
		// Ensure that the other camera properties which depend on the viewport dimension are also updated correctly
		// -> This may lead to a "double" (in sense of "we could probably avoid one when doing more complex tests") calculation, but this in here is just an optimization and should never result in invalid settings
		m_nInternalCameraFlags |= RecalculateFrustum;
		m_nInternalCameraFlags |= RecalculateFrustumVertices;

		// Backup the viewport dimension
		m_nViewportWidth  = nViewportWidth;
		m_nViewportHeight = nViewportHeight;

		// Calculate projection matrix
		const float fAspectRadio = static_cast<float>(m_nViewportWidth)/(static_cast<float>(m_nViewportHeight)*m_fAspect);
		if (GetFlags() & NoZFar)
			m_mProj.PerspectiveFovInfinite(static_cast<float>(m_fFOV*Math::DegToRad), fAspectRadio, m_fZNear);
		else
			m_mProj.PerspectiveFov(static_cast<float>(m_fFOV*Math::DegToRad), fAspectRadio, m_fZNear, m_fZFar);

		// Recalculation done
		m_nInternalCameraFlags &= ~RecalculateProjectionMatrix;
	}

	// Return the projection matrix
	return m_mProj;
}

/**
*  @brief
*    Returns the view rotation offset
*/
const Quaternion &SNCamera::GetViewRotationOffset() const
{
	return m_qViewRotationOffset;
}

/**
*  @brief
*    Sets the view rotation offset
*/
void SNCamera::SetViewRotationOffset(const Quaternion &qViewRotationOffset)
{
	// Set the new view rotation set
	m_qViewRotationOffset = qViewRotationOffset;

	// We have to recalculate the view matrix
	m_nInternalCameraFlags |= RecalculateViewMatrix;
}

/**
*  @brief
*    Returns the view matrix
*/
Matrix3x4 &SNCamera::GetViewMatrix()
{
	// Calculate view matrix if required
	if (m_bAutoUpdate && (m_nInternalCameraFlags & RecalculateViewMatrix)) {
		// Calculate view matrix
		m_mView.View(CalculateViewRotation(), GetTransform().GetPosition());
		if (GetFlags() & FlipY) {
			Matrix3x4 mScale;
			mScale.SetScaleMatrix(1.0f, -1.0f, 1.0f);
			m_mView *= mScale;
		}

		// Recalculation done
		m_nInternalCameraFlags &= ~RecalculateViewMatrix;
	}

	// Return the view matrix
	return m_mView;
}

/**
*  @brief
*    Returns the camera frustum plane set
*/
Frustum &SNCamera::GetFrustum(const Rectangle &cViewport)
{
	// Get the viewport width and height
	const uint32 nViewportWidth  = static_cast<uint32>(cViewport.GetWidth());
	const uint32 nViewportHeight = static_cast<uint32>(cViewport.GetHeight());

	// Calculate frustum if required
	if (m_bAutoUpdate && ((m_nInternalCameraFlags & RecalculateFrustum) ||
		m_nViewportWidth != nViewportWidth || m_nViewportHeight != nViewportHeight)) {
		// Ensure that the other camera properties which depend on the viewport dimension are also updated correctly
		// -> This may lead to a "double" (in sense of "we could probably avoid one when doing more complex tests") calculation, but this in here is just an optimization and should never result in invalid settings
		m_nInternalCameraFlags |= RecalculateProjectionMatrix;
		m_nInternalCameraFlags |= RecalculateFrustumVertices;

		// Backup the viewport dimension
		m_nViewportWidth  = nViewportWidth;
		m_nViewportHeight = nViewportHeight;

		// Concatenate (multiply) the view matrix and the projection matrix
		Matrix4x4 mViewProjection = GetProjectionMatrix(cViewport);
		mViewProjection *= GetViewMatrix();

		// Calculate frustum
		m_cFrustum.CreateViewPlanes(mViewProjection, (GetFlags() & NoZFar) != 0);

		// Recalculation done
		m_nInternalCameraFlags &= ~RecalculateFrustum;
	}

	// Return the frustum
	return m_cFrustum;
}

/**
*  @brief
*    Returns the 8 camera frustum vertices
*/
const Array<Vector3> &SNCamera::GetFrustumVertices(const Rectangle &cViewport)
{
	// Get the viewport width and height
	const uint32 nViewportWidth  = static_cast<uint32>(cViewport.GetWidth());
	const uint32 nViewportHeight = static_cast<uint32>(cViewport.GetHeight());

	// Calculate frustum vertices if required
	if ((m_nInternalCameraFlags & RecalculateFrustumVertices) ||
		m_nViewportWidth != nViewportWidth || m_nViewportHeight != nViewportHeight) {
		// Ensure that the other camera properties which depend on the viewport dimension are also updated correctly
		// -> This may lead to a "double" (in sense of "we could probably avoid one when doing more complex tests") calculation, but this in here is just an optimization and should never result in invalid settings
		m_nInternalCameraFlags |= RecalculateProjectionMatrix;
		m_nInternalCameraFlags |= RecalculateFrustum;

		// Backup the viewport dimension
		m_nViewportWidth  = nViewportWidth;
		m_nViewportHeight = nViewportHeight;

		// Set unit box
		m_cFrustumVertices.Resize(8);
		m_cFrustumVertices[0].SetXYZ(-1.0f, -1.0f, -1.0f);
		m_cFrustumVertices[1].SetXYZ(-1.0f,  1.0f, -1.0f);
		m_cFrustumVertices[2].SetXYZ( 1.0f,  1.0f, -1.0f);
		m_cFrustumVertices[3].SetXYZ( 1.0f, -1.0f, -1.0f);
		m_cFrustumVertices[4].SetXYZ(-1.0f, -1.0f,  1.0f);
		m_cFrustumVertices[5].SetXYZ(-1.0f,  1.0f,  1.0f);
		m_cFrustumVertices[6].SetXYZ( 1.0f,  1.0f,  1.0f);
		m_cFrustumVertices[7].SetXYZ( 1.0f, -1.0f,  1.0f);

		// Get world transform matrix with no scale
		Matrix4x4 mWorld;
		mWorld.FromQuatTrans(CalculateViewRotation(), GetTransform().GetPosition());
		mWorld *= GetProjectionMatrix(cViewport).GetInverted();

		// Project the vertices
		for (uint8 i=0; i<m_cFrustumVertices.GetNumOfElements(); i++)
			m_cFrustumVertices[i] *= mWorld;

		// Recalculation done
		m_nInternalCameraFlags &= ~RecalculateFrustumVertices;
	}

	// Return the frustum vertices
	return m_cFrustumVertices;
}

/**
*  @brief
*    Gets the current camera viewport corners
*/
bool SNCamera::GetViewportCorners(Vector3 &vUpperRight, Vector3 &vLowerRight, Vector3 &vLowerLeft, Vector3 &vUpperLeft, bool bContainerSpace, float fDistance)
{
	// Get the viewport
	const Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();
	const uint32 nViewportWidth  = static_cast<uint32>(cRenderer.GetViewport().GetWidth());
	const uint32 nViewportHeight = static_cast<uint32>(cRenderer.GetViewport().GetHeight());

	// Get near x/y/z
	const float fAspectRadio = static_cast<float>(nViewportWidth)/(static_cast<float>(nViewportHeight)*m_fAspect);
	const float e			 = static_cast<float>(1/Math::Tan(Math::DegToRad*m_fFOV*0.5f));

	// Get viewport corners
	vUpperRight.SetXYZ( fDistance/e,  fAspectRadio*fDistance/e, -fDistance);
	vLowerRight.SetXYZ( fDistance/e, -fAspectRadio*fDistance/e, -fDistance);
	 vLowerLeft.SetXYZ(-fDistance/e, -fAspectRadio*fDistance/e, -fDistance);
	 vUpperLeft.SetXYZ(-fDistance/e,  fAspectRadio*fDistance/e, -fDistance);

	// Transform the corners into container space?
	if (bContainerSpace) {
		Matrix3x4 mWorld = GetViewMatrix();
		mWorld.Invert();
		vUpperRight = mWorld*vUpperRight;
		vLowerRight = mWorld*vLowerRight;
		vLowerLeft  = mWorld*vLowerLeft;
		vUpperLeft  = mWorld*vUpperLeft;
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node position or rotation changed
*/
void SNCamera::OnPositionRotation()
{
	// We have to recalculate some stuff
	m_nInternalCameraFlags |= RecalculateViewMatrix;
	m_nInternalCameraFlags |= RecalculateFrustum;
	m_nInternalCameraFlags |= RecalculateFrustumVertices;
}

/**
*  @brief
*    Calculates and returns the current view rotation
*/
Quaternion SNCamera::CalculateViewRotation() const
{
	// Static constant 180 degree y view rotation offset quaternion
	static const Quaternion ViewRotationOffset(-4.3711388e-008f, 0.0f, 1.0f, 0.0f);

	// Calculate view rotation with a 180 degree y offset (see SNCamera class documentation) and an user controlled view rotation offset
	return GetTransform().GetRotation()*ViewRotationOffset*m_qViewRotationOffset;
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNCamera::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawDebug(cRenderer, pVisNode);

	// Draw anything?
	if (pVisNode && (!(GetDebugFlags() & DebugNoFrustum) || (GetDebugFlags() & DebugFrustumVertices))) {
		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		static const Color4 cColor(0.8f, 1.0f, 0.8f, 1.0f);

		// Draw the camera frustum?
		if (!(GetDebugFlags() & DebugNoFrustum)) {
			// Get world transform matrix with no scale
			Matrix4x4 mWorld;
			mWorld.FromQuatTrans(CalculateViewRotation(), GetTransform().GetPosition());
			mWorld *= GetProjectionMatrix(cRenderer.GetViewport()).GetInverted();

			// Draw
			const Matrix4x4 mWorldViewProjection = pVisNode->GetViewProjectionMatrix()*mWorld;
			cRenderer.GetDrawHelpers().DrawBox(cColor, Vector3::NegativeOne, Vector3::One, mWorldViewProjection, 1.0f);
		}

		// Draw the frustum vertices? (there are ALWAYS 8 of them!)
		if ((GetDebugFlags() & DebugFrustumVertices) && pVisNode->GetParent()) {
			// Draw them - this points are within the 'container space'
			const Array<Vector3> &lstV = GetFrustumVertices(cRenderer.GetViewport());
			for (uint8 i=0; i<lstV.GetNumOfElements(); i++)
				cRenderer.GetDrawHelpers().DrawPoint(cColor, lstV[i], pVisNode->GetParent()->GetWorldViewProjectionMatrix(), 5.0f);
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNCamera::UpdateAABoundingBox()
{
	const Array<Vector3> &lstVertices = GetFrustumVertices(GetSceneContext()->GetRendererContext().GetRenderer().GetViewport());
	if (lstVertices.GetNumOfElements()) {
		const Matrix4x4 &mInvWorld = GetTransform().GetInverseMatrix();

		// Initialize
		AABoundingBox cBox(mInvWorld*lstVertices[0]);

		// Get axis aligned bounding box
		for (uint8 i=1; i<lstVertices.GetNumOfElements(); i++)
			cBox.AppendToCubicHull(mInvWorld*lstVertices[i]);

		// Set the final axis aligned bounding box
		SetAABoundingBox(cBox);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
