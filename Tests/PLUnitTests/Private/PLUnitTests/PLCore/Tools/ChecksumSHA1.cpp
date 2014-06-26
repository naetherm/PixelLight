//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Tools/ChecksumCRC32.h>
#include <PLCore/Tools/ChecksumMD5.h>
#include <PLCore/Tools/ChecksumSHA1.h>
#include <PLCore/File/File.h>
#include <PLCore/String/String.h>

#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(ChecksumSHA1) {
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
		PLCore::ChecksumSHA1 sumSHA1;
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

	// ChecksumSHA1
	// ChecksumSHA1: Get(const PLCore::uint8 nBuffer[], PLCore::uint32 nNumOfBytes)
	TEST_FIXTURE(ConstructTest, ChecksumSHA1__Get_const_uint8_nBuffer____uint32_nNumOfBytes_) {
		// Alphabet
		sChecksum = sumSHA1.Get(reinterpret_cast<const PLCore::uint8*>("abcdefghijklmnopqrstuvwxyz"), static_cast<PLCore::uint32>(strlen("abcdefghijklmnopqrstuvwxyz")));
		CHECK_EQUAL("32d10c7b8cf96570ca04ce37f2a19d84240d3a89", sChecksum.GetASCII());

		// Corrupted alphabet (one of the letters in the alphabet (in this case 'm') is changed to uppercase)
		sChecksum = sumSHA1.Get(reinterpret_cast<const PLCore::uint8*>("abcdefghijklMnopqrstuvwxyz"), static_cast<PLCore::uint32>(strlen("abcdefghijklMnopqrstuvwxyz")));
		CHECK_NOT_EQUAL("32d10c7b8cf96570ca04ce37f2a19d84240d3a89", sChecksum.GetASCII());

		// Alphabet and numbers
		sChecksum = sumSHA1.Get(reinterpret_cast<const PLCore::uint8*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), static_cast<PLCore::uint32>(strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")));
		// [TODO] fails!
		CHECK_EQUAL("761c457bf73b14d27e9e9265c46f4b4dda11f940", sChecksum.GetASCII());

		// Corrupted alphabet and numbers ('9' removed)
		sChecksum = sumSHA1.Get(reinterpret_cast<const PLCore::uint8*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678"), static_cast<PLCore::uint32>(strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678")));
		CHECK_NOT_EQUAL("761c457bf73b14d27e9e9265c46f4b4dda11f940", sChecksum.GetASCII());
	}

	// ChecksumSHA1: Get(const String &sString)
	TEST_FIXTURE(ConstructTest, ChecksumSHA1__Get_const_String__sString_) {
		// Alphabet
		sChecksum = sumSHA1.Get("abcdefghijklmnopqrstuvwxyz");
		CHECK_EQUAL("32d10c7b8cf96570ca04ce37f2a19d84240d3a89", sChecksum.GetASCII());

		// Corrupted alphabet (one of the letters in the alphabet (in this case 'm') is changed to uppercase)
		sChecksum = sumSHA1.Get("abcdefghijklMnopqrstuvwxyz");
		CHECK_NOT_EQUAL("32d10c7b8cf96570ca04ce37f2a19d84240d3a89", sChecksum.GetASCII());

		// Alphabet and numbers
		sChecksum = sumSHA1.Get("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		// [TODO] fails!
		CHECK_EQUAL("761c457bf73b14d27e9e9265c46f4b4dda11f940", sChecksum.GetASCII());

		// Corrupted alphabet and numbers ('9' removed)
		sChecksum = sumSHA1.Get("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678");
		CHECK_NOT_EQUAL("761c457bf73b14d27e9e9265c46f4b4dda11f940", sChecksum.GetASCII());

		// Unicode
		sChecksum = sumSHA1.Get(L"\u65e5\u672c\u8a9e");
#ifdef LINUX
		// Under linux the checksum differs because PLCore::String uses the wchar_t type for storing unicode string data
		// The size of this type differs between windows and linux (under windows the size of wchar_t is 2 byte and under linux the size is 4 byte)
		const char* expectedChecksum = "e9e69ede9a65adc48768e0d52179cfa571344c92";
#else
		const char* expectedChecksum = "5c9941a48c296ef5cc2f267874c8dafeeb36a385";
#endif
		// [TODO] fails!
		CHECK_EQUAL(expectedChecksum, sChecksum.GetASCII());

		// Alphabet and numbers - this time as Unicode
		sChecksum = sumSHA1.Get(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		CHECK_NOT_EQUAL("5c9941a48c296ef5cc2f267874c8dafeeb36a385", sChecksum.GetASCII());
	}

	// ChecksumSHA1: Get(File &cFile)
	TEST_FIXTURE(ConstructTest, ChecksumSHA1__Get_File__cFile_) {
		// Check 'demotest.xml'
		sChecksum = CheckFile(sumSHA1, "../Data/unitTestData/ChecksumTest_demo.xml");
		CHECK_EQUAL("3225f273bb21d4252ac978da0d71fadb1e78eb7f", sChecksum.GetASCII());

		// Check 'tokenizer.txt'
		sChecksum = CheckFile(sumSHA1, "../Data/unitTestData/ChecksumTest_tokenizer.txt");
		// [TODO] fails!
		CHECK_EQUAL("b6f311adc109cfe58ec1156ad926260c725f3945", sChecksum.GetASCII());

		// Check 'test.zip'
		sChecksum = CheckFile(sumSHA1, "../Data/unitTestData/ChecksumTest_test.zip");
		// [TODO] fails!
		CHECK_EQUAL("1e531304afa05d60b5f9d1cd11b81cdbfae90947", sChecksum.GetASCII());
	}

	// ChecksumSHA1: GetFile(const String &sFilename)
	TEST_FIXTURE(ConstructTest, ChecksumSHA1__Get_const_String__sFilename_) {
		// Check 'demotest.xml'
		sChecksum = sumSHA1.GetFile("../Data/unitTestData/ChecksumTest_demo.xml");
		CHECK_EQUAL("3225f273bb21d4252ac978da0d71fadb1e78eb7f", sChecksum.GetASCII());

		// Check 'tokenizer.txt'
		sChecksum = sumSHA1.GetFile("../Data/unitTestData/ChecksumTest_tokenizer.txt");
		// [TODO] fails!
		CHECK_EQUAL("b6f311adc109cfe58ec1156ad926260c725f3945", sChecksum.GetASCII());

		// Check 'test.zip'
		sChecksum = sumSHA1.GetFile("../Data/unitTestData/ChecksumTest_test.zip");
		// [TODO] fails!
		CHECK_EQUAL("1e531304afa05d60b5f9d1cd11b81cdbfae90947", sChecksum.GetASCII());
	}
}