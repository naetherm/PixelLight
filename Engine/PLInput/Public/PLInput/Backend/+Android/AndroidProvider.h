/*********************************************************\
 *  File: AndroidProvider.h                              *
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


#ifndef __PLINPUT_ANDROIDPROVIDER_H__
#define __PLINPUT_ANDROIDPROVIDER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include "PLInput/Backend/Provider.h"


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
struct AInputEvent;
typedef struct AInputEvent AInputEvent;
namespace PLInput {
	class AndroidMouseDevice;
	class AndroidKeyboardDevice;
	class AndroidSplitTouchPadDevice;
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
*    Standard input provider for Android
*/
class AndroidProvider : public Provider {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, AndroidProvider, "PLInput", PLInput::Provider, "Standard input provider for Android")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLINPUT_API AndroidProvider();

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~AndroidProvider();


	//[-------------------------------------------------------]
	//[ Private virtual Provider functions                    ]
	//[-------------------------------------------------------]
	private:
		virtual void QueryDevices() override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called to process the next Android input event
		*
		*  @param[in] cAInputEvent
		*    Android input event to process
		*/
		void OnInputEvent(const struct AInputEvent &cAInputEvent);


	//[-------------------------------------------------------]
	//[ Private slots                                         ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<const struct AInputEvent&> SlotInputEvent;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AndroidKeyboardDevice		*m_pAndroidKeyboardDevice;		/**< Android keyboard device instance, can be a null pointer */
		AndroidSplitTouchPadDevice	*m_pAndroidSplitTouchPadDevice;	/**< Android splitted touchscreen mouse device instance, can be a null pointer */
		AndroidMouseDevice			*m_pAndroidMouseDevice;			/**< Android mouse device instance, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_ANDROIDPROVIDER_H__
