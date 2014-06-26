/*********************************************************\
 *  File: Queue.cpp                                    *
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
#include "PLCore/Container/Queue.h"
#include <queue>

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
SUITE(Queue_Performance) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/
	// general objects for testing
	int testloops = 10000;	// number of iterations
	Queue<int>* PLQueue = new Queue<int>();
	queue<int>* CQueue = new queue<int>();

	TEST(PL_Queue_Push){
		for (int i=0; i<testloops; i++)
			PLQueue->Push(i+48);
	}

	TEST(C_Queue_Push){
		for (int i=0; i<testloops; i++)
			CQueue->push(i+48);
	}

	TEST(PL_Queue_GetNumOfElements){
		for (int i=0; i<testloops; i++)
			PLQueue->GetNumOfElements();
	}

	TEST(C_Queue_size){
		for (int i=0; i<testloops; i++)
			CQueue->size();
	}

	TEST(PL_Queue_Pop){
		for (int i=testloops-1; i>=0; i--)
			PLQueue->Pop();
	}

	TEST(C_Queue_Pop){
		for (int i=0; i<testloops; i++)
			CQueue->pop();
	}
}