//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Core/RefCount.h>
#include <PLCore/Core/SmartPtr.h>
#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(ResourceManagement) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class TestClass : public RefCount<TestClass> {
	public:
		TestClass() : i(0) {}
		int i;
	};

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest()
			: pTest(new TestClass())
		{
			/* some setup */
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Container for testing
		TestClass *pTest;
		SmartPtr<TestClass> cPtr, cPtr2;
	};

	/*
	 * Migrated from PLTestBase
	 */
	TEST_FIXTURE(ConstructTest, RefCount) {
		CHECK_EQUAL(pTest->GetPointer() , pTest );
		CHECK(!(pTest->GetRefCount()));

		CHECK(!(pTest->Release()));

		pTest = new TestClass();
		CHECK_EQUAL(static_cast<uint32>(1), pTest->AddReference());
		CHECK_EQUAL(static_cast<uint32>(1), pTest->GetRefCount());
		CHECK_EQUAL(static_cast<uint32>(2), pTest->AddReference());
		CHECK_EQUAL(static_cast<uint32>(2), pTest->GetRefCount());
		CHECK_EQUAL(static_cast<uint32>(1), pTest->Release());
		CHECK_EQUAL(static_cast<uint32>(1), pTest->GetRefCount());
		CHECK(!(pTest->Release()));
	}

	/*
	 * Migrated from PLTestBase
	 */
	TEST_FIXTURE(ConstructTest, SmartPtr_basic_tests) {
		CHECK(!cPtr.GetPointer());
		CHECK(!cPtr);
		CHECK(cPtr == cPtr2);
	}

	/*
	 * Migrated from PLTestBase
	 */
	TEST_FIXTURE(ConstructTest, SmartPtr_with_RefCount_pointer) {
		cPtr = new TestClass();

		CHECK(cPtr.GetPointer() );
		CHECK(cPtr);
		CHECK(cPtr != cPtr2);
		CHECK_EQUAL(static_cast<uint32>(1), cPtr.GetPointer()->GetRefCount());

		cPtr = cPtr;
		CHECK(cPtr.GetPointer());
		CHECK(cPtr);
		CHECK(cPtr != cPtr2);
		CHECK_EQUAL(static_cast<uint32>(1), cPtr.GetPointer()->GetRefCount());

		cPtr2 = cPtr2;
		CHECK(!(cPtr2.GetPointer()));
		CHECK(!(!cPtr2 == false));
		CHECK(cPtr != cPtr2);

		cPtr2 = cPtr;
		CHECK(cPtr2.GetPointer());
		CHECK(cPtr2);
		CHECK(cPtr == cPtr2);
		CHECK_EQUAL(static_cast<uint32>(2), cPtr.GetPointer()->GetRefCount());

		cPtr = nullptr;
		CHECK_EQUAL(static_cast<uint32>(1), cPtr2.GetPointer()->GetRefCount());

		cPtr = new TestClass();
		cPtr->i = 1;
		*cPtr2 = *cPtr;
		CHECK_EQUAL(cPtr2->i, cPtr->i);
	}

	/*
	 * Migrated from PLTestBase
	 */
	// Raw pointer
	TEST_FIXTURE(ConstructTest, SmartPtr_with_raw_pointer) {
		SmartPtr<int> cRawPtr, cRawPtr2;
		int *pI = new int;
		*pI = 55;
		cRawPtr = pI;

		CHECK(cRawPtr.GetPointer());
		CHECK(cRawPtr);
		CHECK(cRawPtr != cRawPtr2);
		CHECK(cRawPtr == pI);
		CHECK_EQUAL(pI, cRawPtr);
		CHECK_EQUAL(55, *cRawPtr.GetPointer());

		cRawPtr = cRawPtr;
		CHECK(cRawPtr.GetPointer());
		CHECK(cRawPtr);
		CHECK(cRawPtr != cRawPtr2);
		CHECK(cRawPtr != cRawPtr2);
		CHECK_EQUAL(55, *cRawPtr.GetPointer());

		cRawPtr2 = cRawPtr2;
		CHECK(!(cRawPtr2.GetPointer()));
		CHECK(!cRawPtr2 != false);
		CHECK(cRawPtr != cRawPtr2);
		CHECK(cRawPtr != cRawPtr2);

		cRawPtr2 = cRawPtr;
		CHECK(cRawPtr2.GetPointer());
		CHECK(cRawPtr2);
		CHECK(!(cRawPtr != cRawPtr2));
		CHECK_EQUAL(cRawPtr2, cRawPtr);
		CHECK_EQUAL(55, *cRawPtr.GetPointer());

		*pI = 56;
		CHECK_EQUAL(56, *cRawPtr.GetPointer());
		CHECK_EQUAL(56, *cRawPtr2.GetPointer());

		cRawPtr = nullptr;
		CHECK(!(cRawPtr.GetPointer()));
		CHECK(!cRawPtr != false);
		CHECK(cRawPtr != cRawPtr2);
		CHECK_EQUAL(56, *cRawPtr2.GetPointer());
	}
}