/*********************************************************\
 *  File: SystemAndroid.cpp                              *
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
#include <android/log.h>
#include "PLCore/System/SystemAndroid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static events                                  ]
//[-------------------------------------------------------]
Event<const struct AInputEvent&> SystemAndroid::EventInputEvent;	/**< Android input event, input event to be processed as first parameter */


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
android_app   *SystemAndroid::g_pAndroidApp         = nullptr;	/**< Native Android application, can be a null pointer, the given instance is just shared and not destroyed by this class */
AAssetManager *SystemAndroid::g_pAAssetManager      = nullptr;	/**< Android asset manager, can be a null pointer, the given instance is just shared and not destroyed by this class */
bool		   SystemAndroid::g_bConsoleToKernelLog = false;	/**< 'true' if console messages are also written into the Android in-kernel log buffer, else 'false', default is 'false' */


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Emit Android input event
*/
void SystemAndroid::EmitInputEvent(const struct AInputEvent &sAInputEvent)
{
	EventInputEvent(sAInputEvent);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SystemAndroid::SystemAndroid()
{
}

/**
*  @brief
*    Destructor
*/
SystemAndroid::~SystemAndroid()
{
}


//[-------------------------------------------------------]
//[ Private virtual SystemImpl functions                  ]
//[-------------------------------------------------------]
String SystemAndroid::GetPlatform() const
{
	static const String sString = "Android";
	return sString;
}

String SystemAndroid::GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const
{
	// [TODO] Test whether or not the Linux implementation works (compiles and runs correctly) for Android as well
	// -> Compiled when using the Android NDK, but when running it on the device it freezed while loading
	//    the "PLCore.so" shared library. Check this in detail.
	return "";
}

const Console &SystemAndroid::GetConsole() const
{
	return g_bConsoleToKernelLog ? m_cConsole : SystemLinux::m_cConsole;
}

void SystemAndroid::UrgentMessage(const String &sMessage) const
{
	// Write into the Android kernel log buffer (use Androids "logcat" utility to access this system log)
	__android_log_write(ANDROID_LOG_FATAL, "PixelLight", (sMessage.GetFormat() == String::ASCII) ? sMessage.GetASCII() : sMessage.GetUTF8());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
