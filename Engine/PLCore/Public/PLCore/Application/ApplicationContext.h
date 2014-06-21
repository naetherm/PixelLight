/*********************************************************\
 *  File: ApplicationContext.h                           *
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


#ifndef __PLCORE_APPLICATIONCONTEXT_H__
#define __PLCORE_APPLICATIONCONTEXT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Object.h"
#include "PLCore/Core/AbstractContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Application context
*
*  @remarks
*    The application context stores data and information for an application,
*    such as startup information (what was the current directory when the
*    application started) or paths to needed resources.
*/
class ApplicationContext : public Object, public AbstractContext {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, ApplicationContext, "PLCore", PLCore::Object, "Application context")
		#ifdef PLCORE_EXPORTS	// The following is only required when compiling PLCore
			// Methods
			pl_method_0(GetExecutableFilename,	pl_ret_type(String),	"Get absolute path of application executable (native path style, e.g. on Windows: 'C:\MyApplication\x86\Test.exe').",				"")
			pl_method_0(GetExecutableDirectory,	pl_ret_type(String),	"Get directory of executable (native path style, e.g. on Windows: 'C:\MyApplication\x86').",										"")
			pl_method_0(GetAppDirectory,		pl_ret_type(String),	"Get directory of application (native path style, e.g. on Windows: 'C:\MyApplication').",											"")
			pl_method_0(GetStartupDirectory,	pl_ret_type(String),	"Get current directory when the application constructor was called (native path style, e.g. on Windows: 'C:\MyApplication\x86').",	"")
			pl_method_0(GetLogFilename,			pl_ret_type(String),	"Get absolute path to log file, empty if log has not been opened (native path style).",												"")
			pl_method_0(GetConfigFilename,		pl_ret_type(String),	"Get absolute path to config file, empty if no config is used (native path style).",												"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API ApplicationContext();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ApplicationContext();


		//[-------------------------------------------------------]
		//[ Options and data                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get absolute path of application executable
		*
		*  @return
		*    Path to executable (native path style, e.g. on Windows: 'C:\MyApplication\x86\Test.exe')
		*/
		inline String GetExecutableFilename() const;

		/**
		*  @brief
		*    Set absolute path of application executable
		*
		*  @param[in] sExecutableFilename
		*    Path to executable (e.g. on Windows: 'C:\MyApplication\x86\Test.exe', automatically converted internally to native path style)
		*/
		PLCORE_API void SetExecutableFilename(const String &sExecutableFilename);

		/**
		*  @brief
		*    Get command line arguments
		*
		*  @return
		*    List of command line arguments that were passed to the program
		*/
		inline const Array<String> &GetArguments() const;

		/**
		*  @brief
		*    Set command line arguments
		*
		*  @param[in] lstArguments
		*    List of command line arguments that were passed to the program
		*/
		inline void SetArguments(const Array<String> &lstArguments);

		/**
		*  @brief
		*    Get directory of application executable
		*
		*  @return
		*    Directory in which the application executable is (native path style, e.g. on Windows: 'C:\MyApplication\x86')
		*
		*  @remarks
		*    This is just a convenience function and is the same as using
		*      Url(Url(GetExecutableFile()).CutFilename()).Collapse().GetNativePath()
		*/
		inline String GetExecutableDirectory() const;

		/**
		*  @brief
		*    Get directory of application
		*
		*  @return
		*    Directory in which the application is (native path style, e.g. on Windows: 'C:\MyApplication')
		*
		*  @remarks
		*    This is just a convenience function and is the same as using
		*      Url(Url(GetExecutableFile()).CutFilename() + "../").Collapse().GetNativePath()
		*/
		inline String GetAppDirectory() const;

		/**
		*  @brief
		*    Get current directory when the application constructor was called
		*
		*  @return
		*    Current directory that was set when the application constructor was called (native path style, e.g. on Windows: 'C:\MyApplication\x86')
		*/
		inline String GetStartupDirectory() const;

		/**
		*  @brief
		*    Set current directory when the application constructor was called
		*
		*  @param[in] sStartupDirectory
		*    Current directory that was set when the application constructor was called (automatically converted internally to native path style)
		*/
		PLCORE_API void SetStartupDirectory(const String &sStartupDirectory);

		/**
		*  @brief
		*    Get log filename
		*
		*  @return
		*    Absolute path to log file, empty if log has not been opened (native path style)
		*/
		inline String GetLogFilename() const;

		/**
		*  @brief
		*    Set log filename
		*
		*  @param[in] sLog
		*    Absolute path to log file, empty if log has not been opened (automatically converted internally to native path style)
		*/
		PLCORE_API void SetLogFilename(const String &sLog);

		/**
		*  @brief
		*    Get config filename
		*
		*  @return
		*    Absolute path to config file, empty if no config is used (native path style)
		*/
		inline String GetConfigFilename() const;

		/**
		*  @brief
		*    Set config filename
		*
		*  @param[in] sConfig
		*    Absolute path to config file, empty if no config is used (automatically converted internally to native path style)
		*/
		PLCORE_API void SetConfigFilename(const String &sConfig);


		//[-------------------------------------------------------]
		//[ Tool functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets the current directory to the path of the application executable
		*
		*  @note
		*    - Whenever possible, do not manipulate the current directory, this may backfire when you don't expect it
		*    - Because the executable filename is used, which is set within "CoreApplication::Run()",
		*      calling this method from inside a application constructor is not recommended
		*/
		PLCORE_API void ChangeIntoAppDirectory() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		String			m_sExecutableFilename;	/**< Absolute executable filename of the application */
		Array<String>	m_lstArguments;			/**< Argument list */
		String			m_sExecutableDirectory;	/**< Application directory (derived from "m_sExecutableFilename") */
		String			m_sAppDirectory;		/**< Application directory (derived from "m_sExecutableFilename") */
		String			m_sStartupDirectory;	/**< The current directory when the application constructor was called */
		String			m_sLog;					/**< Absolute path to log file, empty if log has not been opened */
		String			m_sConfig;				/**< Absolute path to config file, empty if no config is used */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Application/ApplicationContext.inl"


#endif // __PLCORE_APPLICATIONCONTEXT_H__
