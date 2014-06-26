/*********************************************************\
 *  File: PLCheckMacros.h                                 *
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

#ifndef __PLCHECKMACROS_H_
#define __PLCHECKMACROS_H_
#pragma once

#ifdef CHECK_MATRIX3X3
	#error UnitTest++ redefines CHECK_MATRIX3X3
#endif

#ifdef CHECK_IMPLEMENT
	#error UnitTest++ redefines CHECK_MATRIX3X3
#endif

#ifdef CHECK_NOT_EQUAL
	#errorCHECK_NOT_EQUAL
#endif

#define CHECK_MATRIX3X3(expected, actual, tolerance) \
    do \
    { \
        try { \
            UnitTest::CheckMatrix3x3(*UnitTest::CurrentTest::Results(), expected, actual, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_MATRIX3X3()"); \
        } \
    } while (0)

#define CHECK_IMPLEMENT \
    do \
    { \
        try { \
            UnitTest::CheckMissingImplementation(*UnitTest::CurrentTest::Results(), UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_IMPLEMENT"); \
        } \
    } while (0)

#define CHECK_NOT_EQUAL(expected, actual) \
    do \
    { \
        try { \
            UnitTest::CheckNotEqual(*UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_NOT_EQUAL(" #expected ", " #actual ")"); \
        } \
    } while (0)

#endif // __PLCHECKMACROS_H_
