/*********************************************************\
 *  File: SNConsoleBase.h                                *
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


#ifndef __PLENGINE_COMPOSITING_CONSOLEBASE_H__
#define __PLENGINE_COMPOSITING_CONSOLEBASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNode.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ConsoleCommand;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract console base class
*/
class SNConsoleBase : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Console state
		*/
		enum EState {
			Active       = 0,	/**< Console is active */
			Inactive     = 1,	/**< Console is inactive */
			Activating   = 2,	/**< Console is going active */
			Deactivating = 3	/**< Console is going inactive */
		};

		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoDebugCommands = 1<<10	/**< Do not allow debug commands */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(NoDebugCommands, "Do not allow debug commands")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNConsoleBase, "PLEngine", PLScene::SceneNode, "Abstract console base class")
		// Attributes
			// Overwritten SceneNode attributes
		pl_attribute(Flags,	pl_flag_type(EFlags),	NoCulling,	ReadWrite,	GetSet,	"Flags",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether the console is active or not
		*
		*  @return
		*    'true' if the console is active, else 'false'
		*/
		PL_API bool IsActive() const;

		/**
		*  @brief
		*    Activates the console
		*/
		PL_API void Activate();

		/**
		*  @brief
		*    Deactivates the console
		*/
		PL_API void Deactivate();

		/**
		*  @brief
		*    Returns the current console state
		*
		*  @return
		*    Console state
		*/
		PL_API EState GetState() const;

		/**
		*  @brief
		*    Clear the command history
		*/
		PL_API void ClearCommandHistory();

		/**
		*  @brief
		*    Returns the console description
		*
		*  @return
		*    Console description
		*/
		PL_API virtual PLCore::String GetDescription() const;

		/**
		*  @brief
		*    Registers a new command
		*
		*  @param[in] bDebug
		*    Debug command?
		*  @param[in] sCommand
		*    Command string
		*  @param[in] sParameters
		*    Parameter string
		*  @param[in] sHelp
		*    Help string
		*  @param[in] cFunctor
		*    Execution functor
		*
		*  @remarks
		*    Example:\n
		*    RegisterCommand(0, "/fogcolor", "III", "<r> <g> <b>", Functor<void, ConsoleCommand &>(MyCommandFuncton));
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - Have a look at the definition of ConsoleCommand::EMsgParamID for more information
		*      about the command parameters
		*/
		PL_API bool RegisterCommand(bool bDebug, const PLCore::String &sCommand, const PLCore::String &sParameters,
									const PLCore::String &sHelp, const PLCore::Functor<void, ConsoleCommand &> &cFunctor);

		/**
		*  @brief
		*    Register a new command
		*
		*  @param[in] cCommand
		*    Command which should be registered
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - RegisterCommand() above
		*/
		PL_API bool RegisterCommand(const ConsoleCommand &cCommand);

		/**
		*  @brief
		*    Unregisters a command
		*
		*  @param[in] sCommand
		*    Command to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - eg: "/fogcolor"... this will remove the /fogcolor command
		*/
		PL_API bool UnRegisterCommand(const PLCore::String &sCommand);

		/**
		*  @brief
		*    Unregisters all commands
		*/
		PL_API void UnRegisterAllCommands();

		/**
		*  @brief
		*    Returns the number of registered commands
		*
		*  @return
		*    The number of registered commands
		*/
		PL_API PLCore::uint32 GetNumOfCommands() const;

		/**
		*  @brief
		*    Returns the number of parameters in a given string
		*
		*  @param[in] sString
		*    The string which should be checked
		*
		*  @return
		*    Number of parameters in the given string
		*
		*  @remarks
		*    Example: "/fogcolor 100 20 240" will return the value 3
		*/
		PL_API PLCore::uint32 GetNumOfParamsInString(const PLCore::String &sString) const;

		/**
		*  @brief
		*    Process a keyboard message
		*/
		PL_API void ProcessKeyMessage();

		/**
		*  @brief
		*    Process a command
		*
		*  @param[in] sCommand
		*    Command which should be executed
		*
		*  @note
		*    - If 'sCommand' is empty, the console own command line will be executed
		*/
		PL_API void ProcessCommand(const PLCore::String &sCommand = "");

		/**
		*  @brief
		*    Prints a list of all available console commands into the log
		*
		*  @param[in] bDetailed
		*    Should the list have all available information about each command?
		*/
		PL_API void List(bool bDetailed = false);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PL_API SNConsoleBase();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNConsoleBase();

		/**
		*  @brief
		*    Process the syntactical correct command
		*
		*  @param[in] cCommand
		*    The syntactical correct command
		*/
		PL_API void ProcessCommand(ConsoleCommand &cCommand);

		/**
		*  @brief
		*    Completes the command
		*/
		PL_API void CompleteCommand();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// General
		EState		   m_nState;		/**< Current state of console */
		PLCore::String m_sCommand;		/**< Current command string */
		PLCore::String m_sFoundCommand;	/**< Found command string */
		PLCore::uint32 m_nCursor;		/**< Current cursor position */
		int			   m_nSelStart;		/**< Selection cursor start position */
		int			   m_nSelEnd;		/**< Selection cursor end position */

		// Commands
		PLCore::Array<ConsoleCommand*>				     m_lstCommands;	/**< The collection of registered commands */
		PLCore::HashMap<PLCore::String, ConsoleCommand*> m_mapCommands;	/**< Commands hash map */

		// Command history system
		PLCore::uint32				  m_nCurrentCommand;	/**< Current selected command entry */
		PLCore::Array<PLCore::String> m_lstCommandHistory;	/**< Command history list */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_CONSOLEBASE_H__
