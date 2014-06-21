/*********************************************************\
 *  File: Log.h                                          *
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


#ifndef __PLCORE_LOG_H__
#define __PLCORE_LOG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Core/Singleton.h"
#include "PLCore/Container/Queue.h"
#include "PLCore/Base/Event/Event.h"


//[-------------------------------------------------------]
//[ Macro definitions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Log macro
*
*  @param[in] Level
*    Log level
*  @param[in] Text
*    Text which should be written into the log
*/
#define PL_LOG(Level, Text) { \
	if (PLCore::Log::HasInstance() && PLCore::Log::GetInstance()->GetLogLevel() >= PLCore::Log::Level) { \
		PLCore::Log::LogLevel nLogLevel = PLCore::Log::Level; /* Avoid 'expression is constant' warning */ \
		if (nLogLevel >= PLCore::Log::Debug) { \
			PLCore::String sLogString = Text; \
			PLCore::Log::GetInstance()->Output(PLCore::Log::Level, sLogString + PLCore::String::Format(", %s, %d, %s", __FILE__, __LINE__, __FUNCTION__)); \
		} else PLCore::Log::GetInstance()->Output(PLCore::Log::Level, Text); \
	} \
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class LogFormatter;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Log system class
*
*  @note
*    - A configurable amount of messages is hold in a queue (the last n-messages)
*    - When the queue is full a flush is done
*    - A flush is also done if a specified log level is used in the output function (configurable)
*    - A specified log level includes also all log levels with lower values
*      e.g.: if the overall log level is set to info, log messages marked as
*      warning, error or critical are written to the log, too
*    - You can define as many debug log levels as you want
*    - Implementation of the strategy design pattern, this class is the context
*/
class Log : public Singleton<Log> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Singleton<Log>;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Log levels
		*
		*  @remarks
		*    The log level 0 is reserved for messages which are generated from the
		*    log system itself.
		*/
		enum LogLevel {
			Quiet = 1,	/**< No log outputs */
			Always,		/**< Should be used if the message should be logged always */
			Critical,	/**< Should be used if an critical error occurs */
			Error,		/**< Should be used if an error occurs */
			Warning,	/**< Should be used for warning texts */
			Info,		/**< Should be used for info texts */
			Debug		/**< Should be used for debug level texts */
		};


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		Event<> EventNewEntry;	/**< A new log entry had been added, use e.g. "GetLastMessages()" to access the latest messages */


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		static PLCORE_API Log *GetInstance();
		static PLCORE_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get verbose mode (write log entries to console)
		*
		*  @return
		*    'true' if the log is writing into the console, else 'false'
		*
		*  @note
		*    - In addition to using an optional log formatter, the log also writes into the console in verbose mode
		*    - The log does not need be to be opened to write into the console
		*    - Android: Messages are not written into the console, instead the Android in-kernel log buffer is used
		*              (use Androids "logcat" utility to access this system log)
		*    - By default the log does not write into the console
		*/
		inline bool IsVerbose() const;

		/**
		*  @brief
		*    Set verbose mode (write log entries to console)
		*
		*  @param[in] bVerbose
		*    'true' if the log is writing into the console, else 'false'
		*  @param[in] bVerboseLogLevelPrefix
		*    Should the [LogLevel] prefix be shown?
		*
		*  @see
		*    - IsVerbose()
		*/
		inline void SetVerbose(bool bVerbose, bool bVerboseLogLevelPrefix = true);

		/**
		*  @brief
		*    Open the log file by using a filename
		*
		*  @param[in] sFilename
		*    The log's filename, the log formatter is automatically created by using the filename extension
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the log is already opened or the filename extension is not supported?)
		*
		*  @note
		*    - Supported log filename extensions: "txt" (LogFormatterText), "log" (LogFormatterText), "xml" (LogFormatterXml), "html" (LogFormatterHtml)
		*/
		PLCORE_API bool Open(const String &sFilename);

		/**
		*  @brief
		*    Open the log file by using a filename and an explicit log formatter instance
		*
		*  @param[in] sFilename
		*    The log's filename
		*  @param[in] cLogFormatter
		*    Log formatter instance to be used, if this method returns successfully, the log takes over
		*    the control and is responsible for the destruction of the given instance
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the log is already opened?)
		*/
		PLCORE_API bool Open(const String &sFilename, LogFormatter &cLogFormatter);

		/**
		*  @brief
		*    Close log
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Close();

		/**
		*  @brief
		*    Get log filename
		*
		*  @return
		*    The log filename
		*/
		inline String GetFilename() const;

		/**
		*  @brief
		*    Write a string into the log
		*
		*  @param[in] nLogLevel
		*    Log level
		*  @param[in] sText
		*    Text which should be written into the log
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    The text is written to the log only if the current
		*    log level is greater or equal to the specified value.
		*/
		inline bool Output(uint8 nLogLevel, const String &sText);

		/**
		*  @brief
		*    Get current log level
		*
		*  @return
		*    The current log level
		*/
		inline uint8 GetLogLevel() const;

		/**
		*  @brief
		*    Set current log level
		*
		*  @param[in] nLogLevel
		*    New current log level
		*/
		inline void SetLogLevel(uint8 nLogLevel = Info);

		/**
		*  @brief
		*    Get log level for automatic flush
		*
		*  @return
		*    Log level for explicitly flush
		*/
		inline uint8 GetFlushLogLevel() const;

		/**
		*  @brief
		*    Set log level for automatic flush
		*
		*  @param[in] nFlushLogLevel
		*    Log level for the explicitly flush
		*/
		inline void SetFlushLogLevel(uint8 nFlushLogLevel = Error);

		/**
		*  @brief
		*    Flush output buffer to disk
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there was nothing to flush?)
		*
		*  @note
		*    - Only call this function if really required!
		*/
		inline bool Flush();

		/**
		*  @brief
		*    Get current amount of buffered log messages
		*
		*  @return
		*    Amount of buffered log messages
		*/
		inline uint32 GetBufferCount() const;

		/**
		*  @brief
		*    Set how many log messages should be buffered in memory
		*
		*  @param[in] nBufferCount
		*    Number of messages to buffer
		*
		*  @note
		*    The nBufferCount-last log messages gets buffered
		*/
		PLCORE_API void SetBufferCount(uint32 nBufferCount = 1000);

		/**
		*  @brief
		*    Get a copy of the queue which holds the last log messages
		*
		*  @return
		*    Queue of n-last log messages
		*/
		inline const Queue<String> &GetLastMessages() const;

		/**
		*  @brief
		*    Get a string representation of the given log level
		*
		*  @param[in] nLogLevel
		*    Log level to return the string representation from
		*
		*  @return
		*    The string representation of the given log level
		*/
		PLCORE_API String LogLevelToString(uint8 nLogLevel) const;

		/**
		*  @brief
		*    Get log formatter
		*
		*  @return
		*    The current log formatter object, can be a null pointer, do NOT delete the returned instance!
		*/
		inline LogFormatter *GetLogFormatter() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		Log();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Log(const Log &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Log();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Log &operator =(const Log &cSource);

		/**
		*  @brief
		*    Write a string into the log
		*
		*  @param[in] nLogLevel
		*    Log Level
		*  @param[in] sText
		*    Text which should be written into the log
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Write(uint8 nLogLevel, const String &sText);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		   m_bVerbose;					/**< Write into the standard OS console? */
		bool		   m_bVerboseLogLevelPrefix;	/**< Indicates if the [LogLevel] prefix should be shown within the standard OS console output */
		String		   m_sFilename;					/**< Filename */
		uint8		   m_nLogLevel;					/**< Current log level mode (see LogLevel) */
		uint8		   m_nFlushLogLevel;			/**< If this log level is used the the flush is explicitly done */
		uint32		   m_nFlushMessages;			/**< Holds the number of messages since the last flush */
		Queue<String>  m_qLastMessages;				/**< Holds the last log messages */
		uint32		   m_nBufferedMessages;			/**< How much messages should be buffered */
		LogFormatter  *m_pLogFormatter;				/**< Holds an instance of a log formatter class, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Log/Log.inl"


#endif // __PLCORE_LOG_H__
