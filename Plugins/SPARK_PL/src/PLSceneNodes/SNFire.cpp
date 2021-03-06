/*********************************************************\
 *  File: SNFire.cpp                                     *
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
#include "SPARK_PL/RenderingAPIs/PixelLight/SPK_PLQuadRenderer.h"
#include "SPARK_PL/PLSceneNodes/SNFire.h"
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
pl_class_metadata(SNFire, "SPARK_PL", SPARK_PL::SNSystem, "Fire SPARK particle system scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SNFire)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNFire::SNFire()
{
}

/**
*  @brief
*    Destructor
*/
SNFire::~SNFire()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNFire::InitFunction()
{
	// Call base implementation
	SNSystem::InitFunction();

	// Get the instance of the used renderer
	Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

	// Inits Particle Engine
	// Renderers
	SPK_PLQuadRenderer *pFireRenderer = CreateSPK_PLQuadRenderer(cRenderer);
	pFireRenderer->setScale(0.3f,0.3f);
	pFireRenderer->setTexturingMode(SPK::TEXTURE_2D);
	pFireRenderer->SetTexture(GetSceneContext()->GetRendererContext().GetTextureManager().LoadResource("Data/Textures/SPARK/Fire2.dds"));
	pFireRenderer->setBlending(SPK::BLENDING_ADD);
	pFireRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
	pFireRenderer->setAtlasDimensions(2, 2);

	SPK_PLQuadRenderer *pSmokeRenderer = CreateSPK_PLQuadRenderer(cRenderer);
	pSmokeRenderer->setScale(0.3f, 0.3f);
	pSmokeRenderer->setTexturingMode(SPK::TEXTURE_2D);
	pSmokeRenderer->SetTexture(GetSceneContext()->GetRendererContext().GetTextureManager().LoadResource("Data/Textures/SPARK/Explosion.dds"));
	pSmokeRenderer->setBlending(SPK::BLENDING_ALPHA);
	pSmokeRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
	pSmokeRenderer->setAtlasDimensions(2, 2);

	// Create the SPARK particle models
	SPK::Model *pFireModel = SPK::Model::create(SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA | SPK::FLAG_SIZE | SPK::FLAG_ANGLE | SPK::FLAG_TEXTURE_INDEX,
		SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_ALPHA | SPK::FLAG_ANGLE,
		SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_TEXTURE_INDEX | SPK::FLAG_ANGLE,
		SPK::FLAG_SIZE);
	pFireModel->setParam(SPK::PARAM_RED, 0.8f, 0.9f, 0.8f, 0.9f);
	pFireModel->setParam(SPK::PARAM_GREEN, 0.5f, 0.6f, 0.5f, 0.6f);
	pFireModel->setParam(SPK::PARAM_BLUE, 0.3f);
	pFireModel->setParam(SPK::PARAM_ALPHA, 0.4f, 0.0f);
	pFireModel->setParam(SPK::PARAM_ANGLE, 0.0f, static_cast<float>(2.0f*Math::Pi), 0.0f, static_cast<float>(2.0f*Math::Pi));
	pFireModel->setParam(SPK::PARAM_TEXTURE_INDEX, 0.0f, 4.0f);
	pFireModel->setLifeTime(1.0f, 1.5f);

	SPK::Interpolator *pInterpolator = pFireModel->getInterpolator(SPK::PARAM_SIZE);
	pInterpolator->addEntry(0.5f, 2.0f, 5.0f);
	pInterpolator->addEntry(1.0f, 0.0f);

	SPK::Model *pSmokeModel = SPK::Model::create(SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA | SPK::FLAG_SIZE | SPK::FLAG_ANGLE | SPK::FLAG_TEXTURE_INDEX,
		SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_SIZE | SPK::FLAG_ANGLE,
		SPK::FLAG_TEXTURE_INDEX | SPK::FLAG_ANGLE,
		SPK::FLAG_ALPHA);
	pSmokeModel->setParam(SPK::PARAM_RED, 0.3f, 0.2f);
	pSmokeModel->setParam(SPK::PARAM_GREEN, 0.25f, 0.2f);
	pSmokeModel->setParam(SPK::PARAM_BLUE, 0.2f);
	pSmokeModel->setParam(SPK::PARAM_ALPHA, 0.2f, 0.0f);
	pSmokeModel->setParam(SPK::PARAM_SIZE, 5.0, 10.0f);
	pSmokeModel->setParam(SPK::PARAM_TEXTURE_INDEX, 0.0f, 4.0f);
	pSmokeModel->setParam(SPK::PARAM_ANGLE, 0.0f, static_cast<float>(2.0f*Math::Pi), 0.0f, static_cast<float>(2.0f*Math::Pi));
	pSmokeModel->setLifeTime(5.0f, 5.0f);

	pInterpolator = pSmokeModel->getInterpolator(SPK::PARAM_ALPHA);
	pInterpolator->addEntry(0.0f, 0.0f);
	pInterpolator->addEntry(0.2f, 0.2f);
	pInterpolator->addEntry(1.0f, 0.0f);

	// Create the SPARK particle emitters
	// The emitters are arranged so that the fire looks realistic
	SPK::StraightEmitter *pFireEmitter1 = SPK::StraightEmitter::create(SPK::Vector3D(0.0f, 1.0f, 0.0f));
	pFireEmitter1->setZone(SPK::Sphere::create(SPK::Vector3D(0.0f, -1.0f, 0.0f), 0.5f));
	pFireEmitter1->setFlow(40);
	pFireEmitter1->setForce(1.0f, 2.5f);

	SPK::StraightEmitter *pFireEmitter2 = SPK::StraightEmitter::create(SPK::Vector3D(1.0f, 0.6f, 0.0f));
	pFireEmitter2->setZone(SPK::Sphere::create(SPK::Vector3D(0.15f, -1.2f, 0.075f), 0.1f));
	pFireEmitter2->setFlow(15);
	pFireEmitter2->setForce(0.5f, 1.5f);

	SPK::StraightEmitter *pFireEmitter3 = SPK::StraightEmitter::create(SPK::Vector3D(-0.6f, 0.8f, -0.8f));
	pFireEmitter3->setZone(SPK::Sphere::create(SPK::Vector3D(-0.375f, -1.15f, -0.375f), 0.3f));
	pFireEmitter3->setFlow(15);
	pFireEmitter3->setForce(0.5f, 1.5f);

	SPK::StraightEmitter *pFireEmitter4 = SPK::StraightEmitter::create(SPK::Vector3D(-0.8f, 0.5f, 0.2f));
	pFireEmitter4->setZone(SPK::Sphere::create(SPK::Vector3D(-0.255f, -1.2f, 0.225f), 0.2f));
	pFireEmitter4->setFlow(10);
	pFireEmitter4->setForce(0.5f, 1.5f);

	SPK::StraightEmitter *pFireEmitter5 = SPK::StraightEmitter::create(SPK::Vector3D(0.1f, 0.8f, -1.0f));
	pFireEmitter5->setZone(SPK::Sphere::create(SPK::Vector3D(-0.075f, -1.2f, -0.3f), 0.2f));
	pFireEmitter5->setFlow(10);
	pFireEmitter5->setForce(0.5f, 1.5f);

	SPK::Emitter *smokeEmitter = SPK::SphericEmitter::create(SPK::Vector3D(0.0f, 1.0f, 0.0f), 0.0f, static_cast<float>(0.5f*Math::Pi));
	smokeEmitter->setZone(SPK::Sphere::create(SPK::Vector3D(), 1.2f));
	smokeEmitter->setFlow(25);
	smokeEmitter->setForce(0.5f, 1.0f);

	// Create the SPARK particle groups
	SPK::Group *pFireGroup = SPK::Group::create(pFireModel, 135);
	pFireGroup->addEmitter(pFireEmitter1);
	pFireGroup->addEmitter(pFireEmitter2);
	pFireGroup->addEmitter(pFireEmitter3);
	pFireGroup->addEmitter(pFireEmitter4);
	pFireGroup->addEmitter(pFireEmitter5);
	pFireGroup->setRenderer(pFireRenderer);
	pFireGroup->setGravity(SPK::Vector3D(0.0f, 3.0f, 0.0f));
	pFireGroup->enableAABBComputing(true);

	SPK::Group *pSmokeGroup = SPK::Group::create(pSmokeModel, 135);
	pSmokeGroup->addEmitter(smokeEmitter);
	pSmokeGroup->setRenderer(pSmokeRenderer);
	pSmokeGroup->setGravity(SPK::Vector3D(0.0f, 0.4f, 0.0f));
	pSmokeGroup->enableAABBComputing(true);

	// Create the SPARK particle system
	m_pParticleSystem = SPK::System::create();
	m_pParticleSystem->addGroup(pSmokeGroup);
	m_pParticleSystem->addGroup(pFireGroup);
	m_pParticleSystem->enableAABBComputing(true);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // SPARK_PL
