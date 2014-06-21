/*********************************************************\
 *  File: Time.inl                                       *
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
*    Destructor
*/
inline Time::~Time()
{
}

/**
*  @brief
*    Get the year
*/
inline uint16 Time::GetYear() const
{
	return m_nYear;
}

/**
*  @brief
*    Set the year
*/
inline void Time::SetYear(uint16 nYear)
{
	m_nYear = nYear;
}

/**
*  @brief
*    Get the month
*/
inline Time::EMonth Time::GetMonth() const
{
	return m_nMonth;
}

/**
*  @brief
*    Set the month
*/
inline void Time::SetMonth(EMonth nMonth)
{
	m_nMonth = nMonth;
}

/**
*  @brief
*    Get the day of the month
*/
inline uint8 Time::GetDayOfMonth() const
{
	return m_nDayOfMonth;
}

/**
*  @brief
*    Set the day of the month
*/
inline void Time::SetDayOfMonth(uint8 nDayOfMonth)
{
	if (nDayOfMonth >= 1 && nDayOfMonth <= 31)
		m_nDayOfMonth = nDayOfMonth;
}

/**
*  @brief
*    Get the day of the week
*/
inline Time::EDay Time::GetDayOfWeek() const
{
	return m_nDayOfWeek;
}

/**
*  @brief
*    Set the day of the week
*/
inline void Time::SetDayOfWeek(EDay nDayOfWeek)
{
	m_nDayOfWeek = nDayOfWeek;
}

/**
*  @brief
*    Get the hour
*/
inline uint8 Time::GetHour() const
{
	return m_nHour;
}

/**
*  @brief
*    Set the hour
*/
inline void Time::SetHour(uint8 nHour)
{
	if (nHour <= 23)
		m_nHour = nHour;
}

/**
*  @brief
*    Get the minute
*/
inline uint8 Time::GetMinute() const
{
	return m_nMinute;
}

/**
*  @brief
*    Set the minute
*/
inline void Time::SetMinute(uint8 nMinute)
{
	if (nMinute <= 59)
		m_nMinute = nMinute;
}

/**
*  @brief
*    Get the second
*/
inline uint8 Time::GetSecond() const
{
	return m_nSecond;
}

/**
*  @brief
*    Set the second
*/
inline void Time::SetSecond(uint8 nSecond)
{
	if (nSecond <= 59)
		m_nSecond = nSecond;
}

/**
*  @brief
*    Get the millisecond
*/
inline uint16 Time::GetMillisecond() const
{
	return m_nMillisecond;
}

/**
*  @brief
*    Set the millisecond
*/
inline void Time::SetMillisecond(uint16 nMillisecond)
{
	if (nMillisecond <= 999)
		m_nMillisecond = nMillisecond;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
