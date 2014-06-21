/*********************************************************\
 *  File: ParamsParser.h                                 *
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


#ifndef __PLCORE_TOOLS_PARAMSPARSER_H__
#define __PLCORE_TOOLS_PARAMSPARSER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/RegEx.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class for parsing parameters
*
*  @remarks
*    This class can parse parameter lists given as strings (e.g. "param1=\"hello\" param2=10")
*/
class ParamsParser {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline ParamsParser();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ParamsParser();

		/**
		*  @brief
		*    Parse parameter string
		*
		*  @param[in] sParameters
		*    String containing parameters (e.g. "Param0=\"Hello\" Param1=10)
		*
		*  @return
		*    'true' if the string could be parsed, else 'false'
		*
		*  @remarks
		*    This will first check if the string is a valid parameter string
		*    and then parse the first parameter and value. If the string is not
		*    valid or empty, the function will return 'false'.
		*/
		PLCORE_API bool ParseString(const String &sParameters);

		/**
		*  @brief
		*    Parse next parameter
		*
		*  @return
		*    'true' if the next name/value pair could be parsed, else 'false'
		*/
		PLCORE_API bool Next();

		/**
		*  @brief
		*    Check if the current (last parsed) parameter is valid
		*
		*  @return
		*    'true' if there is a current parameter, else 'false'
		*/
		inline bool HasParam() const;

		/**
		*  @brief
		*    Get name of currently parsed parameter
		*
		*  @return
		*    Parameter name
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Get value of currently parsed parameter
		*
		*  @return
		*    Parameter name
		*/
		inline String GetValue() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String	m_sParameters;	/**< Parsed parameter string */
		RegEx	m_cRegEx;		/**< Regular expression for parsing a parameter string */
		long	m_nParsePos;	/**< Current parsing position (-1 for inactive) */
		String	m_sName;		/**< Name of next parameter */
		String	m_sValue;		/**< Value of next parameter */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Base/Tools/ParamsParser.inl"


#endif // __PLCORE_TOOLS_PARAMSPARSER_H__
