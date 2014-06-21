/*********************************************************\
 *  File: SystemAndroid.inl                              *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the native Android application
*/
inline android_app *SystemAndroid::GetAndroidApp()
{
	return g_pAndroidApp;
}

/**
*  @brief
*    Sets the native Android application
*/
inline void SystemAndroid::SetAndroidApp(android_app *pAndroidApp)
{
	g_pAndroidApp = pAndroidApp;
}

/**
*  @brief
*    Returns the Android asset manager
*/
inline AAssetManager *SystemAndroid::GetAssetManager()
{
	return g_pAAssetManager;
}

/**
*  @brief
*    Sets the Android asset manager
*/
inline void SystemAndroid::SetAssetManager(AAssetManager *pAAssetManager)
{
	g_pAAssetManager = pAAssetManager;
}

/**
*  @brief
*    Returns whether or not console messages are also written into the Android in-kernel log buffer (use Androids "logcat" utility to access this system log)
*/
inline bool SystemAndroid::GetConsoleToKernelLog()
{
	return g_bConsoleToKernelLog;
}

/**
*  @brief
*    Sets whether or not console messages are also written into the Android in-kernel log buffer (use Androids "logcat" utility to access this system log)
*/
inline void SystemAndroid::SetConsoleToKernelLog(bool bConsoleToKernelLog)
{
	g_bConsoleToKernelLog = bConsoleToKernelLog;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
