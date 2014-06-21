/*********************************************************\
 *  File: CoreApplication.inl                            *
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
*    Get pointer to current application
*/
inline CoreApplication *CoreApplication::GetApplication()
{
	// Return global application pointer
	return g_pApplication;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get application context
*/
inline const ApplicationContext &CoreApplication::GetApplicationContext() const
{
	// Return application context
	return m_cApplicationContext;
}

/**
*  @brief
*    Get application name
*/
inline String CoreApplication::GetName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Get application title
*/
inline String CoreApplication::GetTitle() const
{
	// Return title
	return m_sTitle;
}

/**
*  @brief
*    Set application title
*/
inline void CoreApplication::SetTitle(const String &sTitle)
{
	// Set title
	m_sTitle = sTitle;
}

/**
*  @brief
*    Get version of application
*/
inline const Version &CoreApplication::GetVersion() const
{
	// Return version
	return m_cVersion;
}

/**
*  @brief
*    Set version of application
*/
inline void CoreApplication::SetVersion(const Version &cVersion)
{
	// Set version
	m_cVersion = cVersion;
}

/**
*  @brief
*    Check if application uses multi-user environment
*/
inline bool CoreApplication::GetMultiUser() const
{
	// Return multi-user flag
	return m_bMultiUser;
}

/**
*  @brief
*    Set if application uses multi-user environment
*/
inline void CoreApplication::SetMultiUser(bool bMultiUser)
{
	// Set multi-user flag
	m_bMultiUser = bMultiUser;
}

/**
*  @brief
*    Get name of config file
*/
inline String CoreApplication::GetConfigName() const
{
	// Return config name
	return m_sConfigName;
}

/**
*  @brief
*    Set name of config file
*/
inline void CoreApplication::SetConfigName(const String &sConfigName)
{
	// Set config name
	m_sConfigName = sConfigName;
}

/**
*  @brief
*    Get name of log file
*/
inline String CoreApplication::GetLogName() const
{
	// Return log name
	return m_sLogName;
}

/**
*  @brief
*    Set name of log file
*/
inline void CoreApplication::SetLogName(const String &sLogName)
{
	// Set log name
	m_sLogName = sLogName;
}

/**
*  @brief
*    Get subdirectory for application data files
*/
inline String CoreApplication::GetAppDataSubdir() const
{
	// Return application data subdirectory
	return m_sAppDataSubdir;
}

/**
*  @brief
*    Set subdirectory for application data files
*/
inline void CoreApplication::SetAppDataSubdir(const String &sSubdir)
{
	// Set application data subdirectory
	m_sAppDataSubdir = sSubdir;
}

/**
*  @brief
*    Returns the configuration instance
*/
inline Config &CoreApplication::GetConfig()
{
	// Return config
	return m_cConfig;
}

/**
*  @brief
*    Returns whether or not the application is currently running
*/
inline bool CoreApplication::IsRunning() const
{
	// Set config
	return m_bRunning;
}

/**
*  @brief
*    Exit application
*/
inline void CoreApplication::Exit(int nResult)
{
	// Set result
	m_nResult = nResult;

	// Stop application
	m_bRunning = false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
