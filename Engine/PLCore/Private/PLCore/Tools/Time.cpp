/*********************************************************\
 *  File: Time.cpp                                       *
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
#include "PLCore/String/RegEx.h"
#include "PLCore/Tools/Time.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Time   Time::Null;
const Time   Time::UnixEpochStart(1970, January, 1, Thursday, 0, 0, 0, 0);
const Time   Time::DOSEpochStart (1980, January, 1, Tuesday,  0, 0, 0, 0);
const String Time::ShortMonthName[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const String Time::ShortDayName[7]    = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether or not the given year is a leap year
*/
bool Time::IsLeapYear(uint32 nYear)
{
	// The Gregorian calendar was introduced in 1582...
	// Rule: "Every fourth year is a leap year except for century years that are not divisible by 400"
	return ((nYear >= 1582) ? !(nYear%4) && ((nYear%100) || !(nYear%400)) : !(nYear % 4));
}

/**
*  @brief
*    Returns the number of days in a month
*/
uint8 Time::GetDaysPerMonth(EMonth nMonth, uint32 nYear)
{
	switch (nMonth) {
		case January:
		case March:
		case May:
		case July:
		case August:
		case October:
		case December:
			return 31;

		case February:
			if (nYear > 0 && IsLeapYear(nYear))
				return 29;
			else
				return 28;

		case April:
		case June:
		case September:
		case November:
		default:
			return 30;
	}
}

/**
*  @brief
*    Calculates the day of a week
*/
Time::EDay Time::CalculateDayOfWeek(uint8 nDayOfMonth, EMonth nMonth, uint16 nYear)
{
	int nFebruaryDays = IsLeapYear(nYear) ? 29 : 28;
	int a = nYear - 2000;
	int t = a > 0;
	int b = (a - t)/4 + t;
	int c = (a - t)/100;
	c -= c/4;
	int nDayOfWeek = a + b - c;
	int nCurrentMonth = nMonth;
	while (nCurrentMonth--) {
		if (nCurrentMonth == 1 || nCurrentMonth == 3 || nCurrentMonth == 5 || nCurrentMonth == 7 || nCurrentMonth == 8 || nCurrentMonth == 10)
			nDayOfWeek += 31;
		else
			nDayOfWeek += (nCurrentMonth != 2) ? 30 : nFebruaryDays;
	}
	nDayOfWeek += nDayOfMonth + 3;
	nDayOfWeek %= 7;
	return EDay((nDayOfWeek >= 0) ? nDayOfWeek : nDayOfWeek + 7);
}

//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Time::Time() :
	m_nYear(0),
	m_nMonth(January),
	m_nDayOfMonth(1),
	m_nDayOfWeek(Sunday),
	m_nHour(0),
	m_nMinute(0),
	m_nSecond(0),
	m_nMillisecond(0)
{
}

/**
*  @brief
*    Constructor
*/
Time::Time(uint16 nYear, EMonth nMonth, uint8 nDayOfMonth, EDay nDayOfWeek, uint8 nHour, uint8 nMinute, uint8 nSecond, uint16 nMillisecond) :
	m_nYear(nYear),
	m_nMonth(nMonth),
	m_nDayOfMonth(nDayOfMonth),
	m_nDayOfWeek(nDayOfWeek),
	m_nHour(nHour),
	m_nMinute(nMinute),
	m_nSecond(nSecond),
	m_nMillisecond(nMillisecond)
{
}

/**
*  @brief
*    Returns a string representation of the time
*/
String Time::ToString() const
{
	return String::Format("%.3s %.3s%3d %.2d:%.2d:%.2d %d",
						  ShortDayName[m_nDayOfWeek].GetASCII(), ShortMonthName[m_nMonth-1].GetASCII(),
						  m_nDayOfMonth, m_nHour, m_nMinute, m_nSecond, m_nYear);
}

/**
*  @brief
*    Set time from string representation
*/
void Time::FromString(const String &sString)
{
	// Parse string
	static RegEx cRegEx("(?<dow>\\w+)\\s(?<month>\\w+)\\s(?<day>\\w+)\\s(?<hour>\\w+):(?<min>\\w+):(?<sec>\\w+)\\s(?<year>\\w+)");
	if (cRegEx.Match(sString)) {
		// Read time
		m_nYear		   = static_cast<uint16>(cRegEx.GetNameResult("year") .GetInt());
		m_nMonth	   = static_cast<EMonth>(cRegEx.GetNameResult("month").GetInt());
		m_nDayOfMonth  = static_cast<uint8> (cRegEx.GetNameResult("day")  .GetInt());
		m_nHour		   = static_cast<uint8> (cRegEx.GetNameResult("hour") .GetInt());
		m_nMinute	   = static_cast<uint8> (cRegEx.GetNameResult("min")  .GetInt());
		m_nSecond	   = static_cast<uint8> (cRegEx.GetNameResult("sec")  .GetInt());
		m_nMillisecond = 0;

		// Get day of week
		String sDayOfWeek = cRegEx.GetNameResult("dow");
		int nDay = -1;
		for (int i=0; i<7; i++) {
			// Day of week found?
			if (ShortDayName[i] == sDayOfWeek) {
				nDay = i;
				break;
			}
		}

		// Set day of week, if the string was not found, try to calculate it from the date
		if (nDay >= 0)
			m_nDayOfWeek = static_cast<EDay>(nDay);
		else
			m_nDayOfWeek = Time::CalculateDayOfWeek(m_nDayOfMonth, m_nMonth, m_nYear);
	}
}

/**
*  @brief
*    Converts a date from Unix/POSIX format
*/
void Time::SetUnixDate(uint32 nUnixDate)
{
	// Seconds per day
	static const uint32 SecondsPerDay = 24*60*60;

	// Days per month - nonleap
	static const uint32 DaysPerMonth[12] = {
		0,
		(31),
		(31+28),
		(31+28+31),
		(31+28+31+30),
		(31+28+31+30+31),
		(31+28+31+30+31+30),
		(31+28+31+30+31+30+31),
		(31+28+31+30+31+30+31+31),
		(31+28+31+30+31+30+31+31+30),
		(31+28+31+30+31+30+31+31+30+31),
		(31+28+31+30+31+30+31+31+30+31+30),
	};

	uint32 nWork = nUnixDate%SecondsPerDay;
	m_nSecond = static_cast<uint8>(nWork%60);
	nWork /= 60;
	m_nMinute = static_cast<uint8>(nWork%60);
	m_nHour = static_cast<uint8>(nWork/60);
	nWork = nUnixDate/SecondsPerDay;
	m_nDayOfWeek = static_cast<EDay>((4 + nWork)%7);
	uint32 i = 1970;
	for (;; i++) {
		uint32 k = IsLeapYear(i) ? 366 : 365;
		if (nWork >= k)
			nWork -= k;
		else
			break;
	}
	m_nYear = static_cast<uint16>(i);
	m_nDayOfMonth = 1;
	if (IsLeapYear(i) && (nWork>58)) {
		if (nWork == 59)
			m_nDayOfMonth; // 29.2.
		nWork -= 1;
	}

	for (i=11; i && (DaysPerMonth[i]>nWork); i--)
		;	// Nothing to do
	m_nMonth = static_cast<EMonth>(i+1);
	m_nDayOfMonth = static_cast<uint8>(m_nDayOfMonth + (nWork - DaysPerMonth[i]));
}

/**
*  @brief
*    Converts a date from DOS format
*/
void Time::SetDOSDate(uint32 nDOSDate)
{
	uint32 nDate = nDOSDate >> 16;
	m_nYear        = static_cast<uint16>(((nDate    & 0x0FE00) / 0x0200) + 1980);
	m_nMonth       = static_cast<EMonth>(((nDate    & 0x1E0)   / 0x20) + 1);
	m_nDayOfMonth  = static_cast<uint8>  ((nDate    & 0x1f) + 1);
	m_nDayOfWeek   = CalculateDayOfWeek(m_nDayOfMonth, m_nMonth, m_nYear);
	m_nHour        = static_cast<uint8>  ((nDOSDate & 0xF800)  / 0x800);
	m_nMinute      = static_cast<uint8>  ((nDOSDate & 0x7E0)   / 0x20);
	m_nSecond      = static_cast<uint8>  ((nDOSDate & 0x1f)    * 2);
	m_nMillisecond = 0;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
bool Time::operator ==(const Time &cTime) const
{
	return (m_nYear        == cTime.m_nYear       &&
			m_nMonth       == cTime.m_nMonth      &&
			m_nDayOfMonth  == cTime.m_nDayOfMonth &&
			m_nDayOfWeek   == cTime.m_nDayOfWeek  &&
			m_nHour        == cTime.m_nHour       &&
			m_nMinute      == cTime.m_nMinute     &&
			m_nSecond      == cTime.m_nSecond     &&
			m_nMillisecond == cTime.m_nMillisecond);
}

bool Time::operator !=(const Time &cTime) const
{
	return (m_nYear        != cTime.m_nYear       ||
			m_nMonth       != cTime.m_nMonth      ||
			m_nDayOfMonth  != cTime.m_nDayOfMonth ||
			m_nDayOfWeek   != cTime.m_nDayOfWeek  ||
			m_nHour        != cTime.m_nHour       ||
			m_nMinute      != cTime.m_nMinute     ||
			m_nSecond      != cTime.m_nSecond     ||
			m_nMillisecond != cTime.m_nMillisecond);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
