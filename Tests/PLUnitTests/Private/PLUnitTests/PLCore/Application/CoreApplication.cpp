//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Application/CoreApplication.h>
#include <PLCore/String/String.h>

using namespace PLCore;

SUITE(CoreApplication) {
	// general objects for testing
	CoreApplication coreApp;
	String sEmpty = "";
	String sNormal = "I_am_a_String";
	String sLong = "I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!";
	String sExpect = "";

	TEST(Enum_ESignal) {
		CoreApplication::ESignal sig = CoreApplication::SignalInterrupt;
		CHECK_EQUAL(1U, (unsigned int) sig);

		sig = CoreApplication::SignalTerm;
		CHECK_EQUAL(2U, (unsigned int) sig);
	}

	TEST(SetName_EmptyString) {
		// TODO: do we know what we can expect here?
		// initial GetName() == name of this projects target name OR something else if we run the app again in android!
		/*
		sExpect = coreApp.GetName();
		#ifdef _DEBUG
			CHECK_EQUAL("PLUnitTestsD", sExpect.GetASCII());
		#else
			CHECK_EQUAL("PLUnitTests", sExpect.GetASCII());
		#endif
		*/

		coreApp.SetName(sEmpty);
		sExpect = coreApp.GetName();
		CHECK_EQUAL(sExpect.GetASCII(), sEmpty.GetASCII());

		sExpect = coreApp.GetConfigName();
		CHECK_EQUAL(sExpect.GetASCII(), (sEmpty + ".cfg").GetASCII());

		sExpect = coreApp.GetLogName();
		CHECK_EQUAL(sExpect.GetASCII(), (sEmpty + ".log").GetASCII());
	}

	TEST(SetName_NormalString) {
		coreApp.SetName(sNormal);
		sExpect = coreApp.GetName();
		CHECK_EQUAL(sExpect.GetASCII(), sNormal.GetASCII());

		sExpect = coreApp.GetConfigName();
		CHECK_EQUAL(sExpect.GetASCII(), (sNormal + ".cfg").GetASCII());

		sExpect = coreApp.GetLogName();
		CHECK_EQUAL(sExpect.GetASCII(), (sNormal + ".log").GetASCII());
	}

	TEST(SetName_LongString) {
		coreApp.SetName(sLong);
		sExpect = coreApp.GetName();
		CHECK_EQUAL(sExpect.GetASCII(), sLong.GetASCII());

		sExpect = coreApp.GetConfigName();
		CHECK_EQUAL(sExpect.GetASCII(), (sLong + ".cfg").GetASCII());

		sExpect = coreApp.GetLogName();
		CHECK_EQUAL(sExpect.GetASCII(), (sLong + ".log").GetASCII());
	}

	TEST(SetVersion) {
		PLCore::Version vVer, vExpect;
		// set a name for comparison
		vVer.SetName("MyVersion");

		coreApp.SetVersion(vVer);
		vExpect = coreApp.GetVersion();
		CHECK_EQUAL(vExpect.GetName().GetASCII(), vVer.GetName().GetASCII());
	}

	TEST(SetMultiUser) {
		bool bExpect = false;

		coreApp.SetMultiUser(true);
		bExpect = coreApp.GetMultiUser();
		CHECK_EQUAL(bExpect, true);

		coreApp.SetMultiUser(false);
		bExpect = coreApp.GetMultiUser();
		CHECK_EQUAL(bExpect, false);
	}

	TEST(SetTitle_EmptyString) {
		coreApp.SetTitle(sEmpty);
		sExpect = coreApp.GetTitle();
		CHECK_EQUAL(sExpect.GetASCII(), sEmpty.GetASCII());
	}

	TEST(SetTitle_NormalString) {
		coreApp.SetTitle(sNormal);
		sExpect = coreApp.GetTitle();
		CHECK_EQUAL(sExpect.GetASCII(), sNormal.GetASCII());
	}

	TEST(SetTitle_LongString) {
		coreApp.SetTitle(sLong);
		sExpect = coreApp.GetTitle();
		CHECK_EQUAL(sExpect.GetASCII(), sLong.GetASCII());
	}

	TEST(SetConfigName_EmptyString) {
		coreApp.SetConfigName(sEmpty);
		sExpect = coreApp.GetConfigName();
		CHECK_EQUAL(sExpect.GetASCII(), sEmpty.GetASCII());
	}

	TEST(SetConfigName_NormalString) {
		coreApp.SetConfigName(sNormal);
		sExpect = coreApp.GetConfigName();
		CHECK_EQUAL(sExpect.GetASCII(), sNormal.GetASCII());
	}

	TEST(SetConfigName_LongString) {
		coreApp.SetConfigName(sLong);
		sExpect = coreApp.GetConfigName();
		CHECK_EQUAL(sExpect.GetASCII(), sLong.GetASCII());
	}

	TEST(SetLogName_EmptyString) {
		coreApp.SetLogName(sEmpty);
		sExpect = coreApp.GetLogName();
		CHECK_EQUAL(sExpect.GetASCII(), sEmpty.GetASCII());
	}

	TEST(SetLogName_NormalString) {
		coreApp.SetLogName(sNormal);
		sExpect = coreApp.GetLogName();
		CHECK_EQUAL(sExpect.GetASCII(), sNormal.GetASCII());
	}

	TEST(SetLogName_LongString) {
		coreApp.SetLogName(sLong);
		sExpect = coreApp.GetLogName();
		CHECK_EQUAL(sExpect.GetASCII(), sLong.GetASCII());
	}

	TEST(SetAppDataSubdir_EmptyString) {
		// [TODO] SetAppDataSubdir takes ALL strings, e.g. broken paths will be set!
		//        but: doc says you should use SetAppDataSubdir(System::GetInstance()->GetDataDirName("MyApplication"))
		coreApp.SetAppDataSubdir(sEmpty);
		sExpect = coreApp.GetAppDataSubdir();
		CHECK_EQUAL(sExpect.GetASCII(), sEmpty.GetASCII());
	}

	TEST(SetAppDataSubdir_NormalString) {
		coreApp.SetAppDataSubdir(sNormal);
		sExpect = coreApp.GetAppDataSubdir();
		CHECK_EQUAL(sExpect.GetASCII(), sNormal.GetASCII());
	}

	TEST(SetAppDataSubdir_LongString) {
		coreApp.SetAppDataSubdir(sLong);
		sExpect = coreApp.GetAppDataSubdir();
		CHECK_EQUAL(sExpect.GetASCII(), sLong.GetASCII());
	}
}