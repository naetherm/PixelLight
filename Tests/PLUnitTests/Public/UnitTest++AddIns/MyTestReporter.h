/*********************************************************\
 *  File: MyTestReporter.h                                 *
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


#ifndef __PLUNITTESTS_UNITTESTADDINS_MYTESTREPORTER_H_
#define __PLUNITTESTS_UNITTESTADDINS_MYTESTREPORTER_H_
#pragma once

#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestReporter.h>

class MyTestReporter : public UnitTest::TestReporter
{
private:
    virtual void ReportTestStart(UnitTest::TestDetails const& test);
    virtual void ReportFailure(UnitTest::TestDetails const& test, char const* failure);
    virtual void ReportTestFinish(UnitTest::TestDetails const& test, float secondsElapsed);
    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);
};


#endif // __PLUNITTESTS_UNITTESTADDINS_MYTESTREPORTER_H_
