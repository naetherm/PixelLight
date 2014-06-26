//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/List.h>

#include "ContainerTests.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(List_PrimitiveInt) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our List Test Fixture :)
	struct ConstructTestList
	{
		ConstructTestList() {
			/* some setup */
			Lst.Add(1);
			Lst.Add(2);
			Lst.Add(3);
			Lst.Add(4);
		}
		~ConstructTestList() {
			/* some teardown */
		}

		// Container for testing
		List<int> Lst;
	};

	TEST_FIXTURE(ConstructTestList, DerivedContainerFunctions) {
		CheckDerivedContainerFunctions<int>(List<int>::Null, Lst);
	}

	// no more tests needed, i think it's all covered by DerivedContainerFunction-Test
}