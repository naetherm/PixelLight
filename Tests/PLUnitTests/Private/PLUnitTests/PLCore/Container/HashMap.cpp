//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/HashMap.h>

#include "MapTests.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(HashMap_Int_Int) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our List Test Fixture :)
	struct ConstructTestHashMap
	{
		ConstructTestHashMap() {
			/* some setup */
			Hash.Add(1, 1);
			Hash.Add(2, 2);
			Hash.Add(3, 3);
			Hash.Add(4, 4);
		}
		~ConstructTestHashMap() {
			/* some teardown */
		}

		// Container for testing
		HashMap<int, int> Hash;
	};

	TEST_FIXTURE(ConstructTestHashMap, DerivedMapFunctions) {
		CheckDerivedMapFunctions<int,int>(HashMap<int,int>::Null, Hash);
	}

	TEST_FIXTURE(ConstructTestHashMap, CopyConstructor) {
		HashMap<int, int> HashMapCopy = Hash;

		CHECK_EQUAL(Hash.GetNumOfElements(), HashMapCopy.GetNumOfElements());
		CHECK_EQUAL(Hash.Get(1), HashMapCopy.Get(1));
		CHECK_EQUAL(Hash.Get(2), HashMapCopy.Get(2));
		CHECK_EQUAL(Hash.Get(3), HashMapCopy.Get(3));
		CHECK_EQUAL(Hash.Get(4), HashMapCopy.Get(4));
		CHECK_EQUAL(Hash.Get(5), HashMapCopy.Get(5));
	}

	TEST_FIXTURE(ConstructTestHashMap, CopyOperator) {
		HashMap<int, int> HashMapCopy;
		HashMapCopy = Hash;

		CHECK_EQUAL(Hash.GetNumOfElements(), HashMapCopy.GetNumOfElements());
		CHECK_EQUAL(Hash.Get(1), HashMapCopy.Get(1));
		CHECK_EQUAL(Hash.Get(2), HashMapCopy.Get(2));
		CHECK_EQUAL(Hash.Get(3), HashMapCopy.Get(3));
		CHECK_EQUAL(Hash.Get(4), HashMapCopy.Get(4));
		CHECK_EQUAL(Hash.Get(5), HashMapCopy.Get(5));
	}
}