/*********************************************************\
 *  File: LogFormatterText.cpp                           *
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
#include "PLCore/File/File.h"
#include "PLCore/Log/Log.h"
#include "PLCore/Log/LogFormatterText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
LogFormatterText::LogFormatterText()
{
}

/**
*  @brief
*    Destructor
*/
LogFormatterText::~LogFormatterText()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
LogFormatterText::LogFormatterText(const LogFormatterText &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
LogFormatterText &LogFormatterText::operator =(const LogFormatterText &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Private virtual LogFormatter functions                ]
//[-------------------------------------------------------]
bool LogFormatterText::Open(const String &sFilename)
{
	// Open the log file via the helper function of the base-class
	// -> Do not use UTF8 string encoding format so one can also put cryptic none English characters into the log:
	//    While this looks good at first, not every tool can handle UTF8 correctly and in general things get more
	//    complicated has they have to be (just write English log messages and use simple ASCII logs for best compatibility)
	m_pFile = OpenFile(sFilename, String::ASCII);

	// Inform about ASCII and that Unicode characters may not show up correctly
	Output(Log::Always, "This is just a simple ASCII log for best compatibility, Unicode characters may not be shown correctly");

	// Error?
	return (m_pFile != nullptr);
}

bool LogFormatterText::Close()
{
	// Is the file open?
	if (m_pFile) {
		// Close file
		m_pFile->Close();

		// Reset data
		delete m_pFile;
		m_pFile = nullptr;

		// Done
		return true;
	}

	// Log was not even open
	return false; // Error!
}

bool LogFormatterText::Output(uint8 nLogLevel, const String &sText)
{
	if (m_pFile) {
		// Prepare log message
		String sLogMessage;
		if (nLogLevel >= Log::Quiet && m_bShowLogLevelPrefix) {
			sLogMessage  = "[";
			sLogMessage += Log::GetInstance()->LogLevelToString(nLogLevel);
			sLogMessage += "]: ";
		}
		sLogMessage += sText;

		// Write the text and a newline
		if (m_pFile->PrintLn(sLogMessage))
			return true; // Done
	}

	// Error!
	return false;
}

bool LogFormatterText::Flush()
{
	return (m_pFile && m_pFile->Flush());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
