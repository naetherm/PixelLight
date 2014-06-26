#ifndef __CONTAINERTESTS_H__
#define __CONTAINERTESTS_H__
#pragma once

#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Container.h>
#include <PLCore/Container/Array.h>

template <class AType>
void CheckDerivedContainerFunctions(AType &NullTypeInstance, PLCore::Container<AType>& con) {
	/*
	* general PLCore::Container functions
	*
	*/
	PLCore::Array<AType> Emptycon, Appendcon, Copycon, Comparecon, TestArr, Removecon, Itercon;

	// isEmpty_and_Clear
	{
		con.Clear();
		CHECK(con.IsEmpty());
		CHECK_EQUAL(0U, con.GetNumOfElements());

		con.Add();
		CHECK(!con.IsEmpty());
		CHECK_EQUAL(1U, con.GetNumOfElements());
	}

	// IsElement_Within
	{
		con.Clear();
		con.Add(4);
		CHECK(con.IsElement(4));
	}

	// IsElement_NotWithin
	{
		con.Clear();
		con.Add(6);
		CHECK(!con.IsElement(4));
	}

	// GetIndex_ElementWithin
	{
		con.Clear();
		con.Add(4);
		CHECK_EQUAL(0, con.GetIndex(4));
	}

	// GetIndex_ElementNotWithin
	{
		con.Clear();
		con.Add(6);
		CHECK(con.GetIndex(4)<0);
	}

	// Get_Within_Bounds
	{
		con.Clear();
		con.Add(6);
		con.Add(4);

		CHECK_EQUAL(6, con.Get(0U));
		CHECK_EQUAL(4, con.Get(1U));
	}

	// Get_Outside_Bounds
	{
		con.Clear();
		con.Add(4);

		CHECK_EQUAL(NullTypeInstance, con.Get(6U));
	}

	// Operator_AccessElement_Within_Bounds
	{
		con.Clear();
		con.Add(1);
		con.Add(2);

		CHECK_EQUAL(1, con[0U]);
		CHECK_EQUAL(2, con[1U]);
	}

	// Operator_AccessElement_Outside_Bounds
	{
		con.Clear();
		con.Add(1);
		con.Add(2);

		CHECK_EQUAL(NullTypeInstance, con[5U]);
	}

	// Replace_ElementWithin
	{
		con.Clear();
		con.Add(1);
		con.Add(2);

		CHECK(con.Replace(2, 0));
		CHECK_EQUAL(0, con[1]);
	}

	// Replace_ElementNotWithin
	{
		con.Clear();
		con.Add(1);
		con.Add(2);

		CHECK(!con.Replace(4, 0));
		CHECK_EQUAL(2, con[1]);
	}

	// ReplaceAtIndex_Within_Bounds
	{
		con.Clear();
		con.Add(1);
		con.Add(2);

		CHECK(con.ReplaceAtIndex(1U, 0));
		CHECK_EQUAL(0, con[1U]);
	}

	// ReplaceAtIndex_Outside_Bounds_Positive
	{
		con.Clear();
		con.Add(1);
		con.Add(2);

		CHECK(!con.ReplaceAtIndex(5U, 0));
		CHECK_EQUAL(2, con[1U]);
	}

	// Add_NoPassedElement
	{
		con.Clear();

		// we can't check this as we don't know what the element will be initialized with, e.g. if it is initialized with 0 a CHECK() will fail but will be ok otherwise!
		con.Add();
		CHECK_EQUAL(1U, con.GetNumOfElements());
		con.Add();
		CHECK_EQUAL(2U, con.GetNumOfElements());
	}

	// Add_Element
	{
		con.Clear();

		CHECK_EQUAL(1, con.Add(1));
		CHECK_EQUAL(2, con.Add(2));

		CHECK_EQUAL(2U, con.GetNumOfElements());

		CHECK_EQUAL(1, con.Get(0U));
		CHECK_EQUAL(2, con.Get(1U));
	}

		// Add_C_Array
		{
		con.Clear();
		AType ccon[] = {40, 41, 42, 43};

		// append
		CHECK_EQUAL(4U, con.Add(ccon, 4U));

		CHECK_EQUAL(4U, con.GetNumOfElements());
		CHECK_EQUAL(40, con.Get(0U));
		CHECK_EQUAL(41, con.Get(1U));
		CHECK_EQUAL(42, con.Get(2U));
		CHECK_EQUAL(43, con.Get(3U));
		CHECK_EQUAL(NullTypeInstance, con.Get(4U));

		// append
		CHECK_EQUAL(2U, con.Add(ccon, 2));

		CHECK_EQUAL(6U, con.GetNumOfElements());
		CHECK_EQUAL(40, con.Get(0U));
		CHECK_EQUAL(41, con.Get(1U));
		CHECK_EQUAL(42, con.Get(2U));
		CHECK_EQUAL(43, con.Get(3U));
		CHECK_EQUAL(40, con.Get(4U));
		CHECK_EQUAL(41, con.Get(5U));
		CHECK_EQUAL(NullTypeInstance, con.Get(6U));
	}

	// Operator_Add_Element
	{
		con.Clear();
		con += 40;

		CHECK_EQUAL(1U, con.GetNumOfElements());
		CHECK_EQUAL(40, con.Get(0U));
		CHECK_EQUAL(NullTypeInstance, con.Get(1U));

		con += 41;

		CHECK_EQUAL(2U, con.GetNumOfElements());
		CHECK_EQUAL(40, con.Get(0U));
		CHECK_EQUAL(41, con.Get(1U));
	}

	// Add_Container
	{
		con.Clear();
		// constructing Container for adding
		Appendcon.Clear();
		Appendcon.Add(1);
		Appendcon.Add(2);
		Appendcon.Add(3);
		Appendcon.Add(4);

		/*
		TODO: refactor these: each scenario = one TEST()

		SCENARIOS:			return value, description
		Add(Append, 0, 0);	true, all elements from index 0
		Add(Append, 0, 1);	true, element at index 0
		Add(Append, 0, 8);	true, all to index 8 (stops when all elements from appending con are added)
		Add(Append, 2, 0);	true, all from index 2
		Add(Append, 2, 1);	true, element at index 2
		Add(Append, 2, 8);	true, all from index 2
		Add(Append, 9, 0);	false
		Add(Append, 9, 99);	false
		Add(Append);		same as Add(Append, 0, 0)
		*/

		// add all elements
		CHECK(con.Add(Appendcon, 0U, 0U));
		CHECK_EQUAL(4U, con.GetNumOfElements());
		CHECK_EQUAL(1, con.Get(0U));
		CHECK_EQUAL(2, con.Get(1U));
		CHECK_EQUAL(3, con.Get(2U));
		CHECK_EQUAL(4, con.Get(3U));

		// add element at index 0
		CHECK(con.Add(Appendcon, 0U, 1U));
		CHECK_EQUAL(5U, con.GetNumOfElements());
		CHECK_EQUAL(1, con.Get(4U));

		// add all elements
		CHECK(con.Add(Appendcon, 0U, 8U));
		CHECK_EQUAL(9U, con.GetNumOfElements());
		CHECK_EQUAL(1, con.Get(5U));
		CHECK_EQUAL(2, con.Get(6U));
		CHECK_EQUAL(3, con.Get(7U));
		CHECK_EQUAL(4, con.Get(8U));

		// add all elements from index 2
		CHECK(con.Add(Appendcon, 2U, 0U));
		CHECK_EQUAL(11U, con.GetNumOfElements());
		CHECK_EQUAL(3, con.Get(9U));
		CHECK_EQUAL(4, con.Get(10U));

		// add element at index 2
		CHECK(con.Add(Appendcon, 2U, 1U));
		CHECK_EQUAL(12U, con.GetNumOfElements());
		CHECK_EQUAL(3, con.Get(11U));

		// add all elements from index 2 to 8 (stops when all elements of Appendcon are added)
		CHECK(con.Add(Appendcon, 2U, 8U));
		CHECK_EQUAL(14U, con.GetNumOfElements());
		CHECK_EQUAL(3, con.Get(12U));
		CHECK_EQUAL(4, con.Get(13U));

		// add no elements :)
		CHECK(!con.Add(Appendcon, 9U, 0U));
		CHECK_EQUAL(14U, con.GetNumOfElements());

		// add no elements :)
		CHECK(!con.Add(Appendcon, 9U, 99U));
		CHECK_EQUAL(14U, con.GetNumOfElements());

		// add all elements
		CHECK(con.Add(Appendcon));
		CHECK_EQUAL(18U, con.GetNumOfElements());
		CHECK_EQUAL(1, con.Get(14U));
		CHECK_EQUAL(2, con.Get(15U));
		CHECK_EQUAL(3, con.Get(16U));
		CHECK_EQUAL(4, con.Get(17U));
	}

	// Operator_Add_Container
	{
		con.Clear();

		// constructing Container for adding
		Appendcon.Clear();
		Appendcon.Add(1);
		Appendcon.Add(2);
		Appendcon.Add(3);
		Appendcon.Add(4);

		con += Appendcon;

		CHECK_EQUAL(4U, con.GetNumOfElements());
		CHECK_EQUAL(1, con.Get(0U));
		CHECK_EQUAL(2, con.Get(1U));
		CHECK_EQUAL(3, con.Get(2U));
		CHECK_EQUAL(4, con.Get(3U));
	}

	// AddAtIndex_Within_Bounds
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// "adds new" element that is at index, we can't check its value as it may be uninitialized!
		con.AddAtIndex(2U);
		CHECK_EQUAL(5U, con.GetNumOfElements());
	}

	// AddAtIndex_Outside_Bounds_Positive
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		CHECK_EQUAL(NullTypeInstance, con.AddAtIndex(99));
	}

	// AddAtIndex_Outside_Bounds_Negative
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// should append at the end of Container, element not initialized! e.g. we can't check its value
		con.AddAtIndex(-10);
		CHECK_EQUAL(5U, con.GetNumOfElements());
		CHECK(con.Compare(con, 0, 4));
	}

	// AddAtIndex_Element_Within_Bounds
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// inserts element at index
		CHECK_EQUAL(1, con.AddAtIndex(1, 2U));
		CHECK_EQUAL(1, con.Get(2U));
		CHECK_EQUAL(5U, con.GetNumOfElements());
	}

	// AddAtIndex_Element_Outside_Bounds_Positive
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		CHECK_EQUAL(NullTypeInstance, con.AddAtIndex(1, 99));
	}

	// AddAtIndex_Element_Outside_Bounds_Negative
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// should append at the end of Container
		CHECK_EQUAL(1, con.AddAtIndex(1, -10));
		CHECK_EQUAL(5U, con.GetNumOfElements());
		CHECK_EQUAL(1, con.Get(con.GetNumOfElements()-1));
	}

	// Remove_Element_Within
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		CHECK(con.Remove(2));
		CHECK_EQUAL(3U, con.GetNumOfElements());
	}

	// Remove_Element_NotWithin
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// should NOT append at the end of Container
		CHECK(!con.Remove(9));
		CHECK_EQUAL(4U, con.GetNumOfElements());
	}

	// RemoveAtIndex_Inside_Bounds
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		CHECK(con.RemoveAtIndex(2U));
		CHECK_EQUAL(3U, con.GetNumOfElements());
	}

	// RemoveAtIndex_Outside_Bounds
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		CHECK(!con.RemoveAtIndex(99U));
		CHECK_EQUAL(4U, con.GetNumOfElements());
	}

	// Operator_Remove_Element_Within
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		con -= 2;

		CHECK_EQUAL(3, con.Get(1U));
		CHECK_EQUAL(3U, con.GetNumOfElements());
	}

	// Operator_Remove_Element_NotWithin
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		con -= 9;

		CHECK_EQUAL(2, con.Get(1U));
		CHECK_EQUAL(4U, con.GetNumOfElements());
	}

	// Remove_Container
	{
		/*
		TODO: refactor these: each scenario = one TEST()

		SCENARIOS:			return value, description
		Remove(TestArr, 0, 0) true, all elements
		Remove(TestArr, 0, 1) true, element at index 0
		Remove(TestArr, 0, 8) true, all elements
		Remove(TestArr, 1, 0) true, elements from index 1 to end
		Remove(TestArr, 1, 1) true, element at index 1
		Remove(TestArr, 1, 8) true, elements from index 1 to 8 (stops at last element from Removecon)
		Remove(TestArr, 9, 0)	false
		Remove(TestArr, 9, 1) false
		Remove(TestArr, 9, 99) false
		Remove(TestArr)		same as Remove(Removecon, 0, 0)
		*/

		TestArr.Clear();
		TestArr.Add(3);
		TestArr.Add(2);
		TestArr.Add(1);
		TestArr.Add(0);

		// all elements from index 0 to end
		CHECK(con.Remove(TestArr));
		CHECK_EQUAL(1U, con.GetNumOfElements());
		CHECK_EQUAL(4, con.Get(0U));

		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// all elements in Removecon removed
		CHECK(con.Remove(TestArr, 0U, 0U));
		CHECK_EQUAL(1U, con.GetNumOfElements());
		CHECK_EQUAL(4, con.Get(0U));


		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// first element
		CHECK(con.Remove(TestArr, 0U, 1U));
		CHECK_EQUAL(1, con.Get(0U));
		CHECK_EQUAL(2, con.Get(1U));
		CHECK_EQUAL(4, con.Get(2U));
		CHECK_EQUAL(3U, con.GetNumOfElements());


		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// all elements from index 0 to 8
		CHECK(con.Remove(TestArr, 0U, 8U));
		CHECK_EQUAL(1U, con.GetNumOfElements());
		CHECK_EQUAL(4, con.Get(0U));


		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// all elements from index 1 to end
		CHECK(con.Remove(TestArr, 1U, 0U));
		CHECK_EQUAL(2U, con.GetNumOfElements());
		CHECK_EQUAL(3, con.Get(0U));
		CHECK_EQUAL(4, con.Get(1U));


		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// element at index 1
		CHECK(con.Remove(TestArr, 1U, 1U));
		CHECK_EQUAL(3U, con.GetNumOfElements());
		CHECK_EQUAL(1, con.Get(0U));
		CHECK_EQUAL(3, con.Get(1U));
		CHECK_EQUAL(4, con.Get(2U));

		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		// all elements from index 1 to 8 (stops at last element from Removecon)
		CHECK(con.Remove(TestArr, 1U, 8U));
		CHECK_EQUAL(2U, con.GetNumOfElements());
		CHECK_EQUAL(3, con.Get(0U));
		CHECK_EQUAL(4, con.Get(1U));

		// returns false (start index > number of elements)
		CHECK(!con.Remove(TestArr, 9U, 0U));
		CHECK_EQUAL(2U, con.GetNumOfElements());

		// returns false (start index > number of elements)
		CHECK(!con.Remove(TestArr, 9U, 1U));
		CHECK_EQUAL(2U, con.GetNumOfElements());

		// returns false (start index > number of elements)
		CHECK(!con.Remove(TestArr, 9U, 99U));
		CHECK_EQUAL(2U, con.GetNumOfElements());
	}

	// Operator_Remove_Container
	{
		con.Clear();
		con.Add(1);
		con.Add(2);
		con.Add(3);
		con.Add(4);

		Removecon.Clear();
		Removecon.Add(3);
		Removecon.Add(2);
		Removecon.Add(1);
		Removecon.Add(0);

		con -= Removecon;
		CHECK_EQUAL(1U, con.GetNumOfElements());
		CHECK_EQUAL(4, con.Get(0U));
	}

	// Copy
	{
		con.Clear();

		Copycon.Clear();
		Copycon.Add(3);
		Copycon.Add(2);
		Copycon.Add(1);
		Copycon.Add(0);

		/*
		TODO: refactor these: each scenario = one TEST()

		SCENARIOS:			return value, description
		Copy(Copycon, 0, 0) true, all elements
		Copy(Copycon, 0, 1) true, element at index 0
		Copy(Copycon, 0, 8) true, all elements
		Copy(Copycon, 1, 0) true, elements from index 1 to end
		Copy(Copycon, 1, 1) true, element at index 1
		Copy(Copycon, 1, 8) true, elements from index 1 to 8 (stops at last element of Copycon)
		Copy(Copycon, 9, 0)	false
		Copy(Copycon, 9, 1) false
		Copy(Copycon, 9, 99) false
		Copy(Copycon)		same as Copy(Copycon, 0, 0)
		Copy(Emptycon)		false?
		*/

		// copies all elements from index 0 to end
		CHECK(con.Copy(Copycon));
		CHECK(con.Compare(Copycon));

		// copies all elements from index 0 to end
		CHECK(con.Copy(Copycon, 0, 0));
		CHECK(con.Compare(Copycon));

		// copies element at index 0
		CHECK(con.Copy(Copycon, 0, 1));
		CHECK(con.Compare(Copycon, 0, 1));

		// copies all elements
		CHECK(con.Copy(Copycon, 0, 8));
		CHECK(con.Compare(Copycon));

		// copies all elements from index 1 to end
		CHECK(con.Copy(Copycon, 1, 0));
		Copycon.RemoveAtIndex(0);
		CHECK(con.Compare(Copycon, 1, 0));
		Copycon.AddAtIndex(3, 0);

		// copies element at index 1
		CHECK(con.Copy(Copycon, 1, 1));
		Copycon.RemoveAtIndex(0);
		CHECK(con.Compare(Copycon, 0, 1));
		Copycon.AddAtIndex(3, 0);

		// copies all elements from index 1 to 8 (stops at last element of Copycon)
		CHECK(con.Copy(Copycon, 1, 8));
		Copycon.RemoveAtIndex(0);
		CHECK(con.Compare(Copycon));
		Copycon.AddAtIndex(3, 0);

		con.Clear();
		CHECK(!con.Copy(Copycon, 9, 0));
		CHECK(con.Compare(Emptycon));

		CHECK(!con.Copy(Copycon, 9, 1));
		CHECK(con.Compare(Emptycon));

		CHECK(!con.Copy(Copycon, 9, 99));
		CHECK(con.Compare(Emptycon));

		// copy an empty PLCore::Array
		con.Add(0);
		CHECK(con.Copy(Emptycon));
		CHECK(con.Compare(Emptycon));
	}

	// Operator_Copy
	{
		con.Clear();

		Copycon.Clear();
		Copycon.Add(3);
		Copycon.Add(2);
		Copycon.Add(1);
		Copycon.Add(0);

		con = Copycon;
		CHECK(con.Compare(Copycon));

		// copy empty Container
		Copycon.Clear();
		con = Copycon;
		CHECK(con.Compare(Copycon));
	}

	// Compare
	{
		con.Clear();

		Comparecon.Clear();
		Comparecon.Add(3);
		Comparecon.Add(2);
		Comparecon.Add(1);
		Comparecon.Add(0);

		CHECK(con.Compare(Emptycon));

		con = Comparecon;
		CHECK(con.Compare(Comparecon));

		CHECK(con.Compare(Comparecon, 0, 0));
		CHECK(con.Compare(Comparecon, 0, 2));
		CHECK(con.Compare(Comparecon, 0, con.GetNumOfElements()));

		CHECK(con.Compare(Comparecon, 1, 0));
		CHECK(con.Compare(Comparecon, 1, 2));
		CHECK(con.Compare(Comparecon, 1, con.GetNumOfElements()-1));

		CHECK(con.Compare(Comparecon, 1, 4));

		CHECK(!con.Compare(Comparecon, 10, 0));
		CHECK(!con.Compare(Comparecon, 10, 2));
		CHECK(!con.Compare(Comparecon, 10, con.GetNumOfElements()));
	}

	// Operator_Equals
	{
		con.Clear();

		Comparecon.Clear();
		Comparecon.Add(3);
		Comparecon.Add(2);
		Comparecon.Add(1);
		Comparecon.Add(0);

		CHECK(con == Emptycon);

		con = Comparecon;
		CHECK(con == Comparecon);

		con.RemoveAtIndex(1);
		CHECK(!(con == Comparecon));
	}

	// Operator_NotEquals
	{
		con.Clear();

		Comparecon.Clear();
		Comparecon.Add(3);
		Comparecon.Add(2);
		Comparecon.Add(1);
		Comparecon.Add(0);

		CHECK(!(con != Emptycon));

		con = Comparecon;
		CHECK(!(con != Comparecon));

		con.RemoveAtIndex(1);
		CHECK(con != Comparecon);
	}


	// test iterator
	{
		// setting up Containers for testing
		con.Clear();
		Itercon.Clear();
		Itercon.Add(1);
		Itercon.Add(3);

		PLCore::Iterator<AType> iter = con.GetIterator();

		// iterator, no elements
		CHECK(!iter.HasNext());
		CHECK(!iter.HasPrevious());
		CHECK_EQUAL(NullTypeInstance, iter.Next());
		CHECK_EQUAL(NullTypeInstance, iter.Previous());

		// iterator, 2 elements
		con = Itercon;

		iter = con.GetIterator();
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());

		CHECK_EQUAL(1, iter.Next());
		CHECK(iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(3, iter.Next());
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(NullTypeInstance, iter.Next());
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());


		// enditerator, 2 elements
		con = Itercon;

		iter = con.GetEndIterator();
		CHECK(!iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(3, iter.Previous());
		CHECK(iter.HasNext());
		CHECK(iter.HasPrevious());

		CHECK_EQUAL(1, iter.Previous());
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());

		CHECK_EQUAL(NullTypeInstance, iter.Previous());
		CHECK(iter.HasNext());
		CHECK(!iter.HasPrevious());
	}
}
#endif // __CONTAINERTESTS_H__