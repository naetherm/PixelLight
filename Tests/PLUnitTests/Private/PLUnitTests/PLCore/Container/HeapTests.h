#ifndef __HEAPTESTS_H__
#define __HEAPTESTS_H__
#pragma once

#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Heap.h>
#include <PLCore/Container/List.h>

template <class KeyType, class ValType>
void CheckDerivedHeapFunctions(PLCore::Heap<KeyType, ValType>& heap) {
	// backup list to check order of elements
	PLCore::List<ValType> lstElems;

	// isEmpty_and_Clear
	{
		heap.Clear();
		CHECK(heap.IsEmpty());
		CHECK_EQUAL(0U, heap.GetNumOfElements());

		heap.Add(1, 1);
		CHECK(!heap.IsEmpty());
		CHECK_EQUAL(1U, heap.GetNumOfElements());
	}

	// Iterator, empty heap
	{
		heap.Clear();

		PLCore::Iterator<ValType> cIterator = heap.GetIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// Iterator backwards, empty heap
	{
		PLCore::Iterator<ValType> cIterator = heap.GetEndIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// Iterator, one element
	{
		heap.Add(1, 1);

		PLCore::Iterator<ValType> cIterator = heap.GetIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(cIterator.HasNext());
	}

	// Iterator backwards, one element
	{
		PLCore::Iterator<ValType> cIterator = heap.GetEndIterator();

		CHECK(cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// add
	{
		heap.Clear();
		CHECK(heap.Add(5, 0));
		CHECK(heap.Add(2, 3));
		CHECK(heap.Add(1, 4));
		CHECK(heap.Add(4, 1));
		CHECK(heap.Add(3, 2));
		CHECK_EQUAL(5U, heap.GetNumOfElements());
	}

	// Iterator
	{
		PLCore::Iterator<ValType> cIterator = heap.GetIterator();
		lstElems.Clear();

		CHECK(!cIterator.HasPrevious());

		// add all elements to our backup list to check the order of the elements against the backward iterator
		while (cIterator.HasNext())
			lstElems.Add(++cIterator);

		CHECK_EQUAL(5U, lstElems.GetNumOfElements());

		CHECK(cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// Iterator backwards
	{
		PLCore::Iterator<ValType> cIterator = heap.GetEndIterator();

		CHECK(!cIterator.HasNext());

		// check the order of the elements in lstElems against the backward iterator
		for (int i=lstElems.GetNumOfElements()-1; cIterator.HasPrevious(); i--)
			CHECK(lstElems[i] == --cIterator);

		CHECK_EQUAL(lstElems.GetNumOfElements(), heap.GetNumOfElements());

		CHECK(!cIterator.HasPrevious());
		CHECK(cIterator.HasNext());
	}

	// GetTop
	{
		int key, val;
		CHECK(heap.GetTop(&val, &key));
		CHECK_EQUAL(1, key);
		CHECK_EQUAL(4, val);
		CHECK_EQUAL(5U, heap.GetNumOfElements());
	}

	// ExtractTop
	{
		int key, val;
		CHECK(heap.ExtractTop(&val, &key));
		CHECK_EQUAL(1, key);
		CHECK_EQUAL(4, val);
		CHECK_EQUAL(4U, heap.GetNumOfElements());

		// check next set
		CHECK(heap.GetTop(&val, &key));
		CHECK_EQUAL(2, key);
		CHECK_EQUAL(3, val);
	}
}
#endif // __HEAPTESTS_H__
