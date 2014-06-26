/*********************************************************\
 *  File: MyTestReporter.cpp                                 *
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

#include <cstdio>
#include "UnitTest++AddIns/MyTestReporter.h"


void MyTestReporter::ReportTestStart(UnitTest::TestDetails const& test)
{
}
void MyTestReporter::ReportFailure(UnitTest::TestDetails const& test, char const* failure)
{
	printf("*!Failed: %s/%s at line %d\n\t*!Msg   : %s\n\n", test.suiteName, test.testName, test.lineNumber, failure);
}
void MyTestReporter::ReportTestFinish(UnitTest::TestDetails const& test, float secondsElapsed)
{
}
void MyTestReporter::ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
{
	if (failureCount > 0)
		printf("FAILURE: %d out of %d tests failed (%d failures).\n", failedTestCount, totalTestCount, failureCount);
	else
		printf("Success: %d tests passed.\n", totalTestCount);

	printf("Test time: %.4f seconds.\n\n", secondsElapsed);
}
