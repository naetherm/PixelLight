//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Array.h>

#include "ContainerTests.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Array_PrimitiveInt) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTestArray
	{
		ConstructTestArray() {
			/* some setup */
			Arr.Add(1);
			Arr.Add(2);
			Arr.Add(3);
			Arr.Add(4);

			HelperArr.Add(1);
			HelperArr.Add(3);
			HelperArr.Add(6);
			HelperArr.Add(8);
		}
		~ConstructTestArray() {
			/* some teardown */
		}

		// Container for testing
		Array<int> Arr, HelperArr;
	};

	TEST_FIXTURE(ConstructTestArray, DerivedContainerFunctions) {
		CheckDerivedContainerFunctions<int>(Array<int>::Null, Arr);
	}

	TEST_FIXTURE(ConstructTestArray, SetResizeCount_Zero) {
		// initial resizecount==10
		CHECK_EQUAL(10U, Arr.GetResizeCount());

		Arr.SetResizeCount(0U);
		CHECK_EQUAL(0U, Arr.GetResizeCount());
	}

	TEST_FIXTURE(ConstructTestArray, SetResizeCount_100) {
		Arr.SetResizeCount(100U);
		CHECK_EQUAL(100U, Arr.GetResizeCount());
	}

	TEST_FIXTURE(ConstructTestArray, SetResizeCount_4byte) {
		Arr.SetResizeCount(4294967295U);
		CHECK_EQUAL(4294967295U, Arr.GetResizeCount());
	}

	TEST_FIXTURE(ConstructTestArray, Reset) {
		CHECK(Arr.GetNumOfElements()>0U);

		Arr.Reset();
		CHECK_EQUAL(0U, Arr.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestArray, Resize_EmptyArray) {
		Arr.Clear();

		// nothing happens here
		CHECK(Arr.Resize(0U, false, false));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		// nothing happens here
		CHECK(Arr.Resize(0U, false, true));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		// nothing happens here
		CHECK(Arr.Resize(0U, true, false));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		// nothing happens here
		CHECK(Arr.Resize(0U, true, true));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		// size: 10, not added, no init
		CHECK(Arr.Resize(20U, false, false));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());
		Arr.Clear();

		// size: 10, not added, init
		CHECK(Arr.Resize(20U, false, true));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());
		CHECK_EQUAL(0, Arr.Get(0));
		CHECK_EQUAL(0, Arr.Get(9));
		Arr.Clear();

		// size: 10, added, no init
		CHECK(Arr.Resize(20U, true, false));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(20U, Arr.GetNumOfElements());
		Arr.Clear();

		// size: 10, added, init
		CHECK(Arr.Resize(20U, true, true));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(20U, Arr.GetNumOfElements());
		CHECK_EQUAL(0, Arr.Get(0));
		CHECK_EQUAL(0, Arr.Get(9));
	}

	TEST_FIXTURE(ConstructTestArray, Resize_ToSameSize) {
		Arr = HelperArr;
		// nothing happens here
		CHECK(Arr.Resize(10U, false, false));
		CHECK_EQUAL(10U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(4U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// nothing happens here
		CHECK(Arr.Resize(10U, false, true));
		CHECK_EQUAL(10U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(4U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// sets numofelements to maxnumofelements: all elements already added
		CHECK(Arr.Resize(10U, true, false));
		CHECK_EQUAL(10U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(10U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// sets numofelements to maxnumofelements:
		// all elements already added and all elements above numofelements will be initialized with 0
		CHECK(Arr.Resize(10U, true, true));
		CHECK_EQUAL(10U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(10U, Arr.GetNumOfElements());
		CHECK_EQUAL(0, Arr.Get(9));
	}

	TEST_FIXTURE(ConstructTestArray, Resize_ToBiggerArray) {
		// testcases => resize array with content to bigger array

		Arr = HelperArr;
		// clears array
		CHECK(Arr.Resize(0U, false, false));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// clears array
		CHECK(Arr.Resize(0U, false, true));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// clears array
		CHECK(Arr.Resize(0U, true, false));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// clears array
		CHECK(Arr.Resize(0U, true, true));
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(0U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// expands to size: 20, not added, no init, copies existing values -- HelperArr contains 3 values
		CHECK(Arr.Resize(20U, false, false));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(4U, Arr.GetNumOfElements());

		Arr = HelperArr;
		// expands to size: 20, not added, init
		CHECK(Arr.Resize(20U, false, true));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(4U, Arr.GetNumOfElements());
		// old values still have to exist!
		CHECK_EQUAL(3, Arr.Get(1));
		// additional fields have to be initialized with 0
		CHECK_EQUAL(0, Arr.Get(19));

		Arr = HelperArr;
		// expands to size: 20, added, no init
		CHECK(Arr.Resize(20U, true, false));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(20U, Arr.GetNumOfElements());
		// old values still have to exist!
		CHECK_EQUAL(3, Arr.Get(1));

		Arr = HelperArr;
		// expands to size: 20, added, init
		CHECK(Arr.Resize(20U, true, true));
		CHECK_EQUAL(20U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(20U, Arr.GetNumOfElements());
		// old values still have to exist!
		CHECK_EQUAL(3, Arr.Get(1));
		// additional fields have to be initialized with 0
		CHECK_EQUAL(0, Arr.Get(19));
	}

	TEST_FIXTURE(ConstructTestArray, Resize_ToSmallerArray) {
		// testcases => resize array to smaller size

		Arr = HelperArr;
		// resize to 2 elements, preserve these first two elements
		CHECK(Arr.Resize(2U, false, false));
		CHECK_EQUAL(2U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(2U, Arr.GetNumOfElements());
		CHECK_EQUAL(3, Arr.Get(1));

		Arr = HelperArr;
		// resize to 2 elements, preserve these first two elements
		CHECK(Arr.Resize(2U, false, true));
		CHECK_EQUAL(2U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(2U, Arr.GetNumOfElements());
		CHECK_EQUAL(3, Arr.Get(1));

		Arr = HelperArr;
		// resize to 2 elements, preserve these first two elements
		CHECK(Arr.Resize(2U, true, false));
		CHECK_EQUAL(2U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(2U, Arr.GetNumOfElements());
		CHECK_EQUAL(3, Arr.Get(1));

		Arr = HelperArr;
		// resize to 2 elements, preserve these first two elements
		CHECK(Arr.Resize(2U, true, true));
		CHECK_EQUAL(2U, Arr.GetMaxNumOfElements());
		CHECK_EQUAL(2U, Arr.GetNumOfElements());
		CHECK_EQUAL(3, Arr.Get(1));
	}

	// additional tests for derived virtual functions and specific implementation of Array
	TEST_FIXTURE(ConstructTestArray, Add_NoPassedElement_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);

		CHECK_EQUAL(&Array<int>::Null, &Arr.Add());

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, Add_Element_NoResize) {
		Arr.SetResizeCount(0U);
		Arr.Clear();
		Arr.Resize(0U, false);

		CHECK_EQUAL(&Array<int>::Null, &Arr.Add(1));

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, Add_C_Array_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);
		Arr.Resize(2U, false);
		int cArr[] = {1, 2, 3, 4};

		CHECK_EQUAL(2U, Arr.Add(cArr, 4U));
		CHECK_EQUAL(0U, Arr.Add(cArr, 2U));

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, Operator_Add_Element_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);
		Arr += 'a';

		CHECK_EQUAL(0U, Arr.GetNumOfElements());
		CHECK_EQUAL(0U, Arr.GetMaxNumOfElements());

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, Add_Container_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);
		Arr.Resize(2, false);

		// constructing container for adding
		Array<int> AppendArr;
		AppendArr.Add(1);
		AppendArr.Add(2);
		AppendArr.Add(3);
		AppendArr.Add(4);

		// returns false, but inserts elements until Arr is full
		CHECK(!Arr.Add(AppendArr, 0U));
		CHECK_EQUAL(2U, Arr.GetNumOfElements());

		CHECK(!Arr.Add(AppendArr, 0U, 2U));

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, Operator_Add_Container_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);
		Arr.Resize(2U, false, false);

		// constructing container for adding
		Array<int> AppendArr;
		AppendArr.Add(1);
		AppendArr.Add(2);
		AppendArr.Add(3);
		AppendArr.Add(4);

		// adds elements until Arr is full
		Arr += AppendArr;
		CHECK_EQUAL(2U, Arr.GetNumOfElements());
		CHECK_EQUAL(1, Arr.Get(0U));
		CHECK_EQUAL(2, Arr.Get(1U));

		// no elements added, Arr is full
		Arr+=AppendArr;
		CHECK_EQUAL(2U, Arr.GetNumOfElements());

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, AddAtIndex_Within_Bounds_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);
		Arr.Resize(4, false, false);
		Arr.Add(1);
		Arr.Add(2);
		Arr.Add(3);
		Arr.Add(4);

		CHECK_EQUAL(&Array<int>::Null, &Arr.AddAtIndex(2));
		CHECK_EQUAL(3, Arr.Get(2U));

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, AddAtIndex_Element_Within_Bounds_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);
		Arr.Resize(4U, false, false);
		Arr.Add(1);
		Arr.Add(2);
		Arr.Add(3);
		Arr.Add(4);

		CHECK(!Arr.AddAtIndex('x', 2));
		CHECK_EQUAL(3, Arr.Get(2U));

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, AddAtIndex_Element_Outside_Bounds_Negative_NoResize) {
		Arr.Clear();
		Arr.SetResizeCount(0U);
		Arr.Resize(4, false, false);
		Arr.Add(1);
		Arr.Add(2);
		Arr.Add(3);
		Arr.Add(4);

		// should NOT append at the end of container
		CHECK(!Arr.AddAtIndex('x', -10));
		CHECK_EQUAL(4U, Arr.GetNumOfElements());

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}

	TEST_FIXTURE(ConstructTestArray, RemoveAtIndex_Inside_Bounds_With_Resize) {
		Arr.Clear();
		Arr.SetResizeCount(1U);
		Arr.Add(1);
		Arr.Add(2);
		Arr.Add(3);
		Arr.Add(4);

		// check state of array
		CHECK_EQUAL(4U, Arr.GetNumOfElements());
		CHECK_EQUAL(4U, Arr.GetMaxNumOfElements());

		// remove element and check number of elements again, should be one less now
		CHECK(Arr.RemoveAtIndex(1U));
		CHECK_EQUAL(3U, Arr.GetNumOfElements());
		CHECK_EQUAL(3U, Arr.GetMaxNumOfElements());

		// set resizecount back to default
		Arr.SetResizeCount(10U);
	}
}
