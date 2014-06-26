//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Pool.h>

#include "ContainerTests.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Pool_PrimitiveInt) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Pool Test Fixture :)
	struct ConstructTestPool
	{
		ConstructTestPool() {
			/* some setup */
			cPool.Add(1);
			cPool.Add(2);
			cPool.Add(3);
			cPool.Add(4);
		}
		~ConstructTestPool() {
			/* some teardown */
		}

		// Container for testing
		Pool<int> cPool;
	};

	TEST_FIXTURE(ConstructTestPool, DerivedContainerFunctions) {
		CheckDerivedContainerFunctions<int>(Pool<int>::Null, cPool);
	}

	TEST_FIXTURE(ConstructTestPool, FreeElementsFunctions) {
		CHECK_EQUAL(0U, cPool.GetNumOfFreeElements());

		cPool.Remove(2);
		CHECK_EQUAL(1U, cPool.GetNumOfFreeElements());

		cPool.FreeElements();
		CHECK_EQUAL(4U, cPool.GetNumOfFreeElements());

		cPool.RemoveAllFreeElements();
		CHECK_EQUAL(0U, cPool.GetNumOfFreeElements());
	}

	TEST_FIXTURE(ConstructTestPool, FreeSize) {
		CHECK_EQUAL(0U, cPool.GetFreeSize());

		cPool.Remove(2);
		CHECK_EQUAL(sizeof(int), cPool.GetFreeSize());
	}
}