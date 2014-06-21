/*********************************************************\
 *  File: Pipe.inl                                       *
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
inline Pipe::Pipe()
{
	// Initialize handles
	m_hPipe[0] = INVALID_HANDLE;
	m_hPipe[1] = INVALID_HANDLE;
}

/**
*  @brief
*    Copy constructor
*/
inline Pipe::Pipe(const Pipe &cSource)
{
	// Copy Pipe
	m_sName		= cSource.m_sName;
	m_hPipe[0]  = cSource.m_hPipe[0];
	m_hPipe[1]  = cSource.m_hPipe[1];
}

/**
*  @brief
*    Destructor
*/
inline Pipe::~Pipe()
{
}

/**
*  @brief
*    Assignment operator
*/
inline Pipe &Pipe::operator =(const Pipe &cSource)
{
	// Copy Pipe
	m_sName		= cSource.m_sName;
	m_hPipe[0]  = cSource.m_hPipe[0];
	m_hPipe[1]  = cSource.m_hPipe[1];
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
inline bool Pipe::operator ==(const Pipe &cSource) const
{
	// Compare
	return (m_sName == cSource.m_sName && m_hPipe[0] == cSource.m_hPipe[0] && m_hPipe[1] == cSource.m_hPipe[1]);
}

/**
*  @brief
*    Comparison operator
*/
inline bool Pipe::operator !=(const Pipe &cSource) const
{
	// Compare
	return (m_sName != cSource.m_sName || m_hPipe[0] != cSource.m_hPipe[0] || m_hPipe[1] != cSource.m_hPipe[1]);
}

/**
*  @brief
*    Open a pipe by file handles
*/
inline bool Pipe::Open(handle hRead, handle hWrite)
{
	// Save file handles
	m_hPipe[0] = hRead;
	m_hPipe[1] = hWrite;

	// Done
	return true;
}

/**
*  @brief
*    Close both sides of the pipe
*/
inline bool Pipe::Close()
{
	// Close read and write
	return (CloseRead() && CloseWrite());
}

/**
*  @brief
*    Get name of pipe
*/
inline String Pipe::GetName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Get read handle for the pipe
*/
inline handle Pipe::GetReadHandle() const
{
	// Return read-end
	return m_hPipe[0];
}

/**
*  @brief
*    Get write handle for the pipe
*/
inline handle Pipe::GetWriteHandle() const
{
	// Return write-end
	return m_hPipe[1];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
