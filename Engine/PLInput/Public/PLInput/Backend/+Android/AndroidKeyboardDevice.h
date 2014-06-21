/*********************************************************\
 *  File: AndroidKeyboardDevice.h                        *
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


#ifndef __PLINPUT_ANDROIDKEYBOARDDEVICE_H__
#define __PLINPUT_ANDROIDKEYBOARDDEVICE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Backend/UpdateDevice.h"


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
struct AInputEvent;
typedef struct AInputEvent AInputEvent;
namespace PLInput {
	class Button;
	class Keyboard;
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
*    Keyboard implementation for Android
*/
class AndroidKeyboardDevice : public UpdateDevice {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		AndroidKeyboardDevice();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AndroidKeyboardDevice();

		/**
		*  @brief
		*    Call this to process the next key input event
		*
		*  @param[in] cAKeyInputEvent
		*    Key input event to process
		*/
		void OnKeyInputEvent(const struct AInputEvent &cAKeyInputEvent);


	//[-------------------------------------------------------]
	//[ Public virtual UpdateDevice functions                 ]
	//[-------------------------------------------------------]
	public:
		virtual void Update() override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Get key for Android key code
		*
		*  @param[in] cKeyboard
		*    Reference to keyboard device
		*  @param[in] int32_t
		*    Key code
		*
		*  @return
		*    Corresponding key, a null pointer if Android key code is invalid
		*/
		Button *GetKeyboardKey(Keyboard &cKeyboard, int32_t nKeyCode);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<int32_t> m_lstProcessedKeys;
		class KeyInfo {
			public:
				int32_t nKeyCode;
				bool    bPressed;
				bool operator ==(const KeyInfo &cSource) { return false; }
		};
		PLCore::Array<KeyInfo> m_lstDelayedKeys;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_ANDROIDKEYBOARDDEVICE_H__
