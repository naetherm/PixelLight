/*********************************************************\
 *  File: ConsoleCommand.h                               *
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


#ifndef __PLENGINE_COMPOSITING_CONSOLECOMMAND_H__
#define __PLENGINE_COMPOSITING_CONSOLECOMMAND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLCore/Base/Func/Functor.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SNConsoleBase;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Console command
*/
class ConsoleCommand {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SNConsoleBase;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*     Console message parameter types
		*
		*  @remarks
		*  @verbatim
		*     A paramCode of "FFBIS" would mean:
		*     Param Type
		*     0     ParamUnknown
		*     1     ParamFloat
		*     2     ParamInt
		*     3     ParamString
		*     4     ParamBool
		*  @endverbatim
		*/
		enum EMsgParamID {
			ParamUnknown,	/**< May not occur! */
			ParamFloat,		/**< 'F' */
			ParamInt,		/**< 'I' */
			ParamString,	/**< 'S' */
			ParamBool		/**< 'B' */
		};


	//[-------------------------------------------------------]
	//[ Public classes                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Console variable
		*/
		class Variable {


			//[-------------------------------------------------------]
			//[ Public data                                           ]
			//[-------------------------------------------------------]
			public:
				float		   f;
				bool		   b;
				int			   i;
				PLCore::String s;


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Variable() :
					f(0.0f), b(false), i(0)
				{
				}

				// [TODO] Implement this?
				Variable &operator =(const Variable &cSource)
				{
					return *this;
				}
				bool operator ==(const Variable &cVar) const
				{
					return false;
				}


		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] bDebug
		*    'true' if it is a debug command, else 'false'
		*  @param[in] sCommand
		*    Command string
		*  @param[in] sParameters
		*    Parameter string
		*  @param[in] sHelp
		*    Help string
		*  @param[in] cFunctor
		*    Execution functor
		*/
		PL_API ConsoleCommand(bool bDebug, const PLCore::String &sCommand, const PLCore::String &sParameters,
							  const PLCore::String &sHelp, const PLCore::Functor<void, ConsoleCommand &> &cFunctor);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PL_API ConsoleCommand(const ConsoleCommand &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API ~ConsoleCommand();

		/**
		*  @brief
		*    Returns the owner console
		*
		*  @return
		*    Owner console, can be a null pointer
		*/
		PL_API SNConsoleBase *GetConsole() const;

		/**
		*  @brief
		*    Returns whether this is a debug command or not
		*
		*  @return
		*    'true' if it is a debug command, else 'false'
		*/
		PL_API bool IsDebug() const;

		/**
		*  @brief
		*    Returns the command string
		*
		*  @return
		*    Command string
		*/
		PL_API PLCore::String GetCommand() const;

		/**
		*  @brief
		*    Returns the parameter string
		*
		*  @return
		*    Parameter string
		*/
		PL_API PLCore::String GetParameters() const;

		/**
		*  @brief
		*    Returns the help string
		*
		*  @return
		*    Help string
		*/
		PL_API PLCore::String GetHelp() const;

		/**
		*  @brief
		*    Returns the execution functor
		*
		*  @return
		*    Command execution functor
		*/
		PL_API PLCore::Functor<void, ConsoleCommand &> &GetFunctor();

		/**
		*  @brief
		*    Check if the params are valid
		*
		*  @return
		*    'true' if the parameters are valid, else 'false'
		*/
		PL_API bool HasValidParams() const;

		/**
		*  @brief
		*    Returns the number of parameters
		*
		*  @return
		*    Number of command parameters
		*/
		PL_API PLCore::uint32 GetNumOfParams() const;

		/**
		*  @brief
		*    Returns the parameter type of parameter <nr>
		*
		*  @param[in] nNr
		*    Vars index (must be in range from 0..GetNumOfParams())
		*
		*  @return
		*    The parameter type of parameter
		*/
		PL_API EMsgParamID GetParamType(PLCore::uint32 nNr) const;

		/**
		*  @brief
		*    Create vars from params (convert string into vars)
		*
		*  @param[in] sParameters
		*    Parameter string
		*/
		PL_API void CreateVarsFromString(const PLCore::String &sParameters);

		/**
		*  @brief
		*    Returns the vars at the given index
		*
		*  @param[in] nNr
		*    Vars index
		*
		*  @return
		*    The vars at the given index
		*
		*  @note
		*    - CreateVarsFromString() must have been called before this function...
		*/
		PL_API Variable &GetVar(PLCore::uint32 nNr) const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This instance
		*/
		PL_API ConsoleCommand &operator =(const ConsoleCommand &cSource);

		/**
		*  @brief
		*    Compare operator
		*
		*  @param[in] cCommand
		*    Command to compare with
		*/
		PL_API bool operator ==(const ConsoleCommand &cCommand) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SNConsoleBase							*m_pConsole;	/**< Owner console, can be a null pointer */
		bool									 m_bDebug;		/**< Is this a debug command? */
		PLCore::String							 m_sCommand;	/**< For example "/fogcolor" */
		PLCore::String							 m_sParameters;	/**< For example "III" in case you want R G B values (3 integers) */
		PLCore::String							 m_sHelp;		/**< For example "<r> <g> <n>" if you want to display "Usage: /fogcolor <r> <g> <b>" as help */
		PLCore::Functor<void, ConsoleCommand &>  m_cFunctor;	/**< The console functor which executes this command, always valid! */
		PLCore::Array<Variable>					 m_lstVars;		/**< Values for the parameters, which the user entered */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_CONSOLECOMMAND_H__
