//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Application/ApplicationContext.h>
#include <PLCore/String/String.h>
#include <PLCore/Container/Array.h>
#include <PLCore/File/Url.h>

using namespace PLCore;

/*
 * Naming Convention for SUITE:
 * CLASSNAME
 */
SUITE(ApplicationContext) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/
	// general objects for testing
	ApplicationContext App;
	String sEmpty = "";
	String sNormal = "I_am_a_String";
	String sLong = "I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!_I_am_a_very_long_String_that_will_be_repeated_often!";

	// SetExecutableFilename sets:
	// - executable filename
	// - executable directory
	// - application directory
	// and removes trailing '/'
	// and application directory defaults to '.' if no application directory was given
	//
	TEST(SetExecutableFilename_EmptyString) {
		// initially: no Filename!
		CHECK_EQUAL(sEmpty.GetASCII(), App.GetExecutableFilename().GetASCII());

		// set empty name
		App.SetExecutableFilename(sEmpty);
		CHECK_EQUAL(sEmpty.GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory is empty if no path was given
		CHECK_EQUAL(Url("").GetNativePath().GetASCII(), App.GetExecutableDirectory().GetASCII());

		// ApplicationDirectory defaults to "." if no name was given
		CHECK_EQUAL(Url(".").GetNativePath().GetASCII(), App.GetAppDirectory().GetASCII());
	}

	TEST(SetExecutableFilename_NormalString) {
		// set name only
		App.SetExecutableFilename(sNormal);
		CHECK_EQUAL(sNormal.GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory is empty if no path was given
		CHECK_EQUAL(Url("").GetNativePath().GetASCII(), App.GetExecutableDirectory().GetASCII());

		// ApplicationDirectory will be set to ".." if only a executable name was given
		CHECK_EQUAL(Url("..").GetNativePath().GetASCII(), App.GetAppDirectory().GetASCII());
	}

	TEST(SetExecutableFilename_LongName) {
		// set name only
		App.SetExecutableFilename(sLong);
		CHECK_EQUAL(sLong.GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory is empty if no path was given
		CHECK_EQUAL(Url("").GetNativePath().GetASCII(), App.GetExecutableDirectory().GetASCII());

		// ApplicationDirectory will be set to ".." if only a executable name was given
		CHECK_EQUAL(Url("..").GetNativePath().GetASCII(), App.GetAppDirectory().GetASCII());
	}

	TEST(SetExecutableFilename_ShortValidFilePath) {
		String sPath = Url("C:\\").GetNativePath() + sEmpty;

		// set valid path
		App.SetExecutableFilename(sPath);
		CHECK_EQUAL(sPath.GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory
		CHECK_EQUAL("C:", App.GetExecutableDirectory().GetASCII());

		// ApplicationDirectory
		CHECK_EQUAL("C:", App.GetAppDirectory().GetASCII());
	}

	TEST(SetExecutableFilename_AverageValidFilePath) {
		String sPath = Url("C:\\TestPath\\").GetNativePath() + sNormal;

		// set valid path
		App.SetExecutableFilename(sPath);
		CHECK_EQUAL(sPath.GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory
		CHECK_EQUAL(Url("C:\\TestPath").GetNativePath().GetASCII(), App.GetExecutableDirectory().GetASCII());

		// ApplicationDirectory
		CHECK_EQUAL("C:", App.GetAppDirectory().GetASCII());
	}

	TEST(SetExecutableFilename_LongValidFilePath) {
		String sPath = Url("C:\\TestPathRoot\\TestPath\\").GetNativePath() + sLong;

		// set valid path
		App.SetExecutableFilename(sPath);
		CHECK_EQUAL(sPath.GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory
		CHECK_EQUAL(Url("C:\\TestPathRoot\\TestPath").GetNativePath().GetASCII(), App.GetExecutableDirectory().GetASCII());

		// ApplicationDirectory
		CHECK_EQUAL(Url("C:\\TestPathRoot").GetNativePath().GetASCII(), App.GetAppDirectory().GetASCII());
	}

	TEST(SetExecutableFilename_NonValidFilePath) {
		String sPath = Url("!§%&:\\TestPath\\\\").GetNativePath().GetASCII() + sNormal;

		// set nonValid path
		App.SetExecutableFilename(sPath);
		CHECK_EQUAL(Url("TestPath\\" + sNormal).GetNativePath().GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory
		CHECK_EQUAL(Url("TestPath").GetNativePath().GetASCII(), App.GetExecutableDirectory().GetASCII());

		// ApplicationDirectory defaults to '.' if path is not valid
		CHECK_EQUAL(Url(".").GetNativePath().GetASCII(), App.GetAppDirectory().GetASCII());
	}

	TEST(SetExecutableFilename_ValidHttpPath) {
		String sPath = "http://TestDir/" + sNormal;

		// [TODO] should this be possible?
		App.SetExecutableFilename(sPath);
		CHECK_EQUAL(sPath.GetASCII(), App.GetExecutableFilename().GetASCII());

		// ExecutableDirectory
		CHECK_EQUAL(Url("http://TestDir").GetNativePath().GetASCII(), App.GetExecutableDirectory().GetASCII());

		// Not supported
		// CHECK_EQUAL(Url("http:").GetNativePath().GetASCII(), App.GetAppDirectory().GetASCII());
	}

	TEST(SetArguments_NoElements) {
		Array<String> elems;
		CHECK(elems.Compare(App.GetArguments()));

		// set empty arguments array
		App.SetArguments(elems);

		// compare content of both arrays, has to be the same!
		CHECK(elems.Compare(App.GetArguments()));
	}

	TEST(SetArguments_WithElements) {
		Array<String> elems;
		elems.Add(String("Elem1"));
		elems.Add(String("Elem2"));

		// set arguments array with 2 elements
		App.SetArguments(elems);
		CHECK(elems.Compare(App.GetArguments()));
	}

	TEST(SetStartupDirectory_EmptyString) {
		// set empty path
		App.SetStartupDirectory(sEmpty);
		CHECK_EQUAL(sEmpty.GetASCII(), App.GetStartupDirectory().GetASCII());
	}

	TEST(SetStartupDirectory_ValidPath) {
		String sPath = Url("C:\\").GetNativePath().GetASCII() + sNormal;

		// set directory
		App.SetStartupDirectory(sPath);
		CHECK_EQUAL(sPath.GetASCII(), App.GetStartupDirectory().GetASCII());
	}

	TEST(SetLogFilename_EmptyName) {
		// set empty
		App.SetLogFilename(sEmpty);
		CHECK_EQUAL(sEmpty.GetASCII(), App.GetLogFilename().GetASCII());
	}

	TEST(SetLogFilename_ValidName) {
		// set name
		App.SetLogFilename(sNormal);
		CHECK_EQUAL(sNormal.GetASCII(), App.GetLogFilename().GetASCII());
	}

	TEST(SetConfigFilename_EmptyName) {
		// set empty
		App.SetConfigFilename(sEmpty);
		CHECK_EQUAL(sEmpty.GetASCII(), App.GetConfigFilename().GetASCII());
	}

	TEST(SetConfigFilename_ValidName) {
		// set name
		App.SetConfigFilename(sNormal);
		CHECK_EQUAL(sNormal.GetASCII(), App.GetConfigFilename().GetASCII());
	}
}