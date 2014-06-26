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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <iostream>
#include <fstream>
#include "PLCore/Container/HashMap.h"
#include "UnitTest++AddIns/MyPerformanceReporter.h"
#include "PLCore/Tools/Stopwatch.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace std;
using namespace PLCore;

//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
extern ofstream outputFile;
string lastsuitename;
Stopwatch Counter;

void MyPerformanceReporter::ReportTestStart(UnitTest::TestDetails const& test)
{
	Counter.Start();
}
void MyPerformanceReporter::ReportFailure(UnitTest::TestDetails const& test, char const* failure)
{
	printf("*!Failed: %s/%s at line %d\n\t*!Msg   : %s\n\n", test.suiteName, test.testName, test.lineNumber, failure);
}
void MyPerformanceReporter::ReportTestFinish(UnitTest::TestDetails const& test, float secondsElapsed)
{
	Counter.Stop();

	if (lastsuitename != test.suiteName){
		outputFile << endl << endl << test.suiteName <<endl;
		lastsuitename = test.suiteName;
	}
	outputFile << test.testName << ";" << (secondsElapsed*1000) << "ms" << endl;
	
}
void MyPerformanceReporter::ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
{
	outputFile << endl;
	if (failureCount > 0)
		outputFile << "FAILURE: " << failedTestCount << " out of " << totalTestCount << " tests failed ("<< failureCount <<" failures).\n";
	else
		outputFile << "Success: "<< totalTestCount <<" tests passed.\n";

	outputFile << "Test time: "<< secondsElapsed <<" seconds.\n\n";
}
