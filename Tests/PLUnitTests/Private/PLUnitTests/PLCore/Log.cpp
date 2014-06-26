//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Log) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	TEST(GetInstance) {
		// get Log Instance
		Log *pLog = Log::GetInstance();
		CHECK(pLog);

		// open log
		CHECK(pLog->Open("../Data/unitTestData/TestLog.txt"));

		// print out some logs
		PL_LOG(Info, "Log-system started")
		PL_LOG(Error, "Failed to reach the end of the world")

		// close log
		CHECK(pLog->Close());

		// [TODO] check the content of the log?
	}
}