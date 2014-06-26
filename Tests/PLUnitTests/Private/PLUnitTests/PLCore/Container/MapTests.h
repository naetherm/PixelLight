#ifndef __MAPTESTS_H__
#define __MAPTESTS_H__
#pragma once

#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Map.h>
#include <PLCore/Container/List.h>

template <class KeyType, class ValueType>
void CheckDerivedMapFunctions(ValueType &NullTypeInstance, PLCore::Map<KeyType, ValueType>& map) {
	// backup lists to check order of elements
	PLCore::List<ValueType> lstKeys, lstElems;

	// Clear(), IsEmpty(), GetNumOfElements()
	{
		map.Clear();
		CHECK(map.IsEmpty());
		CHECK_EQUAL(0U, map.GetNumOfElements());

		map.Add(0, 0);
		map.Add(1, 0);
		CHECK(!map.IsEmpty());
		CHECK_EQUAL(2U, map.GetNumOfElements());
	}

	// Add(key, value) and Get(key)
	{
		map.Clear();
		CHECK(map.Add(2, 2));
		CHECK(map.Add(5, 5));
		CHECK(map.Add(1, 1));

		CHECK_EQUAL(2, map.Get(2));
		CHECK_EQUAL(5, map.Get(5));
		CHECK_EQUAL(1, map.Get(1));
		CHECK_EQUAL(NullTypeInstance, map.Get(99));
	}

	// Replace(key, newValue)
	{
		CHECK_EQUAL(5, map.Get(5));
		CHECK(map.Replace(5, 6));
		CHECK_EQUAL(6, map.Get(5));

		CHECK(!map.Replace(99, 6));
	}

	// Set(key, Value)
	{
		// adds element
		CHECK(!map.Set(10, 10));

		CHECK_EQUAL(6, map.Get(5));
		CHECK(map.Set(5, 5));
		CHECK_EQUAL(5, map.Get(5));
	}

	// Remove(key)
	{
		CHECK(map.Remove(10));

		CHECK(!map.Remove(10));
	}

	// RemoveValue(value)
	{
		CHECK(map.RemoveValue(5));

		CHECK(!map.RemoveValue(10));
	}

	// Iterator, empty map
	{
		map.Clear();

		PLCore::Iterator<ValueType> cIterator = map.GetIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// KeyIterator, empty map
	{
		PLCore::Iterator<KeyType> cIterator = map.GetKeyIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// EndIterator, empty map
	{
		map.Clear();

		PLCore::Iterator<ValueType> cIterator = map.GetEndIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// EndKeyIterator, empty map
	{
		PLCore::Iterator<KeyType> cIterator = map.GetEndKeyIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// Iterator, one element map
	{
		map.Clear();
		map.Add(1, 1);

		PLCore::Iterator<ValueType> cIterator = map.GetIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(cIterator.HasNext());
	}

	// KeyIterator, one element map
	{
		PLCore::Iterator<KeyType> cIterator = map.GetKeyIterator();

		CHECK(!cIterator.HasPrevious());
		CHECK(cIterator.HasNext());
	}

	// EndIterator, one element map
	{
		PLCore::Iterator<ValueType> cIterator = map.GetEndIterator();

		CHECK(cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// EndKeyIterator, one element map
	{
		PLCore::Iterator<KeyType> cIterator = map.GetEndKeyIterator();

		CHECK(cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// Iterator
	{
		map.Clear();
		CHECK(map.Add(2, 2));
		CHECK(map.Add(5, 5));
		CHECK(map.Add(4, 4));
		CHECK(map.Add(1, 1));

		PLCore::Iterator<ValueType> cIterator = map.GetIterator();

		while (cIterator.HasNext())
			lstElems.Add(++cIterator);

		CHECK(cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// KeyIterator
	{
		PLCore::Iterator<KeyType> cIterator = map.GetKeyIterator();

		while (cIterator.HasNext())
			lstKeys.Add(++cIterator);

		CHECK(cIterator.HasPrevious());
		CHECK(!cIterator.HasNext());
	}

	// EndIterator
	{
		PLCore::Iterator<ValueType> cIterator = map.GetEndIterator();

		// check the order of the elements in lstElems against the backward iterator
		for (int i=lstElems.GetNumOfElements()-1; cIterator.HasPrevious(); i--)
			CHECK(lstElems[i] == --cIterator);

		CHECK_EQUAL(lstElems.GetNumOfElements(), map.GetNumOfElements());

		CHECK(!cIterator.HasPrevious());
		CHECK(cIterator.HasNext());
	}

	// EndKeyIterator
	{
		PLCore::Iterator<KeyType> cIterator = map.GetEndKeyIterator();

		// check the order of the elements in lstKeys against the backward iterator
		for (int i=lstKeys.GetNumOfElements()-1; cIterator.HasPrevious(); i--)
			CHECK(lstKeys[i] == --cIterator);

		CHECK_EQUAL(lstKeys.GetNumOfElements(), map.GetNumOfElements());

		CHECK(!cIterator.HasPrevious());
		CHECK(cIterator.HasNext());
	}
}
#endif // __MAPTESTS_H__