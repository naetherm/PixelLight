//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/FibonacciHeap.h>

#include "HeapTests.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(FibonacciHeap_Int_Int) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our List Test Fixture :)
	struct ConstructTestFibonacciHeap
	{
		ConstructTestFibonacciHeap() {
			/* some setup */
			FiboHeap.Add(1, 1);
			FiboHeap.Add(2, 2);
			FiboHeap.Add(3, 3);
			FiboHeap.Add(4, 4);
		}
		~ConstructTestFibonacciHeap() {
			/* some teardown */
		}

		// Container for testing
		FibonacciHeap<int, int> FiboHeap;
	};

	TEST_FIXTURE(ConstructTestFibonacciHeap, DerivedContainerFunctions) {
		CheckDerivedHeapFunctions<int,int>(FiboHeap);
	}

	TEST_FIXTURE(ConstructTestFibonacciHeap, Consolidate_and_Normalize) {
		FiboHeap.Clear();
		FiboHeap.Add(5, 5);
		FiboHeap.Add(2, 2);
		FiboHeap.Add(1, 1);

		// should NOT be normalized
		CHECK(!FiboHeap.IsNormalized());

		// consolidate heap
		CHECK(FiboHeap.Consolidate());

		// should BE normalized now
		CHECK(FiboHeap.IsNormalized());
	}
}