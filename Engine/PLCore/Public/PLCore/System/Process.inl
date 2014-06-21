/*********************************************************\
 *  File: Process.inl                                    *
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
inline Process::Process() :
	m_hProcess(NULL_HANDLE)
{
}

/**
*  @brief
*    Get input stream
*/
inline File &Process::GetInput()
{
	// Open file if not already done
	if (!m_cFileInput.IsOpen())
		m_cFileInput.Open(File::FileWrite | File::FileText);

	// Return file
	return m_cFileInput;
}

/**
*  @brief
*    Get output stream
*/
inline File &Process::GetOutput()
{
	// Open file if not already done
	if (!m_cFileOutput.IsOpen())
		m_cFileOutput.Open(File::FileRead | File::FileText);

	// Return file
	return m_cFileOutput;
}

/**
*  @brief
*    Get error stream
*/
inline File &Process::GetError()
{
	// Open file if not already done
	if (!m_cFileError.IsOpen())
		m_cFileError.Open(File::FileRead | File::FileText);

	// Return file
	return m_cFileError;
}

/**
*  @brief
*    Get input pipe
*/
inline const Pipe &Process::GetPipeInput() const
{
	// Return pipe
	return m_cPipeInput;
}

/**
*  @brief
*    Get output pipe
*/
inline const Pipe &Process::GetPipeOutput() const
{
	// Return pipe
	return m_cPipeOutput;
}

/**
*  @brief
*    Get error pipe
*/
inline const Pipe &Process::GetPipeError() const
{
	// Return pipe
	return m_cPipeError;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
