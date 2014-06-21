/*********************************************************\
 *  File: InputManager.h                                 *
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


#ifndef __PLINPUT_INPUTMANAGER_H__
#define __PLINPUT_INPUTMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/Singleton.h>
#include <PLCore/Base/Event/Event.h>
#include <PLCore/Container/List.h>
#include <PLCore/Container/HashMap.h>
#include "PLInput/PLInput.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Thread;
	class CriticalSection;
}
namespace PLInput {
	class Provider;
	class Device;
	class Keyboard;
	class Mouse;
	class Control;
	class Controller;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input manager
*
*  @remarks
*    The input manager stores all available devices that are present on the computer and
*    controls the update of input messages.
*/
class InputManager : public PLCore::Singleton<InputManager> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class PLCore::Singleton<InputManager>;
	friend class Provider;
	friend class Control;


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<bool>							EventOnDetectDevices;		/**< Called when device detection has started or stopped */
		PLCore::Event<Controller*, PLCore::String>	EventInputControllerFound;	/**< An input controller has been found. Use this event to for instance connect the input controller to real input devices. Found input controller as first parameter, input semantic as second parameter. */


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		PLINPUT_API static InputManager *GetInstance();
		PLINPUT_API static bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Update input manager once per frame
		*
		*  @remarks
		*    This function must be called once per frame to allow devices to update their status
		*    and to process input messages read from these devices. This is also done to make sure
		*    that input messages are processed synchronously in the main thread, rather than sending
		*    messages from other threads asynchronously.
		*/
		PLINPUT_API void Update();

		/**
		*  @brief
		*    Detect devices
		*
		*  @param[in] bReset
		*    If 'true', delete all input devices and re-detect them all. Otherwise,
		*    only new and removed input devices will be detected.
		*
		*  @remarks
		*    bReset = true should only be used if really necessary, because existing
		*    input handlers will most certainly lose their connection to the device.
		*/
		PLINPUT_API void DetectDevices(bool bReset = false);

		/**
		*  @brief
		*    Get list of input providers
		*
		*  @return
		*    Provider list, do not destroy the returned instances!
		*/
		PLINPUT_API const PLCore::List<Provider*> &GetProviders() const;

		/**
		*  @brief
		*    Get list of detected input providers
		*
		*  @return
		*    Provider list, do not destroy the returned instances!
		*/
		PLINPUT_API PLCore::List<Provider*> &GetProviders();

		/**
		*  @brief
		*    Get a specific input provider
		*
		*  @param[in] sProvider
		*    Name of provider
		*
		*  @return
		*    Provider, or a null pointer if it doesn't exist, do not destroy the returned instance!
		*/
		PLINPUT_API Provider *GetProvider(const PLCore::String &sProvider);

		/**
		*  @brief
		*    Get list of devices
		*
		*  @return
		*    Device list, do not destroy the returned instances!
		*/
		PLINPUT_API PLCore::List<Device*> &GetDevices();

		/**
		*  @brief
		*    Get a specific device
		*
		*  @param[in] sDevice
		*    Name of device
		*
		*  @return
		*    Device, or a null pointer if it doesn't exist, do not destroy the returned instance!
		*/
		PLINPUT_API Device *GetDevice(const PLCore::String &sDevice) const;

		/**
		*  @brief
		*    Get default keyboard device
		*
		*  @return
		*    Default keyboard, can be a null pointer, do not destroy the returned instance!
		*/
		PLINPUT_API Keyboard *GetKeyboard() const;

		/**
		*  @brief
		*    Get default mouse device
		*
		*  @return
		*    Default mouse, can be a null pointer, do not destroy the returned instance!
		*/
		PLINPUT_API Mouse *GetMouse() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		InputManager();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		InputManager(const InputManager &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~InputManager();

		/**
		*  @brief
		*    Destroy all input providers and devices
		*/
		void Clear();

		/**
		*  @brief
		*    Detect devices from a specific provider
		*
		*  @param[in] sProvider
		*    Name of provider
		*  @param[in] bReset
		*    If 'true', delete all input devices and re-detect them all. Otherwise,
		*    only new and removed input devices will be detected.
		*
		*  @remarks
		*    If the provider is already present, it's Detect()-method will be called. Otherwise,
		*    a new instance of the provider will be created, then Detect() will be called as well.
		*/
		void DetectProvider(const PLCore::String &sProvider, bool bReset);

		/**
		*  @brief
		*    Add a new input device
		*
		*  @param[in] pDevice
		*    Input device, shouldn't be a null pointer (but a null pointer is caught internally)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool AddDevice(Device *pDevice);

		/**
		*  @brief
		*    Remove device
		*
		*  @param[in] pDevice
		*    Input device, shouldn't be a null pointer (but a null pointer is caught internally)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool RemoveDevice(Device *pDevice);

		/**
		*  @brief
		*    Remove control
		*
		*  @param[in] pControl
		*    Input control to remove, shouldn't be a null pointer (but a null pointer is caught internally)
		*/
		void RemoveControl(Control *pControl);

		/**
		*  @brief
		*    Update control
		*
		*  @param[in] pControl
		*    Input control, shouldn't be a null pointer (but a null pointer is caught internally)
		*
		*  @remarks
		*    This marks the control as being updated recently, which will fire a message
		*    in the next Update()-call.
		*/
		void UpdateControl(Control *pControl);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Providers and devices
		PLCore::List<Provider*>						 m_lstProviders;		/**< List of providers */
		PLCore::HashMap<PLCore::String, Provider*>	 m_mapProviders;		/**< Hash map of providers */
		PLCore::List<Device*>						 m_lstDevices;			/**< List of devices */
		PLCore::HashMap<PLCore::String, Device*>	 m_mapDevices;			/**< Hash map of devices */
		PLCore::CriticalSection						*m_pCriticalSection;	/**< Critical section for reading/writing input messages, always valid! */
		PLCore::List<Control*>						 m_lstUpdatedControls;	/**< List of controls that have been updated (message list) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_INPUTMANAGER_H__
