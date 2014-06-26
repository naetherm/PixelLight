//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLMath/Graph/GraphPath.h>

#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(GraphPath) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() {
			/* some setup */
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Container for testing
		PLMath::GraphPath graphpath;
	};
}