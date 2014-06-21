/*********************************************************\
 *  File: ApplicationContext.inl                         *
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
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Options and data                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get absolute path of application executable
*/
inline String ApplicationContext::GetExecutableFilename() const
{
	// Return absolute executable filename
	return m_sExecutableFilename;
}

/**
*  @brief
*    Get command line arguments
*/
inline const Array<String> &ApplicationContext::GetArguments() const
{
	// Return argument list
	return m_lstArguments;
}

/**
*  @brief
*    Set command line arguments
*/
inline void ApplicationContext::SetArguments(const Array<String> &lstArguments)
{
	// Set argument list
	m_lstArguments = lstArguments;
}

/**
*  @brief
*    Get directory of application executable
*/
inline String ApplicationContext::GetExecutableDirectory() const
{
	// Return application executable directory
	return m_sExecutableDirectory;
}

/**
*  @brief
*    Get directory of application
*/
inline String ApplicationContext::GetAppDirectory() const
{
	// Return application directory
	return m_sAppDirectory;
}

/**
*  @brief
*    Get current directory when the application constructor was called
*/
inline String ApplicationContext::GetStartupDirectory() const
{
	// Return startup directory
	return m_sStartupDirectory;
}

/**
*  @brief
*    Get log filename
*/
inline String ApplicationContext::GetLogFilename() const
{
	// Return log filename
	return m_sLog;
}

/**
*  @brief
*    Get config filename
*/
inline String ApplicationContext::GetConfigFilename() const
{
	// Return config filename
	return m_sConfig;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
