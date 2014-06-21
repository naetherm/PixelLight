/*********************************************************\
 *  File: SCPhysicsWorld.cpp                             *
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
#include <PLCore/Base/Class.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLPhysics/World.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SCPhysicsWorld)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
bool SCPhysicsWorld::GetSimulationActive() const
{
	return m_bSimulationActive;
}

void SCPhysicsWorld::SetSimulationActive(bool bValue)
{
	if (m_bSimulationActive != bValue) {
		m_bSimulationActive = bValue;
		if (m_pWorld)
			m_pWorld->SetSimulationActive(m_bSimulationActive);
	}
}

float SCPhysicsWorld::GetSimulationSpeed() const
{
	return m_fSimulationSpeed;
}

void SCPhysicsWorld::SetSimulationSpeed(float fValue)
{
	if (m_fSimulationSpeed != fValue) {
		m_fSimulationSpeed = fValue;
		if (m_pWorld)
			m_pWorld->SetSimulationSpeed(m_fSimulationSpeed);
	}
}

float SCPhysicsWorld::GetSimulationQuality() const
{
	return m_fSimulationQuality;
}

void SCPhysicsWorld::SetSimulationQuality(float fValue)
{
	if (m_fSimulationQuality != fValue) {
		m_fSimulationQuality = fValue;
		if (m_pWorld)
			m_pWorld->SetSimulationQuality(m_fSimulationQuality);
	}
}

float SCPhysicsWorld::GetFrameRate() const
{
	return m_fFrameRate;
}

void SCPhysicsWorld::SetFrameRate(float fValue)
{
	if (m_fFrameRate != fValue) {
		m_fFrameRate = fValue;
		if (m_pWorld)
			m_pWorld->SetFrameRate(m_fFrameRate);
	}
}

// [TODO] New RTTI usage
		// ThreadPriorityClass, ThreadPriority
//		} else if (sVar == "ThreadPriorityClass" || sVar == "ThreadPriority") {
//			m_pWorld->SetAttribute(sVar, GetVar(sVar));

const Vector3 &SCPhysicsWorld::GetGravity() const
{
	return m_vGravity;
}

void SCPhysicsWorld::SetGravity(const Vector3 &vValue)
{
	if (m_vGravity != vValue) {
		m_vGravity = vValue;
		if (m_pWorld)
			m_pWorld->SetGravity(m_vGravity);
	}
}

bool SCPhysicsWorld::GetBuoyancyActive() const
{
	return m_bBuoyancyActive;
}

void SCPhysicsWorld::SetBuoyancyActive(bool bValue)
{
	if (m_bBuoyancyActive != bValue) {
		m_bBuoyancyActive = bValue;
		if (m_pWorld)
			m_pWorld->SetBuoyancyActive(m_bBuoyancyActive);
	}
}

float SCPhysicsWorld::GetBuoyancyPlaneY() const
{
	return m_fBuoyancyPlaneY;
}

void SCPhysicsWorld::SetBuoyancyPlaneY(float fValue)
{
	if (m_fBuoyancyPlaneY != fValue) {
		m_fBuoyancyPlaneY = fValue;
		if (m_pWorld)
			m_pWorld->SetBuoyancyPlaneY(m_fBuoyancyPlaneY);
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SCPhysicsWorld::SCPhysicsWorld() :
	PhysicsAPI(this),
	SimulationActive(this),
	SimulationSpeed(this),
	SimulationQuality(this),
	FrameRate(this),
	CacheDirectory(this),
	Gravity(this),
	BuoyancyActive(this),
	BuoyancyPlaneY(this),
	EventHandlerUpdate(&SCPhysicsWorld::OnUpdate, this),
	EventHandlerAABoundingBox(&SCPhysicsWorld::OnAABoundingBox, this),
	m_bSimulationActive(true),
	m_fSimulationSpeed(1.0f),
	m_fSimulationQuality(1.0f),
	m_fFrameRate(60.0f),
	m_nThreadPriorityClass(6), // None
	m_nThreadPriority(3),
	m_vGravity(0.0f, -9.81f, 0.0f),
	m_bBuoyancyActive(false),
	m_fBuoyancyPlaneY(0.0f),
	m_pWorld(nullptr)
{
	// Connect event handler
	SignalAABoundingBox.Connect(EventHandlerAABoundingBox);
}

/**
*  @brief
*    Destructor
*/
SCPhysicsWorld::~SCPhysicsWorld()
{
	// Deactivate the physics simulation
	if (m_pWorld && m_pWorld->IsSimulationActive())
		m_pWorld->SetSimulationActive(false);

	// De-initialize the scene container
	DeInit();

	// Destroy the PL physics world
	if (m_pWorld)
		delete m_pWorld;
}

/**
*  @brief
*    Returns the PL physics world
*/
World *SCPhysicsWorld::GetWorld() const
{
	return m_pWorld;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SCPhysicsWorld::OnUpdate()
{
	// Update the PL physics simulation
	if (m_pWorld && m_bSimulationActive)
		m_pWorld->UpdateSimulation();
}

/**
*  @brief
*    Called when the scene node axis aligned bounding box changed
*/
void SCPhysicsWorld::OnAABoundingBox()
{
	// Use the scene container bounding box for the world size
	if (m_pWorld)
		m_pWorld->SetWorldSize(GetAABoundingBox().vMin, GetAABoundingBox().vMax);
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNode functions        ]
//[-------------------------------------------------------]
void SCPhysicsWorld::InitFunction()
{
	// Call base implementation
	SceneContainer::InitFunction();

	// Is the world already created?
	if (!m_pWorld) {
		// Create the PL physics world
		const Class *pClass = ClassManager::GetInstance()->GetClass(PhysicsAPI.Get());
		if (pClass && pClass->IsDerivedFrom("PLPhysics::World")) {
			Object *pObject = pClass->Create();
			if (pObject) {
				m_pWorld = static_cast<World*>(pObject);

				// Use the scene container bounding box for the world size
				m_pWorld->SetWorldSize(GetAABoundingBox().vMin, GetAABoundingBox().vMax);

				// Setup the world
				m_pWorld->SetAttribute("CacheDirectory",      GetAttribute("CacheDirectory"));
				m_pWorld->SetAttribute("ThreadPriorityClass", GetAttribute("ThreadPriorityClass"));
				m_pWorld->SetAttribute("ThreadPriority",      GetAttribute("ThreadPriority"));
				m_pWorld->SetSimulationActive(m_bSimulationActive);
				m_pWorld->SetSimulationSpeed(m_fSimulationSpeed);
				m_pWorld->SetSimulationQuality(m_fSimulationQuality);
				m_pWorld->SetFrameRate(m_fFrameRate);
				m_pWorld->SetGravity(m_vGravity);
				m_pWorld->SetBuoyancyActive(m_bBuoyancyActive);
				m_pWorld->SetBuoyancyPlaneY(m_fBuoyancyPlaneY);
			}
		}
	}
}

void SCPhysicsWorld::DeInitFunction()
{
	// Deactivate the physics simulation if required
	bool bSimulationActive = false;
	if (m_pWorld) {
		bSimulationActive = m_pWorld->IsSimulationActive();
		if (bSimulationActive)
			m_pWorld->SetSimulationActive(false);
	}

	// Call base implementation
	SceneContainer::DeInitFunction();

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		m_pWorld->SetSimulationActive(bSimulationActive);
}

void SCPhysicsWorld::OnActivate(bool bActivate)
{
	// Call the base implementation
	SceneContainer::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization and logic update)
			OnUpdate();
		} else {
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool SCPhysicsWorld::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Deactivate the physics simulation if required (else the loading may be terrible slow :)
	bool bSimulationActive = false;
	if (m_pWorld) {
		bSimulationActive = m_pWorld->IsSimulationActive();
		if (bSimulationActive)
			m_pWorld->SetSimulationActive(false);
	}

	// Load
	const bool bResult = SceneContainer::LoadByFilename(sFilename, sParams, sMethod);

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		m_pWorld->SetSimulationActive(bSimulationActive);

	// Done
	return bResult;
}

bool SCPhysicsWorld::LoadByFile(File &cFile, const String &sParams, const String &sMethod)
{
	// Deactivate the physics simulation if required (else the loading may be terrible slow :)
	bool bSimulationActive = false;
	if (m_pWorld) {
		bSimulationActive = m_pWorld->IsSimulationActive();
		if (bSimulationActive)
			m_pWorld->SetSimulationActive(false);
	}

	// Load
	const bool bResult = SceneContainer::LoadByFile(cFile, sParams, sMethod);

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		m_pWorld->SetSimulationActive(bSimulationActive);

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
