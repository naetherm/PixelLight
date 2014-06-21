/*********************************************************\
 *  File: Stopwatch.inl                                  *
 *      Stopwatch inline implementation
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
*    Default constructor
*/
inline Stopwatch::Stopwatch() :
	m_bRunning(false),
	m_nStart(0),
	m_nStop(0)
{
}

/**
*  @brief
*    Constructor
*/
inline Stopwatch::Stopwatch(bool bStartAtOnce) :
	m_bRunning(false),
	m_nStart(0),
	m_nStop(0)
{
	if (bStartAtOnce)
		Start();
}

/**
*  @brief
*    Destructor
*/
inline Stopwatch::~Stopwatch()
{
}

/**
*  @brief
*    Returns the number of weeks since the stopwatch was started
*/
inline float Stopwatch::GetWeeks() const
{
	return GetDays()/7.0f;
}

/**
*  @brief
*    Returns the number of days since the stopwatch was started
*/
inline float Stopwatch::GetDays() const
{
	return GetHours()/24.0f;
}

/**
*  @brief
*    Returns the number of hours since the stopwatch was started
*/
inline float Stopwatch::GetHours() const
{
	return GetMinutes()/60.0f;
}

/**
*  @brief
*    Returns the number of minutes since the stopwatch was started
*/
inline float Stopwatch::GetMinutes() const
{
	return GetSeconds()/60.0f;
}

/**
*  @brief
*    Returns the number of seconds since the stopwatch was started
*/
inline float Stopwatch::GetSeconds() const
{
	return GetMilliseconds()/1000.0f;
}

/**
*  @brief
*    Returns the number of milliseconds since the stopwatch was started
*/
inline float Stopwatch::GetMilliseconds() const
{
	return GetMicroseconds()/1000.0f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
