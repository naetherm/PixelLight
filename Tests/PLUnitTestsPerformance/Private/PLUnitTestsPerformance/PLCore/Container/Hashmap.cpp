/*********************************************************\
 *  File: Hashmap.cpp                                    *
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
#include <map>
#include <UnitTest++/UnitTest++.h>
#include "PLCore/Container/HashMap.h"


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
SUITE(Hashmap_Performance) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/
	// general objects for testing
	int testloops = 10000;	// number of iterations
	HashMap<char, int> PLHashmap;
	std::map<char, int> CHashmap;
	

	TEST(PL_Hashmap_Add){
		for (int i=0; i<testloops; i++)
			PLHashmap.Add(static_cast<char>(i+48), i);
	}
	
	TEST(C_Hashmap_Add){
		for (int i=0; i<testloops; i++)
			CHashmap[static_cast<char>(i+48),i] = i;	// operator[a] increases the map size by 1 if a not matched 
	}
	
	TEST(PL_Hashmap_Replace){
		for (int i=0; i<testloops; i++)
			PLHashmap.Replace(static_cast<char>(i+48), i);
	}

	TEST(C_Hashmap_Replace){
		for (int i=0; i<testloops; i++)
			CHashmap[48] = i;
	}

	TEST(PL_Hashmap_Set){
		for (int i=0; i<testloops; i++)
			PLHashmap.Set(48, i);
	}

	TEST(C_Hashmap_Set){
		for (int i=0; i<testloops; i++)
			CHashmap.insert(std::pair<char,int>(48,300));
	}

	TEST(PL_Hashmap_Get){
		for (int i=0; i<testloops; i++)
			PLHashmap.Get(static_cast<char>(testloops-i));
	}

	TEST(C_Hashmap_Get){
		for (int i=0; i<testloops; i++)
			CHashmap.find(static_cast<char>(testloops-i));
	}

	TEST(PL_Hashmap_Remove_Forwards){
		for (int i=0; i<testloops; i++)
			PLHashmap.Remove(i);
	}

	TEST(C_Hashmap_Remove_Forwards){
		for (int i=0; i<testloops; i++)
			CHashmap.erase(static_cast<char>(i));
	}

	TEST(PL_Hashmap_Remove_Backwards){
		for (int i=0; i<testloops; i++)
			PLHashmap.Remove(testloops-i);
	}

	TEST(C_Hashmap_Remove_Backwards){
		for (int i=0; i<testloops; i++)
			CHashmap.erase(static_cast<char>(testloops-i));
	}
}