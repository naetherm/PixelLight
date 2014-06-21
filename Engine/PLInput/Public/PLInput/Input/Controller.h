/*********************************************************\
 *  File: Controller.h                                   *
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


#ifndef __PLINPUT_CONTROLLER_H__
#define __PLINPUT_CONTROLLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLInput/PLInput.h"
#include "PLInput/PLInputDefinitions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Control;
class Button;
class Axis;
class Connection;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input controller
*
*  @remarks
*    A controller represents an input device, which can either be a real device like e.g. a mouse or joystick,
*    or a virtual device that is used to map real input devices to virtual axes and keys. A controller consists
*    of a list of controls, e.g. buttons or axes and provides methods to obtain the status.
*/
class Controller : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class InputManager;
	friend class Provider;
	friend class Control;


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, Controller, "PLInput", PLCore::Object, "Input controller base class")
		// Attributes
		pl_attribute(Type,			pl_enum_type(EControllerType),	ControllerVirtual,	ReadOnly,	GetSet,	"Controller type",			"")
		pl_attribute(Name,			PLCore::String,					"",					ReadOnly,	GetSet,	"Controller name",			"")
		pl_attribute(Description,	PLCore::String,					"",					ReadOnly,	GetSet,	"Controller description",	"")
		pl_attribute(Active,		bool,							true,				ReadWrite,	GetSet,	"State of controller",		"")
		// Signals
		pl_signal_1(SignalOnActivate,	bool,		"Controller has been activated or deactivated. 'true' as parameter if the controller has been activated, else 'false'.",	"")
		pl_signal_1(SignalOnControl,	Control&,	"Control event has occurred, control as parameter",																			"")
		pl_signal_0(SignalOnChanged,				"Controller state has changed",																								"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nType
		*    Controller type
		*  @param[in] sName
		*    Controller name
		*  @param[in] sDescription
		*    Controller description
		*/
		PLINPUT_API Controller(EControllerType nType, const PLCore::String &sName, const PLCore::String &sDescription);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Controller();

		/**
		*  @brief
		*    Get controller type
		*
		*  @return
		*    Controller type
		*/
		PLINPUT_API EControllerType GetType() const;

		/**
		*  @brief
		*    Get controller name
		*
		*  @return
		*    Name
		*/
		PLINPUT_API PLCore::String GetName() const;

		/**
		*  @brief
		*    Get controller description
		*
		*  @return
		*    Description
		*/
		PLINPUT_API PLCore::String GetDescription() const;

		/**
		*  @brief
		*    Check if controller is active
		*
		*  @return
		*    'true' if controller is active, else 'false'
		*
		*  @remarks
		*    If a controller is active, it sends out signals when the state of it's controls has changed.
		*    If a controller is not active, no state changes will occur and all input events from connected
		*    devices will be discarded.
		*/
		PLINPUT_API bool GetActive() const;

		/**
		*  @brief
		*    Activate or deactivate controller
		*
		*  @param[in] bActive
		*    'true' if controller is active, else 'false'
		*
		*  @remarks
		*    Virtual controllers can be activated or deactivated, real input devices
		*    are always active and can not be deactivated.
		*/
		PLINPUT_API void SetActive(bool bActive);

		/**
		*  @brief
		*    Check if the controller's state has changed (for polling)
		*
		*  @return
		*    'true', if the state has changed, else 'false'
		*/
		PLINPUT_API bool HasChanged() const;

		/**
		*  @brief
		*    Get all controls of the controller
		*
		*  @return
		*    List of controls
		*/
		PLINPUT_API const PLCore::List<Control*> &GetControls() const;

		/**
		*  @brief
		*    Get all buttons
		*
		*  @return
		*    List of buttons
		*/
		PLINPUT_API const PLCore::List<Button*> &GetButtons() const;

		/**
		*  @brief
		*    Get all axes
		*
		*  @return
		*    List of axes
		*/
		PLINPUT_API const PLCore::List<Axis*> &GetAxes() const;

		/**
		*  @brief
		*    Get control with a specific name
		*
		*  @param[in] sName
		*    Name of control
		*
		*  @return
		*    Control, or a null pointer if no control with that name could be found
		*/
		PLINPUT_API Control *GetControl(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Get character of last button that was hit
		*
		*  @return
		*    Button character (ASCII), can be '\0'
		*
		*  @remarks
		*    This function returns the character code of the last button that was hit (not pressed!).
		*    The character will then be reset to '\0', so the next call will return '\0', until
		*    a new button is first pressed and then released.
		*/
		PLINPUT_API char GetChar();

		/**
		*  @brief
		*    Get connections
		*
		*  @return
		*    List of connections (both incoming and outgoing), do not destroy the returned connection instances!
		*
		*  @remarks
		*    To determine whether a connection is incoming or outgoing, you can check e.g.
		*    GetOutputControl()->GetController() == this or something similar.
		*/
		PLINPUT_API const PLCore::List<Connection*> &GetConnections();

		/**
		*  @brief
		*    Connect to another controller
		*
		*  @param[in] sControl
		*    Name of control of this controller (output control)
		*  @param[in] pControl
		*    Pointer to control (input control), shouldn't be a null pointer (but a null pointer is caught internally)
		*  @param[in] fScale
		*    Scale factor
		*/
		PLINPUT_API void Connect(const PLCore::String &sControl, Control *pControl, float fScale = 1.0f);

		/**
		*  @brief
		*    Connect to another controller
		*
		*  @param[in] pController
		*    Pointer to controller containing the input controls, shouldn't be a null pointer (but a null pointer is caught internally)
		*  @param[in] sPrefixOut
		*    Prefix for controls of this controller
		*  @param[in] sPrefixIn
		*    Prefix for controls of the other controller
		*
		*  @remarks
		*    This connects all controls of the input controller (pController) to the controls of the output
		*    controller (this), if their names are equal, e.g. pController->"Left" will be connected to this->"Left".
		*    You can also provide a prefix for either or both sides, e.g.: ConnectAll(pOtherController, "", "Camera")
		*    will connect pController->"CameraLeft" to this->"Left".
		*/
		PLINPUT_API void ConnectAll(Controller *pController, const PLCore::String &sPrefixOut, const PLCore::String &sPrefixIn);

		/**
		*  @brief
		*    Disconnect connection
		*
		*  @param[in] pConnection
		*    Connection (must be valid!), on successful disconnect, the given "pConnection" instance becomes invalid
		*/
		PLINPUT_API void Disconnect(Connection *pConnection);


	//[-------------------------------------------------------]
	//[ Public virtual Controller functions                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Update device once per frame
		*
		*  @remarks
		*    This function can be used e.g. to reset any data of a device once per frame. Usually this is
		*    not needed, but some devices (e.g. RawInput mice etc.) need to reset their data once per frame.
		*
		*  @note
		*    - The default implementation is empty
		*/
		PLINPUT_API virtual void Update();

		/**
		*  @brief
		*    Update output controls (LEDs, effects etc.)
		*
		*  @param[in] pControl
		*    Output control that has been changed, must be valid!
		*
		*  @remarks
		*    This function is called whenever an output control such as LED or Effect has been changed.
		*    A device should use this function to update the specific control state on the device (or update
		*    all output controls at the same time)
		*
		*  @note
		*    - The default implementation is empty
		*/
		PLINPUT_API virtual void UpdateOutputControl(Control *pControl);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Add control
		*
		*  @param[in] pControl
		*    Pointer to control, must be valid!
		*/
		PLINPUT_API void AddControl(Control *pControl);

		/**
		*  @brief
		*    Inform controller that a control has changed it's state
		*
		*  @param[in] pControl
		*    Pointer to control, must be valid!
		*/
		PLINPUT_API void InformControl(Control *pControl);

		/**
		*  @brief
		*    Init control list
		*
		*  @param[in] nType
		*    Type of list that is to be filled
		*/
		PLINPUT_API void InitControlList(EControlType nType) const;

		/**
		*  @brief
		*    Add connection
		*
		*  @param[in] pConnection
		*    Connection (must be valid!)
		*/
		PLINPUT_API void AddConnection(Connection *pConnection);

		/**
		*  @brief
		*    Remove connection
		*
		*  @param[in] pConnection
		*    Connection (must be valid!)
		*/
		PLINPUT_API void RemoveConnection(Connection *pConnection);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Controller information and state
		EControllerType								m_nType;			/**< Controller type */
		PLCore::String								m_sName;			/**< Controller name */
		PLCore::String								m_sDescription;		/**< Controller description */
		bool										m_bConfirmed;		/**< Confirmation flag for DetectDevices() */
		bool										m_bActive;			/**< Is the controller active? */
		mutable bool								m_bChanged;			/**< Has the controller's state changed? */

		// Controls
		PLCore::List<Control*>						m_lstControls;		/**< List of all controls */
		PLCore::HashMap<PLCore::String, Control*>	m_mapControls;		/**< Hash map of name -> control */
		mutable PLCore::List<Button*>				m_lstButtons;		/**< List of buttons (filled on use) */
		mutable PLCore::List<Axis*>					m_lstAxes;			/**< List of absolute axes (filled on use) */
		char										m_nChar;			/**< Last hit key character */

		// Connections
		PLCore::List<Connection*>					m_lstConnections;	/**< List of connections */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_CONTROLLER_H__
