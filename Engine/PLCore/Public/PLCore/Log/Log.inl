/*********************************************************\
 *  File: Log.inl                                        *
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
#include "PLCore/Log/LogFormatter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get verbose mode (write log entries to console)
*/
inline bool Log::IsVerbose() const
{
	return m_bVerbose;
}

/**
*  @brief
*    Set verbose mode (write log entries to console)
*/
inline void Log::SetVerbose(bool bVerbose, bool bVerboseLogLevelPrefix)
{
	m_bVerbose				 = bVerbose;
	m_bVerboseLogLevelPrefix = bVerboseLogLevelPrefix;
}

/**
*  @brief
*    Get log filename
*/
inline String Log::GetFilename() const
{
	// Return filename
	return m_sFilename;
}

/**
*  @brief
*    Write a string into the log
*/
inline bool Log::Output(uint8 nLogLevel, const String &sText)
{
	// Check log level
	return (nLogLevel > Quiet && nLogLevel <= m_nLogLevel && Write(nLogLevel, sText));
}

/**
*  @brief
*    Get current log level
*/
inline uint8 Log::GetLogLevel() const
{
	return m_nLogLevel;
}

/**
*  @brief
*    Set current log level
*/
inline void Log::SetLogLevel(uint8 nLogLevel)
{
	// Is this log level already set?
	if (m_nLogLevel != nLogLevel) {
		// Set log level
		m_nLogLevel = nLogLevel;

		// Write log message
		Write(Debug, "[Enter log level \"" + LogLevelToString(nLogLevel) + "\"]");
	}
}

/**
*  @brief
*    Get log level for automatic flush
*/
inline uint8 Log::GetFlushLogLevel() const
{
	return m_nFlushLogLevel;
}

/**
*  @brief
*    Set log level for automatic flush
*/
inline void Log::SetFlushLogLevel(uint8 nFlushLogLevel)
{
	// Change only if the new flush log level is not equal to the current one
	if (nFlushLogLevel != m_nFlushLogLevel) {
		// An explicit flush is necessary when the new flush log level is lower then the old one
		if (nFlushLogLevel < m_nFlushLogLevel)
			Flush();

		// Set the new flush log level
		m_nFlushLogLevel = nFlushLogLevel;
	}
}

/**
*  @brief
*    Flush output buffer to disk
*/
inline bool Log::Flush()
{
	// Is there anything to flush?
	if (m_nFlushMessages && m_pLogFormatter) {
		// Flush on disc
		m_nFlushMessages = 0;
		return m_pLogFormatter->Flush();
	} else {
		// No need to flush anything
		return false;
	}
}

/**
*  @brief
*    Get current amount of buffered log messages
*/
inline uint32 Log::GetBufferCount() const
{
	return m_nBufferedMessages;
}

/**
*  @brief
*    Get a copy of the queue which holds the last log messages
*/
inline const Queue<String> &Log::GetLastMessages() const
{
	return m_qLastMessages;
}

/**
*  @brief
*    Get current log formatter
*/
inline LogFormatter *Log::GetLogFormatter() const
{
	return m_pLogFormatter;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
