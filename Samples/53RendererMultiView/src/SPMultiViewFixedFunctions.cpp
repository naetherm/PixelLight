/*********************************************************\
 *  File: SPMultiViewFixedFunctions.cpp                  *
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
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectPass.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Effect/EffectTechnique.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/MeshManager.h>
#include "SPMultiViewFixedFunctions.h"


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
pl_class_metadata(SPMultiViewFixedFunctions, "", SPMultiView, "Fixed functions multi view surface painter")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLRenderer::Renderer&,	"Parameter constructor",	"")
pl_class_metadata_end(SPMultiViewFixedFunctions)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPMultiViewFixedFunctions::SPMultiViewFixedFunctions(Renderer &cRenderer) : SPMultiView(cRenderer)
{
	// Get the renderer context
	RendererContext &cRendererContext = GetRenderer().GetRendererContext();

	// Create and set a special material & effect
	Material *pMaterial = cRendererContext.GetMaterialManager().Create("MyTeapot");
	if (pMaterial) {
		Effect *pFX = cRendererContext.GetEffectManager().Create("MyTeapot");
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
		m_pMeshHandlerSphere->SetMaterial(0, pMaterial);
		m_pMeshHandlerTorus ->SetMaterial(0, pMaterial);
		m_pMeshHandlerCube  ->SetMaterial(0, pMaterial);
	}
}

/**
*  @brief
*    Destructor
*/
SPMultiViewFixedFunctions::~SPMultiViewFixedFunctions()
{
}


//[-------------------------------------------------------]
//[ Private virtual SPMultiView functions                 ]
//[-------------------------------------------------------]
void SPMultiViewFixedFunctions::DrawScene(uint32 nScene)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

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
		}

		{ // Set the view matrix
			Matrix4x4 mView;
			mView.SetTranslation(0.0f, 0.0f, -10.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, mView);
		}

		{ // Set the projection matrix
			Matrix4x4 mProj;
			const float fAspect      = 1.0f;
			const float fAspectRadio = cRenderer.GetViewport().GetWidth()/(cRenderer.GetViewport().GetHeight()*fAspect);
			mProj.PerspectiveFov(static_cast<float>(45.0f*Math::DegToRad), fAspectRadio, 0.001f, 1000.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProj);
		}

		// Draw the mesh handler representing the scene to draw
		switch (nScene) {
			case 0:
				m_pMeshHandlerSphere->Draw();
				break;

			case 1:
				m_pMeshHandlerTorus->Draw();
				break;

			case 2:
				m_pMeshHandlerCube->Draw();
				break;
		}
	}
}
