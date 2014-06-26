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

#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/String/String.h>
#include "UnitTest++AddIns/MyMobileTestReporter.h"

using namespace PLCore;

void MyMobileTestReporter::ReportTestStart(UnitTest::TestDetails const& test)
{
}
void MyMobileTestReporter::ReportFailure(UnitTest::TestDetails const& test, char const* failure)
{
	System::GetInstance()->GetConsole().Print(String("*!Failed: ") + test.suiteName + "/" + test.testName + " at line " + test.lineNumber + "\n\t*!Msg   : " + failure + "\n\n");
}
void MyMobileTestReporter::ReportTestFinish(UnitTest::TestDetails const& test, float secondsElapsed)
{
}
void MyMobileTestReporter::ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
{
	if (failureCount > 0)
		System::GetInstance()->GetConsole().Print(String("FAILURE: ") + failedTestCount + " out of " + totalTestCount + " tests failed (" + failureCount + " failures).\n");
	else
		System::GetInstance()->GetConsole().Print(String("Success: ") + totalTestCount + " tests passed.\n");

	System::GetInstance()->GetConsole().Print(String("Test time: ") + secondsElapsed + " seconds.\n\n");
}
