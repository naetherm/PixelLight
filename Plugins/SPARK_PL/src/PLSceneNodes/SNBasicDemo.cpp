/*********************************************************\
 *  File: SNBasicDemo.cpp                                *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "SPARK_PL/RenderingAPIs/PixelLight/SPK_PLPointRenderer.h"
#include "SPARK_PL/RenderingAPIs/PixelLight/SPK_PLQuadRenderer.h"
#include "SPARK_PL/PLSceneNodes/SNBasicDemo.h"
#include <SPK.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace SPARK_PL {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNBasicDemo, "SPARK_PL", SPARK_PL::SNSystem, "Basic SPARK particle system demo scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SNBasicDemo)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNBasicDemo::SNBasicDemo() :
	m_pParticleGroup(nullptr),
	m_pParticleModel(nullptr),
	m_fStep(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNBasicDemo::~SNBasicDemo()
{
	// Cleanup
	if (m_pParticleGroup)
		delete m_pParticleGroup;
}


//[-------------------------------------------------------]
//[ Protected virtual SNSystem functions                  ]
//[-------------------------------------------------------]
void SNBasicDemo::Update()
{
	// Get the current time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Changes the color of the model over time
	m_fStep += fTimeDiff*5;
	m_pParticleModel->setParam(SPK::PARAM_RED,   0.6f + 0.4f*Math::Sin(m_fStep));
	m_pParticleModel->setParam(SPK::PARAM_GREEN, 0.6f + 0.4f*Math::Sin(m_fStep + static_cast<float>(Math::Pi*2.0f/3.0f)));
	m_pParticleModel->setParam(SPK::PARAM_BLUE,  0.6f + 0.4f*Math::Sin(m_fStep + static_cast<float>(Math::Pi*4.0f/3.0f)));
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNBasicDemo::InitFunction()
{
	// Call base implementation
	SNSystem::InitFunction();

	// Get the instance of the used renderer
	Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

	// Inits Particle Engine
	SPK::Vector3D vGravity(0.0f, -0.8f, 0.0f);

	// Renderers
	SPK_PLRenderer *pParticleRenderer = nullptr;

	// We use point sprites only if it is available and if point parameters are available as well
	if (cRenderer.GetCapabilities().bPointSprite && cRenderer.GetCapabilities().bPointParameters) {
		SPK_PLPointRenderer *pPointRenderer = CreateSPK_PLPointRenderer(cRenderer);
		pPointRenderer->setType(SPK::POINT_SPRITE);
		pPointRenderer->SetTexture(GetSceneContext()->GetRendererContext().GetTextureManager().LoadResource("Data/Textures/SPARK/Point.dds"));
		pPointRenderer->EnableWorldSize(true);
		pPointRenderer->setSize(0.05f);
		pParticleRenderer = pPointRenderer;

	// We use quads
	} else {
		SPK_PLQuadRenderer *pQuadRenderer = CreateSPK_PLQuadRenderer(cRenderer);
		pQuadRenderer->setTexturingMode(SPK::TEXTURE_2D);
		pQuadRenderer->SetTexture(GetSceneContext()->GetRendererContext().GetTextureManager().LoadResource("Data/Textures/SPARK/Point.dds"));
		pQuadRenderer->setScale(0.05f, 0.05f);
		pParticleRenderer = pQuadRenderer;
	}

	pParticleRenderer->setBlending(SPK::BLENDING_ADD);
	pParticleRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);

	// Create the SPARK particle model
	m_pParticleModel = SPK::Model::create(SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA);
	m_pParticleModel->setParam(SPK::PARAM_ALPHA, 0.8f); // Constant alpha
	m_pParticleModel->setLifeTime(8.0f, 8.0f);

	// Create the SPARK particle emitter
	SPK::SphericEmitter *pParticleEmitter = SPK::SphericEmitter::create(SPK::Vector3D(0.0f, 1.0f, 0.0f), static_cast<float>(0.1f*Math::Pi), static_cast<float>(0.1f*Math::Pi));
	pParticleEmitter->setZone(SPK::Point::create(SPK::Vector3D(0.0f, 0.015f, 0.0f)));
	pParticleEmitter->setFlow(250);
	pParticleEmitter->setForce(1.5f, 1.5f);

	// Create the SPARK particle obstacle
	SPK::Plane *pGroundPlane = SPK::Plane::create();
	SPK::Obstacle *pObstacle = SPK::Obstacle::create(pGroundPlane, SPK::INTERSECT_ZONE, 0.6f, 1.0f);

	// Create the SPARK particle group
	m_pParticleGroup = SPK::Group::create(m_pParticleModel, 2100);
	m_pParticleGroup->addEmitter(pParticleEmitter);
	m_pParticleGroup->addModifier(pObstacle);
	m_pParticleGroup->setRenderer(pParticleRenderer);
	m_pParticleGroup->setGravity(vGravity);
	m_pParticleGroup->enableAABBComputing(true);

	// Create the SPARK particle system
	m_pParticleSystem = SPK::System::create();
	m_pParticleSystem->addGroup(m_pParticleGroup);
	m_pParticleSystem->enableAABBComputing(true);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // SPARK_PL
