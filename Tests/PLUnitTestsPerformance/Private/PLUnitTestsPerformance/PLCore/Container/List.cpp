/*********************************************************\
 *  File: List.cpp                                      *
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
#include "PLCore/Container/List.h"
#include <list>

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
SUITE(List_Performance) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/
	// general objects for testing
	int testloops = 10000;	// number of iterations
	List<int>* PLList = new List<int>();
	list<int>* CList = new list<int>();

	TEST(PL_List_Add_front){
		for (int i=0; i<testloops; i++)
			PLList->AddAtIndex(char(i+48),0);
	}

	TEST(C_List_Add_front){
		for (int i=0; i<testloops; i++)
			CList->push_front(char(i+48));
	}

	TEST(PL_List_Remove_front){
		for (int i=0; i<testloops; i++)
			PLList->RemoveAtIndex(0);
	}

	TEST(C_List_pop_front){
		for (int i=0; i<testloops; i++)
			CList->pop_front();
	}

	TEST(PL_List_Add_back){
		for (int i=0; i<testloops; i++)
			PLList->Add(PLList->GetNumOfElements());
	}

	TEST(C_List_push_back){
		for (int i=0; i<testloops; i++)
			CList->push_back(char(i+48));
	}

	TEST(PL_List_Remove_back){
		for (int i=0; i<testloops; i++)
			PLList->RemoveAtIndex(testloops-i);
	}

	TEST(C_List_remove_back){
		for (int i=0; i<testloops; i++)
			CList->remove(testloops-i);
	}

	TEST(PL_List_Clear){
		for (int i=0; i<testloops; i++)
			PLList->Clear();
	}

	TEST(C_List_clear){
		for (int i=0; i<testloops; i++)
			CList->clear();
	}
	
	
}