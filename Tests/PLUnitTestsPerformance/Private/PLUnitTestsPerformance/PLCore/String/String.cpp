/*********************************************************\
 *  File: String.cpp                                      *
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
#include <fstream>
#include <iostream>
#include <UnitTest++/UnitTest++.h>
#include "PLCore/String/String.h"
#include <string>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace std;
using namespace PLCore;


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
extern ofstream outputFile;


/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(String_Performance) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/
	// general objects for testing
	int testloops = 10000;	// number of iterations
	String* PLString = new String();
	string* CString = new string();
	
	TEST(PL_String_insert_begin){
		for (int i=0; i<testloops; i++)
			PLString->Insert(static_cast<char>(i+48),0);
	}

	TEST(C_String_insert_begin){
		char insert[2];
		insert[1] = '\0';
		for (int i=0; i<testloops; i++){
			insert[0] = char(i+48);
			CString->insert(0, insert);
		}
	}

	TEST(PL_String_insert_mid){
		for (int i=0; i<testloops; i++)
			PLString->Insert(static_cast<char>(i+48),i/2);
	}

	TEST(C_String_insert_mid){
		char insert[2];
		insert[1] = '\0';
		for (int i=0; i<testloops; i++){
			insert[0] = char(i+48);
			CString->insert(i/2, insert);
		}	
	}

	TEST(PL_String_insert_end){
		for (int i=0; i<testloops; i++)
			PLString->Insert(static_cast<char>(i+48),i);
	}

	TEST(C_String_insert_end){
		char insert[2];
		insert[1] = '\0';
		for (int i=0; i<testloops; i++){
			insert[0] = char(i+48);
			CString->insert(i, insert);
		}	
	}

	/*
	Tests for SubString
	*/
	TEST(PL_String_Substring){
		for (int i=0; i<testloops; i++)
			PLString->GetSubstring(i);
	}

	TEST(C_String_Substring){
		char insert[2];
		insert[1] = '\0';
		for (int i=0; i<testloops; i++){
			insert[0] = char(i+48);
			CString->substr(i, insert[0]);
		}	
	}

}