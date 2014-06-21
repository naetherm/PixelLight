/*********************************************************\
 *  File: ConsoleCommand.cpp                             *
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
#include <PLCore/Log/Log.h>
#include <PLCore/String/Tokenizer.h>
#include "PLEngine/Compositing/Console/ConsoleCommand.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ConsoleCommand::ConsoleCommand(bool bDebug, const String &sCommand, const String &sParameters, const String &sHelp, const Functor<void, ConsoleCommand &> &cFunctor) :
	m_pConsole(nullptr),
	m_bDebug(bDebug),
	m_sCommand(sCommand),
	m_sParameters(sParameters),
	m_sHelp(sHelp),
	m_cFunctor(cFunctor)
{
}

/**
*  @brief
*    Copy constructor
*/
ConsoleCommand::ConsoleCommand(const ConsoleCommand &cSource) :
	m_pConsole(nullptr)
{
	*this = cSource;
}

/**
*  @brief
*    Destructor
*/
ConsoleCommand::~ConsoleCommand()
{
	m_lstVars.Clear();
}

/**
*  @brief
*    Returns the owner console
*/
SNConsoleBase *ConsoleCommand::GetConsole() const
{
	return m_pConsole;
}

/**
*  @brief
*    Returns whether this is a debug command or not
*/
bool ConsoleCommand::IsDebug() const
{
	return m_bDebug;
}

/**
*  @brief
*    Returns the command string
*/
String ConsoleCommand::GetCommand() const
{
	return m_sCommand;
}

/**
*  @brief
*    Returns the parameter string
*/
String ConsoleCommand::GetParameters() const
{
	return m_sParameters;
}

/**
*  @brief
*    Returns the help string
*/
String ConsoleCommand::GetHelp() const
{
	return m_sHelp;
}

/**
*  @brief
*    Returns the execution functor
*/
Functor<void, ConsoleCommand &> &ConsoleCommand::GetFunctor()
{
	return m_cFunctor;
}

/**
*  @brief
*    Check if the params are valid
*/
bool ConsoleCommand::HasValidParams() const
{
	// Check for unknown or invalid parameters
	for (uint32 i=0; i<GetNumOfParams(); i++) {
		if (GetParamType(i) == ParamUnknown)
			return false; // There's an invalid parameter!
	}

	// All parameters are valid
	return true;
}

/**
*  @brief
*    Returns the number of parameters
*/
uint32 ConsoleCommand::GetNumOfParams() const
{
	return m_sParameters.GetLength();
}

/**
*  @brief
*    Returns the parameter type of parameter <nr>
*/
ConsoleCommand::EMsgParamID ConsoleCommand::GetParamType(uint32 nNr) const
{
	switch (m_sParameters[nNr]) {
		case 'S':
			return ParamString;

		case 'I':
			return ParamInt;

		case 'F':
			return ParamFloat;

		case 'B':
			return ParamBool;

		default:
			return ParamUnknown;
	}
}

/**
*  @brief
*    Create vars from params (convert string into vars)
*/
void ConsoleCommand::CreateVarsFromString(const String &sParameters)
{
	// Resize the parameter list
	m_lstVars.Resize(GetNumOfParams());

	// Parse the given parameters
	Tokenizer cTokenizer;
	uint32 nParameter = 0;
	cTokenizer.Start(sParameters);
	while (cTokenizer.GetNextToken().GetLength()) {
		Variable &cVar = m_lstVars[nParameter];
		switch (GetParamType(nParameter)) {
			case ParamInt:
				cVar.i = cTokenizer.GetToken().GetInt();
				break;

			case ParamFloat:
				cVar.f = cTokenizer.GetToken().GetFloat();
				break;

			case ParamBool:
				cVar.b = cTokenizer.GetToken().GetBool();
				break;

			case ParamString:
				cVar.s = cTokenizer.GetToken();
				break;

			case ParamUnknown:
			default:
				PL_LOG(Warning, "Using unknown type in console message conversion!")
				break;
		}
		nParameter++;
	}
	cTokenizer.Stop();
}

/**
*  @brief
*    Returns the vars at the given index
*/
ConsoleCommand::Variable &ConsoleCommand::GetVar(uint32 nNr) const
{
	return m_lstVars[nNr];
}

/**
*  @brief
*    Copy operator
*/
ConsoleCommand &ConsoleCommand::operator =(const ConsoleCommand &cSource)
{
	// Copy data
	m_bDebug      = cSource.m_bDebug;
	m_sCommand    = cSource.m_sCommand;
	m_sParameters = cSource.m_sParameters;
	m_sHelp       = cSource.m_sHelp;
	m_cFunctor    = cSource.m_cFunctor;
	m_lstVars     = cSource.m_lstVars;

	// Return this
	return *this;
}

/**
*  @brief
*    Compare operator
*/
bool ConsoleCommand::operator ==(const ConsoleCommand &cCommand) const
{
	return m_bDebug      == cCommand.m_bDebug      &&
		   m_sCommand    == cCommand.m_sCommand    &&
		   m_sParameters == cCommand.m_sParameters &&
		   m_sHelp       == cCommand.m_sHelp;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
