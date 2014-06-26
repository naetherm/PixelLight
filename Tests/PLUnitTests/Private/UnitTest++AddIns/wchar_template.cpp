/*********************************************************\
 *  File: wchar_template.cpp                                *
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
#include <stdlib.h>
#include "UnitTest++AddIns/wchar_template.h"

namespace UnitTest {
	void CheckWcharEqual(TestResults& results, wchar_t const* expected, wchar_t const* actual,
		TestDetails const& details){
			using namespace std;

			if (wcscmp(expected, actual))
			{
				UnitTest::MemoryOutStream stream;

				// Convert expected and actual to a char*
				size_t expectedOrigsize = wcslen(expected) + 1;
				size_t actualOrigsize = wcslen(actual) + 1;

				char *expectedCharstring = new char[expectedOrigsize + 1];
				char *actualCharstring = new char[actualOrigsize + 1];

				size_t newExpectedSize = wcstombs(expectedCharstring, expected, expectedOrigsize);
				size_t newActualSize = wcstombs(actualCharstring, actual, actualOrigsize);

				if (newExpectedSize == -1 || newActualSize == -1) {
					// encountered unicode characters which we cannot display -> show a message
					stream << "Encountered Unicode character, printing wchar_t* values!\n\t\t" << "  Expected wchar_t: " << expected << " but was wchar_t: " << actual;
				} else {
					stream << "Expected wchar_t: " << expectedCharstring << " but was wchar_t: " << actualCharstring;
				}

				free(expectedCharstring);
				free(actualCharstring);

				results.OnTestFailure(details, stream.GetText());
			}
	}

	void CheckEqual(TestResults& results, wchar_t const* expected, wchar_t const* actual, TestDetails const& details){
		CheckWcharEqual(results, expected, actual, details);
	}

	void CheckEqual(TestResults& results, wchar_t* expected, wchar_t* actual, TestDetails const& details){
		CheckWcharEqual(results, expected, actual, details);
	}

	void CheckEqual(TestResults& results, wchar_t* expected, wchar_t const* actual, TestDetails const& details){
		CheckWcharEqual(results, expected, actual, details);
	}

	void CheckEqual(TestResults& results, wchar_t const* expected, wchar_t* actual, TestDetails const& details){
		CheckWcharEqual(results, expected, actual, details);
	}

	void CheckWcharNotEqual(TestResults& results, wchar_t const* expected, wchar_t const* actual,
		TestDetails const& details){
			using namespace std;

			if (!wcscmp(expected, actual))
			{
				UnitTest::MemoryOutStream stream;

				// Convert expected and actual to a char*
				size_t expectedOrigsize = wcslen(expected) + 1;
				size_t actualOrigsize = wcslen(actual) + 1;

				char *expectedCharstring = new char[expectedOrigsize + 1];
				char *actualCharstring = new char[actualOrigsize + 1];

				size_t newExpectedSize = wcstombs(expectedCharstring, expected, expectedOrigsize);
				size_t newActualSize = wcstombs(actualCharstring, actual, actualOrigsize);

				if (newExpectedSize == -1 || newActualSize == -1) {
					// encountered unicode characters which we cannot display -> show a message
					stream << "Encountered Unicode character, printing wchar_t* values!\n\t\t" << "  Expected wchar_t: " << expected << " but was wchar_t: " << actual;
				} else {
					stream << "Expected not equal (wchar_t): " << expectedCharstring << " == " << actualCharstring;
				}

				free(expectedCharstring);
				free(actualCharstring);

				results.OnTestFailure(details, stream.GetText());
			}
	}

	void CheckNotEqual(TestResults& results, wchar_t const* expected, wchar_t const* actual, TestDetails const& details){
		CheckWcharNotEqual(results, expected, actual, details);
	}

	void CheckNotEqual(TestResults& results, wchar_t* expected, wchar_t* actual, TestDetails const& details){
		CheckWcharNotEqual(results, expected, actual, details);
	}

	void CheckNotEqual(TestResults& results, wchar_t* expected, wchar_t const* actual, TestDetails const& details){
		CheckWcharNotEqual(results, expected, actual, details);
	}

	void CheckNotEqual(TestResults& results, wchar_t const* expected, wchar_t* actual, TestDetails const& details){
		CheckWcharNotEqual(results, expected, actual, details);
	}
}