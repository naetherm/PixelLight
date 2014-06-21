/*********************************************************\
 *  File: LogFormatterXml.cpp                            *
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
#include "PLCore/Log/LogFormatterXml.h"


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
LogFormatterXml::LogFormatterXml()
{
}

/**
*  @brief
*    Destructor
*/
LogFormatterXml::~LogFormatterXml()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
LogFormatterXml::LogFormatterXml(const LogFormatterXml &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
LogFormatterXml &LogFormatterXml::operator =(const LogFormatterXml &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Returns the XML formated text the given log level
*/
String LogFormatterXml::GetXmlFormatedText(uint8 nLogLevel, const String &sText) const
{
	String sTags     = "\t<a>|</a>";
	String sLogLevel = Log::GetInstance()->LogLevelToString(nLogLevel);

	if (sLogLevel.GetLength() > 0) {
		// Replace the 'a' with the text representation of the log level
		sTags.Replace('a', sLogLevel);
	} else {
		// Replace the 'a' with intern (because used log level is == 0 => internal log message)
		sTags.Replace("a", "Intern");
	}

	// Replace the '|' with the text in sText
	sTags.Replace('|', sText);

	return sTags;
}


//[-------------------------------------------------------]
//[ Private virtual LogFormatter functions                ]
//[-------------------------------------------------------]
bool LogFormatterXml::Open(const String &sFilename)
{
	// Open the log file via the helper function of the base class, use UTF8 string encoding format so one can also put cryptic none English characters into the log
	m_pFile = OpenFile(sFilename, String::UTF8);

	// Error?
	if (m_pFile) {
		// Write the start tags
		if (m_pFile->PrintLn("<?xml version=\"1.0\" ?>\n<Log>"))
			return true; // Done
	}

	// Error!
	return false;
}

bool LogFormatterXml::Close()
{
	// Is the file open?
	if (m_pFile) {
		// Write the end tag
		if (m_pFile->PrintLn("</Log>"))
			return true; // Done

		// Close file
		m_pFile->Close();

		// Reset data
		delete m_pFile;
		m_pFile = nullptr;

		// Done
		return true;
	}

	// Error, the log was not even open!
	return false;
}

bool LogFormatterXml::Output(uint8 nLogLevel, const String &sText)
{
	if (m_pFile) {
		// Write the text and a newline
		if (m_pFile->PrintLn(GetXmlFormatedText(nLogLevel, sText)))
			return true; // Done
	}

	// Error!
	return false;
}

bool LogFormatterXml::Flush()
{
	return m_pFile && m_pFile->Flush();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
