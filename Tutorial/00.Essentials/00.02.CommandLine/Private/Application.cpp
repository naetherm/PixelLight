/*********************************************************\
 *  File: Application.cpp                                *
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
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include "Application.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Application)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application::Application() : CoreApplication()
{
	// Register command line options:
	//   02CommandLine [-u/--uppercase] [-p/--prefix <prefix>] <text> [postfix]
	m_cCommandLine.AddParameter	("Prefix",    "-p", "--prefix",		"Text prefix",						"",	false);
	m_cCommandLine.AddFlag		("Uppercase", "-u", "--uppercase",	"Convert the text to upper case",		false);
	m_cCommandLine.AddArgument	("Text",							"Text string",						"",	true);
	m_cCommandLine.AddArgument	("Postfix",							"Text postfix",						"",	false);
}

/**
*  @brief
*    Destructor
*/
Application::~Application()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void Application::Main()
{
	// Get text, prefix and postfix
	String sText    = m_cCommandLine.GetValue("Text");
	String sPrefix  = m_cCommandLine.GetValue("Prefix");
	String sPostfix = m_cCommandLine.GetValue("Postfix");
	if (sPrefix .GetLength() > 0)
		sText = sPrefix + ' ' + sText;
	if (sPostfix.GetLength() > 0)
		sText = sText   + ' ' + sPostfix;

	// Get upper case option
	if (m_cCommandLine.IsValueSet("-u"))
		sText.ToUpper();

	// Print text
	System::GetInstance()->GetConsole().Print("Text: '" + sText + "'\n");

	// Print additional arguments
	if (m_cCommandLine.GetNumOfAdditionalArguments()) {
		System::GetInstance()->GetConsole().Print("\nAddition arguments:\n");
		for (uint32 i=0; i<m_cCommandLine.GetNumOfAdditionalArguments(); i++)
			System::GetInstance()->GetConsole().Print("- '" + m_cCommandLine.GetAdditionalArgument(i) + "'\n");
	}
}
