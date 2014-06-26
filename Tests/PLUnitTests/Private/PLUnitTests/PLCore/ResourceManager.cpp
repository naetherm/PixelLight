//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/String/String.h>
#include <PLCore/Container/ResourceManager.h>
#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(ResourceManager) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class MiniClass : public Resource<MiniClass> {
	public:
		int i;
		MiniClass(const String &sName, ResourceManager<MiniClass> *pManager) :
		Resource<MiniClass>(sName, pManager)
		{
			i = 0;
		}
	};

	class MyManager : public ResourceManager<MiniClass> {
	private:
		virtual MiniClass *CreateResource(const String &sName)
		{
			return new MiniClass(sName, this);
		}
	};

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest()
		{
			/* some setup */
		}
		~ConstructTest() {
			/* some teardown */
		}

		// testing objects
		MyManager lstManager;
	};

	TEST_FIXTURE(ConstructTest, ResourceManager) {
		// Create resources
		MiniClass *pMini0 = lstManager.Create("Mini 0");
		MiniClass *pMini1 = lstManager.Create("Mini 1");
		MiniClass *pMini2 = lstManager.Create("Mini 2");

		CHECK(pMini0);
		CHECK(pMini1);
		CHECK(pMini2);

		CHECK_EQUAL(3U, lstManager.GetNumOfElements());

		CHECK_EQUAL(pMini0, lstManager.GetByIndex(0));
		CHECK_EQUAL(pMini0, lstManager.GetByName("Mini 0"));
		CHECK_EQUAL(pMini1, lstManager.GetByIndex(1));
		CHECK_EQUAL(pMini1, lstManager.GetByName("Mini 1"));
		CHECK_EQUAL(pMini2, lstManager.GetByIndex(2));
		CHECK_EQUAL(pMini2, lstManager.GetByName("Mini 2"));

		CHECK(lstManager.Clear());
		CHECK(!lstManager.GetNumOfElements());
	}


}