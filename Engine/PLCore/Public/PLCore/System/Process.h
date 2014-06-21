/*********************************************************\
 *  File: Process.h                                      *
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


#ifndef __PLCORE_PROCESS_H__
#define __PLCORE_PROCESS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/System/Pipe.h"
#include "PLCore/File/File.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class for running and controlling an external process
*/
class Process {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline Process();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~Process();

		/**
		*  @brief
		*    Starts a process
		*
		*  @param[in] sCommand
		*    Command to run
		*  @param[in] sArguments
		*    Arguments for the command
		*/
		PLCORE_API void Execute(const String &sCommand, const String &sArguments);

		/**
		*  @brief
		*    Starts a process with redirected input/output
		*
		*  @param[in] sCommand
		*    Command to run
		*  @param[in] sArguments
		*    Arguments for the command
		*
		*  @remarks
		*    This function creates unnamed pipes to redirect input and output. To access
		*    theses pipes, use the appropriate functions like GetInput(), GetOutput() a.s.o.
		*    If you want to specify the pipes by yourself, see the other two variations of
		*    ExecuteRedirectIO().
		*/
		PLCORE_API void ExecuteRedirectIO(const String &sCommand, const String &sArguments);

		/**
		*  @brief
		*    Starts a process with redirected input/output
		*
		*  @param[in] sCommand
		*    Command to run
		*  @param[in] sArguments
		*    Arguments for the command
		*  @param[in] cPipeIn
		*    Pipe for input
		*  @param[in] cPipeOut
		*    Pipe for output
		*  @param[in] cPipeErr
		*    Pipe for error
		*/
		PLCORE_API void ExecuteRedirectIO(const String &sCommand, const String &sArguments,
										  const Pipe &cPipeIn, const Pipe &cPipeOut, const Pipe &cPipeErr);

		/**
		*  @brief
		*    Starts a process with redirected input/output
		*
		*  @param[in] sCommand
		*    Command to run
		*  @param[in] sArguments
		*    Arguments for the command
		*  @param[in] hPipeIn
		*    System handle for the input pipe
		*  @param[in] hPipeOut
		*    System handle for the output pipe
		*  @param[in] hPipeErr
		*    System handle for the error pipe
		*
		*  @note
		*    - On Linux, the handle is a file handle of type int
		*    - On Windows, the handle is a file handle of type HANDLE
		*/
		PLCORE_API void ExecuteRedirectIO(const String &sCommand, const String &sArguments,
										  handle hPipeIn, handle hPipeOut, handle hPipeErr);

		/**
		*  @brief
		*    Returns if the process is running
		*
		*  @return
		*    'true' if running, else 'false'
		*/
		PLCORE_API bool IsRunning() const;

		/**
		*  @brief
		*    Terminates the process
		*/
		PLCORE_API void Terminate();

		/**
		*  @brief
		*    Get input stream
		*
		*  @return
		*    Input file for the process
		*/
		inline File &GetInput();

		/**
		*  @brief
		*    Get output stream
		*
		*  @return
		*    Output file for the process
		*/
		inline File &GetOutput();

		/**
		*  @brief
		*    Get error stream
		*
		*  @return
		*    Error file for the process
		*/
		inline File &GetError();

		/**
		*  @brief
		*    Get input pipe
		*
		*  @return
		*    Input pipe for the process
		*/
		inline const Pipe &GetPipeInput() const;

		/**
		*  @brief
		*    Get output pipe
		*
		*  @return
		*    Output pipe for the process
		*/
		inline const Pipe &GetPipeOutput() const;

		/**
		*  @brief
		*    Get error pipe
		*
		*  @return
		*    Error pipe for the process
		*/
		inline const Pipe &GetPipeError() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Process(const Process &cSource);

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
		Process &operator =(const Process &cSource);

		/**
		*  @brief
		*    Creates a process with redirected IO
		*
		*  @param[in] sCommand
		*    Command to run
		*  @param[in] sArguments
		*    Arguments for the command
		*  @param[in] hPipeInRd
		*    System handle for the input pipe (read-end)
		*  @param[in] hPipeInWr
		*    System handle for the input pipe (write-end)
		*  @param[in] hPipeOutRd
		*    System handle for the output pipe (read-end)
		*  @param[in] hPipeOutWr
		*    System handle for the output pipe (write-end)
		*  @param[in] hPipeErrRd
		*    System handle for the error pipe (read-end)
		*  @param[in] hPipeErrWr
		*    System handle for the error pipe (write-end)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool CreateProcessRedirectIO(const String &sCommand, const String &sArguments,
									 handle hPipeInRd,  handle hPipeInWr,
									 handle hPipeOutRd, handle hPipeOutWr,
									 handle hPipeErrRd, handle hPipeErrWr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input/output redirection
		Pipe m_cPipeInput;		/**< Pipe 'Input' */
		Pipe m_cPipeOutput;		/**< Pipe 'Output' */
		Pipe m_cPipeError;		/**< Pipe 'Error' */
		File m_cFileInput;		/**< File 'Input' */
		File m_cFileOutput;		/**< File 'Output' */
		File m_cFileError;		/**< File 'Error' */

		// System dependent process handle
		#if defined(LINUX)
			int m_hProcess;
		#elif defined(WIN32)
			handle m_hProcess;
		#endif


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/System/Process.inl"


#endif // __PLCORE_PROCESS_H__
