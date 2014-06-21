/*********************************************************\
 *  File: CommandLine.cpp                                *
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
#include "PLCore/String/Tokenizer.h"
#include "PLCore/String/RegEx.h"
#include "PLCore/System/System.h"
#include "PLCore/System/Console.h"
#include "PLCore/Tools/CommandLineOption.h"
#include "PLCore/Tools/CommandLine.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Global helper functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add spaces to a string to ensure that it has a certain length
*
*  @param[in] sString
*    Input string
*  @param[in] nLength
*    Desired length of output string
*
*  @return
*    String that has been extended to the given length
*/
String SetStringLength(const String &sString, uint32 nLength)
{
	String sAdd;
	for (uint32 i=sString.GetLength(); i<nLength; i++)
		sAdd = sAdd + ' ';
	return sString + sAdd;
}

/**
*  @brief
*    Get maximum of two values
*
*  @param[in] nA
*    First value
*  @param[in] nB
*    Second value
*
*  @return
*    The greater of the two values
*/
uint32 Max(uint32 nA, uint32 nB)
{
	return (nA > nB ? nA : nB);
}

/**
*  @brief
*    Get minimum of two values
*
*  @param[in] nA
*    First value
*  @param[in] nB
*    Second value
*
*  @return
*    The lower of the two values
*/
uint32 Min(uint32 nA, uint32 nB)
{
	return (nA < nB ? nA : nB);
}

/**
*  @brief
*    Print text with automatic line breaks
*
*  @param[in] nIndent
*    Column at which the text output starts
*  @param[in] sText
*    Text to print
*/
void PrintDescription(uint32 nIndent, const String &sText)
{
	// Print description
	if (nIndent + sText.GetLength() < 79) {
		// Description still fits on the line, just print it out
		System::GetInstance()->GetConsole().Print(sText + '\n');
	} else {
		// Description is too long to fit on the line
			  uint32 nPos   = nIndent;
		const String sEmpty = SetStringLength("", nPos);

		// Get text word for word
		static RegEx cRegEx("\\s*([^\\s]+)");
		uint32 nParsePos = 0;
		while (cRegEx.Match(sText, nParsePos)) {
			// Get next word
						 nParsePos = cRegEx.GetPosition();
			const String sWord     = cRegEx.GetResult(0);
			if (nPos > nIndent) {
				System::GetInstance()->GetConsole().Print(' ');
				nPos++;
			}

			// Break line when necessary
			nPos += sWord.GetLength();
			if (nPos >= 79) {
				System::GetInstance()->GetConsole().Print('\n');
				System::GetInstance()->GetConsole().Print(sEmpty);
				nPos = nIndent + sWord.GetLength();
			}

			// Print word
			System::GetInstance()->GetConsole().Print(sWord);
		}

		// Break line for next option to be consistent to the other if-branch
		System::GetInstance()->GetConsole().Print('\n');
	}
}


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Convert an command array to a string
*/
String CommandLine::ArgumentsToString(const Array<String> &lstArray)
{
	// Are there any elements within the given array?
	if (lstArray.GetNumOfElements()) {
		String sString;

		// Loop through all elements of the given array
		for (uint32 i=0; i<lstArray.GetNumOfElements(); i++) {
			if (i > 0)
				sString += ' ';
			if (sString.IndexOf(' ') >= 0) {
				sString += '\"';
				sString += lstArray[i];
				sString += '\"';
			} else {
				sString += lstArray[i];
			}
		}

		// Return the constructed string
		return sString;
	} else {
		// Return the constructed string - which is astoundingly empty :D
		return "";
	}
}

/**
*  @brief
*    Convert a command string to an array
*/
Array<String> CommandLine::StringToArguments(const String &sCmdLine)
{
	// Initialize the tokenizer
	Tokenizer cTokenizer;
	cTokenizer.Start(sCmdLine);

	// Loop through all tokens
	Array<String> lstArray;
	String sToken = cTokenizer.GetNextToken();
	while (sToken.GetLength()) {
		// Separator found?
		if (sToken == '\'' || sToken == '\"') {
			sToken = cTokenizer.GetNextToken();
			lstArray.Add(sToken);
			sToken = cTokenizer.GetNextToken();
		} else {
			lstArray.Add(sToken);
		}

		// Next token, please
		sToken = cTokenizer.GetNextToken();
	}
	cTokenizer.Stop();

	// Return the constructed string array
	return lstArray;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Delete all options
*/
void CommandLine::Clear()
{
	// Delete options
	for (uint32 i=0; i<m_lstOptions.GetNumOfElements(); i++)
		delete m_lstOptions[i];

	// Clear lists
	m_lstOptions.Clear();
	m_mapOptions.Clear();
	m_lstParameters.Clear();
	m_bError = false;
}

/**
*  @brief
*    Add parameter
*/
bool CommandLine::AddParameter(const String &sName, const String &sShort, const String &sLong, const String &sDescription, const String &sDefault, bool bRequired)
{
	// Check that a name and at least one flag name is given and the names have not been used before
	if ( (sName .GetLength() && m_mapOptions.Get(sName) == nullptr) && (sShort.GetLength() || sLong.GetLength()) &&
		 (sShort.GetLength() == 0 || m_mapOptions.Get(sShort) == nullptr) &&
		 (sLong .GetLength() == 0 || m_mapOptions.Get(sLong)  == nullptr) )
	{
		// Create option
		CommandLineOption *pOption = new CommandLineOption();
		pOption->SetType(CommandLineOption::OptionParam);
		pOption->SetRequired(bRequired);
		pOption->SetName(sName);
		pOption->SetShortName(sShort);
		pOption->SetLongName(sLong);
		pOption->SetDescription(sDescription);
		pOption->SetDefault(sDefault);

		// Add option to lists
		m_lstOptions.Add(pOption);
		m_mapOptions.Add(sName, pOption);
		if (sShort.GetLength())
			m_mapOptions.Add(sShort, pOption);
		if (sLong .GetLength())
			m_mapOptions.Add(sLong,  pOption);
		return true;
	}

	// Error, wrong arguments given
	return false;
}

/**
*  @brief
*    Add flag (on/off)
*/
bool CommandLine::AddFlag(const String &sName, const String &sShort, const String &sLong, const String &sDescription, bool bRequired)
{
	// Check that a name and at least one flag name is given and the names have not been used before
	if ( (sName .GetLength() && m_mapOptions.Get(sName) == nullptr) && (sShort.GetLength() || sLong.GetLength()) &&
		 (sShort.GetLength() == 0 || m_mapOptions.Get(sShort) == nullptr) &&
		 (sLong .GetLength() == 0 || m_mapOptions.Get(sLong)  == nullptr) )
	{
		// Create option
		CommandLineOption *pOption = new CommandLineOption();
		pOption->SetType(CommandLineOption::OptionFlag);
		pOption->SetRequired(bRequired);
		pOption->SetName(sName);
		pOption->SetShortName(sShort);
		pOption->SetLongName(sLong);
		pOption->SetDescription(sDescription);

		// Add option to lists
		m_lstOptions.Add(pOption);
		m_mapOptions.Add(sName, pOption);
		if (sShort.GetLength())
			m_mapOptions.Add(sShort, pOption);
		if (sLong .GetLength())
			m_mapOptions.Add(sLong,  pOption);
		return true;
	}

	// Error, wrong arguments given
	return false;
}

/**
*  @brief
*    Add argument
*/
bool CommandLine::AddArgument(const String &sName, const String &sDescription, const String &sDefault, bool bRequired)
{
	// Check that a name is given and has not been used before
	if (sName.GetLength() && m_mapOptions.Get(sName) == nullptr) {
		// Create option
		CommandLineOption *pOption = new CommandLineOption();
		pOption->SetType(CommandLineOption::OptionArgument);
		pOption->SetRequired(bRequired);
		pOption->SetName(sName);
		pOption->SetDescription(sDescription);
		pOption->SetDefault(sDefault);

		// Add option to lists
		m_lstOptions.Add(pOption);
		if (sName.GetLength())
			m_mapOptions.Add(sName, pOption);
		return true;
	}

	// Error, wrong arguments given
	return false;
}

/**
*  @brief
*    Parse command line arguments
*/
bool CommandLine::ParseCommandLine(const Array<String> &lstArgs)
{
	// Clear parameters
	m_lstParameters.Clear();

	// Set default values and get enumerate arguments
	List<CommandLineOption*> lstArguments;
	for (uint32 i=0; i<m_lstOptions.GetNumOfElements(); i++) {
		// Set default value
		m_lstOptions[i]->SetValue(m_lstOptions[i]->GetDefault());

		// Add arguments to list
		if (m_lstOptions[i]->GetType() == CommandLineOption::OptionArgument)
			lstArguments.Add(m_lstOptions[i]);
	}

	// Loop through arguments
	m_bError = false;
	uint32 nArgument = 0;
	CommandLineOption *pLastOption = nullptr;
	String sLastOption;
	for (uint32 i=0; i<lstArgs.GetNumOfElements(); i++) {
		// Get argument
		String sArg = lstArgs[i];
		String sOption  = "";
		String sOptions = "";
		String sValue   = "";
		if (sArg.GetSubstring(0, 2) == "--") {
			// Long option found
			sOptions = ""; 
			sOption  = sArg;
			sValue   = "";
		} else if (sArg.GetSubstring(0, 1) == '-') {
			// Short option(s) found
			sOptions = sArg.GetSubstring(2);
			sOption  = sArg.GetSubstring(0, 2);
			sValue   = "";
		} else {
			sOptions = "";
			sOption  = "";
			sValue   = sArg;
		}

		// Get options
		while (sOption.GetLength()) {
			// Process option
			if (pLastOption) {
				// Error: There was still an option waiting for it's value!
				System::GetInstance()->GetConsole().Print("Parameter " + sLastOption + " needs a value!\n");
				m_bError = true;
				return m_bError;
			} else {
				// Get option
				pLastOption = GetOption(sOption);
				sLastOption = sOption;
				if (!pLastOption) {
					// Error: Unknown option!
					System::GetInstance()->GetConsole().Print("Unknown parameter " + sOption + "!\n");
					m_bError = true;
					return m_bError;
				}

				// Set 'true' for boolean options or wait for a value
				if (pLastOption->GetType() == CommandLineOption::OptionFlag) {
					pLastOption->SetValue("true");
					pLastOption = nullptr;
					sLastOption = "";
				}
			}

			// Next option available?
			if (sOptions.GetLength()) {
				// Yes, get next option
				sOption  = '-' + sOptions.GetSubstring(0, 1);
				sOptions = sOptions.GetSubstring(1);
			} else {
				// No more options
				sOption = "";
			}
		}

		// Get value
		if (sValue.GetLength()) {
			// Process value
			if (pLastOption) {
				// Set option value
				pLastOption->SetValue(sValue);
				pLastOption = nullptr;
				sLastOption = "";
			} else {
				// Argument
				if (nArgument < lstArguments.GetNumOfElements()) {
					// Set value of defined argument
					lstArguments[nArgument]->SetValue(sValue);
					nArgument++;
				} else {
					// Add additional argument
					m_lstParameters.Add(sValue);
				}
			}
		}
	}

	// Produce an error if not all required options have been set
	for (uint32 i=0; i<m_lstOptions.GetNumOfElements() && !m_bError; i++) {
		// Check if option has been set
		if (m_lstOptions[i]->IsRequired() && !m_lstOptions[i]->IsSet())
			m_bError = true;
	}

	// Return error-status
	return m_bError;
}

/**
*  @brief
*    Check if an option value is set ('true' for boolean options or any other than "" for string values)
*/
bool CommandLine::IsValueSet(const String &sName) const
{
	// Get option
	const CommandLineOption *pOption = GetOption(sName);
	return pOption ? pOption->IsSet() : false;
}

/**
*  @brief
*    Get option value
*/
String CommandLine::GetValue(const String &sName) const
{
	// Get option
	const CommandLineOption *pOption = GetOption(sName);
	return pOption ? pOption->GetValue() : "";
}

/**
*  @brief
*    Display a help text with all available options on the console
*/
void CommandLine::PrintHelp(const String &sProgramName) const
{
	// Print synopsis
	System::GetInstance()->GetConsole().Print("Usage: " + sProgramName + " [OPTIONS]");
	for (uint32 i=0; i<m_lstOptions.GetNumOfElements(); i++) {
		const CommandLineOption *pOption = m_lstOptions[i];
		if (pOption->GetType() == CommandLineOption::OptionArgument) {
			if (pOption->IsRequired())
				System::GetInstance()->GetConsole().Print(" <" + pOption->GetName() + ">");
			else
				System::GetInstance()->GetConsole().Print(" [" + pOption->GetName() + "]");
		}
	}
	System::GetInstance()->GetConsole().Print("\n\n");

	// Do not print anything, if no options are available
	if (m_lstOptions.GetNumOfElements() > 0) {
		// Get maximum size of option names
		uint32 nMaxName  = 1;
		uint32 nMaxShort = 2;
		uint32 nMaxLong  = 4;
		uint32 nMaxAll   = 1;
		for (uint32 i=0; i<m_lstOptions.GetNumOfElements(); i++) {
			const CommandLineOption *pOption = m_lstOptions[i];
			if (pOption->GetType() == CommandLineOption::OptionFlag) {
				nMaxShort = Max(pOption->GetShortName().GetLength(), nMaxShort);
				nMaxLong  = Max(pOption->GetLongName() .GetLength(), nMaxLong );
			} else if (pOption->GetType() == CommandLineOption::OptionParam) {
				nMaxShort = Max(pOption->GetShortName().GetLength(), nMaxShort);
				nMaxLong  = Max(pOption->GetLongName() .GetLength(), nMaxLong );
				nMaxName  = Max(pOption->GetName().GetLength(),		 nMaxName);
			} else if (pOption->GetType() == CommandLineOption::OptionArgument) {
				nMaxAll   = Max(pOption->GetName().GetLength(),		 nMaxAll);
			}
		}
		nMaxAll = Max(nMaxShort + nMaxLong + nMaxName + 5, nMaxAll);

		// Print arguments
		System::GetInstance()->GetConsole().Print("Arguments:\n");
		for (uint32 i=0; i<m_lstOptions.GetNumOfElements(); i++) {
			// Get option
			CommandLineOption *pOption = m_lstOptions[i];
			if (pOption->GetType() == CommandLineOption::OptionArgument) {
				// Print option
				String sInfo = "  " + SetStringLength(pOption->GetName(), nMaxAll) + "  ";
				System::GetInstance()->GetConsole().Print(sInfo);

				// Print description
				PrintDescription(sInfo.GetLength(), pOption->GetDescription());
			}
		}
		System::GetInstance()->GetConsole().Print('\n');

		// Print options
		System::GetInstance()->GetConsole().Print("Available options:\n");
		for (uint32 i=0; i<m_lstOptions.GetNumOfElements(); i++) {
			// Get option
			CommandLineOption *pOption = m_lstOptions[i];
			if (pOption->GetType() == CommandLineOption::OptionFlag || pOption->GetType() == CommandLineOption::OptionParam) {
				// Print option
				String sName  = pOption->GetName();
				bool bComma = (pOption->GetShortName().GetLength() > 0 && pOption->GetLongName().GetLength() > 0);
				String sShort = SetStringLength(pOption->GetShortName(), nMaxShort);
				String sLong  = SetStringLength(pOption->GetLongName (), nMaxLong );
				String sInfo  = sShort + (bComma ? ", " : "  ") + sLong;
				if (pOption->GetType() == CommandLineOption::OptionParam)
					sInfo += " <" + sName + ">";
				sInfo = "  " + SetStringLength(sInfo, nMaxAll) + "  ";
				System::GetInstance()->GetConsole().Print(sInfo);

				// Print description
				PrintDescription(sInfo.GetLength(), pOption->GetDescription());
			}
		}
		System::GetInstance()->GetConsole().Print('\n');
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
