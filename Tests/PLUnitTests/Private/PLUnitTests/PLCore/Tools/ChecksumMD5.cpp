//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Tools/ChecksumMD5.h>
#include <PLCore/File/File.h>
#include <PLCore/String/String.h>

#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(ChecksumMD5) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest()
			/* some setup */
		{
		}
		~ConstructTest() {
			/* some teardown */
		}

		// Container for testing
		PLCore::ChecksumMD5 sumMD5;
		PLCore::String sChecksum;
	};

	PLCore::String CheckFile(PLCore::Checksum &cChecksum, const PLCore::String &sFilename)
	{
		// Check the given parameter
		if (!sFilename.GetLength()) return "";

		// Open the file
		PLCore::File cFile(sFilename);

		// Fool the checksum function ;-)
		/* Assertion...
		String sChecksum = Checksum::Get(sClass, pFile);
		if (sChecksum.GetLength()) {
		pFile->Release();

		// Error!
		return "";
		}
		*/

		// Open the file
		if (!cFile.Open(PLCore::File::FileRead)) {

			// Error!
			return "";
		}

		// Get the checksum
		PLCore::String sChecksum = cChecksum.Get(cFile);

		// Check the current file offset
		if (cFile.Tell()) sChecksum = "";

		// Close the file
		cFile.Close();

		// Done
		return sChecksum;
	};

	// ChecksumMD5
	// ChecksumMD5: Get(const PLCore::uint8 nBuffer[], PLCore::uint32 nNumOfBytes)
	TEST_FIXTURE(ConstructTest, ChecksumMD5__Get_const_uint8_nBuffer____uint32_nNumOfBytes_) {
		// Alphabet
		sChecksum = sumMD5.Get(reinterpret_cast<const PLCore::uint8*>("abcdefghijklmnopqrstuvwxyz"), static_cast<PLCore::uint32>(strlen("abcdefghijklmnopqrstuvwxyz")));
		CHECK_EQUAL("c3fcd3d76192e4007dfb496cca67e13b", sChecksum.GetASCII());

		// Corrupted alphabet (one of the letters in the alphabet (in this case 'm') is changed to uppercase)
		sChecksum = sumMD5.Get(reinterpret_cast<const PLCore::uint8*>("abcdefghijklMnopqrstuvwxyz"), static_cast<PLCore::uint32>(strlen("abcdefghijklMnopqrstuvwxyz")));
		CHECK_NOT_EQUAL("c3fcd3d76192e4007dfb496cca67e13b", sChecksum.GetASCII());

		// Alphabet and numbers
		sChecksum = sumMD5.Get(reinterpret_cast<const PLCore::uint8*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), static_cast<PLCore::uint32>(strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")));
		// [TODO] fails!
		CHECK_EQUAL("d174ab98d277d9f5a5611c2c9f419d9f", sChecksum.GetASCII());

		// Corrupted alphabet and numbers ('9' removed)
		sChecksum = sumMD5.Get(reinterpret_cast<const PLCore::uint8*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678"), static_cast<PLCore::uint32>(strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678")));
		CHECK_NOT_EQUAL("d174ab98d277d9f5a5611c2c9f419d9f", sChecksum.GetASCII());
	}

	// ChecksumMD5: Get(const String &sString)
	TEST_FIXTURE(ConstructTest, ChecksumMD5__Get_const_String__sString_) {
		// Alphabet
		sChecksum = sumMD5.Get("abcdefghijklmnopqrstuvwxyz");
		CHECK_EQUAL("c3fcd3d76192e4007dfb496cca67e13b", sChecksum.GetASCII());

		// Corrupted alphabet (one of the letters in the alphabet (in this case 'm') is changed to uppercase)
		sChecksum = sumMD5.Get("abcdefghijklMnopqrstuvwxyz");
		CHECK_NOT_EQUAL("c3fcd3d76192e4007dfb496cca67e13b", sChecksum.GetASCII());

		// Alphabet and numbers
		sChecksum = sumMD5.Get("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		// [TODO] fails!
		CHECK_EQUAL("d174ab98d277d9f5a5611c2c9f419d9f", sChecksum.GetASCII());

		// Corrupted alphabet and numbers ('9' removed)
		sChecksum = sumMD5.Get("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678");
		CHECK_NOT_EQUAL("d174ab98d277d9f5a5611c2c9f419d9f", sChecksum.GetASCII());

		// Unicode
		// [TODO] warning: universal character name encountered in source
		sChecksum = sumMD5.Get(L"\u65e5\u672c\u8a9e");
#ifdef LINUX
		// Under linux the checksum differs because PLCore::String uses the wchar_t type for storing unicode string data
		// The size of this type differs between windows and linux (under windows the size of wchar_t is 2 byte and under linux the size is 4 byte)
		const char* expectedChecksum = "5cc5f3b56ed974723ce4cb6c63fbb7a1";
#else
		const char* expectedChecksum = "965f626b62b499198df91ae051f13bd2";
#endif
		CHECK_EQUAL(expectedChecksum, sChecksum.GetASCII());

		// Alphabet and numbers - this time as Unicode
		sChecksum = sumMD5.Get(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		CHECK_NOT_EQUAL("d174ab98d277d9f5a5611c2c9f419d9f", sChecksum.GetASCII());
	}

	// ChecksumMD5: Get(File &cFile)
	TEST_FIXTURE(ConstructTest, ChecksumMD5__Get_File__cFile_) {
		// Check 'demotest.xml'
		sChecksum = CheckFile(sumMD5, "../Data/unitTestData/ChecksumTest_demo.xml");
		CHECK_EQUAL("313a0294c72a84409c9dd6267d44730f", sChecksum.GetASCII());

		// Check 'tokenizer.txt'
		sChecksum = CheckFile(sumMD5, "../Data/unitTestData/ChecksumTest_tokenizer.txt");
		// [TODO] fails!
		CHECK_EQUAL("3ef605c74b399d7d869afb40de4cede4", sChecksum.GetASCII());

		// Check 'test.zip'
		sChecksum = CheckFile(sumMD5, "../Data/unitTestData/ChecksumTest_test.zip");
		// [TODO] fails!
		CHECK_EQUAL("898d399c87c96a6f73814b1940fbca16", sChecksum.GetASCII());
	}

	// ChecksumMD5: GetFile(const String &sFilename)
	TEST_FIXTURE(ConstructTest, ChecksumMD5__Get_const_String__sFilename_) {
		// Check 'demotest.xml'
		sChecksum = sumMD5.GetFile("../Data/unitTestData/ChecksumTest_demo.xml");
		CHECK_EQUAL("313a0294c72a84409c9dd6267d44730f", sChecksum.GetASCII());

		// Check 'tokenizer.txt'
		sChecksum = sumMD5.GetFile("../Data/unitTestData/ChecksumTest_tokenizer.txt");
		// [TODO] fails!
		CHECK_EQUAL("3ef605c74b399d7d869afb40de4cede4", sChecksum.GetASCII());

		// Check 'test.zip'
		sChecksum = sumMD5.GetFile("../Data/unitTestData/ChecksumTest_test.zip");
		// [TODO] fails!
		CHECK_EQUAL("898d399c87c96a6f73814b1940fbca16", sChecksum.GetASCII());
	}
}