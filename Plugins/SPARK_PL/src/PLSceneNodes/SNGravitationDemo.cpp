/*********************************************************\
 *  File: SNGravitationDemo.cpp                          *
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
#include <PLScene/Scene/SceneContext.h>
#include "SPARK_PL/RenderingAPIs/PixelLight/SPK_PLLineTrailRenderer.h"
#include "SPARK_PL/PLSceneNodes/SNGravitationDemo.h"
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
pl_class_metadata(SNGravitationDemo, "SPARK_PL", SPARK_PL::SNSystem, "Gravitation SPARK particle system demo scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SNGravitationDemo)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNGravitationDemo::SNGravitationDemo() :
	m_pParticleGroup(nullptr),
	m_pMassGroup(nullptr),
	m_pParticleModel(nullptr),
	m_fStep(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNGravitationDemo::~SNGravitationDemo()
{
	// Cleanup
	if (m_pParticleGroup)
		delete m_pParticleGroup;
	if (m_pMassGroup)
		delete m_pMassGroup;
}


//[-------------------------------------------------------]
//[ Protected virtual SNSystem functions                  ]
//[-------------------------------------------------------]
void SNGravitationDemo::Update()
{
	// Get the current time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Changes the color of the model over time
	m_fStep += fTimeDiff*2;
	const SPK::Vector3D vColor(0.6f + 0.4f*Math::Sin(m_fStep), 0.6f + 0.4f*Math::Sin(m_fStep + static_cast<float>(Math::Pi*2.0f/3.0f)), 0.6f + 0.4f*Math::Sin(m_fStep + static_cast<float>(Math::Pi*4.0f/3.0f)));
	m_pParticleModel->setParam(SPK::PARAM_RED,   Math::Max(0.0f, vColor.x - 0.25f), Math::Min(1.0f, vColor.x + 0.25f));
	m_pParticleModel->setParam(SPK::PARAM_GREEN, Math::Max(0.0f, vColor.y - 0.25f), Math::Min(1.0f, vColor.y + 0.25f));
	m_pParticleModel->setParam(SPK::PARAM_BLUE,  Math::Max(0.0f, vColor.z - 0.25f), Math::Min(1.0f, vColor.z + 0.25f));

	if (m_pMassGroup) {
		// If the number of particles in the mass group is 0 (at init or when the system is reinitialized),
		// a number of particles equal to NumOfPointMasses is added
		if (!m_pMassGroup->getNbParticles()) {
			for (uint32 i=0; i<NumOfPointMasses; i++)
				m_pMassGroup->addParticles(1, SPK::Vector3D(2.0f, 0.0f, 0.0f), SPK::Vector3D(Math::GetRandMinMaxFloat(-1.0f, 0.0f), Math::GetRandMinMaxFloat(-0.5f, 0.5f), Math::GetRandMinMaxFloat(-0.5f, 0.5f)));
			m_pMassGroup->flushAddedParticles(); // to ensure particles are added
		}
	
		// The point masses are attached one by one to the particles in the m_pMassGroup so that they follow their moves
		for (uint32 i=0; i<NumOfPointMasses; i++)
			m_pPointMasses[i]->setPosition(m_pMassGroup->getParticle(i).position());
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNGravitationDemo::InitFunction()
{
	// Call base implementation
	SNSystem::InitFunction();

	// Get the instance of the used renderer
	Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

	// Renderers
	static const uint32 NumberOfSegments = 32;
	SPARK_PL::SPK_PLLineTrailRenderer *pSPK_PLLineTrailRenderer = CreateSPK_PLLineTrailRenderer(cRenderer);
	pSPK_PLLineTrailRenderer->setBlending(SPK::BLENDING_ADD);
	pSPK_PLLineTrailRenderer->SetDuration(2.0f);
	pSPK_PLLineTrailRenderer->SetNumOfSamples(NumberOfSegments);
	pSPK_PLLineTrailRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);

	// Create the SPARK particle models
	SPK::Model *pMassModel = SPK::Model::create();
	pMassModel->setImmortal(true);

	m_pParticleModel = SPK::Model::create(SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA,
		SPK::FLAG_ALPHA, SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE);
	m_pParticleModel->setParam(SPK::PARAM_ALPHA, 0.1f, 0.0f);
	m_pParticleModel->setLifeTime(5.0f, 8.0f);

	// Create the SPARK particle emitter
	SPK::RandomEmitter *pParticleEmitter = SPK::RandomEmitter::create();
	pParticleEmitter->setFlow(400);
	pParticleEmitter->setForce(0.0f, 0.1f);

	// This is the point mass that will attract the other point masses to make them move
	SPK::PointMass *pCenterPointMass = SPK::PointMass::create();
	pCenterPointMass->setMass(0.6f);
	pCenterPointMass->setMinDistance(0.01f);

	// Create the SPARK particle groups
	m_pParticleGroup = SPK::Group::create(m_pParticleModel, 4100);
	m_pParticleGroup->addEmitter(pParticleEmitter);
	m_pParticleGroup->setRenderer(pSPK_PLLineTrailRenderer);
	m_pParticleGroup->enableAABBComputing(true);

	m_pMassGroup = SPK::Group::create(pMassModel, NumOfPointMasses);
	m_pMassGroup->addModifier(pCenterPointMass);
	m_pMassGroup->setRenderer(nullptr);
	m_pMassGroup->enableAABBComputing(true);

	// Creates the point masses that will attract the particles
	for (uint32 i=0; i<NumOfPointMasses; i++) {
		m_pPointMasses[i] = SPK::PointMass::create();
		m_pPointMasses[i]->setMass(3.0f);
		m_pPointMasses[i]->setMinDistance(0.05f);
		m_pParticleGroup->addModifier(m_pPointMasses[i]);
	}

	// Create the SPARK particle system
	m_pParticleSystem = SPK::System::create();
	m_pParticleSystem->addGroup(m_pMassGroup);
	m_pParticleSystem->addGroup(m_pParticleGroup);
	m_pParticleSystem->enableAABBComputing(true);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // SPARK_PL
