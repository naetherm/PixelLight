//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Tools/ChecksumCRC32.h>
#include <PLCore/File/File.h>
#include <PLCore/String/String.h>

#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(ChecksumCRC32) {
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
		ChecksumCRC32 sumCRC;
		String sChecksum;
	};

	String CheckFile(Checksum &cChecksum, const String &sFilename)
	{
		// Check the given parameter
		if (!sFilename.GetLength()) return "";

		// Open the file
		File cFile(sFilename);

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
		if (!cFile.Open(File::FileRead)) {

			// Error!
			return "";
		}

		// Get the checksum
		String sChecksum = cChecksum.Get(cFile);

		// Check the current file offset
		if (cFile.Tell()) sChecksum = "";

		// Close the file
		cFile.Close();

		// Done
		return sChecksum;
	};

	// ChecksumCRC32
	// ChecksumCRC32: Get(const uint8 nBuffer[], uint32 nNumOfBytes)
	TEST_FIXTURE(ConstructTest, ChecksumCRC32__Get_const_uint8_nBuffer____uint32_nNumOfBytes_) {
		// Alphabet
		sChecksum = sumCRC.Get(reinterpret_cast<const uint8*>("abcdefghijklmnopqrstuvwxyz"), static_cast<uint32>(strlen("abcdefghijklmnopqrstuvwxyz")));
		CHECK_EQUAL(String("4c2750bd").ToLower(), sChecksum.ToLower());

		// Corrupted alphabet (one of the letters in the alphabet (in this case 'm') is changed to uppercase)
		sChecksum = sumCRC.Get(reinterpret_cast<const uint8*>("abcdefghijklMnopqrstuvwxyz"), static_cast<uint32>(strlen("abcdefghijklMnopqrstuvwxyz")));
		CHECK_NOT_EQUAL("4c2750bd", sChecksum.ToLower());

		// Alphabet and numbers
		sChecksum = sumCRC.Get(reinterpret_cast<const uint8*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), static_cast<uint32>(strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")));
		CHECK_EQUAL(String("1FC2E6D2").ToLower(), sChecksum.ToLower());

		// Corrupted alphabet and numbers ('9' removed)
		sChecksum = sumCRC.Get(reinterpret_cast<const uint8*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678"), static_cast<uint32>(strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678")));
		CHECK_NOT_EQUAL("170fccc", sChecksum.ToLower());
	}

	// ChecksumCRC32: Get(const String &sString)
	TEST_FIXTURE(ConstructTest, ChecksumCRC32__Get_const_String__sString_) {
		// Alphabet
		sChecksum = sumCRC.Get("abcdefghijklmnopqrstuvwxyz");
		CHECK_EQUAL(String("4c2750bd").ToLower(), sChecksum.ToLower());

		// Corrupted alphabet (one of the letters in the alphabet (in this case 'm') is changed to uppercase)
		sChecksum = sumCRC.Get("abcdefghijklMnopqrstuvwxyz");
		CHECK_NOT_EQUAL("4c2750bd", sChecksum.ToLower());

		// Alphabet and numbers
		sChecksum = sumCRC.Get("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		CHECK_EQUAL(String("1fc2e6d2"), sChecksum.ToLower());

		// Corrupted alphabet and numbers ('9' removed)
		sChecksum = sumCRC.Get("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345678");
		CHECK_NOT_EQUAL("1fc2e6d2", sChecksum.ToLower());

		// Unicode
		sChecksum = sumCRC.Get(L"\u65e5\u672c\u8a9e");
#ifdef LINUX
		// Under linux the checksum differs because PLCore::String uses the wchar_t type for storing unicode string data
		// The size of this type differs between windows and linux (under windows the size of wchar_t is 2 byte and under linux the size is 4 byte)
		const char* expectedChecksum = "db95855e";
#else
		const char* expectedChecksum = "e34ad326";
#endif
		CHECK_EQUAL(expectedChecksum, sChecksum.ToLower());

		// Alphabet and numbers - this time as Unicode
		sChecksum = sumCRC.Get(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		CHECK_NOT_EQUAL("e34ad326", sChecksum.ToLower());
	}

	// ChecksumCRC32: Get(File &cFile)
	TEST_FIXTURE(ConstructTest, ChecksumCRC32__Get_File__cFile_) {
		// Check 'demotest.xml'
		sChecksum = CheckFile(sumCRC, "../Data/unitTestData/ChecksumTest_demo.xml");
		CHECK_EQUAL(String("f7a94f8d").ToLower(), sChecksum.ToLower());

		// Check 'tokenizer.txt'
		sChecksum = CheckFile(sumCRC, "../Data/unitTestData/ChecksumTest_tokenizer.txt");
		CHECK_EQUAL(String("843cd3ed").ToLower(), sChecksum.ToLower());

		// Check 'test.zip'
		sChecksum = CheckFile(sumCRC, "../Data/unitTestData/ChecksumTest_test.zip");
		CHECK_EQUAL(String("8076cc81").ToLower(), sChecksum.ToLower());
	}

	// ChecksumCRC32: GetFile(const String &sFilename)
	TEST_FIXTURE(ConstructTest, ChecksumCRC32__Get_const_String__sFilename_) {
		// Check 'demotest.xml'
		sChecksum = sumCRC.GetFile("../Data/unitTestData/ChecksumTest_demo.xml");
		CHECK_EQUAL(String("f7a94f8d").ToLower(), sChecksum.ToLower());

		// Check 'tokenizer.txt'
		sChecksum = sumCRC.GetFile("../Data/unitTestData/ChecksumTest_tokenizer.txt");
		CHECK_EQUAL(String("843cd3ed").ToLower(), sChecksum.ToLower());

		// Check 'test.zip'
		sChecksum = sumCRC.GetFile("../Data/unitTestData/ChecksumTest_test.zip");
		CHECK_EQUAL(String("8076cc81").ToLower(), sChecksum.ToLower());
	}
}