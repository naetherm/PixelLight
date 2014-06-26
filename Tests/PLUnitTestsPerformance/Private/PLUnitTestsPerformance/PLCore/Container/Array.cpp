/*********************************************************\
 *  File: Array.cpp                                      *
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
#include <UnitTest++/UnitTest++.h>
#include "PLCore/Container/Array.h"
#include <vector>

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
SUITE(Array_Performance) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/
	// general objects for testing
	int testloops = 10000;	// number of iterations
	Array<int>* PLArray = new Array<int>();
	vector<int>* CArray = new vector<int>();

	TEST(PL_Array_Add_at_end){
		for (int i=0; i<testloops; i++)
			PLArray->Add(char(i+48));
	}

	TEST(C_Array_Add_at_end){
		for (int i=0; i<testloops; i++)
			CArray->push_back(char(i+48));
	}

	TEST(PL_Array_Add_at_Index){
		for (int i=0; i<testloops; i++)
			PLArray->AddAtIndex(char(i+48), i);
	}

	// [TODO] This is broken
	/*TEST(C_Array_Add_at_Index){
		vector<int>::iterator it;
		for (int i=0; i<testloops; i++){
			CArray->insert(it, 0, char(i+48));
			it++;
		}
	}*/
	
	TEST(PL_Array_GetSize){
		for (int i=0; i<testloops; i++)
			PLArray->GetSize();
	}

	TEST(C_Array_GetSize){
		for (int i=0; i<testloops; i++)
			CArray->size();
	}

	// No real C function for swapping two ELEMENTS of an array/vector
	/*TEST(PL_Array_ReplaceAtIndex){
		for (int i=0; i<testloops; i++)
			PLArray->ReplaceAtIndex(testloops-i, i);
	}*/

	/*TEST(C_Array_swap){
		for (int i=0; i<testloops; i++)
			swap(CArray[testloops-i-1], CArray[i]);
	}*/

	TEST(PL_Array_Reset){
		for (int i=0; i<testloops; i++)
			PLArray->Reset();
	}

	TEST(C_Array_clear){
		for (int i=0; i<testloops; i++)
			CArray->clear();
	}

}