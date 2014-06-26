//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/SimpleList.h>
#include <PLCore/Container/Array.h>

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(SimpleList_PrimitiveInt) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our List Test Fixture :)
	struct ConstructTestSimpleList
	{
		ConstructTestSimpleList() {
			/* some setup */
		}
		~ConstructTestSimpleList() {
			/* some teardown */
		}

		// Container for testing
		SimpleList<int> Lst;
	};

	// Tests migrated from PLTestBase
	TEST_FIXTURE(ConstructTestSimpleList, IsEmpty_and_Clear) {
		CHECK(Lst.IsEmpty());
		CHECK_EQUAL(0U, Lst.GetNumOfElements());

		Lst.Add();

		CHECK(!Lst.IsEmpty());
		CHECK_EQUAL(1U, Lst.GetNumOfElements());

		Lst.Clear();

		CHECK(Lst.IsEmpty());
	}

	TEST_FIXTURE(ConstructTestSimpleList, Add) {
		// Add() return uninitialized Value, don't check it!
		Lst.Add();
		CHECK_EQUAL(1U, Lst.GetNumOfElements());

		CHECK_EQUAL(1, Lst.Add(1));
		CHECK_EQUAL(1, Lst[1]);

		CHECK_EQUAL(0, Lst.Add(0));
		CHECK_EQUAL(0, Lst[2]);

		// add array
		int vals[4] = {1,3,2,5};
		CHECK_EQUAL(3U, Lst.Add(vals, 3U));

		CHECK_EQUAL(6U, Lst.GetNumOfElements());
		CHECK_EQUAL(1, Lst[3]);
		CHECK_EQUAL(3, Lst[4]);
		CHECK_EQUAL(2, Lst[5]);

		// add 0 elements
		CHECK_EQUAL(0U, Lst.Add(vals, 0U));
		CHECK_EQUAL(6U, Lst.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestSimpleList, Operator_Add) {
		Lst += 5;
		Lst += 0;
		Lst += -8;
		CHECK_EQUAL(3U, Lst.GetNumOfElements());
		CHECK_EQUAL(5, Lst[0]);
		CHECK_EQUAL(0, Lst[1]);
		CHECK_EQUAL(-8, Lst[2]);
	}

	TEST_FIXTURE(ConstructTestSimpleList, AddAtIndex) {
		CHECK_EQUAL(&SimpleList<int>::Null, &Lst.AddAtIndex(0, 99));

		CHECK_EQUAL(0, Lst.AddAtIndex(0, 0));
		CHECK_EQUAL(8, Lst.AddAtIndex(8, 0));
		// append value
		CHECK_EQUAL(-8, Lst.AddAtIndex(-8, -5));

		CHECK_EQUAL(3U, Lst.GetNumOfElements());
		CHECK_EQUAL(8, Lst[0]);
		CHECK_EQUAL(0, Lst[1]);
		CHECK_EQUAL(-8, Lst[2]);
	}

	TEST_FIXTURE(ConstructTestSimpleList, Iterator) {
		// setting up Containers for testing
		Lst.Clear();

		PLCore::Array<int> Itercon;
		Itercon.Clear();
		Itercon.Add(1);
		Itercon.Add(3);

		PLCore::Iterator<int> iter = Lst.GetIterator();

		// iterator, no elements
		CHECK(!iter.HasNext());
		CHECK(!iter.HasPrevious());
		CHECK_EQUAL(&SimpleList<int>::Null, &iter.Next());
		CHECK_EQUAL(&SimpleList<int>::Null, &iter.Previous());

		// iterator, 2 elements
		Lst = Itercon;

		iter = Lst.GetIterator();
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());

		CHECK_EQUAL(1, iter.Next());
		CHECK(iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(3, iter.Next());
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(&SimpleList<int>::Null, &iter.Next());
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());

		Lst.Clear();
		iter = Lst.GetEndIterator();

		// enditerator, no elements
		CHECK(!iter.HasNext());
		CHECK(!iter.HasPrevious());
		CHECK_EQUAL(&SimpleList<int>::Null, &iter.Next());
		CHECK_EQUAL(&SimpleList<int>::Null, &iter.Previous());

		// enditerator, 2 elements
		Lst = Itercon;

		iter = Lst.GetEndIterator();
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(3, iter.Previous());
		CHECK(iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(1, iter.Previous());
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());

		CHECK_EQUAL(&SimpleList<int>::Null, &iter.Previous());
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());
	}

	TEST_FIXTURE(ConstructTestSimpleList, RemoveAtIndex) {
		Lst.Add(8);
		Lst.Add(1);
		Lst.Add(-6);

		CHECK(Lst.RemoveAtIndex(1));
		CHECK(!Lst.RemoveAtIndex(8));
		CHECK_EQUAL(8, Lst[0]);
		CHECK_EQUAL(-6, Lst[1]);
	}
}
