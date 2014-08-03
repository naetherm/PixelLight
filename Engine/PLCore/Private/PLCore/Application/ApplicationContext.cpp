/*********************************************************\
 *  File: ApplicationContext.cpp                         *
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
#include "PLCore/File/Url.h"
#include "PLCore/System/System.h"
#include "PLCore/Application/ApplicationContext.h"


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_begin_class(ApplicationContext, PLCore)
	pl_desc("Application context.")

	pl_method(GetExecutableFilename)
		pl_desc("Get absolute path of application executable (native path style, e.g. on Windows: 'C:\\MyApplication\\x86\\Test.exe').")

	pl_method(GetExecutableDirectory)
		pl_desc("Get directory of executable (native path style, e.g. on Windows: 'C:\\MyApplication\\x86').")

	pl_method(GetAppDirectory)
		pl_desc("Get directory of application (native path style, e.g. on Windows: 'C:\\MyApplication').")

	pl_method(GetStartupDirectory)
		pl_desc("Get current directory when the application constructor was called (native path style, e.g. on Windows: 'C:\\MyApplication\\x86').")

	pl_method(GetLogFilename)
		pl_desc("Get absolute path to log file, empty if log has not been opened (native path style).")

	pl_method(GetConfigFilename)
		pl_desc("Get absolute path to config file, empty if no config is used (native path style).")

pl_end_class()


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ApplicationContext::ApplicationContext()
{
}

/**
*  @brief
*    Destructor
*/
ApplicationContext::~ApplicationContext()
{
}


//[-------------------------------------------------------]
//[ Options and data                                      ]
//[-------------------------------------------------------]

/**
*  @brief
*    Set absolute path of application executable
*/
void ApplicationContext::SetExecutableFilename(const String &sExecutableFilename)
{
	// Set absolute executable filename, ensure it's native path style
	m_sExecutableFilename = Url(sExecutableFilename).GetNativePath();

	// Save executable directory, if there's no given executable filename, do nothing special in here
	m_sExecutableDirectory = m_sExecutableFilename.GetLength() ? Url(Url(m_sExecutableFilename).CutFilename()).Collapse().GetNativePath() : "";

	// Save application directory, if there's no given executable filename, do nothing special in here
	m_sAppDirectory = m_sExecutableFilename.GetLength() ? Url(m_sExecutableDirectory + "../").Collapse().GetNativePath() : "";

	// Remove the '/' at the end (due usage of the Url-class, we know there's a '/' at the end!)
	m_sExecutableDirectory.Delete(m_sExecutableDirectory.GetLength() - 1);
	m_sAppDirectory.Delete(m_sAppDirectory.GetLength() - 1);

	// No empty string, please (it should always be possible to add e.g. '/Data' without problems)
	if (!m_sAppDirectory.GetLength())
		m_sAppDirectory = '.';
}

/**
*  @brief
*    Set current directory when the application constructor was called
*/
void ApplicationContext::SetStartupDirectory(const String &sStartupDirectory)
{
	// Set startup directory, ensure it's native path style
	m_sStartupDirectory = Url(sStartupDirectory).GetNativePath();
}

/**
*  @brief
*    Set log filename
*/
void ApplicationContext::SetLogFilename(const String &sLog)
{
	// Set log filename, ensure it's native path style
	m_sLog = Url(sLog).GetNativePath();
}

/**
*  @brief
*    Set config filename
*/
void ApplicationContext::SetConfigFilename(const String &sConfig)
{
	// Set config filename, ensure it's native path style
	m_sConfig = Url(sConfig).GetNativePath();
}


//[-------------------------------------------------------]
//[ Tool functions                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the current directory to the path of the application executable
*/
void ApplicationContext::ChangeIntoAppDirectory() const
{
	// Use the executable directory as the current directory
	System::GetInstance()->SetCurrentDir(Url(m_sExecutableFilename).CutFilename());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore