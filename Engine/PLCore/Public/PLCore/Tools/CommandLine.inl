/*********************************************************\
 *  File: CommandLine.inl                                *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline CommandLine::CommandLine() :
	m_bError(false)
{
}

/**
*  @brief
*    Destructor
*/
inline CommandLine::~CommandLine()
{
	// Clear options
	Clear();
}

/**
*  @brief
*    Get number of registered options
*/
inline uint32 CommandLine::GetNumOfOptions() const
{
	// Return number of options stored in our array
	return m_lstOptions.GetNumOfElements();
}

/**
*  @brief
*    Get option by index
*/
inline CommandLineOption *CommandLine::GetOption(uint32 nIndex) const
{
	// Return option
	return m_lstOptions[nIndex];
}

/**
*  @brief
*    Get option by name
*/
inline CommandLineOption *CommandLine::GetOption(const String &sName) const
{
	// Return option
	return m_mapOptions.Get(sName);
}

/**
*  @brief
*    Check if there were any errors parsing the command line arguments
*/
inline bool CommandLine::HasErrors() const
{
	// Return error flag
	return m_bError;
}

/**
*  @brief
*    Get number of additional arguments that have been defined
*/
inline uint32 CommandLine::GetNumOfAdditionalArguments() const
{
	// Return number of additional arguments
	return m_lstParameters.GetNumOfElements();
}

/**
*  @brief
*    Get additional argument
*/
inline String CommandLine::GetAdditionalArgument(uint32 nIndex) const
{
	// Return argument
	return m_lstParameters[nIndex];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
