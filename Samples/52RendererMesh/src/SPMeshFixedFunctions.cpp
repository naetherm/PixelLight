/*********************************************************\
 *  File: SPMeshFixedFunctions.cpp                       *
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
#include <PLCore/Tools/Timing.h>
#include <PLMath/Rectangle.h>
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectPass.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Effect/EffectTechnique.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/MeshManager.h>
#include "SPMeshFixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SPMeshFixedFunctions, "", SPMesh, "Fixed functions mesh surface painter")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLRenderer::Renderer&,	"Parameter constructor",	"")
pl_class_metadata_end(SPMeshFixedFunctions)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPMeshFixedFunctions::SPMeshFixedFunctions(Renderer &cRenderer) : SPMesh(cRenderer)
{
	// Get the renderer context
	RendererContext &cRendererContext = GetRenderer().GetRendererContext();

	// Create and set a special material & effect
	Material *pMaterial = cRendererContext.GetMaterialManager().Create("MyMesh");
	if (pMaterial) {
		Effect *pFX = cRendererContext.GetEffectManager().Create("MyMesh");
		if (pFX) {
			EffectTechnique *pTechnique = pFX->AddTechnique();
			if (pTechnique) {
				pFX->SelectTechnique(0);
				EffectPass *pEffectPass = pTechnique->AddPass();
				if (pEffectPass)
					pEffectPass->GetFixedFunctionsRenderStates().SetLighting(true);
			}
			pMaterial->SetEffect(pFX);
		}
		m_pMeshHandler->SetMaterial(0, pMaterial);
	}
}

/**
*  @brief
*    Destructor
*/
SPMeshFixedFunctions::~SPMeshFixedFunctions()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::SurfacePainter functions  ]
//[-------------------------------------------------------]
void SPMeshFixedFunctions::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Gray);

	// Fixed functions support required
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		{ // Setup light
			FixedFunctions::Light sLight;
			pFixedFunctions->GetDefaultLightSettings(sLight);
			sLight.cAmbient.SetRGBA(0.1f, 0.1f, 0.1f, 1.0f);
			sLight.cDiffuse.SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);
			sLight.nType = FixedFunctions::LightType::Point;
			sLight.vPosition.SetXYZ(0.0f, 0.0f, 1.5f);
			sLight.fLinearAttenuation = 0.3f;
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);
			pFixedFunctions->SetLightEnabled(0, false);
			pFixedFunctions->SetLight		(0, sLight);
			pFixedFunctions->SetLightEnabled(0, true);
		}

		{ // Set the world matrix
			// Build a rotation matrix by using a given Euler angle around the y-axis
			Matrix4x4 mWorld;
			mWorld.FromEulerAngleY(static_cast<float>(m_fRotation*Math::DegToRad));
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);

			// Increase the rotation by the current time difference (time past since the last frame)
			// -> Normally, such work should NOT be performed within the rendering step, but we want
			//    to keep the implementation simple in here...
			m_fRotation += Timing::GetInstance()->GetTimeDifference()*50;
		}

		{ // Set the view matrix
			Matrix4x4 mView;
			mView.SetTranslation(0.0f, -0.1f, -0.5f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, mView);
		}

		{ // Set the projection matrix
			Matrix4x4 mProj;
			const float fAspect      = 1.0f;
			const float fAspectRadio = cRenderer.GetViewport().GetWidth()/(cRenderer.GetViewport().GetHeight()*fAspect);
			mProj.PerspectiveFov(static_cast<float>(45.0f*Math::DegToRad), fAspectRadio, 0.001f, 1000.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProj);
		}

		// Draw the mesh handlers mesh
		m_pMeshHandler->Draw();
	}
}
