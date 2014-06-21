/*********************************************************\
 *  File: SystemAndroid.h                                *
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


#ifndef __PLCORE_SYSTEM_ANDROID_H__
#define __PLCORE_SYSTEM_ANDROID_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Event/Event.h"
#include "PLCore/System/ConsoleAndroid.h"
#include "PLCore/System/SystemLinux.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct android_app;
typedef struct android_app android_app;
struct AAssetManager;
typedef struct AAssetManager AAssetManager;
struct AInputEvent;
typedef struct AInputEvent AInputEvent;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Android 'System' implementation
*/
class SystemAndroid : public SystemLinux {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class System;


	//[-------------------------------------------------------]
	//[ Public static events                                  ]
	//[-------------------------------------------------------]
	public:
		static PLCORE_API Event<const struct AInputEvent&> EventInputEvent;	/**< Android input event, input event to be processed as first parameter */


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the native Android application
		*
		*  @return
		*    The native Android application, can be a null pointer, do not delete the returned instance
		*/
		static inline android_app *GetAndroidApp();

		/**
		*  @brief
		*    Sets the native Android application
		*
		*  @param[in] pAndroidApp
		*    Native Android application, can be a null pointer, the given instance is just shared and not destroyed by this class
		*/
		static inline void SetAndroidApp(android_app *pAndroidApp);

		/**
		*  @brief
		*    Returns the Android asset manager
		*
		*  @return
		*    The Android asset manager, can be a null pointer, do not delete the returned instance
		*
		*  @note
		*    - Information can also be received thru the native Android application pointer received via "GetAndroidApp()"
		*      (native Android activity only, that's why this special asset manager method exists in here)
		*/
		static inline AAssetManager *GetAssetManager();

		/**
		*  @brief
		*    Sets the Android asset manager
		*
		*  @param[in] pAAssetManager
		*    Android asset manager, can be a null pointer, the given instance is just shared and not destroyed by this class
		*/
		static inline void SetAssetManager(AAssetManager *pAAssetManager);

		/**
		*  @brief
		*    Emit Android input event
		*
		*  @param[in] sAInputEvent
		*    Android input event to emit
		*/
		static PLCORE_API void EmitInputEvent(const struct AInputEvent &sAInputEvent);

		/**
		*  @brief
		*    Returns whether or not console messages are also written into the Android in-kernel log buffer (use Androids "logcat" utility to access this system log)
		*
		*  @return
		*    'true' if console messages are also written into the Android in-kernel log buffer, else 'false'
		*/
		static inline bool GetConsoleToKernelLog();

		/**
		*  @brief
		*    Sets whether or not console messages are also written into the Android in-kernel log buffer (use Androids "logcat" utility to access this system log)
		*
		*  @param[in] bConsoleToKernelLog
		*    'true' if console messages are also written into the Android in-kernel log buffer, else 'false', default is 'false'
		*/
		static inline void SetConsoleToKernelLog(bool bConsoleToKernelLog);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		SystemAndroid();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SystemAndroid();


	//[-------------------------------------------------------]
	//[ Private virtual SystemImpl functions                  ]
	//[-------------------------------------------------------]
	private:
		virtual String GetPlatform() const override;
		virtual String GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const override;
		virtual const Console &GetConsole() const override;
		virtual void UrgentMessage(const String &sMessage) const override;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static PLCORE_API android_app   *g_pAndroidApp;			/**< Native Android application, can be a null pointer, the given instance is just shared and not destroyed by this class */
		static PLCORE_API AAssetManager *g_pAAssetManager;		/**< Android asset manager, can be a null pointer, the given instance is just shared and not destroyed by this class */
		static PLCORE_API bool			 g_bConsoleToKernelLog;	/**< 'true' if console messages are also written into the Android in-kernel log buffer, else 'false', default is 'false' */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ConsoleAndroid m_cConsole;	/**< Console instance (messages will be written into the Android in-kernel log buffer) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/System/SystemAndroid.inl"


#endif // __PLCORE_SYSTEM_ANDROID_H__
