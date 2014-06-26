//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLMath/AABoundingBox.h>
#include <PLMath/Intersect.h>

#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Intersect) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() {
			/* some setup */
			cAABoundingBox1.vMin = PLMath::Vector3(0.0f, 0.0f, 0.0f);
			cAABoundingBox1.vMax = PLMath::Vector3(1.0f, 1.0f, 1.0f);
			cAABoundingBox2.vMin = PLMath::Vector3(0.0f, 0.0f, 0.0f);
			cAABoundingBox2.vMax = PLMath::Vector3(1.0f, 1.0f, 1.0f);
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Container for testing
		PLMath::AABoundingBox cAABoundingBox1;
		PLMath::AABoundingBox cAABoundingBox2;
	};

	TEST_FIXTURE(ConstructTest, AABoxAABox) {
		// Both AABB are identical
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), true);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), true);

		// No intersection
		cAABoundingBox2.vMin = PLMath::Vector3(2.0f, 0.0f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(3.0f, 1.0f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), false);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), false);

		// Intersection x
		cAABoundingBox2.vMin = PLMath::Vector3(1.1f, 0.0f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(2.1f, 1.0f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), false);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), false);
		// Intersection x
		cAABoundingBox2.vMin = PLMath::Vector3(1.0f, 0.0f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(2.0f, 1.0f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), true);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), true);
		// Intersection x
		cAABoundingBox2.vMin = PLMath::Vector3(-1.1f, 0.0f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(-0.1f, 1.0f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), false);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), false);
		// Intersection x
		cAABoundingBox2.vMin = PLMath::Vector3(-1.0f, 0.0f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(-0.0f, 1.0f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), true);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), true);

		// Intersection y
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, 1.1f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, 2.1f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), false);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), false);
		// Intersection y
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, 1.0f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, 2.0f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), true);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), true);
		// Intersection y
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, -1.1f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, -0.1f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), false);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), false);
		// Intersection y
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, -1.0f, 0.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, -0.0f, 1.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), true);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), true);

		// Intersection z
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, 0.0f, 1.1f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, 1.0f, 2.1f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), false);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), false);
		// Intersection z
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, 0.0f, 1.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, 1.0f, 2.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), true);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), true);
		// Intersection z
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, 0.0f, -1.1f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, 1.0f, -0.1f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), false);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), false);
		// Intersection z
		cAABoundingBox2.vMin = PLMath::Vector3(0.0f, 0.0f, -1.0f);
		cAABoundingBox2.vMax = PLMath::Vector3(1.0f, 1.0f, -0.0f);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1.vMin, cAABoundingBox1.vMax, cAABoundingBox2.vMin, cAABoundingBox2.vMax), true);
		CHECK_EQUAL(PLMath::Intersect::AABoxAABox(cAABoundingBox1, cAABoundingBox2), true);
	}
}