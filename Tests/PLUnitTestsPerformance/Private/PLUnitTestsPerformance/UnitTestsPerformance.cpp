/*********************************************************\
 *  File: UnitTestsPerformance.cpp                       *
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
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <UnitTest++/UnitTest++.h>
#include <UnitTest++AddIns/RunAllTests.h>
#include <UnitTest++AddIns/MyPerformanceReporter.h>
#include <PLCore/PLCore.h>	// For "nullptr" in case the compiler doesn't support this C++11 feature


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
std::ofstream outputFile;


//[-------------------------------------------------------]
//[ Unit tests program entry point                        ]
//[-------------------------------------------------------]
int UnitTestsPerformance()
{
	// Get the current time as ASCII string
	char szTime[9];
	char szDate[11];

	#ifdef WIN32
		_strtime(szTime);
		_strdate(szDate);
		// replace ':' (not valid within filenames) by '-'
		szTime[2] = szTime[5] = '-';
		szDate[2] = szDate[5] = '-';

	#else
		struct tm sTime;
		struct timeval sNow;
		gettimeofday(&sNow, nullptr);
		localtime_r(&sNow.tv_sec, &sTime);
		sprintf(szTime, "%2d_%2d_%2d_%2d-%2d-%2d", sTime.tm_hour, sTime.tm_min, sTime.tm_sec, sTime.tm_yday, sTime.tm_mon, sTime.tm_year);
	#endif

	// Open the results output file
	char szFilename[256];

	sprintf(szFilename, "PLUnitTestsPerformance_Result_[%s][%s].csv", szTime, szDate);
	outputFile.open(szFilename);

	// Create an reporter instance
	MyPerformanceReporter cReporter;

	// Run all unit tests
	const int nResult = UnitTest::RunAllTests(cReporter);

	// Close the results output file
	outputFile.close();

	// Done
	return nResult;
}
