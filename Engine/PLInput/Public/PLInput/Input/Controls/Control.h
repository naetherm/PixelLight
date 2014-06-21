/*********************************************************\
 *  File: Control.h                                     *
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


#ifndef __PLINPUT_CONTROL_H__
#define __PLINPUT_CONTROL_H__
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
class Controller;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input control base class
*
*  @remarks
*    A control is part of an input controller, e.g. a button or an axis.
*/
class Control : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, Control, "PLInput", PLCore::Object, "Input control base class")
		#ifdef PLINPUT_EXPORTS	// The following is only required when compiling PLInput
			// Methods
			pl_method_0(GetController,	pl_ret_type(Controller*),					"Returns the controller that owns the control, can be a null pointer.",											"")
			pl_method_0(GetType,		pl_ret_type(pl_enum_type(EControlType)),	"Returns the type of control.",																					"")
			pl_method_0(IsInputControl,	pl_ret_type(bool),							"Check if control is input or output control. Returns 'true' if control is input control, 'false' if output.",	"")
			pl_method_0(GetName,		pl_ret_type(PLCore::String),				"Returns the control name.",																					"")
			pl_method_0(GetDescription,	pl_ret_type(PLCore::String),				"Returns the control description.",																				"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pController
		*    Owning controller, can, but shouldn't be a null pointer
		*  @param[in] nType
		*    Control type
		*  @param[in] sName
		*    Control name
		*  @param[in] sDescription
		*    Control description
		*/
		PLINPUT_API Control(Controller *pController, EControlType nType, const PLCore::String &sName, const PLCore::String &sDescription);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Control();

		/**
		*  @brief
		*    Get controller
		*
		*  @return
		*    Pointer to controller that owns the control, can be a null pointer
		*/
		PLINPUT_API Controller *GetController() const;

		/**
		*  @brief
		*    Get control type
		*
		*  @return
		*    Type of control
		*/
		PLINPUT_API EControlType GetType() const;

		/**
		*  @brief
		*    Check if control is input or output control
		*
		*  @return
		*    'true' if control is input control, 'false' if output
		*/
		PLINPUT_API bool IsInputControl() const;

		/**
		*  @brief
		*    Get control name
		*
		*  @return
		*    Name
		*/
		PLINPUT_API PLCore::String GetName() const;

		/**
		*  @brief
		*    Get control description
		*
		*  @return
		*    Description
		*/
		PLINPUT_API PLCore::String GetDescription() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Inform input manager that control has been changed
		*/
		PLINPUT_API void InformUpdate();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Control data
		Controller		*m_pController;		/**< Owning controller, can be a null pointer */
		EControlType	 m_nType;			/**< Control type */
		PLCore::String	 m_sName;			/**< Control name */
		PLCore::String	 m_sDescription;	/**< Control description */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_CONTROL_H__
