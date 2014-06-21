/*********************************************************\
 *  File: Connection.h                                   *
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


#ifndef __PLINPUT_CONNECTION_H__
#define __PLINPUT_CONNECTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/PLInput.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Control;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Connection between two controllers/controls
*/
class Connection {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pInput
		*    Input control, can be a null pointer
		*  @param[in] pOutput
		*    Output control, can be a null pointer
		*  @param[in] fScale
		*    Scale factor
		*/
		PLINPUT_API Connection(Control *pInput, Control *pOutput, float fScale = 1.0f);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Connection();

		/**
		*  @brief
		*    Get input control
		*
		*  @return
		*    Pointer to control that is on the input side of the connection, can be a null pointer
		*/
		PLINPUT_API Control *GetInputControl() const;

		/**
		*  @brief
		*    Get output control
		*
		*  @return
		*    Pointer to control that is on the output side of the connection, can be a null pointer
		*/
		PLINPUT_API Control *GetOutputControl() const;

		/**
		*  @brief
		*    Check if connection is value
		*
		*  @return
		*    'true' if connection is valid, else 'false'
		*
		*  @remarks
		*    A connection is invalid e.g. when you try to connect different types of controls
		*    without using the proper connection type (see derived classes for connection classes
		*    that can convert values into other types).
		*    It is also not valid to use a control of a device as an output, because devices can only be
		*    used as input, not as output of controls (a device is controlled by the physical device only).
		*/
		PLINPUT_API bool IsValid() const;


	//[-------------------------------------------------------]
	//[ Public virtual Connection functions                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Pass value from input to output
		*/
		PLINPUT_API virtual void PassValue();

		/**
		*  @brief
		*    Pass value backwards from output to input
		*/
		PLINPUT_API virtual void PassValueBackwards();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Control data
		Control	*m_pInput;		/**< Input control, can be a null pointer */
		Control	*m_pOutput;		/**< Output control, can be a null pointer */
		bool	 m_bValid;		/**< 'true' if connection is valid, else 'false' */
		float	 m_fScale;		/**< Scale factor */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_CONNECTION_H__
