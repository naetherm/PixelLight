//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/File/File.h>
#include <PLCore/File/Directory.h>
#include <PLCore/File/FileSearch.h>

#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(FileSystem) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	TEST(Read_File_List_Of_Zip) {
		// Read directory list of 'test.zip'
		Directory cDir("../Data/unitTestData/FileSystemTest.zip/");
		CHECK(cDir.Exists());
		CHECK(!cDir.IsFile());
		CHECK(cDir.IsDirectory());

		if (cDir.Exists() && cDir.IsDirectory()) {
			// Read directory list
			FileSearch cSearch(cDir);
			bool bDirectory = false;
			bool bTestTxt   = false;
			bool bTest2Zip  = false;
			int  nFound     = 0;

			while (cSearch.HasNextFile()) {
				String sFile = cSearch.GetNextFile();
				nFound++;

				// directory/
				if (sFile == "dir/")
					bDirectory = true;

				// test.txt
				else if (sFile == "aTestLog.txt")
					bTestTxt = true;

				// test2.zip
				else if (sFile == "Main.zip")
					bTest2Zip = true;
			}
			CHECK_EQUAL(3, nFound);
			CHECK(bDirectory);
			CHECK(bTestTxt);
			CHECK(bTest2Zip);
		}
	}

	TEST(Read_File_In_Zip) {
		// Open 'aTestLog.txt' inside 'FileSystemTest.zip/'
		File cFile("../Data/unitTestData/FileSystemTest.zip/aTestLog.txt");

		CHECK(cFile.Exists());
		CHECK(cFile.IsFile());

		if (cFile.Exists() && cFile.IsFile()) {
			CHECK(cFile.Open(File::FileRead));

			// Open file
			if (cFile.Open(File::FileRead)) {
				// Read text
				String sString;
				sString = cFile.GetS();		// Read 'This is the first test-text,'
				CHECK_EQUAL("[Always]: This is just a simple ASCII log for best compatibility, Unicode characters may not be shown correctly\n", sString);

				sString = cFile.GetS();		// Read 'which is inside a ZIP-file.'
				CHECK_EQUAL("[Info]: Log-system started\n", sString);

				sString = cFile.GetS();		// Read ''
				CHECK_EQUAL("[Error]: Failed to reach the end of the world\n", sString);

				sString = cFile.GetS();		// Read 'blah'
				CHECK_EQUAL("Close log\n", sString);

				sString = cFile.GetS();		// Read ''
				CHECK_EQUAL("", sString);

				// Close the file
				cFile.Close();
			};
		}
	}
}