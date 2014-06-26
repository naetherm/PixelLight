//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/FastPool.h>
#include <iostream>

#include "ContainerTests.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(FastPool) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Element class for Fastpool
	class TestFastPoolElement : public PLCore::FastPoolElement<TestFastPoolElement> {
	public:
		int i;
		TestFastPoolElement() : i(0) {}
		TestFastPoolElement(int nOther) : i(nOther) { }
		bool operator ==(const TestFastPoolElement &cOther) const
		{
			return i == cOther.i;
		}
		bool operator ==(int nOther) const
		{
			return i == nOther;
		}
		bool operator !=(int nOther) const
		{
			return i != nOther;
		}
		bool operator !=(const TestFastPoolElement &cOther) const
		{
			return i != cOther.i;
		}
		bool operator !() const
		{
			return !this->i;
		}
	};

	// global operator for left-hand comparison, e.g.:	1 == TestFastPoolElement, and stream operator for error message output
	bool operator ==(int i, const TestFastPoolElement& rhs) {
		return i == rhs.i;
	}
	std::ostream& operator << (std::ostream& stream, TestFastPoolElement const& obj)
    {
		return stream << obj.i;
    }

	// Our Pool Test Fixture :)
	struct ConstructTestFastPool
	{
		ConstructTestFastPool() {
			/* some setup */
			cPool.Add(1);
			cPool.Add(2);
			cPool.Add(3);
			cPool.Add(4);
		}
		~ConstructTestFastPool() {
			/* some teardown */
		}

		// Container for testing
		FastPool<TestFastPoolElement> cPool;
	};

	TEST_FIXTURE(ConstructTestFastPool, DerivedContainerFunctions) {
		CheckDerivedContainerFunctions<TestFastPoolElement>(FastPool<TestFastPoolElement>::Null, cPool);
	}

	TEST_FIXTURE(ConstructTestFastPool, FreeElementsFunctions) {
		CHECK_EQUAL(0U, cPool.GetNumOfFreeElements());

		cPool.Remove(2);
		CHECK_EQUAL(1U, cPool.GetNumOfFreeElements());

		cPool.FreeElements();
		CHECK_EQUAL(4U, cPool.GetNumOfFreeElements());

		cPool.RemoveAllFreeElements();
		CHECK_EQUAL(0U, cPool.GetNumOfFreeElements());
	}

	TEST_FIXTURE(ConstructTestFastPool, FreeSize) {
		CHECK_EQUAL(0U, cPool.GetFreeSize());

		cPool.Remove(2);
		CHECK_EQUAL(sizeof(TestFastPoolElement), cPool.GetFreeSize());
	}
}