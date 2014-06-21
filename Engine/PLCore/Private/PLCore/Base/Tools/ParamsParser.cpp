/*********************************************************\
 *  File: ParamsParser.cpp                               *
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
#include "PLCore/Base/Tools/ParamsParser.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parse parameter string
*/
bool ParamsParser::ParseString(const String &sParameters)
{
	// Save parameter string
	m_sParameters = sParameters;
	if (sParameters.GetLength()) {
		// Get first expression
		m_nParsePos = 0;
		return Next();
	}

	// Parameter string is empty
	return false;
}

/**
*  @brief
*    Parse next parameter
*/
bool ParamsParser::Next()
{
	// Parse next expression
	if (m_nParsePos > -1 && m_cRegEx.Match(m_sParameters, m_nParsePos)) {
		// Get expression and new parse position
		m_nParsePos			= m_cRegEx.GetPosition();
		String sExpression	= m_cRegEx.GetResult(0);

		// Process the found expression
		if (sExpression[sExpression.GetLength()-1] == '\'') {
			static RegEx cRegEx("\\s*(\\w*)\\s*=\\s*'?\\s*([^\']*)\\s*'?");
			if (cRegEx.Match(sExpression)) {
				// Get name and value
				m_sName  = cRegEx.GetResult(0);
				m_sValue = cRegEx.GetResult(1);
				return true;
			}
		} else {
			static RegEx cRegEx("\\s*(\\w*)\\s*=\\s*\"?\\s*([^\"]*)\\s*\"?");
			if (cRegEx.Match(sExpression)) {
				// Get name and value
				m_sName  = cRegEx.GetResult(0);
				m_sValue = cRegEx.GetResult(1);
				return true;
			}
		}
	}

	// Error, could not parse next expression
	m_sName		= "";
	m_sValue	= "";
	m_nParsePos	= -1;
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
