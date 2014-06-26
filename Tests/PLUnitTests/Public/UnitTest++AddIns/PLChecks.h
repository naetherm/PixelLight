/*********************************************************\
 *  File: PLChecks.h
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

#ifndef __PLCHECKS_H_
#define __PLCHECKS_H_
#pragma once

#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestResults.h>
#include <PLMath/Matrix3x3.h>

namespace UnitTest {
	void CheckMatrix3x3(TestResults& results, PLMath::Matrix3x3 const& expected, PLMath::Matrix3x3 const& actual, float const& tolerance,
		TestDetails const& details);

	void CheckMissingImplementation(TestResults& results, TestDetails const& details);

	template< typename Expected, typename Actual >
	void CheckNotEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
	{
		if (expected == actual)
		{
			UnitTest::MemoryOutStream stream;
			stream << "Expected not equal: " << expected << " == " << actual;

			results.OnTestFailure(details, stream.GetText());
		}
	}

	void CheckNotEqual(TestResults& results, char const* expected, char const* actual, TestDetails const& details);
	void CheckNotEqual(TestResults& results, char* expected, char* actual, TestDetails const& details);
	void CheckNotEqual(TestResults& results, char* expected, char const* actual, TestDetails const& details);
	void CheckNotEqual(TestResults& results, char const* expected, char* actual, TestDetails const& details);

	// PLCore::String::EFormat
	void CheckEqual(TestResults& results, PLCore::String::EFormat const expected, PLCore::String::EFormat const actual, TestDetails const& details);

	// PLCore::String
	void CheckEqual(TestResults& results, PLCore::String const expected, PLCore::String const actual, TestDetails const& details);

	void CheckEqual(TestResults& results, char const* expected, PLCore::String const actual, TestDetails const& details);
	void CheckEqual(TestResults& results, char* expected, PLCore::String const actual, TestDetails const& details);
	void CheckEqual(TestResults& results, PLCore::String const expected, char const* actual,  TestDetails const& details);
	void CheckEqual(TestResults& results, PLCore::String const expected, char* actual, TestDetails const& details);

	void CheckNotEqual(TestResults& results, char const* expected, PLCore::String const actual, TestDetails const& details);
	void CheckNotEqual(TestResults& results, char* expected, PLCore::String const actual, TestDetails const& details);
	void CheckNotEqual(TestResults& results, PLCore::String const expected, char const* actual,  TestDetails const& details);
	void CheckNotEqual(TestResults& results, PLCore::String const expected, char* actual, TestDetails const& details);
}

#endif // __PLCHECKS_H_
