//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Container/Stack.h>

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Stack) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our List Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() {
			/* some setup */
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Container for testing
		Stack<int> stack;
	};

// Tests migrated from PLTestBase
	TEST_FIXTURE(ConstructTest, Clear_and_GetNumOfElements) {
		CHECK_EQUAL(0U, stack.GetNumOfElements());

		stack.Push(1);
		stack.Push(3);
		stack.Push(4);
		stack.Push(2);

		CHECK_EQUAL(4U, stack.GetNumOfElements());

		stack.Clear();

		CHECK_EQUAL(0U, stack.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTest, Push) {
		CHECK(stack.Push(1));
		CHECK(stack.Push(3));
		CHECK(stack.Push(4));
		CHECK(stack.Push(2));

		CHECK_EQUAL(4U, stack.GetNumOfElements());
	}

	TEST_FIXTURE(ConstructTest, Pop) {
		stack.Push(1);
		stack.Push(3);
		stack.Push(4);
		stack.Push(2);

		int ele;

		CHECK(stack.Pop(&ele));
		CHECK_EQUAL(2, ele);

		CHECK(stack.Pop(&ele));
		CHECK_EQUAL(4, ele);

		CHECK(stack.Pop(&ele));
		CHECK_EQUAL(3, ele);

		CHECK(stack.Pop(&ele));
		CHECK_EQUAL(1, ele);

		CHECK_EQUAL(0U, stack.GetNumOfElements());

		CHECK(!stack.Pop(&ele));
	}

	TEST_FIXTURE(ConstructTest, Copy) {
		stack.Push(1);
		stack.Push(3);
		stack.Push(4);
		stack.Push(2);

		Stack<int> stackCopy(stack);

		CHECK_EQUAL(4U, stackCopy.GetNumOfElements());

		int ele;

		CHECK(stackCopy.Pop(&ele));
		CHECK_EQUAL(2, ele);

		CHECK(stackCopy.Pop(&ele));
		CHECK_EQUAL(4, ele);

		CHECK(stackCopy.Pop(&ele));
		CHECK_EQUAL(3, ele);

		CHECK(stackCopy.Pop(&ele));
		CHECK_EQUAL(1, ele);

		CHECK_EQUAL(0U, stackCopy.GetNumOfElements());
	}
}