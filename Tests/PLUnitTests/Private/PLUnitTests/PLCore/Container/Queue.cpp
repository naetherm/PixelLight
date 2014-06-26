//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Queue.h>

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Queue) {
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
		PLCore::Queue<int> que;
	};

	// Tests migrated from PLTestBase
	TEST_FIXTURE(ConstructTest, Clear_and_GetNumOfElements) {
		CHECK_EQUAL(0U, que.GetNumOfElements());

		que.Push(1);
		que.Push(3);
		que.Push(4);
		que.Push(2);

		CHECK_EQUAL(4U, que.GetNumOfElements());

		que.Clear();

		CHECK_EQUAL(0U, que.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTest, Push) {
		CHECK(que.Push(1));
		CHECK(que.Push(3));
		CHECK(que.Push(4));
		CHECK(que.Push(2));

		CHECK_EQUAL(4U, que.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTest, Pop) {
		que.Push(1);
		que.Push(3);
		que.Push(4);
		que.Push(2);

		int ele;

		CHECK(que.Pop(&ele));
		CHECK_EQUAL(1, ele);

		CHECK(que.Pop(&ele));
		CHECK_EQUAL(3, ele);

		CHECK(que.Pop(&ele));
		CHECK_EQUAL(4, ele);

		CHECK(que.Pop(&ele));
		CHECK_EQUAL(2, ele);

		CHECK_EQUAL(0U, que.GetNumOfElements());

		CHECK(!que.Pop(&ele));
	}

	TEST_FIXTURE(ConstructTest, Copy) {
		que.Push(1);
		que.Push(3);
		que.Push(4);
		que.Push(2);

		PLCore::Queue<int> qCopy(que);

		CHECK_EQUAL(4U, qCopy.GetNumOfElements());

		int ele;

		CHECK(qCopy.Pop(&ele));
		CHECK_EQUAL(1, ele);

		CHECK(qCopy.Pop(&ele));
		CHECK_EQUAL(3, ele);

		CHECK(qCopy.Pop(&ele));
		CHECK_EQUAL(4, ele);

		CHECK(qCopy.Pop(&ele));
		CHECK_EQUAL(2, ele);

		CHECK_EQUAL(0U, qCopy.GetNumOfElements());
	}
}