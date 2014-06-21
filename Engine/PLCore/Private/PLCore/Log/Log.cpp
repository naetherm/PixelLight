/*********************************************************\
 *  File: Log.cpp                                        *
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
#ifdef ANDROID
	#include <android/log.h>
#else
	#include <stdio.h>	// For "fputs" and "stdout" inside "Log::Write()"
#endif
#include "PLCore/File/Url.h"
#include "PLCore/Log/LogFormatterText.h"
#include "PLCore/Log/LogFormatterXml.h"
#include "PLCore/Log/LogFormatterHtml.h"
#include "PLCore/Log/Log.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<Log>;


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
// For fast Log::LogLevelToString()
const String g_sLogLevelToString[] = {
	"Quiet",
	"Always",
	"Critical",
	"Error",
	"Warning",
	"Info",
	"Debug"
};


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
Log *Log::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Log>::GetInstance();
}

bool Log::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Log>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Open the log file by using a filename
*/
bool Log::Open(const String &sFilename)
{
	// Ensure that the log is currently closed
	Close();

	// Get the filename extension
	const String sExtension = Url(sFilename).GetExtension();

	// Create log formatter instance by using the filename extension
	LogFormatter *pLogFormatter = nullptr;
	if (sExtension == "txt" || sExtension == "log")
		pLogFormatter = new LogFormatterText();
	else if (sExtension == "xml")
		pLogFormatter = new LogFormatterXml();
	else if (sExtension == "html")
		pLogFormatter = new LogFormatterHtml();

	// Open the log if a log formatter instance was successfully created
	if (pLogFormatter) {
		if (Open(sFilename, *pLogFormatter))
			return true; // Done
		else
			delete pLogFormatter; // Cleanup
	}

	// Error!
	return false;
}

/**
*  @brief
*    Open the log file by using a filename and an explicit log formatter instance
*/
bool Log::Open(const String &sFilename, LogFormatter &cLogFormatter)
{
	// Ensure that the log is currently closed
	Close();

	// Inform the log formatter that the log should be opened
	if (cLogFormatter.Open(sFilename)) {
		m_sFilename     = sFilename;
		m_pLogFormatter = &cLogFormatter;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Close log
*/
bool Log::Close()
{
	// Is the file open?
	if (m_pLogFormatter) {
		// Close the log formatter
		bool bResult = true; // By default, all went fine
		if (m_sFilename.GetLength()) {
			// Write log message
			Write(0, "Close log");

			// Inform the log formatter that the log should be closed
			bResult = m_pLogFormatter->Close();
		}

		// Destroy the log formatter
		delete m_pLogFormatter;
		m_pLogFormatter = nullptr;

		// Reset data
		m_sFilename = "";

		// Done
		return bResult;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Set how many log messages should be buffered in memory
*/
void Log::SetBufferCount(uint32 nBufferCount)
{
	// Make an explicit flush
	Flush();

	// Remove surplus log messages
	if (m_nBufferedMessages > nBufferCount) {
		const uint32 nToRemove = m_nBufferedMessages-nBufferCount;
		for (uint32 i=0; i<nToRemove; i++)
			m_qLastMessages.Pop();
	}

	// Set the new number of buffered messaged
	m_nBufferedMessages = nBufferCount;
}

/**
*  @brief
*    Get a string representation of the given log level
*/
String Log::LogLevelToString(uint8 nLogLevel) const
{
	if (!nLogLevel)
		return "";
	else if (nLogLevel > Debug)
		return String("Debug") + (nLogLevel-Debug);
	else
		return g_sLogLevelToString[nLogLevel-1];
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Log::Log() :
	m_bVerbose(false),
	m_bVerboseLogLevelPrefix(true),
	m_nLogLevel(Info),
	m_nFlushLogLevel(Error),
	m_nFlushMessages(0),
	m_nBufferedMessages(1000),
	m_pLogFormatter(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
Log::Log(const Log &cSource) :
	m_bVerbose(false),
	m_bVerboseLogLevelPrefix(true),
	m_nLogLevel(Info),
	m_nFlushLogLevel(Error),
	m_nFlushMessages(0),
	m_nBufferedMessages(1000),
	m_pLogFormatter(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
Log::~Log()
{
	// Close log
	Close();
}

/**
*  @brief
*    Copy operator
*/
Log &Log::operator =(const Log &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Writes a string into the log
*/
bool Log::Write(uint8 nLogLevel, const String &sText)
{
	// Is there any text to write down?
	if (sText.GetLength()) {
		// Write into the standard OS console?
		if (m_bVerbose) {
			// Prepare log message
			String sLogMessage;
			if (nLogLevel >= Quiet && m_bVerboseLogLevelPrefix) {
				sLogMessage  = '[';
				sLogMessage += LogLevelToString(nLogLevel);
				sLogMessage += "]: ";
			}
			sLogMessage += sText;
			sLogMessage += '\n';

			// Write the text into the OS console - it would be nice if this could be done using 'File::StandardOutput.Print()',
			// but this may cause problems when de-initializing the static variables :/
			#ifdef WIN32
				(sLogMessage.GetFormat() == String::ASCII) ? fputs(sLogMessage.GetASCII(), stdout) : fputws(sLogMessage.GetUnicode(), stdout);
			#elif ANDROID
				// Lookout! "__android_log_write" doesn't check for null pointer!
				const char *pszLogMessage = sLogMessage.GetASCII();
				if (pszLogMessage) {
					android_LogPriority nAndroidLogPriority = ANDROID_LOG_DEFAULT;
					switch (nLogLevel) {
						// No log outputs
						case Quiet:
							nAndroidLogPriority = ANDROID_LOG_SILENT;
							break;

						// Should be used if the message should be logged always
						case Always:
							nAndroidLogPriority = ANDROID_LOG_VERBOSE;
							break;

						// Should be used if an critical error occurs
						case Critical:
							nAndroidLogPriority = ANDROID_LOG_FATAL;
							break;

						// Should be used if an error occurs
						case Error:
							nAndroidLogPriority = ANDROID_LOG_ERROR;
							break;

						// Should be used for warning texts
						case Warning:
							nAndroidLogPriority = ANDROID_LOG_WARN;
							break;

						// Should be used for info texts
						case Info:
							nAndroidLogPriority = ANDROID_LOG_INFO;
							break;

						// Should be used for debug level texts
						case Debug:
							nAndroidLogPriority = ANDROID_LOG_DEBUG;
							break;
					}
					__android_log_write(nAndroidLogPriority, "PixelLight", (sLogMessage.GetFormat() == String::ASCII) ? sLogMessage.GetASCII() : sLogMessage.GetUTF8());
				}
			#else
				fputs((sLogMessage.GetFormat() == String::ASCII) ? sLogMessage.GetASCII() : sLogMessage.GetUTF8(), stdout);
			#endif
		}

		// Is there a log formatter?
		bool bResult = true; // By default, all went fine
		if (m_pLogFormatter) {
			// Write the text
			bResult = m_pLogFormatter->Output(nLogLevel, sText);
			if (bResult) {
				// Remove the latest message from the list if the limit is reached
				if (m_qLastMessages.GetNumOfElements() == m_nBufferedMessages) {
					// Remove the oldest message
					m_qLastMessages.Pop();
				}

				// Write the message in the queue
				if (m_nBufferedMessages && nLogLevel > Quiet)
					m_qLastMessages.Push('(' + LogLevelToString(nLogLevel) + ") " + sText);

				// Should be the output buffer written to the file?
				m_nFlushMessages++;
				if (nLogLevel >= m_nFlushLogLevel || m_nFlushMessages >= m_nBufferedMessages) {
					m_nFlushMessages = 0;
					if (!m_pLogFormatter->Flush())
						bResult = false; // Error!
				}

				// Emit the event
				EventNewEntry();
			}
		}

		// Done
		return bResult;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
