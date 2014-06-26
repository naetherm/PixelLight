//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Bitset.h>
#include <PLCore/Container/Array.h>

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Bitset) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Bitset Test Fixture :)
	struct ConstructTestBitset
	{
		ConstructTestBitset() {
			/* some setup */
			bitset.Add(true);
			bitset.Add(false);
			bitset.Add(false);
			bitset.Add(true);
		}
		~ConstructTestBitset() {
			/* some teardown */
		}

		// Container for testing
		Bitset bitset, Comparecon, Emptycon, Itercon, Copycon, Removecon, Appendcon;
		Array<bool> TestArr;
	};

	TEST_FIXTURE(ConstructTestBitset, IsEmpty_and_Clear)
	{
		bitset.Clear();
		CHECK(bitset.IsEmpty());
		CHECK_EQUAL(0U, bitset.GetNumOfElements());

		bitset.Add();
		CHECK(!bitset.IsEmpty());
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, IsElement_Within)
	{
		bitset.Clear();
		bitset.Add(true);
		CHECK(bitset.IsElement(true));
	}

	TEST_FIXTURE(ConstructTestBitset, IsElement_NotWithin)
	{
		bitset.Clear();
		bitset.Add(true);
		CHECK(!bitset.IsElement(false));
	}

	TEST_FIXTURE(ConstructTestBitset, GetIndex_ElementWithin)
	{
		bitset.Clear();
		bitset.Add(true);
		CHECK_EQUAL(0, bitset.GetIndex(true));
	}

	TEST_FIXTURE(ConstructTestBitset, GetIndex_ElementNotWithin)
	{
		bitset.Clear();
		bitset.Add(true);
		CHECK(bitset.GetIndex(false)<0);
	}

	TEST_FIXTURE(ConstructTestBitset, Get_Within_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);

		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
	}

	TEST_FIXTURE(ConstructTestBitset, Get_Outside_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);

		CHECK_EQUAL(&PLCore::Bitset::Null, &bitset.Get(6U));
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_AccessElement_Within_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);

		CHECK_EQUAL(true, bitset[0]);
		CHECK_EQUAL(false, bitset[1]);
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_AccessElement_Outside_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);

		CHECK_EQUAL(&PLCore::Bitset::Null, &bitset[5]);
	}

	TEST_FIXTURE(ConstructTestBitset, Replace_ElementWithin)
	{
		bitset.Clear();
		bitset.Add(false);
		bitset.Add(false);

		CHECK(bitset.Replace(false, true));
		CHECK_EQUAL(true, bitset[0]);
	}

	TEST_FIXTURE(ConstructTestBitset, Replace_ElementNotWithin)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(true);

		CHECK(!bitset.Replace(false, true));
		CHECK_EQUAL(true, bitset[0]);
		CHECK_EQUAL(true, bitset[1]);
	}

	TEST_FIXTURE(ConstructTestBitset, ReplaceAtIndex_Within_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);

		CHECK(bitset.ReplaceAtIndex(1U, true));
		CHECK_EQUAL(true, bitset[1]);
	}

	TEST_FIXTURE(ConstructTestBitset, ReplaceAtIndex_Outside_Bounds_Positive)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);

		CHECK(!bitset.ReplaceAtIndex(5U, false));
		CHECK_EQUAL(true, bitset[0]);
		CHECK_EQUAL(false, bitset[1]);
	}

	TEST_FIXTURE(ConstructTestBitset, Add_NoPassedElement)
	{
		bitset.Clear();

		// we can't check this as we don't know what the element will be initialized with, e.g. if it is initialized with 'false' a CHECK() will fail but will be ok otherwise!
		bitset.Add();
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		bitset.Add();
		CHECK_EQUAL(2U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, Add_Element)
	{
		bitset.Clear();

		CHECK_EQUAL(true, bitset.Add(true));
		CHECK_EQUAL(false, bitset.Add(false));

		CHECK_EQUAL(2U, bitset.GetNumOfElements());

		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
	}

	TEST_FIXTURE(ConstructTestBitset, Add_C_Array)
	{
		bitset.Clear();
		bool ccon[] = {true, false, true, true};

		// append
		CHECK_EQUAL(4U, bitset.Add(ccon, 4U));

		CHECK_EQUAL(4U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
		CHECK_EQUAL(true, bitset.Get(2U));
		CHECK_EQUAL(true, bitset.Get(3U));
		CHECK_EQUAL(&PLCore::Bitset::Null, &bitset.Get(4U));

		// append
		CHECK_EQUAL(2U, bitset.Add(ccon, 2));

		CHECK_EQUAL(6U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
		CHECK_EQUAL(true, bitset.Get(2U));
		CHECK_EQUAL(true, bitset.Get(3U));
		CHECK_EQUAL(true, bitset.Get(4U));
		CHECK_EQUAL(false, bitset.Get(5U));
		CHECK_EQUAL(&PLCore::Bitset::Null, &bitset.Get(6U));
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_Add_Element)
	{
		bitset.Clear();
		bitset += true;

		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(&PLCore::Bitset::Null, &bitset.Get(1U));

		bitset += false;

		CHECK_EQUAL(2U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
	}

	TEST_FIXTURE(ConstructTestBitset, Add_Container)
	{
		bitset.Clear();
		// constructing Container for adding
		Appendcon.Clear();
		Appendcon.Add(true);
		Appendcon.Add(true);
		Appendcon.Add(false);
		Appendcon.Add(true);

		//TODO: refactor these: each scenario = one TEST()

		//SCENARIOS:			return value, description
		//Add(Append, 0, 0);	true, all elements from index 0
		//Add(Append, 0, 1);	true, element at index 0
		//Add(Append, 0, 8);	true, all to index 8 (stops when all elements from appending con are added)
		//Add(Append, 2, 0);	true, all from index 2
		//Add(Append, 2, 1);	true, element at index 2
		//Add(Append, 2, 8);	true, all from index 2
		//Add(Append, 9, 0);	false
		//Add(Append, 9, 99);	false
		//Add(Append);		same as Add(Append, 0, 0)


		// add all elements
		CHECK(bitset.Add(Appendcon, 0U, 0U));
		CHECK_EQUAL(4U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(true, bitset.Get(1U));
		CHECK_EQUAL(false, bitset.Get(2U));
		CHECK_EQUAL(true, bitset.Get(3U));

		// add element at index 0
		CHECK(bitset.Add(Appendcon, 0U, 1U));
		CHECK_EQUAL(5U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(4U));

		// add all elements
		CHECK(bitset.Add(Appendcon, 0U, 8U));
		CHECK_EQUAL(9U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(5U));
		CHECK_EQUAL(true, bitset.Get(6U));
		CHECK_EQUAL(false, bitset.Get(7U));
		CHECK_EQUAL(true, bitset.Get(8U));

		// add all elements from index 2
		CHECK(bitset.Add(Appendcon, 2U, 0U));
		CHECK_EQUAL(11U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(9U));
		CHECK_EQUAL(true, bitset.Get(10U));

		// add element at index 2
		CHECK(bitset.Add(Appendcon, 2U, 1U));
		CHECK_EQUAL(12U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(11U));

		// add all elements from index 2 to 8 (stops when all elements of Appendcon are added)
		CHECK(bitset.Add(Appendcon, 2U, 8U));
		CHECK_EQUAL(14U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(12U));
		CHECK_EQUAL(true, bitset.Get(13U));

		// add no elements :)
		CHECK(!bitset.Add(Appendcon, 9U, 0U));
		CHECK_EQUAL(14U, bitset.GetNumOfElements());

		// add no elements :)
		CHECK(!bitset.Add(Appendcon, 9U, 99U));
		CHECK_EQUAL(14U, bitset.GetNumOfElements());

		// add all elements
		CHECK(bitset.Add(Appendcon));
		CHECK_EQUAL(18U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(14U));
		CHECK_EQUAL(true, bitset.Get(15U));
		CHECK_EQUAL(false, bitset.Get(16U));
		CHECK_EQUAL(true, bitset.Get(17U));
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_Add_Container)
	{
		bitset.Clear();

		// constructing Container for adding
		Appendcon.Clear();
		Appendcon.Add(true);
		Appendcon.Add(false);
		Appendcon.Add(true);
		Appendcon.Add(true);

		bitset += Appendcon;

		CHECK_EQUAL(4U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
		CHECK_EQUAL(true, bitset.Get(2U));
		CHECK_EQUAL(true, bitset.Get(3U));
	}

	TEST_FIXTURE(ConstructTestBitset, AddAtIndex_Within_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		// "adds new" element that is at index, we can't check its value as it may be uninitialized!
		CHECK(bitset.AddAtIndex(2U));
		CHECK(bitset.Get(2U));
		CHECK_EQUAL(5U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, AddAtIndex_Outside_Bounds_Positive)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		CHECK_EQUAL(&PLCore::Bitset::Null, &bitset.AddAtIndex(99));
	}

	TEST_FIXTURE(ConstructTestBitset, AddAtIndex_Outside_Bounds_Negative)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		// should append at the end of Container, element not initialized! e.g. we can't check its value, only the number of elements it holds
		CHECK(&PLCore::Bitset::Null != &bitset.AddAtIndex(-10));
		CHECK_EQUAL(5U, bitset.GetNumOfElements());
		CHECK(bitset.Compare(bitset, 0, 4));
	}

	TEST_FIXTURE(ConstructTestBitset, AddAtIndex_Element_Within_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		// inserts element at index
		CHECK_EQUAL(false, bitset.AddAtIndex(false, 2U));
		CHECK_EQUAL(true, bitset.Get(3U));
		CHECK_EQUAL(5U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, AddAtIndex_Element_Outside_Bounds_Positive)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		CHECK_EQUAL(&PLCore::Bitset::Null, &bitset.AddAtIndex(false, 99));
	}

	TEST_FIXTURE(ConstructTestBitset, AddAtIndex_Element_Outside_Bounds_Negative)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		// should append at the end of Container
		CHECK_EQUAL(false, bitset.AddAtIndex(false, -10));
		CHECK_EQUAL(5U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(bitset.GetNumOfElements()-1));
	}

	TEST_FIXTURE(ConstructTestBitset, Remove_Element_Within)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		CHECK(bitset.Remove(false));
		CHECK_EQUAL(3U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, Remove_Element_NotWithin)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(true);
		bitset.Add(true);

		CHECK(!bitset.Remove(false));
		CHECK_EQUAL(3U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, RemoveAtIndex_Inside_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		CHECK(bitset.RemoveAtIndex(2U));
		CHECK_EQUAL(3U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, RemoveAtIndex_Outside_Bounds)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		CHECK(!bitset.RemoveAtIndex(99U));
		CHECK_EQUAL(4U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_Remove_Element_Within)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(true);
		bitset.Add(true);

		bitset -= false;

		CHECK_EQUAL(true, bitset.Get(1U));
		CHECK_EQUAL(3U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_Remove_Element_NotWithin)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(true);
		bitset.Add(true);

		bitset -= false;

		CHECK_EQUAL(true, bitset.Get(1U));
		CHECK_EQUAL(3U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, Remove_Container)
	{
		//TODO: refactor these: each scenario = one TEST()

		//SCENARIOS:			return value, description
		//Remove(TestArr, 0, 0) true, all elements
		//Remove(TestArr, 0, 1) true, element at index 0
		//Remove(TestArr, 0, 8) true, all elements
		//Remove(TestArr, 1, 0) true, elements from index 1 to end
		//Remove(TestArr, 1, 1) true, element at index 1
		//Remove(TestArr, 1, 8) true, elements from index 1 to 8 (stops at last element from Removecon)
		//Remove(TestArr, 9, 0)	false
		//Remove(TestArr, 9, 1) false
		//Remove(TestArr, 9, 99) false
		//Remove(TestArr)		same as Remove(Removecon, 0, 0)

		//bitset.Add(true);
		//bitset.Add(false);
		//bitset.Add(false);
		//bitset.Add(true);

		TestArr.Clear();
		TestArr.Add(true);
		TestArr.Add(true);
		TestArr.Add(false);
		TestArr.Add(true);

		// all elements from index 0 to end
		CHECK(bitset.Remove(TestArr));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(0U));

		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(false);
		bitset.Add(true);

		// all elements in TestArr removed
		CHECK(bitset.Remove(TestArr, 0U, 0U));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(0U));


		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(false);
		bitset.Add(true);

		// first element
		CHECK(bitset.Remove(TestArr, 0U, 1U));
		CHECK_EQUAL(false, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
		CHECK_EQUAL(true, bitset.Get(2U));
		CHECK_EQUAL(3U, bitset.GetNumOfElements());


		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(false);
		bitset.Add(true);

		// all elements from index 0 to 8
		CHECK(bitset.Remove(TestArr, 0U, 8U));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(0U));


		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(false);
		bitset.Add(true);

		// all elements from index 1 to end
		CHECK(bitset.Remove(TestArr, 1U, 0U));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(0U));


		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(false);
		bitset.Add(true);

		// element at index 1
		CHECK(bitset.Remove(TestArr, 1U, 1U));
		CHECK_EQUAL(3U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(0U));
		CHECK_EQUAL(false, bitset.Get(1U));
		CHECK_EQUAL(true, bitset.Get(2U));

		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(false);
		bitset.Add(true);

		// all elements from index 1 to 8 (stops at last element from Removecon)
		CHECK(bitset.Remove(TestArr, 1U, 8U));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		CHECK_EQUAL(false, bitset.Get(0U));

		// returns false (start index > number of elements)
		CHECK(!bitset.Remove(TestArr, 9U, 0U));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());

		// returns false (start index > number of elements)
		CHECK(!bitset.Remove(TestArr, 9U, 1U));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());

		// returns false (start index > number of elements)
		CHECK(!bitset.Remove(TestArr, 9U, 99U));
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_Remove_Container)
	{
		bitset.Clear();
		bitset.Add(true);
		bitset.Add(false);
		bitset.Add(false);
		bitset.Add(false);

		Removecon.Clear();
		Removecon.Add(false);
		Removecon.Add(false);
		Removecon.Add(false);
		Removecon.Add(false);

		bitset -= Removecon;
		CHECK_EQUAL(1U, bitset.GetNumOfElements());
		CHECK_EQUAL(true, bitset.Get(0U));
	}

	TEST_FIXTURE(ConstructTestBitset, Copy)
	{
		bitset.Clear();

		Copycon.Clear();
		Copycon.Add(false);
		Copycon.Add(true);
		Copycon.Add(false);
		Copycon.Add(false);

		//TODO: refactor these: each scenario = one TEST()

		//SCENARIOS:			return value, description
		//Copy(Copycon, 0, 0) true, all elements
		//Copy(Copycon, 0, 1) true, element at index 0
		//Copy(Copycon, 0, 8) true, all elements
		//Copy(Copycon, 1, 0) true, elements from index 1 to end
		//Copy(Copycon, 1, 1) true, element at index 1
		//Copy(Copycon, 1, 8) true, elements from index 1 to 8 (stops at last element of Copycon)
		//Copy(Copycon, 9, 0)	false
		//Copy(Copycon, 9, 1) false
		//Copy(Copycon, 9, 99) false
		//Copy(Copycon)		same as Copy(Copycon, 0, 0)
		//Copy(Emptycon)		false?

		// copies all elements from index 0 to end
		CHECK(bitset.Copy(Copycon));
		CHECK(bitset.Compare(Copycon));

		// copies all elements from index 0 to end
		CHECK(bitset.Copy(Copycon, 0, 0));
		CHECK(bitset.Compare(Copycon));

		// copies element at index 0
		CHECK(bitset.Copy(Copycon, 0, 1));
		CHECK(bitset.Compare(Copycon, 0, 1));

		// copies all elements
		CHECK(bitset.Copy(Copycon, 0, 8));
		CHECK(bitset.Compare(Copycon));

		// copies all elements from index 1 to end
		CHECK(bitset.Copy(Copycon, 1, 0));
		Copycon.RemoveAtIndex(0);
		CHECK(bitset.Compare(Copycon, 1, 0));
		Copycon.AddAtIndex(true, 0);

		// copies element at index 1
		CHECK(bitset.Copy(Copycon, 1, 1));
		Copycon.RemoveAtIndex(0);
		CHECK(bitset.Compare(Copycon, 0, 1));
		Copycon.AddAtIndex(true, 0);

		// copies all elements from index 1 to 8 (stops at last element of Copycon)
		CHECK(bitset.Copy(Copycon, 1, 8));
		Copycon.RemoveAtIndex(0);
		CHECK(bitset.Compare(Copycon));
		Copycon.AddAtIndex(true, 0);

		bitset.Clear();
		CHECK(!bitset.Copy(Copycon, 9, 0));
		CHECK(bitset.Compare(Emptycon));

		CHECK(!bitset.Copy(Copycon, 9, 1));
		CHECK(bitset.Compare(Emptycon));

		CHECK(!bitset.Copy(Copycon, 9, 99));
		CHECK(bitset.Compare(Emptycon));

		// copy an empty PLCore::Array
		bitset.Add(false);
		CHECK(bitset.Copy(Emptycon));
		CHECK(bitset.Compare(Emptycon));
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_Copy)
	{
		bitset.Clear();

		Copycon.Clear();
		Comparecon.Add(false);
		Comparecon.Add(true);
		Comparecon.Add(false);
		Comparecon.Add(false);

		bitset = Copycon;
		CHECK(bitset.Compare(Copycon));

		// copy empty Container
		Copycon.Clear();
		bitset = Copycon;
		CHECK(bitset.Compare(Copycon));
	}

	TEST_FIXTURE(ConstructTestBitset, Compare)
	{
		bitset.Clear();

		Comparecon.Clear();
		Comparecon.Add(false);
		Comparecon.Add(false);
		Comparecon.Add(true);
		Comparecon.Add(false);
		Comparecon.Add(false);

		CHECK(bitset.Compare(Emptycon));

		bitset = Comparecon;
		CHECK(bitset.Compare(Comparecon));

		CHECK(bitset.Compare(Comparecon, 0, 0));
		CHECK(bitset.Compare(Comparecon, 0, 2));
		CHECK(bitset.Compare(Comparecon, 0, bitset.GetNumOfElements()));

		CHECK(bitset.Compare(Comparecon, 1, 0));
		CHECK(bitset.Compare(Comparecon, 1, 2));
		CHECK(bitset.Compare(Comparecon, 1, bitset.GetNumOfElements()-1));

		// TODO: review this case :) -- returns false
		// functions add/remove would crop number 4 down to 2 because this is the maximum number of elements in PLCore::Container
		//
		CHECK(bitset.Compare(Comparecon, 1, 4));

		CHECK(!bitset.Compare(Comparecon, 10, 0));
		CHECK(!bitset.Compare(Comparecon, 10, 2));
		CHECK(!bitset.Compare(Comparecon, 10, bitset.GetNumOfElements()));
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_Equals)
	{
		bitset.Clear();

		Comparecon.Clear();
		Comparecon.Add(false);
		Comparecon.Add(true);
		Comparecon.Add(false);
		Comparecon.Add(false);

		CHECK(bitset == Emptycon);

		bitset = Comparecon;
		CHECK(bitset == Comparecon);

		bitset.RemoveAtIndex(1);
		CHECK(!(bitset == Comparecon));
	}

	TEST_FIXTURE(ConstructTestBitset, Operator_NotEquals)
	{
		bitset.Clear();

		Comparecon.Clear();
		Comparecon.Add(false);
		Comparecon.Add(true);
		Comparecon.Add(false);
		Comparecon.Add(false);

		CHECK(!(bitset != Emptycon));

		bitset = Comparecon;
		CHECK(!(bitset != Comparecon));

		bitset.RemoveAtIndex(1);
		CHECK(bitset != Comparecon);
	}

	TEST_FIXTURE(ConstructTestBitset, test_iterator)
	{
		// setting up Containers for testing
		bitset.Clear();
		Itercon.Clear();
		Itercon.Add(true);
		Itercon.Add(false);

		PLCore::Iterator<bool> iter = bitset.GetIterator();

		// iterator, no elements
		CHECK(!iter.HasNext());
		CHECK(!iter.HasPrevious());
		CHECK_EQUAL(&PLCore::Bitset::Null, &iter.Next());
		CHECK_EQUAL(&PLCore::Bitset::Null, &iter.Previous());

		// iterator, 2 elements
		bitset = Itercon;

		iter = bitset.GetIterator();
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());

		CHECK_EQUAL(true, iter.Next());
		CHECK(iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(false, iter.Next());
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(&PLCore::Bitset::Null, &iter.Next());
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());


		// enditerator, 2 elements
		bitset = Itercon;

		iter = bitset.GetEndIterator();
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(false, iter.Previous());
		CHECK(iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(true, iter.Previous());
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());

		CHECK_EQUAL(&PLCore::Bitset::Null, &iter.Previous());
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());
	}

	TEST_FIXTURE(ConstructTestBitset, Resize) {
		// NOTE: copied from PLTestBase
		//
		// Resize()
		Bitset cBitset(1000U, true, true);
		for (uint32 i=0; i<1000; i++)
			CHECK(!cBitset.IsSet(i));

		cBitset.Clear();
		cBitset.Resize(1000U, true);
		cBitset.Resize(1500U, true, true);

		// We start at 1000 to check because the other bits are not initialized :)
		for (uint32 i=1000; i<1500; i++) {
			CHECK(!cBitset.IsSet(i));
		}

		cBitset.Clear();
		cBitset.Resize(2U, false, false);
		CHECK_EQUAL(cBitset.GetMaxNumOfElements(), 2U);
		CHECK_EQUAL(cBitset.GetNumOfElements(), 0U);

		cBitset.Resize(2U, true, false);
		CHECK_EQUAL(cBitset.GetMaxNumOfElements(), 2U);
		CHECK_EQUAL(cBitset.GetNumOfElements(), 2U);

		cBitset.RemoveAtIndex(1U);
		cBitset.Resize(2U, true, true);
		CHECK_EQUAL(cBitset.GetMaxNumOfElements(), 2U);
		CHECK_EQUAL(cBitset.GetNumOfElements(), 2U);
		CHECK_EQUAL(cBitset[1U], false);

		cBitset.Resize(0U);
		CHECK_EQUAL(cBitset.GetMaxNumOfElements(), 0U);
		CHECK_EQUAL(cBitset.GetNumOfElements(), 0U);
	}
}