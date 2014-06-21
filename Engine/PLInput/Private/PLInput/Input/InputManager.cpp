/*********************************************************\
 *  File: InputManager.cpp                               *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Base/Class.h>
#include <PLCore/System/MutexGuard.h>
#include <PLCore/System/CriticalSection.h>
#include "PLInput/Input/Controller.h"
#include "PLInput/Input/InputManager.h"
#include "PLInput/Input/Devices/Device.h"
#include "PLInput/Input/Controls/Control.h"
#include "PLInput/Backend/Provider.h"


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
namespace PLCore {
	template class Singleton<PLInput::InputManager>;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
InputManager *InputManager::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<InputManager>::GetInstance();
}

bool InputManager::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<InputManager>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Update input manager once per frame
*/
void InputManager::Update()
{
	// Lock data
	m_pCriticalSection->Lock();

		// Copy list of controls that have changed
		List<Control*> lstUpdatedControls = m_lstUpdatedControls;

		// Clear list
		m_lstUpdatedControls.Clear();

	// Unlock data
	m_pCriticalSection->Unlock();

	// Inform controllers about changed controls
	for (uint32 i=0; i<lstUpdatedControls.GetNumOfElements(); i++) {
		// Get control and controller
		Control    *pControl    = lstUpdatedControls[i];
		Controller *pController = pControl->GetController();

		// Inform controller
		pController->InformControl(pControl);
	}

	// Update devices
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++)
		m_lstDevices[i]->Update();
}

/**
*  @brief
*    Detect devices
*/
void InputManager::DetectDevices(bool bReset)
{
	// Lock data
	const MutexGuard cMutexGuard(*m_pCriticalSection);

	// Delete all existing providers and devices?
	if (bReset)
		Clear();

	// Send event that device detection has started
	EventOnDetectDevices(true);

	// [HACK] ProviderWindowsJoystick must be detected after InputHID, otherwise everything will be detected just as joysticks
	bool bWindowsJoystick = false;

	// Query available input providers
	List<const Class*> lstClasses;
	ClassManager::GetInstance()->GetClasses(lstClasses, "PLInput::Provider", Recursive, NoBase, NoAbstract);
	Iterator<const Class*> cIterator = lstClasses.GetIterator();
	PL_LOG(Info, "InputManager: Detecting input devices")
	while (cIterator.HasNext()) {
		// Get provider name
		String sName = cIterator.Next()->GetClassName();
		PL_LOG(Info, "InputManager: Detecting device of input provider '" + sName + '\'')

		if (sName == "PLInput::LegacyJoystickProvider")
			bWindowsJoystick = true;
		else
			DetectProvider(sName, bReset);
	}
	if (bWindowsJoystick)
		DetectProvider("PLInput::LegacyJoystickProvider", bReset);

	// Send event that device detection has stopped
	EventOnDetectDevices(false);

	// Done
	PL_LOG(Info, "InputManager: Detecting done.")
}

/**
*  @brief
*    Get list of input providers
*/
List<Provider*> &InputManager::GetProviders()
{
	// Return provider list
	return m_lstProviders;
}

/**
*  @brief
*    Get a specific input provider
*/
Provider *InputManager::GetProvider(const String &sProvider)
{
	// Get provider from hash map
	return m_mapProviders.Get(sProvider);
}

/**
*  @brief
*    Get list of devices
*/
List<Device*> &InputManager::GetDevices()
{
	// Return device list
	return m_lstDevices;
}

/**
*  @brief
*    Get a specific device
*/
Device *InputManager::GetDevice(const String &sDevice) const
{
	// Get device from hash map
	return m_mapDevices.Get(sDevice);
}

/**
*  @brief
*    Get default keyboard device
*/
Keyboard *InputManager::GetKeyboard() const
{
	return reinterpret_cast<Keyboard*>(GetDevice("Keyboard"));
}

/**
*  @brief
*    Get default mouse device
*/
Mouse *InputManager::GetMouse() const
{
	return reinterpret_cast<Mouse*>(GetDevice("Mouse"));
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
InputManager::InputManager() :
	m_pCriticalSection(new CriticalSection())
{
}

/**
*  @brief
*    Copy constructor
*/
InputManager::InputManager(const InputManager &cSource) :
	m_pCriticalSection(nullptr)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Destructor
*/
InputManager::~InputManager()
{
	// Shut down
	Clear();

	// Destroy critical section
	delete m_pCriticalSection;
}

/**
*  @brief
*    Destroy all input providers and devices
*/
void InputManager::Clear()
{
	// Destroy all input providers
	for (uint32 i=0; i<m_lstProviders.GetNumOfElements(); i++)
		delete m_lstProviders[i];
	m_lstProviders.Clear();
	m_mapProviders.Clear();

	// Destroy all left-over input devices (usually, all devices should have been destroyed by their providers)
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++)
		delete m_lstDevices[i];
	m_lstDevices.Clear();
	m_mapDevices.Clear();
}

/**
*  @brief
*    Detect devices from a specific provider
*/
void InputManager::DetectProvider(const String &sProvider, bool bReset)
{
	// Check if the provider is already present
	Provider *pProvider = GetProvider(sProvider);
	if (!pProvider) {
		// Create provider
		const Class *pClass = ClassManager::GetInstance()->GetClass(sProvider);
		if (pClass)
			pProvider = static_cast<Provider*>(pClass->Create());

		// Add provider
		if (pProvider) {
			m_lstProviders.Add(pProvider);
			m_mapProviders.Set(sProvider, pProvider);
		}
	}

	// Detect devices
	if (pProvider)
		pProvider->DetectDevices(bReset);
}

/**
*  @brief
*    Add a new input device
*/
bool InputManager::AddDevice(Device *pDevice)
{
	// Check if the device can be added
	if (pDevice && !m_mapDevices.Get(pDevice->GetName())) {
		m_lstDevices.Add(pDevice);
		m_mapDevices.Set(pDevice->GetName(), pDevice);
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Remove a device
*/
bool InputManager::RemoveDevice(Device *pDevice)
{
	// Check device
	if (pDevice && m_lstDevices.IsElement(pDevice)) {
		m_lstDevices.Remove(pDevice);
		m_mapDevices.Remove(pDevice->GetName());
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Remove control
*/
void InputManager::RemoveControl(Control *pControl)
{
	// Valid pointer?
	if (pControl) {
		// Lock data
		const MutexGuard cMutexGuard(*m_pCriticalSection);

		// Remove control from list (if it's within the list at all)
		m_lstUpdatedControls.Remove(pControl);
	}
}

/**
*  @brief
*    Update control
*/
void InputManager::UpdateControl(Control *pControl)
{
	// Valid pointer?
	if (pControl) {
		// Lock data
		const MutexGuard cMutexGuard(*m_pCriticalSection);

		// Add control to list, but only if it's not already within the list!
		if (!m_lstUpdatedControls.IsElement(pControl))
			m_lstUpdatedControls.Add(pControl);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
