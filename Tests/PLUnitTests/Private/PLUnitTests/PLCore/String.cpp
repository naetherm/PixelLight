//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#ifdef LINUX
#include <wchar.h>
#include <PLCore/PLCoreLinuxWrapper.h>
#endif
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#include <UnitTest++/UnitTest++.h>
#include <PLCore/String/String.h>
#include "UnitTest++AddIns/wchar_template.h"
#include "UnitTest++AddIns/PLCheckMacros.h"

#include "UnitTest++AddIns/PLChecks.h"
#include <UnitTest++AddIns/LocaleWrapper.h>

using namespace PLCore;

/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(String) {
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
		PLCore::String sString, sStringT;
		const wchar_t *pszTempWide;
		const char *pszTempUTF8;
		const char *pszTemp;
	};

	/*
	* Tests migrated from PLTestBase
	*/
	TEST_FIXTURE(ConstructTest, Constructors) {
		{
			String sTest;
			CHECK(!(sTest.GetLength()));
			CHECK(!(strlen(sTest.GetASCII())));
		}
		{
			String sTest('a');
			CHECK_EQUAL(1U, sTest.GetLength());
			CHECK_EQUAL("a", sTest.GetASCII());
		}
		{
			String sTest('\0');
			CHECK(!(sTest.GetLength()));
		}
		{
			String sTest(L'a');
			CHECK_EQUAL(1U, sTest.GetLength());
			CHECK_EQUAL(L"a", sTest.GetUnicode());
		}
		{
			// [TODO] next line emits "warning C4428: universal-character-name encountered in source"
			String sTest(L'\u9f8d');
			CHECK_EQUAL(1U, sTest.GetLength());
			CHECK_EQUAL(L"\u9f8d", sTest.GetUnicode());
		}
		{
			String sTest(L'\0');
			CHECK(!(sTest.GetLength()));
		}
		{
			String sTest(static_cast<const char*>(nullptr));
			CHECK(!(sTest.GetLength()));
		}
		{
			String sTest("%");
			CHECK_EQUAL(1U, sTest.GetLength());
			CHECK_EQUAL("%", sTest.GetASCII());
		}
		{
			String sTest("123");
			CHECK_EQUAL(3U, sTest.GetLength());
			CHECK_EQUAL("123", sTest.GetASCII());
		}
		{
			char *pszTest = new char[4];
			strcpy(pszTest, "123");
			String sTest(pszTest, false);
			CHECK_EQUAL(3U, sTest.GetLength());
			CHECK_EQUAL("123" , sTest.GetASCII());
			CHECK_EQUAL(pszTest, sTest.GetASCII());
		}
		{
			String sTest(static_cast<const wchar_t*>(nullptr));
			CHECK(!(sTest.GetLength()));
		}
		{
			String sTest(L"%");
			CHECK_EQUAL(1U, sTest.GetLength());
			CHECK_EQUAL(L"%", sTest.GetUnicode());
		}
		{
			String sTest(L"123");
			CHECK_EQUAL(3U, sTest.GetLength());
			CHECK_EQUAL(L"123", sTest.GetUnicode());
		}
		{
			String sTest(L"\u65e5\u672c\u8a9e");
			CHECK_EQUAL(3U, sTest.GetLength());
			CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sTest.GetUnicode());
		}
		{
			wchar_t *pszTest = new wchar_t[4];
			wcscpy(pszTest, L"\u65e5\u672c\u8a9e");
			String sTest(pszTest, false);
			CHECK_EQUAL(3U, sTest.GetLength());
			CHECK_EQUAL(L"\u65e5\u672c\u8a9e" , sTest.GetUnicode());
			CHECK_EQUAL(pszTest, sTest.GetUnicode());
		}
	}

	TEST_FIXTURE(ConstructTest, GetLength) {
		sString = "";
		CHECK(!(sString.GetLength()));

		sString = "1";
		CHECK_EQUAL(1U, sString.GetLength());

		sString = "This 1_6";
		CHECK_EQUAL(8U, sString.GetLength());

		sString = static_cast<char*>(nullptr);
		CHECK(!(sString.GetLength()));

		sString = static_cast<const char*>(nullptr);
		CHECK(!(sString.GetLength()));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(3U, sString.GetLength());

		sString = "";
		CHECK(!(sString.GetLength()));
	}

	TEST_FIXTURE(ConstructTest, Format_const_char) {
		CHECK(!(String::Format(static_cast<const char*>(nullptr)).GetLength()));
		CHECK(!(String::Format("").GetLength()));
		CHECK_EQUAL("1", String::Format("1").GetASCII());
		CHECK_EQUAL("1_5", String::Format("1%s5", "_").GetASCII());
		CHECK_EQUAL("5.12 1_5", String::Format("%g 1%s5", 5.12f, "_").GetASCII());
		CHECK(!(String::Format(static_cast<const char*>(nullptr)).GetLength()));
	}

	TEST_FIXTURE(ConstructTest, Format_const_w_char_t) {
		CHECK(!(String::Format(static_cast<const wchar_t*>(nullptr)).GetLength()));
		CHECK(!(String::Format(L"").GetLength()));
		CHECK_EQUAL(L"1", String::Format(L"1").GetUnicode());
		CHECK_EQUAL(L"1_5", String::Format(L"1%s5", L"_").GetUnicode());
		CHECK_EQUAL(L"5.12 1_5", String::Format(L"%g 1%s5", 5.12f, L"_").GetUnicode());
		CHECK(!(String::Format(static_cast<const wchar_t*>(nullptr)).GetLength()));
		CHECK_EQUAL(L"1_\u65e5\u672c\u8a9e_5", String::Format(L"1%s5", L"_\u65e5\u672c\u8a9e_").GetUnicode());
		CHECK_EQUAL(L"0", String::Format(L"%d", -0).GetUnicode());
		CHECK_EQUAL(L"-0.0000", String::Format(L"%.4f", -0.0f).GetUnicode());
		CHECK_EQUAL(L"-0", String::Format(L"%g", -0.0f).GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, GetFormat) {
		sString = "";
		CHECK_EQUAL(String::ASCII, sString.GetFormat());

		sString = "1234";
		CHECK_EQUAL(String::ASCII, sString.GetFormat());

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(String::Unicode, sString.GetFormat());
	}

	TEST_FIXTURE(ConstructTest, GetNumOfBytes) {
		sString = "";
		CHECK(! (sString.GetNumOfBytes(sString.GetFormat())));

		sString = "1234";
		CHECK_EQUAL(4U, sString.GetNumOfBytes(sString.GetFormat()));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(3U*sizeof(wchar_t), sString.GetNumOfBytes(sString.GetFormat()));
	}

	TEST_FIXTURE(ConstructTest, Operator_GetIndex) {
		sString = "";
		CHECK_EQUAL('\0', sString[static_cast<uint32>(0)]);

		sString = "1234";
		CHECK_EQUAL(sString[static_cast<uint32>(0)] ,  '1');
		CHECK_EQUAL(sString[static_cast<uint32>(1)] ,  '2');
		CHECK_EQUAL(sString[static_cast<uint32>(2)] ,  '3');
		CHECK_EQUAL(sString[static_cast<uint32>(3)] ,  '4');
		CHECK_EQUAL(sString[static_cast<uint32>(5)] ,  '\0');
	}

	TEST_FIXTURE(ConstructTest, GetASCII_FromASCII) {
		sString = "";
		CHECK(sString.GetASCII());
		CHECK(!(strcmp(sString.GetASCII(), "")));

		sString = "abcd";
		CHECK(sString.GetASCII());
		CHECK(!(strcmp(sString.GetASCII(), "abcd")));

		pszTemp = sString.GetASCII();
		sString.ToLower();
		CHECK_EQUAL(sString.GetASCII() ,  pszTemp);

		sString.Insert("Hi");
		CHECK_EQUAL(sString.GetASCII(), pszTemp);
	}

	TEST_FIXTURE(ConstructTest, GetASCII_FromUnicode) {
		sString = L"";
		CHECK(sString.GetASCII());
		CHECK(!(strcmp(sString.GetASCII(), "")));

		sString = L"abcd";
		CHECK(sString.GetASCII());
		CHECK(!(strcmp(sString.GetASCII(), "abcd")));

		pszTemp = sString.GetASCII();
		sString.ToLower();
		CHECK_EQUAL(sString.GetASCII() ,  pszTemp);

		sString.Insert(L"Hi");
		CHECK(sString.GetASCII()  != pszTemp);
	}

	TEST_FIXTURE(ConstructTest, GetUnicode_FromASCII) {
		sString = "";
		CHECK(sString.GetUnicode());
		CHECK(!(wcscmp(sString.GetUnicode(), L"")));

		sString = "abcd";
		CHECK(sString.GetUnicode());
		CHECK(!(wcscmp(sString.GetUnicode(), L"abcd")));

		pszTempWide = sString.GetUnicode();
		sString.ToLower();
		CHECK_EQUAL(sString.GetUnicode() ,  pszTempWide);

		sString.Insert("Hi");
		CHECK(sString.GetUnicode()  != pszTempWide);
	}

	TEST_FIXTURE(ConstructTest, GetUnicode_FromUnicode) {
		sString = L"";
		CHECK(sString.GetUnicode());
		CHECK(!(wcscmp(sString.GetUnicode(), L"")));

		sString = L"abcd";
		CHECK(sString.GetUnicode());
		CHECK(!(wcscmp(sString.GetUnicode(), L"abcd")));

		pszTempWide = sString.GetUnicode();
		sString.ToLower();
		CHECK_EQUAL(sString.GetUnicode() ,  pszTempWide);

		sString.Insert(L"Hi");
		CHECK_EQUAL(sString.GetUnicode(), pszTempWide);
	}

	TEST_FIXTURE(ConstructTest, GetUTF8_FromASCII) {
		sString = "";
		CHECK(sString.GetUTF8());
		CHECK(!(strcmp(sString.GetUTF8(), "")));

		sString = "abcd";
		CHECK(sString.GetUTF8());
		CHECK(!(strcmp(sString.GetUTF8(), "abcd")));

		pszTempUTF8 = sString.GetUTF8();
		sString.ToLower();
		CHECK_EQUAL(sString.GetUTF8() ,  pszTempUTF8);

		sString.Insert("Hi");
		CHECK_EQUAL(sString.GetUTF8(), pszTempUTF8);
	}

	TEST_FIXTURE(ConstructTest, GetUTF8_FromUnicode) {
		sString = L"";
		CHECK(sString.GetUTF8());
		CHECK(!(strcmp(sString.GetUTF8(), "")));

		sString = L"abcd";
		CHECK(sString.GetUTF8());
		CHECK(!(strcmp(sString.GetUTF8(), "abcd")));

		pszTempUTF8 = sString.GetUTF8();
		sString.ToLower();
		CHECK_EQUAL(pszTempUTF8, sString.GetUTF8());

		sString.Insert(L"Hi");
		CHECK_EQUAL(sString.GetUTF8(), pszTempUTF8);
	}

	TEST_FIXTURE(ConstructTest, Operator_Copy_String) {
		sStringT = "";
		sString = sStringT;
		CHECK_EQUAL(0U, sString.GetLength());
		CHECK_EQUAL(sStringT.GetASCII(), sString.GetASCII());

		sStringT = "1234";
		sString = sStringT;
		CHECK_EQUAL(4U, sString.GetLength());
		CHECK_EQUAL(sStringT.GetASCII(), sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Copy_charPtr) {
		sString = static_cast<const char*>(nullptr);
		CHECK_EQUAL(0U, sString.GetLength());
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		CHECK_EQUAL(0U, sString.GetLength());
		CHECK_EQUAL("", sString.GetASCII());

		sString = "1234";
		CHECK_EQUAL(4U, sString.GetLength());
		CHECK_EQUAL("1234", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Copy_wchar_tPtr) {
		sString = static_cast<const wchar_t*>(nullptr);
		CHECK_EQUAL(0U, sString.GetLength());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		CHECK_EQUAL(0U, sString.GetLength());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(3U, sString.GetLength());
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_String) {
		sString  = "";
		sStringT = "";
		sString  = sString + sStringT;
		CHECK_EQUAL("", sString.GetASCII());

		sString  = "A";
		sStringT = "";
		sString  = sString + sStringT;
		CHECK_EQUAL("A", sString.GetASCII());

		sString  = "A12";
		sStringT = "";
		sString  = sString + sStringT;
		CHECK_EQUAL("A12", sString.GetASCII());

		sString  = "";
		sStringT = "B";
		sString  = sString + sStringT;
		CHECK_EQUAL("B", sString.GetASCII());

		sString  = "";
		sStringT = "B34";
		sString  = sString + sStringT;
		CHECK_EQUAL("B34", sString.GetASCII());

		sString  = "A";
		sStringT = "B";
		sString  = sString + sStringT;
		CHECK_EQUAL("AB", sString.GetASCII());

		sString  = "A12";
		sStringT = "B34";
		sString  = sString + sStringT;
		CHECK_EQUAL("A12B34", sString.GetASCII());

		sString  = "A";
		sStringT = "%";
		sString  = sString + sStringT;
		CHECK_EQUAL("A%", sString.GetASCII());

		sString  = "A";
		sStringT = L"%";
		sString  = sString + sStringT;
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());

		sString  = L"A";
		sStringT = "%";
		sString  = sString + sStringT;
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_charPtr) {
		sString = "";
		sString = sString + static_cast<const char*>(nullptr);
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		sString = sString + "";
		CHECK_EQUAL("", sString.GetASCII());

		sString = "A";
		sString = sString + static_cast<const char*>(nullptr);
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "A";
		sString = sString + "";
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "A12";
		sString = sString + static_cast<const char*>(nullptr);
		CHECK_EQUAL("A12", sString.GetASCII());

		sString = "A12";
		sString = sString + "";
		CHECK_EQUAL("A12", sString.GetASCII());

		sString = "";
		sString = sString + "B";
		CHECK_EQUAL("B", sString.GetASCII());

		sString = "";
		sString = sString + "B34";
		CHECK_EQUAL("B34", sString.GetASCII());

		sString = "A";
		sString = sString + "B";
		CHECK_EQUAL("AB", sString.GetASCII());

		sString = "A12";
		sString = sString + "B34";
		CHECK_EQUAL("A12B34", sString.GetASCII());

		sString  = "A";
		sString = sString + "%";
		CHECK_EQUAL("A%", sString.GetASCII());

		sString  = L"A";
		sString = sString + "%";
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_wchar_tPtr) {
		sString = L"";
		sString = sString + static_cast<const wchar_t*>(nullptr);
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		sString = sString + L"";
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"A";
		sString = sString + static_cast<const wchar_t*>(nullptr);
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"A";
		sString = sString + L"";
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"A12";
		sString = sString + static_cast<const wchar_t*>(nullptr);
		CHECK_EQUAL(L"A12", sString.GetUnicode());

		sString = L"A12";
		sString = sString + L"";
		CHECK_EQUAL(L"A12", sString.GetUnicode());

		sString = L"";
		sString = sString + L"B";
		CHECK_EQUAL(L"B", sString.GetUnicode());

		sString = L"";
		sString = sString + L"B34";
		CHECK_EQUAL(L"B34", sString.GetUnicode());

		sString = L"A";
		sString = sString + L"B";
		CHECK_EQUAL(L"AB", sString.GetUnicode());

		sString = L"A12";
		sString = sString + L"B34";
		CHECK_EQUAL(L"A12B34", sString.GetUnicode());

		sString  = L"A";
		sString = sString + L"%";
		CHECK_EQUAL(L"A%", sString.GetUnicode());

		sString  = L"\u65e5\u672c";
		sString = sString + L"\u8a9e";
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sString.GetUnicode());

		sString  = "A";
		sString = sString + L"%";
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_char_Ptr_left_hand) {
		sString = "";
		sString = static_cast<const char*>(nullptr) + sString;
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		sString = "" + sString;
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		sString = "A" + sString;
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "";
		sString = "A12" + sString;
		CHECK_EQUAL("A12", sString.GetASCII());

		sString = "B";
		sString = static_cast<const char*>(nullptr) + sString;
		CHECK_EQUAL("B", sString.GetASCII());

		sString = "B";
		sString = "" + sString;
		CHECK_EQUAL("B", sString.GetASCII());

		sString = "B34";
		sString = static_cast<const char*>(nullptr) + sString;
		CHECK_EQUAL("B34", sString.GetASCII());

		sString = "B34";
		sString = "" + sString;
		CHECK_EQUAL("B34", sString.GetASCII());

		sString = "B";
		sString = "A" + sString;
		CHECK_EQUAL("AB", sString.GetASCII());

		sString = "B34";
		sString = "A12" + sString;
		CHECK_EQUAL("A12B34", sString.GetASCII());

		sString  = "A";
		sString =  "%" + sString;
		CHECK_EQUAL("%A", sString.GetASCII());

		sString  = L"A";
		sString = "%" + sString;
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"%A", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_wchar_t_Ptr_left_hand) {
		sString = L"";
		sString = static_cast<const wchar_t*>(nullptr) + sString;
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		sString = L"" + sString;
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		sString = L"A" + sString;
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"";
		sString = L"A12" + sString;
		CHECK_EQUAL(L"A12", sString.GetUnicode());

		sString = L"B";
		sString = static_cast<const wchar_t*>(nullptr) + sString;
		CHECK_EQUAL(L"B", sString.GetUnicode());

		sString = L"B";
		sString = L"" + sString;
		CHECK_EQUAL(L"B", sString.GetUnicode());

		sString = L"B34";
		sString = static_cast<const wchar_t*>(nullptr) + sString;
		CHECK_EQUAL(L"B34", sString.GetUnicode());

		sString = L"B34";
		sString = L"" + sString;
		CHECK_EQUAL(L"B34", sString.GetUnicode());

		sString = L"B";
		sString = L"A" + sString;
		CHECK_EQUAL(L"AB", sString.GetUnicode());

		sString = L"B34";
		sString = L"A12" + sString;
		CHECK_EQUAL(L"A12B34", sString.GetUnicode());

		sString  = L"A";
		sString =  L"%" + sString;
		CHECK_EQUAL(L"%A", sString.GetUnicode());

		sString  = L"\u672c\u8a9e";
		sString =  L"\u65e5" + sString;
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sString.GetUnicode());

		sString  = "A";
		sString = L"%" + sString;
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"%A", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_String_left_hand) {
		sString  = "";
		sStringT = "";
		sString  += sStringT;
		CHECK_EQUAL("", sString.GetASCII());

		sString  = "A";
		sStringT = "";
		sString  += sStringT;
		CHECK_EQUAL("A", sString.GetASCII());

		sString  = "A12";
		sStringT = "";
		sString  += sStringT;
		CHECK_EQUAL("A12", sString.GetASCII());

		sString  = "";
		sStringT = "B";
		sString  += sStringT;
		CHECK_EQUAL("B", sString.GetASCII());

		sString  = "";
		sStringT = "B34";
		sString  += sStringT;
		CHECK_EQUAL("B34", sString.GetASCII());

		sString  = "A";
		sStringT = "B";
		sString  += sStringT;
		CHECK_EQUAL("AB", sString.GetASCII());

		sString  = "A12";
		sStringT = "B34";
		sString  += sStringT;
		CHECK_EQUAL("A12B34", sString.GetASCII());

		sString  = "A";
		sStringT = "%";
		sString  += sStringT;
		CHECK_EQUAL("A%", sString.GetASCII());

		sString  = "A";
		sStringT = L"%";
		sString  += sStringT;
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());

		sString  = L"A";
		sStringT = "%";
		sString  += sStringT;
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_charPtr) {
		sString = "";
		sString += static_cast<const char*>(nullptr);
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		sString += "";
		CHECK_EQUAL("", sString.GetASCII());

		sString = "A";
		sString += static_cast<const char*>(nullptr);
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "A";
		sString += "";
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "A12";
		sString += static_cast<const char*>(nullptr);
		CHECK_EQUAL("A12", sString.GetASCII());

		sString = "A12";
		sString += "";
		CHECK_EQUAL("A12", sString.GetASCII());

		sString = "";
		sString += "B";
		CHECK_EQUAL("B", sString.GetASCII());

		sString = "";
		sString += "B34";
		CHECK_EQUAL("B34", sString.GetASCII());

		sString = "A";
		sString += "B";
		CHECK_EQUAL("AB", sString.GetASCII());

		sString = "A12";
		sString += "B34";
		CHECK_EQUAL("A12B34", sString.GetASCII());

		sString = "A";
		sString += "%";
		CHECK_EQUAL("A%", sString.GetASCII());

		sString = L"A";
		sString += "%";
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_wchar_tPtr) {
		sString = L"";
		sString += static_cast<const wchar_t*>(nullptr);
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		sString += L"";
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"A";
		sString += static_cast<const wchar_t*>(nullptr);
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"A";
		sString += L"";
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"A12";
		sString += static_cast<const wchar_t*>(nullptr);
		CHECK_EQUAL(L"A12", sString.GetUnicode());

		sString = L"A12";
		sString += L"";
		CHECK_EQUAL(L"A12", sString.GetUnicode());

		sString = L"";
		sString += L"B";
		CHECK_EQUAL(L"B", sString.GetUnicode());

		sString = L"";
		sString += L"B34";
		CHECK_EQUAL(L"B34", sString.GetUnicode());

		sString = L"A";
		sString += L"B";
		CHECK_EQUAL(L"AB", sString.GetUnicode());

		sString = L"A12";
		sString += L"B34";
		CHECK_EQUAL(L"A12B34", sString.GetUnicode());

		sString = L"A";
		sString += L"%";
		CHECK_EQUAL(L"A%", sString.GetUnicode());

		sString = L"\u65e5\u672c";
		sString += L"\u8a9e";
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sString.GetUnicode());

		sString = "A";
		sString += L"%";
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"A%", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Smaller_String) {
		sString = "";
		sStringT = "";
		CHECK(!(sString < sStringT));

		sString = "A";
		sStringT = "";
		CHECK(!(sString < sStringT));

		sString = "";
		sStringT = "A";
		CHECK((sString < sStringT));

		sString = "A";
		sStringT = "A";
		CHECK(!(sString < sStringT));

		sString = "AB";
		sStringT = "A";
		CHECK(!(sString < sStringT));

		sString  = "AB";
		sStringT = L"A";
		CHECK(!(sString < sStringT));

		sString  = L"AB";
		sStringT = "A";
		CHECK(!(sString < sStringT));
	}

	TEST_FIXTURE(ConstructTest, Operator_Smaller_charPtr) {
		sString = "";
		sStringT = "";
		CHECK(!(sString < sStringT));

		sString = "A";
		sStringT = "";
		CHECK(!(sString < sStringT));

		sString = "";
		sStringT = "A";
		CHECK((sString < sStringT));

		sString = "A";
		sStringT = "A";
		CHECK(!(sString < sStringT));

		sString = "AB";
		sStringT = "A";
		CHECK(!(sString < sStringT));

		sString  = "AB";
		sStringT = L"A";
		CHECK(!(sString < sStringT));

		sString  = L"AB";
		sStringT = "A";
		CHECK(!(sString < sStringT));
	}

	TEST_FIXTURE(ConstructTest, Operator_Smaller_wchar_tPtr) {
		sString = L"";
		CHECK(!(sString < static_cast<const wchar_t*>(nullptr)));
		CHECK(!(sString < L""));

		sString = L"A";
		CHECK(!(sString < L""));

		sString = L"";
		CHECK((sString < L"A"));

		sString = L"A";
		CHECK(!(sString < L"A"));

		sString = L"AB";
		CHECK(!(sString < L"A"));

		sString = L"\u8a9e";
		CHECK(!(sString < L"\u65e5\u672c"));

		sString = "AB";
		CHECK(!(sString < L"A"));
	}

	TEST_FIXTURE(ConstructTest, Operator_Greater_String) {
		sString = "";
		sStringT = "";
		CHECK(!(sString > sStringT));

		sString = "";
		sStringT = "A";
		CHECK(!(sString > sStringT));

		sString = "A";
		sStringT = "";
		CHECK((sString > sStringT));

		sString = "A";
		sStringT = "A";
		CHECK(!(sString > sStringT));

		sString = "A";
		sStringT = "AB";
		CHECK(!(sString > sStringT));

		sString  = "A";
		sStringT = L"AB";
		CHECK(!(sString > sStringT));

		sString  = L"A";
		sStringT = "AB";
		CHECK(!(sString > sStringT));
	}

	TEST_FIXTURE(ConstructTest, Operator_Greater_charPtr) {
		sString = "";
		CHECK(!(sString > static_cast<const char*>(nullptr)));
		CHECK(!(sString > ""));

		sString = "A";
		CHECK((sString > ""));

		sString = "";
		CHECK(!(sString > "A"));

		sString = "A";
		CHECK(!(sString > "A"));

		sString = "AB";
		CHECK((sString > "A"));

		sString = L"A";
		CHECK(!(sString > "AB"));
	}

	TEST_FIXTURE(ConstructTest, Operator_Greater_wchar_tPtr) {
		sString = L"";
		CHECK(!(sString > static_cast<const wchar_t*>(nullptr)));
		CHECK(!(sString > L""));

		sString = L"A";
		CHECK((sString > L""));

		sString = L"";
		CHECK(!(sString > L"A"));

		sString = L"A";
		CHECK(!(sString > L"A"));

		sString = L"AB";
		CHECK((sString > L"A"));

		sString = L"\u8a9e";
		CHECK((sString > L"\u65e5\u672c"));

		sString = "AB";
		CHECK((sString > L"A"));
	}

	TEST_FIXTURE(ConstructTest, Operator_Equal_String) {
		sString = "";
		sStringT = "";
		CHECK(sString == sStringT);

		sString = "A";
		sStringT = "";
		CHECK(!(sString == sStringT));

		sString = "A12";
		sStringT = "";
		CHECK(!(sString == sStringT));

		sString = "";
		sStringT = "B";
		CHECK(!(sString == sStringT));

		sString = "";
		sStringT = "B34";
		CHECK(!(sString == sStringT));

		sString = "A";
		sStringT = "B";
		CHECK(!(sString == sStringT));

		sString = "A12";
		sStringT = "B";
		CHECK(!(sString == sStringT));

		sString = "A";
		sStringT = "B34";
		CHECK(!(sString == sStringT));

		sString = "A12";
		sStringT = "B34";
		CHECK(!(sString == sStringT));

		sString = "A";
		sStringT = "A";
		CHECK(sString == sStringT);

		sString = "A12";
		sStringT = "A12";
		CHECK(sString == sStringT);

		sString = "A12";
		sStringT = sString;
		CHECK(sString == sStringT);

		sString  = "A123";
		sStringT = L"A12";
		CHECK(!(sString == sStringT));

		sString  = "A13";
		sStringT = L"A12";
		CHECK(!(sString == sStringT));

		sString  = "A12";
		sStringT = L"A12";
		CHECK(sString == sStringT);

		sString  = L"A123";
		sStringT = "A12";
		CHECK(!(sString == sStringT));

		sString  = L"A13";
		sStringT = "A12";
		CHECK(!(sString == sStringT));

		sString  = L"A12";
		sStringT = "A12";
		CHECK(sString == sStringT);
	}

	TEST_FIXTURE(ConstructTest, Operator_Equal_charPtr) {
		sString = "";
		CHECK(sString == static_cast<const char*>(nullptr));

		sString = "";
		CHECK(sString == "");

		sString = "A";
		CHECK(!(sString == static_cast<const char*>(nullptr)));

		sString = "A";
		CHECK(!(sString == ""));

		sString = "A12";
		CHECK(!(sString == static_cast<const char*>(nullptr)));

		sString = "A12";
		CHECK(!(sString == ""));

		sString = "";
		CHECK(!(sString == "B"));

		sString = "";
		CHECK(!(sString == "B34"));

		sString = "A";
		CHECK(!(sString == "B"));

		sString = "A12";
		CHECK(!(sString == "B"));

		sString = "A";
		CHECK(!(sString == "B34"));

		sString = "A12";
		CHECK(!(sString == "B34"));

		sString = "A";
		CHECK(sString == "A");

		sString = "A12";
		CHECK(sString == "A12");

		sString  = L"A123";
		CHECK(!(sString == "A12"));

		sString  = L"A13";
		CHECK(!(sString == "A12"));

		sString  = L"A12";
		CHECK(sString == "A12");
	}

	TEST_FIXTURE(ConstructTest, Operator_Equal_wchar_tPtr) {
		sString = L"";
		CHECK(sString == static_cast<const wchar_t*>(nullptr));

		sString = L"";
		CHECK(sString == L"");

		sString = L"A";
		CHECK(!(sString == static_cast<const wchar_t*>(nullptr)));

		sString = L"A";
		CHECK(!(sString == L""));

		sString = L"A12";
		CHECK(!(sString == static_cast<const wchar_t*>(nullptr)));

		sString = L"A12";
		CHECK(!(sString == L""));

		sString = L"";
		CHECK(!(sString == L"B"));

		sString = L"";
		CHECK(!(sString == L"B34"));

		sString = L"A";
		CHECK(!(sString == L"B"));

		sString = L"A12";
		CHECK(!(sString == L"B"));

		sString = L"A";
		CHECK(!(sString == L"B34"));

		sString = L"A12";
		CHECK(!(sString == L"B34"));

		sString = L"A";
		CHECK(sString == L"A");

		sString = L"A12";
		CHECK(sString == L"A12");

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(sString == L"\u65e5\u672c\u8a9e");

		sString = L"\u65e5\u672c";
		CHECK(!(sString == L"\u672c\u8a9e"));

		sString  = L"A123";
		CHECK(!(sString == "A12"));

		sString  = L"A13";
		CHECK(!(sString == "A12"));

		sString  = L"A12";
		CHECK(sString == "A12");
	}

	TEST_FIXTURE(ConstructTest, Operator_Not_Equal_String) {
		sString = "";
		sStringT = "";
		CHECK(!(sStringT != sString));

		sString = "A";
		sStringT = "";
		CHECK((sString  != sStringT));

		sString = "A12";
		sStringT = "";
		CHECK((sString  != sStringT));

		sString = "";
		sStringT = "B";
		CHECK((sString  != sStringT));

		sString = "";
		sStringT = "B34";
		CHECK((sString  != sStringT));

		sString = "A";
		sStringT = "B";
		CHECK((sString  != sStringT));

		sString = "A12";
		sStringT = "B";
		CHECK((sString  != sStringT));

		sString = "A";
		sStringT = "B34";
		CHECK((sString  != sStringT));

		sString = "A12";
		sStringT = "B34";
		CHECK((sString  != sStringT));

		sString = "A";
		sStringT = "A";
		CHECK(!(sStringT != sString));

		sString = "A12";
		sStringT = "A12";
		CHECK(!(sStringT != sString));

		sString = "A12";
		sStringT = sString;
		CHECK(!(sStringT != sString));

		sString  = "A123";
		sStringT = L"A12";
		CHECK((sString  != sStringT));

		sString  = "A13";
		sStringT = L"A12";
		CHECK((sString  != sStringT));

		sString  = "A12";
		sStringT = L"A12";
		CHECK(!(sStringT != sString));

		sString  = L"A123";
		sStringT = "A12";
		CHECK((sString  != sStringT));

		sString  = L"A13";
		sStringT = "A12";
		CHECK((sString  != sStringT));

		sString  = L"A12";
		sStringT = "A12";
		CHECK(!(sStringT != sString));
	}

	TEST_FIXTURE(ConstructTest, Operator_Not_Equal_charPtr) {
		sString = "";
		CHECK( static_cast<const char*>(nullptr) != sString);

		sString = "";
		CHECK(!(sString != ""));

		sString = "A";
		CHECK((sString  != static_cast<const char*>(nullptr)));

		sString = "A";
		CHECK((sString != ""));

		sString = "A12";
		CHECK((sString  != static_cast<const char*>(nullptr)));

		sString = "A12";
		CHECK((sString != ""));

		sString = "";
		CHECK((sString != "B"));

		sString = "";
		CHECK((sString != "B34"));

		sString = "A";
		CHECK((sString != "B"));

		sString = "A12";
		CHECK((sString != "B34"));

		sString = "A";
		CHECK(!(sString != "A"));

		sString = "A12";
		CHECK(!(sString != "A12"));

		sString  = L"A123";
		CHECK((sString != "A12"));

		sString  = L"A13";
		CHECK((sString != "A12"));

		sString  = L"A12";
		CHECK(!(sString != "A12"));
	}

	TEST_FIXTURE(ConstructTest, Operator_Not_Equal_wchar_tPtr) {
		sString = L"";
		CHECK(!(sString != static_cast<const wchar_t*>(nullptr)));

		sString = L"";
		CHECK(!(sString != L""));

		sString = L"A";
		CHECK((sString  != static_cast<const wchar_t*>(nullptr)));

		sString = L"A";
		CHECK((sString != L""));

		sString = L"A12";
		CHECK((sString  != static_cast<const wchar_t*>(nullptr)));

		sString = L"A12";
		CHECK((sString != L""));

		sString = L"";
		CHECK((sString != L"B"));

		sString = L"";
		CHECK((sString != L"B34"));

		sString = L"A";
		CHECK((sString != L"B"));

		sString = L"A12";
		CHECK((sString != L"B34"));

		sString = L"A";
		CHECK(!(sString != L"A"));

		sString = L"A12";
		CHECK(!(sString != L"A12"));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(!(sString != L"\u65e5\u672c\u8a9e"));

		sString = L"\u65e5\u672c";
		CHECK((sString != L"\u672c\u8a9e"));

		sString  = L"A123";
		CHECK((sString != "A12"));

		sString  = L"A13";
		CHECK((sString != "A12"));

		sString  = L"A12";
		CHECK(!(sString != "A12"));
	}

	TEST_FIXTURE(ConstructTest, Compare_String) {
		sString = "";
		sStringT = "";
		CHECK(sString.Compare(sStringT));

		sString = "Test";
		sStringT = "";
		CHECK(!(sString.Compare(sStringT)));

		sString = "";
		sStringT = "Test";
		CHECK(!(sString.Compare(sStringT)));

		sString = "Test";
		sStringT = "Test";
		CHECK(sString.Compare(sStringT));
		CHECK(!(sString.Compare(sStringT, 1, 10)));
		CHECK(sString.Compare(sStringT, 0, 10));

		sString = "Test";
		sStringT = "TEst";
		CHECK(!(sString.Compare(sStringT)));

		sString = "Test";
		sStringT = sString;
		CHECK(sString.Compare(sStringT));

		sString = "Test";
		sStringT = "Tests";
		CHECK(!(sString.Compare(sStringT)));

		sString = "Test";
		sStringT = "Tests";
		CHECK(sString.Compare(sStringT, 0, 4));

		sString = "sTests";
		sStringT = "Test";
		CHECK(sString.Compare(sStringT, 1, 4));

		sString = "sTests";
		sStringT = "Test";
		CHECK(!(sString.Compare(sStringT, 1, 5)));
		CHECK(!(sString.Compare(sStringT, 10)));
		CHECK(!(sString.Compare(sStringT, 10, 5)));
		CHECK(!(sString.Compare(sStringT, 1, 10)));
		CHECK(!(sString.Compare(sStringT, 0, 10)));

		sString = "sTests";
		sStringT = L"Test";
		CHECK(!(sString.Compare(sStringT, 1, 5)));
		CHECK(!(sString.Compare(sStringT, 10)));
		CHECK(!(sString.Compare(sStringT, 10, 5)));
		CHECK(!(sString.Compare(sStringT, 1, 10)));
		CHECK(!(sString.Compare(sStringT, 0, 10)));

		sString = L"sTests";
		sStringT = "Test";
		CHECK(!(sString.Compare(sStringT, 1, 5)));
		CHECK(!(sString.Compare(sStringT, 10)));
		CHECK(!(sString.Compare(sStringT, 10, 5)));
		CHECK(!(sString.Compare(sStringT, 1, 10)));
		CHECK(!(sString.Compare(sStringT, 0, 10)));
	}

	TEST_FIXTURE(ConstructTest, Compare_charPtr) {
		sString = "";
		CHECK(sString.Compare(static_cast<const char*>(nullptr)));

		sString = "";
		CHECK(sString.Compare(""));

		sString = "Test";
		CHECK(!(sString.Compare(static_cast<const char*>(nullptr))));

		sString = "Test";
		CHECK(!(sString.Compare("")));

		sString = "";
		CHECK(!(sString.Compare("Test")));

		sString = "Test";
		CHECK(sString.Compare("Test"));
		CHECK(!(sString.Compare("Test", 1, 10)));
		CHECK(sString.Compare("Test", 0, 10));

		sString = "Test";
		CHECK(!(sString.Compare("TEst")));

		sString = "Tests";
		CHECK(!(sString.Compare("Test")));

		sString = "Tests";
		CHECK(sString.Compare("Test", 0, 4));

		sString = "sTests";
		CHECK(sString.Compare("Test", 1, 4));

		sString = "sTests";
		CHECK(!(sString.Compare("Test", 1, 5)));
		CHECK(!(sString.Compare("Test", 10)));
		CHECK(!(sString.Compare("Test", 10, 5)));
		CHECK(!(sString.Compare("Test", 1, 10)));
		CHECK(!(sString.Compare("Test", 0, 10)));

		sString = L"sTests";
		CHECK(!(sString.Compare("Test", 1, 5)));
		CHECK(!(sString.Compare("Test", 10)));
		CHECK(!(sString.Compare("Test", 10, 5)));
		CHECK(!(sString.Compare("Test", 1, 10)));
		CHECK(!(sString.Compare("Test", 0, 10)));
	}

	TEST_FIXTURE(ConstructTest, Compare_wchar_tPtr) {
		sString = L"";
		CHECK(sString.Compare(static_cast<const wchar_t*>(nullptr)));

		sString = L"";
		CHECK(sString.Compare(L""));

		sString = L"Test";
		CHECK(!(sString.Compare(static_cast<const wchar_t*>(nullptr))));

		sString = L"Test";
		CHECK(!(sString.Compare(L"")));

		sString = L"";
		CHECK(!(sString.Compare(L"Test")));

		sString = L"Test";
		CHECK(sString.Compare(L"Test"));
		CHECK(!(sString.Compare(L"Test", 1, 10)));
		CHECK(sString.Compare(L"Test", 0, 10));

		sString = L"Test";
		CHECK(!(sString.Compare(L"TEst")));

		sString = L"Tests";
		CHECK(!(sString.Compare(L"Test")));

		sString = L"Tests";
		CHECK(sString.Compare(L"Test", 0, 4));

		sString = L"sTests";
		CHECK(sString.Compare(L"Test", 1, 4));

		sString = L"sTests";
		CHECK(!(sString.Compare(L"Test", 1, 5)));
		CHECK(!(sString.Compare(L"Test", 10)));
		CHECK(!(sString.Compare(L"Test", 10, 5)));
		CHECK(!(sString.Compare(L"Test", 1, 10)));
		CHECK(!(sString.Compare(L"Test", 0, 10)));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(!(sString.Compare(L"\u65e5\u672c\u8a9e", 1, 4)));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(sString.Compare(L"\u672c\u8a9e", 1, 4));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(sString.Compare(L"\u65e5\u672c\u8a9e", 0, 4));

		sString = "sTests";
		CHECK(!(sString.Compare(L"Test", 1, 5)));
		CHECK(!(sString.Compare(L"Test", 10)));
		CHECK(!(sString.Compare(L"Test", 10, 5)));
		CHECK(!(sString.Compare(L"Test", 1, 10)));
		CHECK(!(sString.Compare(L"Test", 0, 10)));
	}

	TEST_FIXTURE(ConstructTest, Compare_NoCase_String) {
		sString = "";
		sStringT = "";
		CHECK(sString.CompareNoCase(sStringT));

		sString = "Test";
		sStringT = "";
		CHECK(!(sString.CompareNoCase(sStringT)));

		sString = "";
		sStringT = "Test";
		CHECK(!(sString.CompareNoCase(sStringT)));

		sString = "Test";
		sStringT = "Test";
		CHECK(sString.CompareNoCase(sStringT));
		CHECK(!(sString.CompareNoCase(sStringT, 1, 10)));
		CHECK(sString.CompareNoCase(sStringT, 0, 10));

		sString = "Test";
		sStringT = "TEst";
		CHECK(sString.CompareNoCase(sStringT));

		sString = "Test";
		sStringT = sString;
		CHECK(sString.CompareNoCase(sStringT));

		sString = "Test";
		sStringT = "Tests";
		CHECK(!(sString.CompareNoCase(sStringT)));

		sString = "Tests";
		sStringT = "Test";
		CHECK(sString.CompareNoCase(sStringT, 0, 4));

		sString = "sTests";
		sStringT = "Test";
		CHECK(sString.CompareNoCase(sStringT, 1, 4));

		sString = "sTests";
		sStringT = "Test";
		CHECK(!(sString.CompareNoCase(sStringT, 1, 5)));
		CHECK(!(sString.CompareNoCase(sStringT, 10)));
		CHECK(!(sString.CompareNoCase(sStringT, 10, 5)));
		CHECK(!(sString.CompareNoCase(sStringT, 1, 10)));
		CHECK(!(sString.CompareNoCase(sStringT, 0, 10)));

		sString = "sTests";
		sStringT = L"Test";
		CHECK(!(sString.CompareNoCase(sStringT, 1, 5)));
		CHECK(!(sString.CompareNoCase(sStringT, 10)));
		CHECK(!(sString.CompareNoCase(sStringT, 10, 5)));
		CHECK(!(sString.CompareNoCase(sStringT, 1, 10)));
		CHECK(!(sString.CompareNoCase(sStringT, 0, 10)));

		sString = L"sTests";
		sStringT = "Test";
		CHECK(!(sString.CompareNoCase(sStringT, 1, 5)));
		CHECK(!(sString.CompareNoCase(sStringT, 10)));
		CHECK(!(sString.CompareNoCase(sStringT, 10, 5)));
		CHECK(!(sString.CompareNoCase(sStringT, 1, 10)));
		CHECK(!(sString.CompareNoCase(sStringT, 0, 10)));
	}

	TEST_FIXTURE(ConstructTest, Compare_NoCase_charPtr) {
		sString = "";
		CHECK(sString.CompareNoCase(static_cast<const char*>(nullptr)));

		sString = "";
		CHECK(sString.CompareNoCase(""));

		sString = "Test";
		CHECK(!(sString.CompareNoCase(static_cast<const char*>(nullptr))));

		sString = "Test";
		CHECK(!(sString.CompareNoCase("")));

		sString = "";
		CHECK(!(sString.CompareNoCase("Test")));

		sString = "Test";
		CHECK(sString.CompareNoCase("Test"));
		CHECK(!(sString.CompareNoCase("Test", 1, 10)));
		CHECK(sString.CompareNoCase("Test", 0, 10));

		sString = "Test";
		CHECK(sString.CompareNoCase("TEst"));

		sString = "Test";
		CHECK(!(sString.CompareNoCase("Tests")));

		sString = "Tests";
		CHECK(sString.CompareNoCase("Test", 0, 4));

		sString = "sTests";
		CHECK(sString.CompareNoCase("Test", 1, 4));

		sString = "sTests";
		CHECK(!(sString.CompareNoCase("Test", 1, 5)));
		CHECK(!(sString.CompareNoCase("Test", 10)));
		CHECK(!(sString.CompareNoCase("Test", 10, 5)));
		CHECK(!(sString.CompareNoCase("Test", 1, 10)));
		CHECK(!(sString.CompareNoCase("Test", 0, 10)));

		sString = L"sTests";
		CHECK(!(sString.CompareNoCase("Test", 1, 5)));
		CHECK(!(sString.CompareNoCase("Test", 10)));
		CHECK(!(sString.CompareNoCase("Test", 10, 5)));
		CHECK(!(sString.CompareNoCase("Test", 1, 10)));
		CHECK(!(sString.CompareNoCase("Test", 0, 10)));
	}

	TEST_FIXTURE(ConstructTest, Compare_NoCase_wchar_tPtr) {
		sString = L"";
		CHECK(sString.CompareNoCase(static_cast<const wchar_t*>(nullptr)));

		sString = L"";
		CHECK(sString.CompareNoCase(L""));

		sString = L"Test";
		CHECK(!(sString.CompareNoCase(static_cast<const wchar_t*>(nullptr))));

		sString = L"Test";
		CHECK(!(sString.CompareNoCase(L"")));

		sString = L"";
		CHECK(!(sString.CompareNoCase(L"Test")));

		sString = L"Test";
		CHECK(sString.CompareNoCase(L"Test"));
		CHECK(!(sString.CompareNoCase(L"Test", 1, 10)));
		CHECK(sString.CompareNoCase(L"Test", 0, 10));

		sString = L"Test";
		CHECK(sString.CompareNoCase(L"TEst"));

		sString = L"Test";
		CHECK(!(sString.CompareNoCase(L"Tests")));

		sString = L"Tests";
		CHECK(sString.CompareNoCase(L"Test", 0, 4));

		sString = L"sTests";
		CHECK(sString.CompareNoCase(L"Test", 1, 4));

		sString = L"sTests";
		CHECK(!(sString.CompareNoCase(L"Test", 1, 5)));
		CHECK(!(sString.CompareNoCase(L"Test", 10)));
		CHECK(!(sString.CompareNoCase(L"Test", 10, 5)));
		CHECK(!(sString.CompareNoCase(L"Test", 1, 10)));
		CHECK(!(sString.CompareNoCase(L"Test", 0, 10)));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(!(sString.CompareNoCase(L"\u65e5\u672c\u8a9e", 1, 4)));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(sString.CompareNoCase(L"\u672c\u8a9e", 1, 4));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(sString.CompareNoCase(L"\u65e5\u672c\u8a9e", 0, 4));

		sString = "sTests";
		CHECK(!(sString.CompareNoCase(L"Test", 1, 5)));
		CHECK(!(sString.CompareNoCase(L"Test", 10)));
		CHECK(!(sString.CompareNoCase(L"Test", 10, 5)));
		CHECK(!(sString.CompareNoCase(L"Test", 1, 10)));
		CHECK(!(sString.CompareNoCase(L"Test", 0, 10)));
	}

	TEST_FIXTURE(ConstructTest, IsAlphabetic_ASCII) {
		sString = "";
		CHECK(!(sString.IsAlphabetic()));

		sString = ".";
		CHECK(!(sString.IsAlphabetic()));

		sString = "46";
		CHECK(!(sString.IsAlphabetic()));

		sString = "46.23";
		CHECK(!(sString.IsAlphabetic()));

		sString = "46.23.7";
		CHECK(!(sString.IsAlphabetic()));

		sString = "AbC";
		CHECK(sString.IsAlphabetic());

		sString = "Ab-C";
		CHECK(!(sString.IsAlphabetic()));

		sString = "Ab5C";
		CHECK(!(sString.IsAlphabetic()));
	}

	TEST_FIXTURE(ConstructTest, IsAlphabetic_Unicode) {
#ifdef LINUX
		// Under linux we have to change the locale != C because iswalpha is locale dependent and the "C" locale doesn't know anything about unicode
		// We assume that the active locale on the system is unicode capable, because we can't know which locale are available on the system to set it to a specific one
		LocaleWrapper changeLocale;
#endif
		sString = L"";
		CHECK(!(sString.IsAlphabetic()));

		sString = L".";
		CHECK(!(sString.IsAlphabetic()));

		sString = L"46";
		CHECK(!(sString.IsAlphabetic()));

		sString = L"46.23";
		CHECK(!(sString.IsAlphabetic()));

		sString = L"46.23.7";
		CHECK(!(sString.IsAlphabetic()));

		sString = L"AbC";
		CHECK(sString.IsAlphabetic());

		sString = L"Ab-C";
		CHECK(!(sString.IsAlphabetic()));

		sString = L"Ab5C";
		CHECK(!(sString.IsAlphabetic()));

		sString = L"\u65e5\u672c";
		CHECK(sString.IsAlphabetic());

		sString = L"\u65e51\u672c";
		CHECK(!(sString.IsAlphabetic()));

		sString = L"\u65e5\u4E00\u672c";
		CHECK(sString.IsAlphabetic());
	}

	TEST_FIXTURE(ConstructTest, IsAlphaNumeric_ASCII) {
		sString = "";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = ".";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = "46";
		CHECK(sString.IsAlphaNumeric());

		sString = "46.23";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = "46.23.7";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = "AbC";
		CHECK(sString.IsAlphaNumeric());

		sString = "Ab-C";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = "Ab5C";
		CHECK(sString.IsAlphaNumeric());
	}

	TEST_FIXTURE(ConstructTest, IsAlphaNumeric_Unicode) {
#ifdef LINUX
		// Under linux we have to change the locale != C because iswalpha is locale dependent and the "C" locale doesn't know anything about unicode
		// We assume that the active locale on the system is unicode capable, because we can't know which locale are available on the system to set it to a specific one
		LocaleWrapper changeLocale;
#endif
		sString = L"";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = L".";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = L"46";
		CHECK(sString.IsAlphaNumeric());

		sString = L"46.23";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = L"46.23.7";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = L"AbC";
		CHECK(sString.IsAlphaNumeric());

		sString = L"Ab-C";
		CHECK(!(sString.IsAlphaNumeric()));

		sString = L"Ab5C";
		CHECK(sString.IsAlphaNumeric());

		sString = L"\u65e5\u672c";
		CHECK(sString.IsAlphaNumeric());

		sString = L"\u65e51\u672c";
		CHECK(sString.IsAlphaNumeric());

		sString = L"\u65e5\u4E00\u672c";
		CHECK(sString.IsAlphaNumeric());
	}

	TEST_FIXTURE(ConstructTest, IsNumeric_ASCII) {
		sString = "";
		CHECK(!(sString.IsNumeric()));

		sString = ".";
		CHECK(!(sString.IsNumeric()));

		sString = "46";
		CHECK(sString.IsNumeric());

		sString = "46.23";
		CHECK(!(sString.IsNumeric()));

		sString = "46.23.7";
		CHECK(!(sString.IsNumeric()));

		sString = "AbC";
		CHECK(!(sString.IsNumeric()));

		sString = "Ab5C";
		CHECK(!(sString.IsNumeric()));
	}

	TEST_FIXTURE(ConstructTest, IsNumeric_Unicode) {
		sString = L"";
		CHECK(!(sString.IsNumeric()));

		sString = L".";
		CHECK(!(sString.IsNumeric()));

		sString = L"46";
		CHECK(sString.IsNumeric());

		sString = L"46.23";
		CHECK(!(sString.IsNumeric()));

		sString = L"46.23.7";
		CHECK(!(sString.IsNumeric()));

		sString = L"AbC";
		CHECK(!(sString.IsNumeric()));

		sString = L"Ab5C";
		CHECK(!(sString.IsNumeric()));

		sString = L"\u65e5\u672c";
		CHECK(!(sString.IsNumeric()));

		sString = L"\u65e51\u672c";
		CHECK(!(sString.IsNumeric()));

		sString = L"\u65e5\u4E00\u672c";
		CHECK(!(sString.IsNumeric()));

		sString = L"1";
		CHECK(sString.IsNumeric());

		sString = L"\u4E00";
		CHECK(!(sString.IsNumeric()));
	}

	TEST_FIXTURE(ConstructTest, IsSubstring_String) {
		sString = "";
		CHECK(sString.IsSubstring(String("")));
		CHECK(!(sString.IsSubstring(String("12"))));

		sString = "1234";
		CHECK(sString.IsSubstring(String("12")));
		CHECK(sString.IsSubstring(String("1234")));
		CHECK(sString.IsSubstring(String("34")));
		CHECK(sString.IsSubstring(String("")));
		CHECK(!(sString.IsSubstring(String("A"))));
		CHECK(!(sString.IsSubstring(String("12345"))));

		sString = "1234";
		CHECK(sString.IsSubstring(String(L"12")));
		CHECK(sString.IsSubstring(String(L"1234")));
		CHECK(sString.IsSubstring(String(L"34")));
		CHECK(sString.IsSubstring(String(L"")));

		sString = L"1234";
		CHECK(sString.IsSubstring(String("12")));
		CHECK(sString.IsSubstring(String("1234")));
		CHECK(sString.IsSubstring(String("34")));
		CHECK(sString.IsSubstring(String("")));
	}

	TEST_FIXTURE(ConstructTest, IsSubstring_charPtr) {
		sString = "";
		CHECK(sString.IsSubstring(""));
		CHECK(!(sString.IsSubstring("12")));

		sString = "1234";
		CHECK(sString.IsSubstring("12"));
		CHECK(sString.IsSubstring("1234"));
		CHECK(sString.IsSubstring("34"));
		CHECK(sString.IsSubstring(""));
		CHECK(!(sString.IsSubstring("A")));
		CHECK(!(sString.IsSubstring("12345")));

		sString = L"1234";
		CHECK(sString.IsSubstring("12"));
		CHECK(sString.IsSubstring("1234"));
		CHECK(sString.IsSubstring("34"));
		CHECK(sString.IsSubstring(""));
		CHECK(!(sString.IsSubstring("A")));
		CHECK(!(sString.IsSubstring("12345")));
	}

	TEST_FIXTURE(ConstructTest, IsSubstring_wchar_tPtr) {
		sString = L"";
		CHECK(sString.IsSubstring(L""));
		CHECK(!(sString.IsSubstring(L"12")));

		sString = L"1234";
		CHECK(sString.IsSubstring(L"12"));
		CHECK(sString.IsSubstring(L"1234"));
		CHECK(sString.IsSubstring(L"34"));
		CHECK(sString.IsSubstring(L""));
		CHECK(!(sString.IsSubstring(L"A")));
		CHECK(!(sString.IsSubstring(L"12345")));
		CHECK(!(sString.IsSubstring(L"\u4E00")));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(sString.IsSubstring(L"\u65e5\u672c\u8a9e"));
		CHECK(sString.IsSubstring(L"\u8a9e"));
		CHECK(sString.IsSubstring(L"\u672c\u8a9e"));
		CHECK(!(sString.IsSubstring(L"\u65e5\u8a9e")));

		sString = "1234";
		CHECK(sString.IsSubstring(L"12"));
		CHECK(sString.IsSubstring(L"1234"));
		CHECK(sString.IsSubstring(L"34"));
		CHECK(sString.IsSubstring(L""));
	}

	TEST_FIXTURE(ConstructTest, Check_for_character_using_GetLength_and_IsSubstring_ASCII) {
		sString = "";
		CHECK(sString.GetLength() != 1);
		CHECK(String("").IsSubstring(sString));

		sString = "12";
		CHECK(sString.GetLength() != 1);
		CHECK(!(String("5").IsSubstring(sString)));

		sString = "1";
		CHECK(sString.GetLength() == 1);
		CHECK(String("1").IsSubstring(sString));

		sString = "1";
		CHECK(sString.GetLength() == 1);
		CHECK(String("d]16").IsSubstring(sString));

		sString = "1";
		CHECK(sString.GetLength() == 1);
		CHECK(!(String("d]6").IsSubstring(sString)));

		sString = "1";
		CHECK(sString.GetLength() == 1);
		CHECK(!(String("").IsSubstring(sString)));
	}

	TEST_FIXTURE(ConstructTest, Check_for_character_using_GetLength_and_IsSubstring_Unicode) {

		sString = L"";
		CHECK(sString.GetLength() != 1);
		CHECK(String(L"").IsSubstring(sString));

		sString = L"12";
		CHECK(sString.GetLength() != 1);
		CHECK(!(String(L"5").IsSubstring(sString)));

		sString = L"1";
		CHECK(sString.GetLength() == 1);
		CHECK(String(L"1").IsSubstring(sString));

		sString = L"1";
		CHECK(sString.GetLength() == 1);
		CHECK(String(L"d]16").IsSubstring(sString));

		sString = L"1";
		CHECK(sString.GetLength() == 1);
		CHECK(!(String(L"d]6").IsSubstring(sString)));

		sString = L"1";
		CHECK(sString.GetLength() == 1);
		CHECK(!(String(L"").IsSubstring(sString)));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK(String(L"\u65e5\u672c\u8a9e").IsSubstring(sString));
		CHECK(String(L"\u672c\u65e5\u672c\u8a9e\u672c").IsSubstring(sString));
		CHECK(!(String(L"\u672c\u8a9e\u672c").IsSubstring(sString)));
		CHECK(!(String(L"").IsSubstring(sString)));
		CHECK(!(String(L"\u8a9e").IsSubstring(sString)));

		sString = L"";
		CHECK(String(L"\u65e5\u672c\u8a9e").IsSubstring(sString));
	}

	TEST_FIXTURE(ConstructTest, IndexOf_String) {
		sString = "";
		CHECK(!(sString.IndexOf(String("")) >= 0));
		CHECK(!(sString.IndexOf(String("12")) >= 0));

		sString = "12345634";
		CHECK_EQUAL(0, sString.IndexOf(String("12")));
		CHECK(!(sString.IndexOf(String("12"), 2) >= 0));
		CHECK_EQUAL(0, sString.IndexOf(String("1234")));
		CHECK_EQUAL(2, sString.IndexOf(String("34")));
		CHECK_EQUAL(6, sString.IndexOf(String("34"), 4));
		CHECK(!(sString.IndexOf(String("34"), 8) >= 0));
		CHECK(!(sString.IndexOf(String(""))      >= 0));
		CHECK(!(sString.IndexOf(String("A")) >= 0));
		CHECK(!(sString.IndexOf(String("12346")) >= 0));

		sString = "12345634";
		CHECK_EQUAL(0, sString.IndexOf(String(L"12")));
		CHECK(!(sString.IndexOf(String(L"12"), 2) >= 0));
		CHECK_EQUAL(0, sString.IndexOf(String(L"1234")));
		CHECK_EQUAL(2, sString.IndexOf(String(L"34")));
		CHECK_EQUAL(6, sString.IndexOf(String(L"34"), 4));
		CHECK(!(sString.IndexOf(String(L"34"), 8) >= 0));
		CHECK(!(sString.IndexOf(String(L""))      >= 0));

		sString = L"12345634";
		CHECK_EQUAL(0, sString.IndexOf(String("12")));
		CHECK(!(sString.IndexOf(String("12"), 2) >= 0));
		CHECK_EQUAL(0, sString.IndexOf(String("1234")));
		CHECK_EQUAL(2, sString.IndexOf(String("34")));
		CHECK_EQUAL(6, sString.IndexOf(String("34"), 4));
		CHECK(!(sString.IndexOf(String("34"), 8) >= 0));
		CHECK(!(sString.IndexOf(String(""))      >= 0));
	}

	TEST_FIXTURE(ConstructTest, IndexOf_charPtr) {
		sString = "";
		CHECK(!(sString.IndexOf("") >= 0));
		CHECK(!(sString.IndexOf("12") >= 0));

		sString = "12345634";
		CHECK_EQUAL(0, sString.IndexOf("12"));
		CHECK(!(sString.IndexOf("12", 2) >= 0));
		CHECK_EQUAL(0, sString.IndexOf("1234"));
		CHECK_EQUAL(2, sString.IndexOf("34"));
		CHECK_EQUAL(6, sString.IndexOf("34", 4));
		CHECK(!(sString.IndexOf("34", 8) >= 0));
		CHECK(!(sString.IndexOf(static_cast<const char*>(nullptr)) >= 0));
		CHECK(!(sString.IndexOf("")      >= 0));
		CHECK(!(sString.IndexOf("A") >= 0));
		CHECK(!(sString.IndexOf("12346") >= 0));

		sString = L"12345634";
		CHECK_EQUAL(0, sString.IndexOf("12"));
		CHECK(!(sString.IndexOf("12", 2) >= 0));
		CHECK_EQUAL(0, sString.IndexOf("1234"));
		CHECK_EQUAL(2, sString.IndexOf("34"));
		CHECK_EQUAL(6, sString.IndexOf("34", 4));
		CHECK(!(sString.IndexOf("34", 8) >= 0));
		CHECK(!(sString.IndexOf(static_cast<const char*>(nullptr)) >= 0));
		CHECK(!(sString.IndexOf("")      >= 0));
	}

	TEST_FIXTURE(ConstructTest, IndexOf_wchar_tPtr) {
		sString = L"";
		CHECK(!(sString.IndexOf(L"") >= 0));
		CHECK(!(sString.IndexOf(L"12") >= 0));

		sString = L"12345634";
		CHECK_EQUAL(0, sString.IndexOf(L"12"));
		CHECK(!(sString.IndexOf(L"12", 2) >= 0));
		CHECK_EQUAL(0, sString.IndexOf(L"1234"));
		CHECK_EQUAL(2, sString.IndexOf(L"34"));
		CHECK_EQUAL(6, sString.IndexOf(L"34", 4));
		CHECK(!(sString.IndexOf(L"34", 8) >= 0));
		CHECK(!(sString.IndexOf(static_cast<const wchar_t*>(nullptr)) >= 0));
		CHECK(!(sString.IndexOf(L"")      >= 0));
		CHECK(!(sString.IndexOf(L"A") >= 0));
		CHECK(!(sString.IndexOf(L"12346") >= 0));
		CHECK(!(sString.IndexOf(L"\u65e5")    >= 0));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(0, sString.IndexOf(L"\u65e5\u672c\u8a9e"));
		CHECK_EQUAL(2, sString.IndexOf(L"\u8a9e"));
		CHECK(!(sString.IndexOf(L"") >= 0));

		sString = "12345634";
		CHECK_EQUAL(0, sString.IndexOf(L"12"));
		CHECK(!(sString.IndexOf(L"12", 2) >= 0));
		CHECK_EQUAL(0, sString.IndexOf(L"1234"));
		CHECK_EQUAL(2, sString.IndexOf(L"34"));
		CHECK_EQUAL(6, sString.IndexOf(L"34", 4));
		CHECK(!(sString.IndexOf(L"34", 8) >= 0));
		CHECK(!(sString.IndexOf(static_cast<const wchar_t*>(nullptr)) >= 0));
		CHECK(!(sString.IndexOf(L"")      >= 0));
	}

	TEST_FIXTURE(ConstructTest, LastIndexOf_String) {
		sString = "";
		CHECK(!(sString.LastIndexOf(String("")) >= 0));
		CHECK(!(sString.LastIndexOf(String("12")) >= 0));

		sString = "1234563412";
		CHECK_EQUAL(8, sString.LastIndexOf(String("12")));
		CHECK_EQUAL(0, sString.LastIndexOf(String("12"), 4));
		CHECK_EQUAL(0, sString.LastIndexOf(String("1234")));
		CHECK_EQUAL(6, sString.LastIndexOf(String("34")));
		CHECK_EQUAL(2, sString.LastIndexOf(String("34"), 4));
		CHECK(!(sString.LastIndexOf(String("34"), 1) >= 0));
		CHECK(!(sString.LastIndexOf(String(""))      >= 0));
		CHECK(!(sString.LastIndexOf(String("A")) >= 0));
		CHECK(!(sString.LastIndexOf(String("12346")) >= 0));

		sString = "1234563412";
		CHECK_EQUAL(8, sString.LastIndexOf(String(L"12")));
		CHECK_EQUAL(0, sString.LastIndexOf(String(L"12"), 4));
		CHECK_EQUAL(0, sString.LastIndexOf(String(L"1234")));
		CHECK_EQUAL(6, sString.LastIndexOf(String(L"34")));
		CHECK_EQUAL(2, sString.LastIndexOf(String(L"34"), 4));
		CHECK(!(sString.LastIndexOf(String(L"34"), 1) >= 0));
		CHECK(!(sString.LastIndexOf(String(L""))      >= 0));

		sString = L"1234563412";
		CHECK_EQUAL(8, sString.LastIndexOf(String("12")));
		CHECK_EQUAL(0, sString.LastIndexOf(String("12"), 4));
		CHECK_EQUAL(0, sString.LastIndexOf(String("1234")));
		CHECK_EQUAL(6, sString.LastIndexOf(String("34")));
		CHECK_EQUAL(2, sString.LastIndexOf(String("34"), 4));
		CHECK(!(sString.LastIndexOf(String("34"), 1) >= 0));
		CHECK(!(sString.LastIndexOf(String(""))      >= 0));
	}

	TEST_FIXTURE(ConstructTest, LastIndexOf_charPtr) {
		sString = "";
		CHECK(!(sString.LastIndexOf("") >= 0));
		CHECK(!(sString.LastIndexOf("12") >= 0));

		sString = "1234563412";
		CHECK_EQUAL(8, sString.LastIndexOf("12"));
		CHECK_EQUAL(0, sString.LastIndexOf("12", 4));
		CHECK_EQUAL(0, sString.LastIndexOf("1234"));
		CHECK_EQUAL(6, sString.LastIndexOf("34"));
		CHECK_EQUAL(2, sString.LastIndexOf("34", 4));
		CHECK(!(sString.LastIndexOf("34", 1) >= 0));
		CHECK(!(sString.LastIndexOf(static_cast<const char*>(nullptr)) >= 0));
		CHECK(!(sString.LastIndexOf("")      >= 0));
		CHECK(!(sString.LastIndexOf("A") >= 0));
		CHECK(!(sString.LastIndexOf("12346") >= 0));

		sString = L"1234563412";
		CHECK_EQUAL(8, sString.LastIndexOf("12"));
		CHECK_EQUAL(0, sString.LastIndexOf("12", 4));
		CHECK_EQUAL(0, sString.LastIndexOf("1234"));
		CHECK_EQUAL(6, sString.LastIndexOf("34"));
		CHECK_EQUAL(2, sString.LastIndexOf("34", 4));
		CHECK(!(sString.LastIndexOf("34", 1) >= 0));
		CHECK(!(sString.LastIndexOf(static_cast<const char*>(nullptr)) >= 0));
		CHECK(!(sString.LastIndexOf("")      >= 0));
	}

	TEST_FIXTURE(ConstructTest, LastIndexOf_wchar_tPtr) {
		sString = L"";
		CHECK(!(sString.LastIndexOf(L"") >= 0));
		CHECK(!(sString.LastIndexOf(L"12") >= 0));

		sString = L"1234563412";
		CHECK_EQUAL(8, sString.LastIndexOf(L"12"));
		CHECK_EQUAL(0, sString.LastIndexOf(L"12", 4));
		CHECK_EQUAL(0, sString.LastIndexOf(L"1234"));
		CHECK_EQUAL(6, sString.LastIndexOf(L"34"));
		CHECK_EQUAL(2, sString.LastIndexOf(L"34", 4));
		CHECK(!(sString.LastIndexOf(L"34", 1) >= 0));
		CHECK(!(sString.LastIndexOf(static_cast<const wchar_t*>(nullptr)) >= 0));
		CHECK(!(sString.LastIndexOf(L"")      >= 0));
		CHECK(!(sString.LastIndexOf(L"A") >= 0));
		CHECK(!(sString.LastIndexOf(L"12346") >= 0));
		CHECK(!(sString.LastIndexOf(L"\u65e5")    >= 0));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(0, sString.LastIndexOf(L"\u65e5\u672c\u8a9e"));
		CHECK_EQUAL(2, sString.LastIndexOf(L"\u8a9e"));
		CHECK(!(sString.LastIndexOf(L"") >= 0));

		sString = "1234563412";
		CHECK_EQUAL(8, sString.LastIndexOf(L"12"));
		CHECK_EQUAL(0, sString.LastIndexOf(L"12", 4));
		CHECK_EQUAL(0, sString.LastIndexOf(L"1234"));
		CHECK_EQUAL(6, sString.LastIndexOf(L"34"));
		CHECK_EQUAL(2, sString.LastIndexOf(L"34", 4));
		CHECK(!(sString.LastIndexOf(L"34", 1) >= 0));
		CHECK(!(sString.LastIndexOf(static_cast<const wchar_t*>(nullptr)) >= 0));
		CHECK(!(sString.LastIndexOf(L"")      >= 0));
	}

	TEST_FIXTURE(ConstructTest, GetSubstring_ASCII) {
		sString = "";
		CHECK(!(sString.GetSubstring(0).GetLength()));
		CHECK(!(sString.GetSubstring(5, 5).GetLength()));

		sString = "1234";
		sStringT = sString.GetSubstring(0, 2);
		CHECK(!(_strnicmp(sStringT.GetASCII(), "12",   2)));

		sStringT = sString.GetSubstring(0);
		CHECK(!(_strnicmp(sStringT.GetASCII(), "1234", 4)));

		sStringT = sString.GetSubstring(2);
		CHECK(!(_strnicmp(sStringT.GetASCII(), "34",   2)));

		sStringT = sString.GetSubstring(2, 4);
		CHECK(!(_strnicmp(sStringT.GetASCII(), "34",   2)));
		CHECK(!(sString.GetSubstring(5).GetLength()));
	}

	TEST_FIXTURE(ConstructTest, GetSubstring_Unicode) {
		sString = L"";
		CHECK(!(sString.GetSubstring(0).GetLength()));
		CHECK(!(sString.GetSubstring(5, 5).GetLength()));

		sString = L"1234";
		sStringT = sString.GetSubstring(0, 2);
		CHECK(!(_wcsnicmp(sStringT.GetUnicode(), L"12",   2)));

		sStringT = sString.GetSubstring(0);
		CHECK(!(_wcsnicmp(sStringT.GetUnicode(), L"1234", 4)));

		sStringT = sString.GetSubstring(2);
		CHECK(!(_wcsnicmp(sStringT.GetUnicode(), L"34",   2)));

		sStringT = sString.GetSubstring(2, 4);
		CHECK(!(_wcsnicmp(sStringT.GetUnicode(), L"34",   2)));
		CHECK(!(sString.GetSubstring(5).GetLength()));

		sString = L"\u65e5\u672c\u8a9e";
		sStringT = sString.GetSubstring(0, 2);
		CHECK(!(_wcsnicmp(sStringT.GetUnicode(), L"\u65e5\u672c", 2)));

		sStringT = sString.GetSubstring(1, 2);
		CHECK(!(_wcsnicmp(sStringT.GetUnicode(), L"\u672c\u8a9e", 2)));

		sStringT = sString.GetSubstring(1, 1);
		CHECK(!(_wcsnicmp(sStringT.GetUnicode(), L"\u672c",       1)));
	}

	TEST_FIXTURE(ConstructTest, ToLower_ASCII) {
		sString = "";
		CHECK(!(sString.ToLower().GetLength()));

		sString = "56";
		CHECK_EQUAL("56", sString.ToLower().GetASCII());

		sString = "AbcDE";
		CHECK_EQUAL("abcde", sString.ToLower().GetASCII());

		sString = "Ab23cD!E";
		CHECK_EQUAL("ab23cd!e", sString.ToLower().GetASCII());
	}

	TEST_FIXTURE(ConstructTest, ToLower_Unicode) {
		sString = L"";
		CHECK(!(sString.ToLower().GetLength()));

		sString = L"56";
		CHECK_EQUAL(L"56", sString.ToLower().GetUnicode());

		sString = L"AbcDE";
		CHECK_EQUAL(L"abcde", sString.ToLower().GetUnicode());

		sString = L"Ab23cD!E";
		CHECK_EQUAL(L"ab23cd!e", sString.ToLower().GetUnicode());

		sString = L"\u65e5";
		CHECK_EQUAL(L"\u65e5", sString.ToLower().GetUnicode());

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sString.ToLower().GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, ToUpper_ASCII) {
		sString = "";
		CHECK(!(sString.ToUpper().GetLength()));

		sString = "56";
		CHECK_EQUAL("56", sString.ToUpper().GetASCII());

		sString = "AbcDE";
		CHECK_EQUAL("ABCDE", sString.ToUpper().GetASCII());

		sString = "Ab23cD!E";
		CHECK_EQUAL("AB23CD!E", sString.ToUpper().GetASCII());
	}

	TEST_FIXTURE(ConstructTest, ToUpper_Unicode) {
		sString = L"";
		CHECK(!(sString.ToUpper().GetLength()));

		sString = L"56";
		CHECK_EQUAL(L"56", sString.ToUpper().GetUnicode());

		sString = L"AbcDE";
		CHECK_EQUAL(L"ABCDE", sString.ToUpper().GetUnicode());

		sString = L"Ab23cD!E";
		CHECK_EQUAL(L"AB23CD!E", sString.ToUpper().GetUnicode());

		sString = L"\u65e5";
		CHECK_EQUAL(L"\u65e5", sString.ToUpper().GetUnicode());

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sString.ToUpper().GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Delete_ASCII) {
		sString = "";
		CHECK(!(sString.Delete().GetLength()));
		CHECK(!(sString.Delete(5, 5).GetLength()));

		sString = "1";
		CHECK(!(sString.Delete().GetLength()));

		sString = "123";
		CHECK_EQUAL("13", sString.Delete(1, 1).GetASCII());
		CHECK_EQUAL("3", sString.Delete(0, 1).GetASCII());

		sString = "1234";
		CHECK_EQUAL("34", sString.Delete(0, 2).GetASCII());

		sString = "12345";
		CHECK_EQUAL("345", sString.Delete(0, 2).GetASCII());

		sString = "1234";
		CHECK_EQUAL("1234", sString.Delete(5, 1).GetASCII());
		CHECK_EQUAL("123", sString.Delete(sString.GetLength()-1, 1).GetASCII());
		CHECK(!(sString.Delete(0, 10).GetLength()));
	}

	TEST_FIXTURE(ConstructTest, Delete_Unicode) {
		sString = L"";
		CHECK(!(sString.Delete().GetLength()));
		CHECK(!(sString.Delete(5, 5).GetLength()));

		sString = L"1";
		CHECK(!(sString.Delete().GetLength()));

		sString = L"123";
		CHECK_EQUAL(L"13", sString.Delete(1, 1).GetUnicode());
		CHECK_EQUAL(L"3", sString.Delete(0, 1).GetUnicode());

		sString = L"1234";
		CHECK_EQUAL(L"34", sString.Delete(0, 2).GetUnicode());

		sString = L"12345";
		CHECK_EQUAL(L"345", sString.Delete(0, 2).GetUnicode());

		sString = L"1234";
		CHECK_EQUAL(L"1234", sString.Delete(5, 1).GetUnicode());
		CHECK_EQUAL(L"123", sString.Delete(sString.GetLength()-1, 1).GetUnicode());
		CHECK(!(sString.Delete(0, 10).GetLength()));

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(L"\u8a9e", sString.Delete(0, 2).GetUnicode());

		sString = L"\u8a9e";
		CHECK_EQUAL(L"", sString.Delete(0).GetUnicode());

		sString = L"\u65e5\u672c\u8a9e";
		CHECK_EQUAL(L"\u65e5\u672c", sString.Delete(2, 2).GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Insert_String) {
		sString = "";
		CHECK(!(sString.Insert(String("")).GetLength()));
		CHECK(!(sString.Insert(String("12"), 5).GetLength()));
		CHECK_EQUAL("12", sString.Insert(String("12"), 0, 5).GetASCII());
		CHECK_EQUAL("0 12", sString.Insert(String("0 ")).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert(String("34"), sString.GetLength()).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert(String(""), 2, 10).GetASCII());
		CHECK_EQUAL("0 12 2.5 34", sString.Insert(String(" 2.5 "), 4).GetASCII());

		sString = "";
		CHECK_EQUAL(4U , sString.Insert(String("Data/"), 0, 4).GetLength());
		CHECK_EQUAL("Data", sString.GetASCII());

		sString = "";
		CHECK(!(sString.Insert(String(L"")).GetLength()));
		CHECK(!(sString.Insert(String(L"12"), 5).GetLength()));
		CHECK_EQUAL("12", sString.Insert(String(L"12"), 0, 5).GetASCII());
		CHECK_EQUAL("0 12", sString.Insert(String(L"0 ")).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert(String(L"34"), sString.GetLength()).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert(String(L""), 2, 10).GetASCII());
		CHECK_EQUAL("0 12 2.5 34", sString.Insert(String(L" 2.5 "), 4).GetASCII());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());

		sString = L"";
		CHECK(!(sString.Insert(String("")).GetLength()));
		CHECK(!(sString.Insert(String("12"), 5).GetLength()));
		CHECK_EQUAL("12", sString.Insert(String("12"), 0, 5).GetASCII());
		CHECK_EQUAL("0 12", sString.Insert(String("0 ")).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert(String("34"), sString.GetLength()).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert(String(""), 2, 10).GetASCII());
		CHECK_EQUAL("0 12 2.5 34", sString.Insert(String(" 2.5 "), 4).GetASCII());
		CHECK_EQUAL(String::ASCII, sString.GetFormat());

		sString = L"12";
		CHECK_EQUAL(4U, sString.Insert(String("Hi")).GetLength());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());
	}

	TEST_FIXTURE(ConstructTest, Insert_charPtr) {
		sString = "";
		CHECK(!(sString.Insert(static_cast<const char*>(nullptr)).GetLength()));
		CHECK(!(sString.Insert("").GetLength()));
		CHECK(!(sString.Insert("12", 5).GetLength()));
		CHECK_EQUAL("12", sString.Insert("12", 0, 5).GetASCII());
		CHECK_EQUAL("0 12", sString.Insert("0 ").GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert("34", sString.GetLength()).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert("", 2, 10).GetASCII());
		CHECK_EQUAL("0 12 2.5 34", sString.Insert(" 2.5 ", 4).GetASCII());

		sString = L"";
		CHECK(!(sString.Insert(static_cast<const char*>(nullptr)).GetLength()));
		CHECK(!(sString.Insert("").GetLength()));
		CHECK(!(sString.Insert("12", 5).GetLength()));
		CHECK_EQUAL("12", sString.Insert("12", 0, 5).GetASCII());
		CHECK_EQUAL("0 12", sString.Insert("0 ").GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert("34", sString.GetLength()).GetASCII());
		CHECK_EQUAL("0 1234", sString.Insert("", 2, 10).GetASCII());
		CHECK_EQUAL("0 12 2.5 34", sString.Insert(" 2.5 ", 4).GetASCII());
		CHECK_EQUAL(String::ASCII, sString.GetFormat());

		sString = L"12";
		CHECK_EQUAL(4U, sString.Insert("Hi").GetLength());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());
	}

	TEST_FIXTURE(ConstructTest, Insert_wchar_tPtr) {
		sString = L"";
		CHECK(!(sString.Insert(static_cast<const wchar_t*>(nullptr)).GetLength()));
		CHECK(!(sString.Insert(L"").GetLength()));
		CHECK(!(sString.Insert(L"12", 5).GetLength()));
		CHECK_EQUAL(L"12", sString.Insert(L"12", 0, 5).GetUnicode());
		CHECK_EQUAL(L"0 12", sString.Insert(L"0 ").GetUnicode());
		CHECK_EQUAL(L"0 1234", sString.Insert(L"34", sString.GetLength()).GetUnicode());
		CHECK_EQUAL(L"0 1234", sString.Insert(L"", 2, 10).GetUnicode());
		CHECK_EQUAL(L"0 12 2.5 34", sString.Insert(L" 2.5 ", 4).GetUnicode());

		sString = L"";
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e", sString.Insert(L"\u65e5\u672c\u8a9e").GetUnicode());
		CHECK_EQUAL(L"\u65e5\u672c\u8a9e\u65e5\u672c\u8a9e", sString.Insert(L"\u65e5\u672c\u8a9e").GetUnicode());

		sString = "";
		CHECK(!(sString.Insert(static_cast<const wchar_t*>(nullptr)).GetLength()));
		CHECK(!(sString.Insert(L"").GetLength()));
		CHECK(!(sString.Insert(L"12", 5).GetLength()));
		CHECK_EQUAL(L"12", sString.Insert(L"12", 0, 5).GetUnicode());
		CHECK_EQUAL(L"0 12", sString.Insert(L"0 ").GetUnicode());
		CHECK_EQUAL(L"0 1234", sString.Insert(L"34", sString.GetLength()).GetUnicode());
		CHECK_EQUAL(L"0 1234", sString.Insert(L"", 2, 10).GetUnicode());
		CHECK_EQUAL(L"0 12 2.5 34", sString.Insert(L" 2.5 ", 4).GetUnicode());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());
	}

	TEST_FIXTURE(ConstructTest, Copy_charPtr) {
		pszTemp = " Ab_1 2 ";
		sString.Copy(static_cast<const char*>(nullptr));
		CHECK(!(sString.GetLength()));

		sString.Copy(pszTemp);
		CHECK_EQUAL(pszTemp, sString.GetASCII());

		sString.Copy(pszTemp, 3);
		CHECK_EQUAL(" Ab", sString.GetASCII());

		sString.Copy(pszTemp+3, 3);
		CHECK_EQUAL("_1 ", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Copy_wchar_tPtr) {
		pszTempWide = L" Ab_1 2 ";
		sString.Copy(static_cast<const wchar_t*>(nullptr));
		CHECK(!(sString.GetLength()));

		sString.Copy(pszTempWide);
		CHECK_EQUAL(pszTempWide, sString.GetUnicode());

		sString.Copy(pszTempWide, 3);
		CHECK_EQUAL(L" Ab", sString.GetUnicode());

		sString.Copy(pszTempWide+3, 3);
		CHECK_EQUAL(L"_1 ", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Replace_char) {
		sString = "";
		CHECK(!(sString.Replace(' ', ' ')));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "1234";
		CHECK(!(sString.Replace(' ', '5')));
		CHECK_EQUAL("1234", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(0U, sString.Replace('1', '1'));
		CHECK_EQUAL("123141", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace('1', '_'));
		CHECK_EQUAL("_23_4_", sString.GetASCII());

		sString = "_123141";
		CHECK_EQUAL(3U, sString.Replace('1', '_'));
		CHECK_EQUAL("__23_4_", sString.GetASCII());

		sString = "|";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL("+", sString.GetASCII());

		sString = "|<>";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL("+<>", sString.GetASCII());

		sString = "<>|";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL("<>+", sString.GetASCII());

		sString = "<>||";
		CHECK_EQUAL(2U, sString.Replace('|', '+'));
		CHECK_EQUAL("<>++", sString.GetASCII());

		sString = "<|>||";
		CHECK_EQUAL(3U, sString.Replace('|', '+'));
		CHECK_EQUAL("<+>++", sString.GetASCII());

		sString = "<>|<>";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL("<>+<>", sString.GetASCII());

		sString = "<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace('|', '+'));
		CHECK_EQUAL("<>+<>+<>", sString.GetASCII());

		sString = L"";
		CHECK(!(sString.Replace(' ', ' ')));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"1234";
		CHECK(!(sString.Replace(' ', '5')));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"1234", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(0U, sString.Replace('1', '1'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(3U, sString.Replace('1', '_'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"_23_4_", sString.GetUnicode());

		sString = L"_123141";
		CHECK_EQUAL(3U, sString.Replace('1', '_'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"__23_4_", sString.GetUnicode());

		sString = L"|";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"+", sString.GetUnicode());

		sString = L"|<>";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"+<>", sString.GetUnicode());

		sString = L"<>|";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>+", sString.GetUnicode());

		sString = L"<>||";
		CHECK_EQUAL(2U, sString.Replace('|', '+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>++", sString.GetUnicode());

		sString = L"<|>||";
		CHECK_EQUAL(3U, sString.Replace('|', '+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<+>++", sString.GetUnicode());

		sString = L"<>|<>";
		CHECK_EQUAL(1U, sString.Replace('|', '+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>+<>", sString.GetUnicode());

		sString = L"<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace('|', '+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>+<>+<>", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Replace_wchar_t) {
		sString = L"";
		CHECK(!(sString.Replace(L' ', L' ')));
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"1234";
		CHECK(!(sString.Replace(L' ', L'5')));
		CHECK_EQUAL(L"1234", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(0U, sString.Replace(L'1', L'1'));
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(3U, sString.Replace(L'1', L'_'));
		CHECK_EQUAL(L"_23_4_", sString.GetUnicode());

		sString = L"_123141";
		CHECK_EQUAL(3U, sString.Replace(L'1', L'_'));
		CHECK_EQUAL(L"__23_4_", sString.GetUnicode());

		sString = L"|";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(L"+", sString.GetUnicode());

		sString = L"|<>";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(L"+<>", sString.GetUnicode());

		sString = L"<>|";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(L"<>+", sString.GetUnicode());

		sString = L"<>||";
		CHECK_EQUAL(2U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(L"<>++", sString.GetUnicode());

		sString = L"<|>||";
		CHECK_EQUAL(3U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(L"<+>++", sString.GetUnicode());

		sString = L"<>|<>";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(L"<>+<>", sString.GetUnicode());

		sString = L"<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(L"<>+<>+<>", sString.GetUnicode());

		sString = L"<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(L'|', L'\u672c'));
		CHECK_EQUAL(L"<>\u672c<>\u672c<>", sString.GetUnicode());

		sString = "";
		CHECK(!(sString.Replace(L' ', L' ')));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = "1234";
		CHECK(!(sString.Replace(L' ', L'5')));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"1234", sString.GetUnicode());

		sString = "123141";
		CHECK_EQUAL(0U, sString.Replace(L'1', L'1'));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace(L'1', L'_'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"_23_4_", sString.GetUnicode());

		sString = "_123141";
		CHECK_EQUAL(3U, sString.Replace(L'1', L'_'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"__23_4_", sString.GetUnicode());

		sString = "|";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"+", sString.GetUnicode());

		sString = "|<>";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"+<>", sString.GetUnicode());

		sString = "<>|";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>+", sString.GetUnicode());

		sString = "<>||";
		CHECK_EQUAL(2U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>++", sString.GetUnicode());

		sString = "<|>||";
		CHECK_EQUAL(3U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<+>++", sString.GetUnicode());

		sString = "<>|<>";
		CHECK_EQUAL(1U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>+<>", sString.GetUnicode());

		sString = "<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(L'|', L'+'));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>+<>+<>", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Replace_String) {
		sString = "";
		CHECK(!(sString.Replace(String(' '), String(' '))));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "1234";
		CHECK(!(sString.Replace(String(' '), String('5'))));
		CHECK_EQUAL("1234", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(0U, sString.Replace(String('1'), String('1')));
		CHECK_EQUAL("123141", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace(String('1'), String('_')));
		CHECK_EQUAL("_23_4_", sString.GetASCII());

		sString = "_123141";
		CHECK_EQUAL(3U, sString.Replace(String('1'), String('_')));
		CHECK_EQUAL("__23_4_", sString.GetASCII());

		sString = "123412";
		CHECK_EQUAL(0U, sString.Replace(String("12"), String("12")));
		CHECK_EQUAL("123412", sString.GetASCII());

		sString = "123412";
		CHECK_EQUAL(2U, sString.Replace(String("12"), String("1")));
		CHECK_EQUAL("1341", sString.GetASCII());

		sString = "51123412";
		CHECK_EQUAL(2U, sString.Replace(String("12"), String("1_2")));
		CHECK_EQUAL("511_2341_2", sString.GetASCII());

		sString = "123412";
		CHECK_EQUAL(1U, sString.Replace(String("123412"), String("")));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "Test 123412 This";
		CHECK_EQUAL(0U, sString.Replace(String("ahoioi"), String("nope")));
		CHECK_EQUAL("Test 123412 This", sString.GetASCII());

		sString = "<>|<>";
		CHECK_EQUAL(1U, sString.Replace(String("|"), String("Hi")));
		CHECK_EQUAL("<>Hi<>", sString.GetASCII());

		sString = "<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(String("|"), String("Hi")));
		CHECK_EQUAL("<>Hi<>Hi<>", sString.GetASCII());

		sString = "<++++>";
		CHECK_EQUAL(2U, sString.Replace(String("++"), String("+")));
		CHECK_EQUAL("<++>", sString.GetASCII());

		sString = L"";
		CHECK(!(sString.Replace(String(' '), String(' '))));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = L"1234";
		CHECK(!(sString.Replace(String(' '), String('5'))));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("1234", sString.GetASCII());

		sString = L"123141";
		CHECK_EQUAL(0U, sString.Replace(String('1'), String('1')));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("123141", sString.GetASCII());

		sString = L"123141";
		CHECK_EQUAL(3U, sString.Replace(String('1'), String('_')));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("_23_4_", sString.GetASCII());

		sString = L"_123141";
		CHECK_EQUAL(3U, sString.Replace(String('1'), String('_')));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("__23_4_", sString.GetASCII());

		sString = L"123412";
		CHECK_EQUAL(0U, sString.Replace(String("12"), String("12")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("123412", sString.GetASCII());

		sString = L"123412";
		CHECK_EQUAL(2U, sString.Replace(String("12"), String("1")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("1341", sString.GetASCII());

		sString = L"51123412";
		CHECK_EQUAL(2U, sString.Replace(String("12"), String("1_2")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("511_2341_2", sString.GetASCII());

		sString = L"123412";
		CHECK_EQUAL(1U, sString.Replace(String("123412"), String("")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = L"Test 123412 This";
		CHECK_EQUAL(0U, sString.Replace(String("ahoioi"), String("nope")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("Test 123412 This", sString.GetASCII());

		sString = L"<>|<>";
		CHECK_EQUAL(1U, sString.Replace(String("|"), String("Hi")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("<>Hi<>", sString.GetASCII());

		sString = L"<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(String("|"), String("Hi")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("<>Hi<>Hi<>", sString.GetASCII());

		sString = L"<++++>";
		CHECK_EQUAL(2U, sString.Replace(String("++"), String("+")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("<++>", sString.GetASCII());

		sString = "";
		CHECK(!(sString.Replace(String(L' '), String(L' '))));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = "1234";
		CHECK(!(sString.Replace(String(' '), String(L'5'))));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("1234", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(0U, sString.Replace(String(L'1'), String(L'1')));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace(String(L'1'), String('_')));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"_23_4_", sString.GetUnicode());

		sString = "_123141";
		CHECK_EQUAL(3U, sString.Replace(String(L'1'), String(L'_')));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"__23_4_", sString.GetUnicode());

		sString = "123412";
		CHECK_EQUAL(0U, sString.Replace(String(L"12"), String(L"12")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"123412", sString.GetUnicode());

		sString = "123412";
		CHECK_EQUAL(2U, sString.Replace(String("12"), String(L"1")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"1341", sString.GetUnicode());

		sString = "51123412";
		CHECK_EQUAL(2U, sString.Replace(String(L"12"), String(L"1_2")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"511_2341_2", sString.GetUnicode());

		sString = "123412";
		CHECK_EQUAL(1U, sString.Replace(String(L"123412"), String("")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = "Test 123412 This";
		CHECK_EQUAL(0U, sString.Replace(String("ahoioi"), String(L"nope")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("Test 123412 This", sString.GetASCII());

		sString = "<>|<>";
		CHECK_EQUAL(1U, sString.Replace(String(L"|"), String("Hi")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>Hi<>", sString.GetUnicode());

		sString = "<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(String("|"), String(L"Hi")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>Hi<>Hi<>", sString.GetUnicode());

		sString = "<++++>";
		CHECK_EQUAL(2U, sString.Replace(String(L"++"), String(L"+")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<++>", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Replace_charPtr) {
		sString = "";
		CHECK(!(sString.Replace(static_cast<const char*>(nullptr), nullptr)));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		CHECK(!(sString.Replace(" ", static_cast<const char*>(nullptr))));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		CHECK(!(sString.Replace(static_cast<const char*>(nullptr), " ")));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "";
		CHECK(!(sString.Replace(" ", " ")));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "1234";
		CHECK(!(sString.Replace(" ", "5")));
		CHECK_EQUAL("1234", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(0U, sString.Replace("1", "1"));
		CHECK_EQUAL("123141", sString.GetASCII());

		sString = "123141";
		CHECK(!(sString.Replace(static_cast<const char*>(nullptr), "_")));
		CHECK_EQUAL("123141", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace("1", static_cast<const char*>(nullptr)));
		CHECK_EQUAL("234", sString.GetASCII());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace("1", "_"));
		CHECK_EQUAL("_23_4_", sString.GetASCII());

		sString = "_123141";
		CHECK_EQUAL(3U, sString.Replace("1", "_"));
		CHECK_EQUAL("__23_4_", sString.GetASCII());

		sString = "123412";
		CHECK_EQUAL(0U, sString.Replace("12", "12"));
		CHECK_EQUAL("123412", sString.GetASCII());

		sString = "123412";
		CHECK_EQUAL(2U, sString.Replace("12", "1"));
		CHECK_EQUAL("1341", sString.GetASCII());

		sString = "51123412";
		CHECK_EQUAL(2U, sString.Replace("12", "1_2"));
		CHECK_EQUAL("511_2341_2", sString.GetASCII());

		sString = "123412";
		CHECK_EQUAL(1U, sString.Replace("123412", ""));
		CHECK_EQUAL("", sString.GetASCII());

		sString = "Test 123412 This";
		CHECK_EQUAL(0U, sString.Replace("ahoioi", "nope"));
		CHECK_EQUAL("Test 123412 This", sString.GetASCII());

		sString = "<>|<>";
		CHECK_EQUAL(1U, sString.Replace("|", "Hi"));
		CHECK_EQUAL("<>Hi<>", sString.GetASCII());

		sString = "<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace("|", "Hi"));
		CHECK_EQUAL("<>Hi<>Hi<>", sString.GetASCII());

		sString = "<++++>";
		CHECK_EQUAL(2U, sString.Replace("++", "+"));
		CHECK_EQUAL("<++>", sString.GetASCII());

		sString = L"";
		CHECK(!(sString.Replace(static_cast<const char*>(nullptr), nullptr)));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = L"";
		CHECK(!(sString.Replace(" ", static_cast<const char*>(nullptr))));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = L"";
		CHECK(!(sString.Replace(static_cast<const char*>(nullptr), " ")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = L"";
		CHECK(!(sString.Replace(" ", " ")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = L"1234";
		CHECK(!(sString.Replace(" ", "5")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("1234", sString.GetASCII());

		sString = L"123141";
		CHECK_EQUAL(0U, sString.Replace("1", "1"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("123141", sString.GetASCII());

		sString = L"123141";
		CHECK(!(sString.Replace(static_cast<const char*>(nullptr), "_")));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("123141", sString.GetASCII());

		sString = L"123141";
		CHECK_EQUAL(3U, sString.Replace("1", static_cast<const char*>(nullptr)));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"234", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(3U, sString.Replace("1", "_"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"_23_4_", sString.GetUnicode());

		sString = L"_123141";
		CHECK_EQUAL(3U, sString.Replace("1", "_"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"__23_4_", sString.GetUnicode());

		sString = L"123412";
		CHECK_EQUAL(0U, sString.Replace("12", "12"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("123412", sString.GetASCII());

		sString = L"123412";
		CHECK_EQUAL(2U, sString.Replace("12", "1"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"1341", sString.GetUnicode());

		sString = L"51123412";
		CHECK_EQUAL(2U, sString.Replace("12", "1_2"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"511_2341_2", sString.GetUnicode());

		sString = L"123412";
		CHECK_EQUAL(1U, sString.Replace("123412", ""));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL("", sString.GetASCII());

		sString = L"Test 123412 This";
		CHECK_EQUAL(0U, sString.Replace("ahoioi", "nope"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL("Test 123412 This", sString.GetASCII());

		sString = L"<>|<>";
		CHECK_EQUAL(1U, sString.Replace("|", "Hi"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>Hi<>", sString.GetUnicode());

		sString = L"<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace("|", "Hi"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>Hi<>Hi<>", sString.GetUnicode());

		sString = L"<++++>";
		CHECK_EQUAL(2U, sString.Replace("++", "+"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<++>", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Replace_wchar_tPtr) {
		sString = L"";
		CHECK(!(sString.Replace(static_cast<const wchar_t*>(nullptr), nullptr)));
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		CHECK(!(sString.Replace(L" ", static_cast<const wchar_t*>(nullptr))));
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		CHECK(!(sString.Replace(static_cast<const wchar_t*>(nullptr), L" ")));
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"";
		CHECK(!(sString.Replace(L" ", L" ")));
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"1234";
		CHECK(!(sString.Replace(L" ", L"5")));
		CHECK_EQUAL(L"1234", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(0U, sString.Replace(L"1", L"1"));
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = L"123141";
		CHECK(!(sString.Replace(static_cast<const wchar_t*>(nullptr), L"_")));
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(3U, sString.Replace(L"1", static_cast<const wchar_t*>(nullptr)));
		CHECK_EQUAL(L"234", sString.GetUnicode());

		sString = L"123141";
		CHECK_EQUAL(3U, sString.Replace(L"1", L"_"));
		CHECK_EQUAL(L"_23_4_", sString.GetUnicode());

		sString = L"_123141";
		CHECK_EQUAL(3U, sString.Replace(L"1", L"_"));
		CHECK_EQUAL(L"__23_4_", sString.GetUnicode());

		sString = L"123412";
		CHECK_EQUAL(0U, sString.Replace(L"12", L"12"));
		CHECK_EQUAL(L"123412", sString.GetUnicode());

		sString = L"123412";
		CHECK_EQUAL(2U, sString.Replace(L"12", L"1"));
		CHECK_EQUAL(L"1341", sString.GetUnicode());

		sString = L"51123412";
		CHECK_EQUAL(2U, sString.Replace(L"12", L"1_2"));
		CHECK_EQUAL(L"511_2341_2", sString.GetUnicode());

		sString = L"123412";
		CHECK_EQUAL(1U, sString.Replace(L"123412", L""));
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L"Test 123412 This";
		CHECK_EQUAL(0U, sString.Replace(L"ahoioi", L"nope"));
		CHECK_EQUAL(L"Test 123412 This", sString.GetUnicode());

		sString = L"<>|<>";
		CHECK_EQUAL(1U, sString.Replace(L"|", L"Hi"));
		CHECK_EQUAL(L"<>Hi<>", sString.GetUnicode());

		sString = L"<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(L"|", L"Hi"));
		CHECK_EQUAL(L"<>Hi<>Hi<>", sString.GetUnicode());

		sString = L"<++++>";
		CHECK_EQUAL(2U, sString.Replace(L"++", L"+"));
		CHECK_EQUAL(L"<++>", sString.GetUnicode());

		sString = L"<++++>";
		CHECK_EQUAL(2U, sString.Replace(L"++", L"\u672c"));
		CHECK_EQUAL(L"<\u672c\u672c>", sString.GetUnicode());

		sString = "";
		CHECK(!(sString.Replace(static_cast<const wchar_t*>(nullptr), nullptr)));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = "";
		CHECK(!(sString.Replace(L" ", static_cast<const wchar_t*>(nullptr))));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = "";
		CHECK(!(sString.Replace(static_cast<const wchar_t*>(nullptr), L" ")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = "";
		CHECK(!(sString.Replace(L" ", L" ")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = "1234";
		CHECK(!(sString.Replace(L" ", L"5")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"1234", sString.GetUnicode());

		sString = "123141";
		CHECK_EQUAL(0U, sString.Replace(L"1", L"1"));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = "123141";
		CHECK(!(sString.Replace(static_cast<const wchar_t*>(nullptr), L"_")));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"123141", sString.GetUnicode());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace(L"1", static_cast<const wchar_t*>(nullptr)));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"234", sString.GetUnicode());

		sString = "123141";
		CHECK_EQUAL(3U, sString.Replace(L"1", L"_"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"_23_4_", sString.GetUnicode());

		sString = "_123141";
		CHECK_EQUAL(3U, sString.Replace(L"1", L"_"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"__23_4_", sString.GetUnicode());

		sString = "123412";
		CHECK_EQUAL(0U, sString.Replace(L"12", L"12"));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"123412", sString.GetUnicode());

		sString = "123412";
		CHECK_EQUAL(2U, sString.Replace(L"12", L"1"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"1341", sString.GetUnicode());

		sString = "51123412";
		CHECK_EQUAL(2U, sString.Replace(L"12", L"1_2"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"511_2341_2", sString.GetUnicode());

		sString = "123412";
		CHECK_EQUAL(1U, sString.Replace(L"123412", L""));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = "Test 123412 This";
		CHECK_EQUAL(0U, sString.Replace(L"ahoioi", L"nope"));
		CHECK_EQUAL(String::ASCII , sString.GetFormat());
		CHECK_EQUAL(L"Test 123412 This", sString.GetUnicode());

		sString = "<>|<>";
		CHECK_EQUAL(1U, sString.Replace(L"|", L"Hi"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>Hi<>", sString.GetUnicode());

		sString = "<>|<>|<>";
		CHECK_EQUAL(2U, sString.Replace(L"|", L"Hi"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<>Hi<>Hi<>", sString.GetUnicode());

		sString = "<++++>";
		CHECK_EQUAL(2U, sString.Replace(L"++", L"+"));
		CHECK_EQUAL(String::Unicode , sString.GetFormat());
		CHECK_EQUAL(L"<++>", sString.GetUnicode());

		sString = "<++++>";
	}

	TEST_FIXTURE(ConstructTest, SetCharacter_char) {
		sString = "";
		CHECK(!(sString.SetCharacter(0, 'a')));
		CHECK(!(sString.SetCharacter(1, 'a')));
		CHECK(!(sString.GetLength()));

		sString = "b";
		CHECK(sString.SetCharacter(0, 'a'));
		CHECK(!(sString.SetCharacter(1, 'a')));
		CHECK_EQUAL(1U, sString.GetLength());
		CHECK_EQUAL("a", sString.GetASCII());

		sString = "bb";
		CHECK(sString.SetCharacter(0, 'a'));
		CHECK(!(!sString.SetCharacter(1, '+')));
		CHECK_EQUAL(2U, sString.GetLength());
		CHECK_EQUAL("a+", sString.GetASCII());

		sString = L"";
		CHECK(!(sString.SetCharacter(0, 'a')));
		CHECK(!(sString.SetCharacter(1, 'a')));
		CHECK(!(sString.GetLength()));
		CHECK_EQUAL(String::ASCII, sString.GetFormat());

		sString = L"b";
		CHECK(sString.SetCharacter(0, 'a'));
		CHECK(!(sString.SetCharacter(1, 'a')));
		CHECK_EQUAL(1U, sString.GetLength());
		CHECK_EQUAL("a" , sString.GetASCII());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());

		sString = L"bb";
		CHECK(sString.SetCharacter(0, 'a'));
		CHECK(!(!sString.SetCharacter(1, '+')));
		CHECK_EQUAL(2U, sString.GetLength());
		CHECK_EQUAL("a+" , sString.GetASCII());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());
	}

	TEST_FIXTURE(ConstructTest, SetCharacter_wchar_t) {
		sString = L"";
		CHECK(!(sString.SetCharacter(0, L'a')));
		CHECK(!(sString.SetCharacter(1, L'a')));
		CHECK(!(sString.GetLength()));

		sString = L"b";
		CHECK(sString.SetCharacter(0, L'a'));
		CHECK(!(sString.SetCharacter(1, L'a')));
		CHECK_EQUAL(1U, sString.GetLength());
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString = L"bb";
		CHECK(sString.SetCharacter(0, L'a'));
		CHECK(!(!sString.SetCharacter(1, L'+')));
		CHECK_EQUAL(2U, sString.GetLength());
		CHECK_EQUAL(L"a+", sString.GetUnicode());

		sString = L"\u672cb";
		CHECK(sString.SetCharacter(0, L'\u672c'));
		CHECK(!(!sString.SetCharacter(1, L'\u672c')));
		CHECK_EQUAL(2U, sString.GetLength());
		CHECK_EQUAL(L"\u672c\u672c", sString.GetUnicode());

		sString = "";
		CHECK(!(sString.SetCharacter(0, L'a')));
		CHECK(!(sString.SetCharacter(1, L'a')));
		CHECK(!(sString.GetLength()));
		CHECK_EQUAL(String::ASCII, sString.GetFormat());

		sString = "b";
		CHECK(sString.SetCharacter(0, L'a'));
		CHECK(!(sString.SetCharacter(1, L'a')));
		CHECK_EQUAL(1U, sString.GetLength());
		CHECK_EQUAL(L"a" , sString.GetUnicode());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());

		sString = "bb";
		CHECK(sString.SetCharacter(0, L'a'));
		CHECK(!(!sString.SetCharacter(1, L'+')));
		CHECK_EQUAL(2U, sString.GetLength());
		CHECK_EQUAL(L"a+" , sString.GetUnicode());
		CHECK_EQUAL(String::Unicode, sString.GetFormat());
	}

	TEST_FIXTURE(ConstructTest, TrimLeading_ASCII) {
		sString = "";
		CHECK(!(sString.TrimLeading().GetLength()));

		sString = "123";
		CHECK_EQUAL("123", sString.TrimLeading().GetASCII());

		sString = "123  ";
		CHECK_EQUAL("123  ", sString.TrimLeading().GetASCII());

		sString = "  123";
		CHECK_EQUAL("123", sString.TrimLeading().GetASCII());

		sString = "  123  ";
		CHECK_EQUAL("123  ", sString.TrimLeading().GetASCII());

		sString = "  ";
		CHECK(!(sString.TrimLeading().GetLength()));
	}

	TEST_FIXTURE(ConstructTest, TrimLeading_Unicode) {
		sString = L"";
		CHECK(!(sString.TrimLeading().GetLength()));

		sString = L"123";
		CHECK_EQUAL(L"123", sString.TrimLeading().GetUnicode());

		sString = L"123  ";
		CHECK_EQUAL(L"123  ", sString.TrimLeading().GetUnicode());

		sString = L"  123";
		CHECK_EQUAL(L"123", sString.TrimLeading().GetUnicode());

		sString = L"  123  ";
		CHECK_EQUAL(L"123  ", sString.TrimLeading().GetUnicode());

		sString = L"  ";
		CHECK(!(sString.TrimLeading().GetLength()));

		sString = L"  \u65e5\u672c \u8a9e  ";
		CHECK_EQUAL(L"\u65e5\u672c \u8a9e  ", sString.TrimLeading().GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, TrimTrailing_ASCII) {
		sString = "";
		CHECK(!(sString.TrimTrailing().GetLength()));

		sString = "123";
		CHECK_EQUAL("123", sString.TrimTrailing().GetASCII());

		sString = "  123";
		CHECK_EQUAL("  123", sString.TrimTrailing().GetASCII());

		sString = "123  ";
		CHECK_EQUAL("123", sString.TrimTrailing().GetASCII());

		sString = "  123  ";
		CHECK_EQUAL("  123", sString.TrimTrailing().GetASCII());

		sString = "  ";
		CHECK(!(sString.TrimTrailing().GetLength()));
	}

	TEST_FIXTURE(ConstructTest, TrimTrailing_Unicode) {
		sString = L"";
		CHECK(!(sString.TrimTrailing().GetLength()));

		sString = L"123";
		CHECK_EQUAL(L"123", sString.TrimTrailing().GetUnicode());

		sString = L"  123";
		CHECK_EQUAL(L"  123", sString.TrimTrailing().GetUnicode());

		sString = L"123  ";
		CHECK_EQUAL(L"123", sString.TrimTrailing().GetUnicode());

		sString = L"  123  ";
		CHECK_EQUAL(L"  123", sString.TrimTrailing().GetUnicode());

		sString = L"  ";
		CHECK(!(sString.TrimTrailing().GetLength()));

		sString = L"  \u65e5\u672c \u8a9e  ";
		CHECK_EQUAL(L"  \u65e5\u672c \u8a9e", sString.TrimTrailing().GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Trim_ASCII) {
		sString = "";
		CHECK(!(sString.Trim().GetLength()));

		sString = "123";
		CHECK_EQUAL("123", sString.Trim().GetASCII());

		sString = "  123";
		CHECK_EQUAL("123", sString.Trim().GetASCII());

		sString = "123  ";
		CHECK_EQUAL("123", sString.Trim().GetASCII());

		sString = "  123  ";
		CHECK_EQUAL("123", sString.Trim().GetASCII());

		sString = "  ";
		CHECK(!(sString.Trim().GetLength()));
	}

	TEST_FIXTURE(ConstructTest, Trim_Unicode) {
		sString = L"";
		CHECK(!(sString.Trim().GetLength()));

		sString = L"123";
		CHECK_EQUAL(L"123", sString.Trim().GetUnicode());

		sString = L"  123";
		CHECK_EQUAL(L"123", sString.Trim().GetUnicode());

		sString = L"123  ";
		CHECK_EQUAL(L"123", sString.Trim().GetUnicode());

		sString = L"  123  ";
		CHECK_EQUAL(L"123", sString.Trim().GetUnicode());

		sString = L"  ";
		CHECK(!(sString.Trim().GetLength()));

		sString = L"  \u65e5\u672c \u8a9e  ";
		CHECK_EQUAL(L"\u65e5\u672c \u8a9e", sString.Trim().GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, IsValidInteger) {
		sString = "";
		CHECK(!(sString.IsValidInteger()));

		sString = "0";
		CHECK(sString.IsValidInteger());

		sString = "false";
		CHECK(!(sString.IsValidInteger()));

		sString = "Timmy!";
		CHECK(!(sString.IsValidInteger()));

		sString = "1365";
		CHECK(sString.IsValidInteger());

		sString = "024";
		CHECK(sString.IsValidInteger());

		sString = "-0242";
		CHECK(sString.IsValidInteger());

		sString = "-0242-";
		CHECK(!(sString.IsValidInteger()));

		sString = "+0242-";
		CHECK(!(sString.IsValidInteger()));

		sString = "+0242";
		CHECK(sString.IsValidInteger());
	}

	TEST_FIXTURE(ConstructTest, IsValidFloat) {
		sString = "";
		CHECK(!(sString.IsValidFloat()));

		sString = "0";
		CHECK(sString.IsValidFloat());

		sString = "false";
		CHECK(!(sString.IsValidFloat()));

		sString = "Timmy!";
		CHECK(!(sString.IsValidFloat()));

		sString = "1365";
		CHECK(sString.IsValidFloat());

		sString = "024";
		CHECK(sString.IsValidFloat());

		sString = "-0242";
		CHECK(sString.IsValidFloat());

		sString = "-0242-";
		CHECK(!(sString.IsValidFloat()));

		sString = "+0242-";
		CHECK(!(sString.IsValidFloat()));

		sString = "+0242";
		CHECK(sString.IsValidFloat());

		sString = "0.";
		CHECK(sString.IsValidFloat());

		sString = ".0";
		CHECK(sString.IsValidFloat());

		sString = "1.0";
		CHECK(sString.IsValidFloat());

		sString = "1.0.2";
		CHECK(!(sString.IsValidFloat()));

		sString = "-1.0";
		CHECK(sString.IsValidFloat());

		sString = "1.0e24";
		CHECK(sString.IsValidFloat());

		sString = "1.0e-24";
		CHECK(sString.IsValidFloat());

		sString = "1.e0e-24";
		CHECK(!(sString.IsValidFloat()));
	}

	TEST_FIXTURE(ConstructTest, GetBool_ASCII) {
		sString = "";
		CHECK(!(sString.GetBool()));

		sString = "0";
		CHECK(!(sString.GetBool()));

		sString = "false";
		CHECK(!(sString.GetBool()));

		sString = "False";
		CHECK(!(sString.GetBool()));

		sString = "1";
		CHECK(sString.GetBool());

		sString = "true";
		CHECK(sString.GetBool());

		sString = "True";
		CHECK(sString.GetBool());
	}

	TEST_FIXTURE(ConstructTest, GetBool_Unicode) {
		sString = L"";
		CHECK(!(sString.GetBool()));

		sString = L"0";
		CHECK(!(sString.GetBool()));

		sString = L"false";
		CHECK(!(sString.GetBool()));

		sString = L"False";
		CHECK(!(sString.GetBool()));

		sString = L"1";
		CHECK(sString.GetBool());

		sString = L"true";
		CHECK(sString.GetBool());

		sString = L"True";
		CHECK(sString.GetBool());

		sString = L"\u672c";
		CHECK(!(sString.GetBool()));
	}

	TEST_FIXTURE(ConstructTest, GetChar) {
		sString = "";
		CHECK_EQUAL('\0', sString.GetChar());

		sString = "a";
		CHECK_EQUAL('\0', sString.GetChar());

		sString = "65";
		CHECK_EQUAL('A', sString.GetChar());
	}

	TEST_FIXTURE(ConstructTest, GetCharWide) {
		sString = L"";
		CHECK_EQUAL(L'\0', sString.GetChar());

		sString = L"a";
		CHECK_EQUAL(L'\0', sString.GetChar());
	}

	TEST_FIXTURE(ConstructTest, GetInt_ASCII) {
		sString = "";
		CHECK(!(sString.GetInt()));

		sString = "0";
		CHECK(!(sString.GetInt()));

		sString = "5";
		CHECK_EQUAL(5, sString.GetInt());
	}

	TEST_FIXTURE(ConstructTest, GetInt_Unicode) {
		sString = L"";
		CHECK(!(sString.GetInt()));

		sString = L"0";
		CHECK(!(sString.GetInt()));

		sString = L"5";
		CHECK_EQUAL(5, sString.GetInt());

		sString = L"\u672c";
		CHECK_EQUAL(0, sString.GetInt());
	}

	TEST_FIXTURE(ConstructTest, GetFloat_ASCII) {
		sString = "";
		CHECK(!(sString.GetFloat()));

		sString = "0";
		CHECK(!(sString.GetFloat()));

		sString = "5";
		CHECK_EQUAL(5.0f, sString.GetFloat());

		sString = "5.0";
		CHECK_EQUAL(5.0f, sString.GetFloat());

		sString = "5.5";
		CHECK_EQUAL(5.5f, sString.GetFloat());
	}

	TEST_FIXTURE(ConstructTest, GetFloat_Unicode) {
		sString = L"";
		CHECK(!(sString.GetFloat()));

		sString = L"0";
		CHECK(!(sString.GetFloat()));

		sString = L"5";
		CHECK_EQUAL(5.0f, sString.GetFloat());

		sString = L"5.0";
		CHECK_EQUAL(5.0f, sString.GetFloat());

		sString = L"5.5";
		CHECK_EQUAL(5.5f, sString.GetFloat());

		sString = L"\u672c";
		CHECK_EQUAL(0.0f, sString.GetFloat());
	}

	TEST_FIXTURE(ConstructTest, GetDouble_ASCII) {
		sString = "";
		CHECK(!(sString.GetDouble()));

		sString = "0";
		CHECK(!(sString.GetDouble()));

		sString = "5";
		CHECK_EQUAL(5.0, sString.GetDouble());

		sString = "5.0";
		CHECK_EQUAL(5.0, sString.GetDouble());

		sString = "5.5";
		CHECK_EQUAL(5.5, sString.GetDouble());
	}

	TEST_FIXTURE(ConstructTest, GetDouble_Unicode) {
		sString = L"";
		CHECK(!(sString.GetDouble()));

		sString = L"0";
		CHECK(!(sString.GetDouble()));

		sString = L"5";
		CHECK_EQUAL(5.0, sString.GetDouble());

		sString = L"5.0";
		CHECK_EQUAL(5.0, sString.GetDouble());

		sString = L"5.5";
		CHECK_EQUAL(5.5, sString.GetDouble());

		sString = L"\u672c";
		CHECK_EQUAL(0.0, sString.GetDouble());
	}

	TEST_FIXTURE(ConstructTest, BoolValue) {
		sString = false;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = true;
		CHECK_EQUAL("1", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, CharValue) {
		sString = 'a';
		CHECK_EQUAL("a", sString.GetASCII());

		sString = static_cast<char>(65);
		CHECK_EQUAL("A", sString.GetASCII());

		sString = 'A';
		CHECK_EQUAL("A", sString.GetASCII());

		sString = '\0';
		CHECK_EQUAL("", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Wchar_tValue) {
		sString = L'a';
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString = static_cast<wchar_t>(65);
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L'A';
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L'\0';
		CHECK_EQUAL(L"", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, IntValue) {
		sString = 0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = -0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = 65;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = -65;
		CHECK_EQUAL("-65", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, FloatValue) {
		sString = 0.0f;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = -0.0f;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = 65.0f;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = 65.1001f;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = -65.0f;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = -65.1001f;
		CHECK_EQUAL("-65.1001", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, DoubleValue) {
		sString = 0.0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = -0.0;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = 65.0;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = 65.1001;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = -65.0;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = -65.1001;
		CHECK_EQUAL("-65.1001", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Bool) {
		sString = "";
		sString = sString + false;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = sString + true;
		CHECK_EQUAL("1", sString.GetASCII());

		sString = sString + false;
		CHECK_EQUAL("10", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Char) {
		sString = "";
		sString = sString + 'a';
		CHECK_EQUAL("a", sString.GetASCII());

		sString = "";
		sString = sString + static_cast<char>(65);
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "";
		sString = sString + 'A';
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "";
		sString = sString + '\0';
		CHECK_EQUAL("", sString.GetASCII());

		sString = sString + 'a';
		CHECK_EQUAL("a", sString.GetASCII());

		sString = sString + 'b';
		CHECK_EQUAL("ab", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Wchar_t) {
		sString = L"";
		sString = sString + L'a';
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString = L"";
		sString = sString + static_cast<wchar_t>(65);
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"";
		sString = sString + L'A';
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"";
		sString = sString + L'\0';
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = sString + L'a';
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString = sString + L'b';
		CHECK_EQUAL(L"ab", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Int) {
		sString = "";
		sString = sString + 0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = sString + -0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = sString + 65;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString = sString + -65;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = sString + 1;
		CHECK_EQUAL("-651", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Float) {
		sString = "";
		sString = sString + 0.0f;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = sString + -0.0f;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = "";
		sString = sString + 65.0f;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString = sString + 65.1001f;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = "";
		sString = sString + -65.0f;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = "";
		sString = sString + -65.1001f;
		CHECK_EQUAL("-65.1001", sString.GetASCII());

		sString = sString + 9.0f;
		CHECK_EQUAL("-65.10019", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Double) {
		sString = "";
		sString = sString + 0.0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = sString + -0.0;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = "";
		sString = sString + 65.0;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString = sString + 65.1001;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = "";
		sString = sString + -65.0;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = "";
		sString = sString + -65.1001;
		CHECK_EQUAL("-65.1001", sString.GetASCII());

		sString = sString + 9.0;
		CHECK_EQUAL("-65.10019", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Bool_left_hand) {
		sString = "";
		sString = false + sString;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = true + sString;
		CHECK_EQUAL("1", sString.GetASCII());

		sString = false + sString;
		CHECK_EQUAL("01", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Char_left_hand) {
		sString = "";
		sString = 'a' + sString;
		CHECK_EQUAL("a", sString.GetASCII());

		sString = "";
		sString = static_cast<char>(65) + sString;
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "";
		sString = 'A' + sString;
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "";
		sString = '\0' + sString;
		CHECK_EQUAL("", sString.GetASCII());

		sString = 'a' + sString;
		CHECK_EQUAL("a", sString.GetASCII());

		sString = 'b' + sString;
		CHECK_EQUAL("ba", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Wchar_t_left_hand) {
		sString = L"";
		sString = L'a' + sString;
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString = L"";
		sString = static_cast<wchar_t>(65) + sString;
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"";
		sString = L'A' + sString;
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"";
		sString = L'\0' + sString;
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString = L'a' + sString;
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString = L'b' + sString;
		CHECK_EQUAL(L"ba", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Int_left_hand) {
		sString = "";
		sString = 0 + sString;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = -0 + sString;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = 65 + sString;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString = -65 + sString;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = 1 + sString;
		CHECK_EQUAL("1-65", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Float_left_hand) {
		sString = "";
		sString = 0.0f + sString;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = -0.0f + sString;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = "";
		sString = 65.0f + sString;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString = 65.1001f + sString;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = "";
		sString = -65.0f + sString;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = "";
		sString = -65.1001f + sString;
		CHECK_EQUAL("-65.1001", sString.GetASCII());

		sString = 9.0f + sString;
		CHECK_EQUAL("9-65.1001", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Double_left_hand) {
		sString = "";
		sString = 0.0 + sString;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString = -0.0 + sString;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = "";
		sString = 65.0 + sString;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString = 65.1001 + sString;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = "";
		sString = -65.0 + sString;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = "";
		sString = -65.1001 + sString;
		CHECK_EQUAL("-65.1001", sString.GetASCII());

		sString = 9.0 + sString;
		CHECK_EQUAL("9-65.1001", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Bool) {
		sString = "";
		sString += false;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString += true;
		CHECK_EQUAL("1", sString.GetASCII());

		sString += false;
		CHECK_EQUAL("10", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Char) {
		sString = "";
		sString += 'a';
		CHECK_EQUAL("a", sString.GetASCII());

		sString = "";
		sString += static_cast<char>(65);
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "";
		sString += 'A';
		CHECK_EQUAL("A", sString.GetASCII());

		sString = "";
		sString += '\0';
		CHECK_EQUAL("", sString.GetASCII());

		sString += 'a';
		CHECK_EQUAL("a", sString.GetASCII());

		sString += 'b';
		CHECK_EQUAL("ab", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Wchar_t) {
		sString = L"";
		sString += L'a';
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString = L"";
		sString += static_cast<wchar_t>(65);
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"";
		sString += L'A';
		CHECK_EQUAL(L"A", sString.GetUnicode());

		sString = L"";
		sString += L'\0';
		CHECK_EQUAL(L"", sString.GetUnicode());

		sString += L'a';
		CHECK_EQUAL(L"a", sString.GetUnicode());

		sString += L'b';
		CHECK_EQUAL(L"ab", sString.GetUnicode());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Int) {
		sString = "";
		sString += 0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString += -0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString += 65;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString += -65;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString += 1;
		CHECK_EQUAL("-651", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Float) {
		sString = "";
		sString += 0.0f;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString += -0.0f;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = "";
		sString += 65.0f;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString += 65.1001f;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = "";
		sString += -65.0f;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = "";
		sString += -65.1001f;
		CHECK_EQUAL("-65.1001", sString.GetASCII());

		sString += 9.0f;
		CHECK_EQUAL("-65.10019", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Equals_Double) {
		sString = "";
		sString += 0.0;
		CHECK_EQUAL("0", sString.GetASCII());

		sString = "";
		sString += -0.0;
		CHECK_EQUAL("-0", sString.GetASCII());

		sString = "";
		sString += 65.0;
		CHECK_EQUAL("65", sString.GetASCII());

		sString = "";
		sString += 65.1001;
		CHECK_EQUAL("65.1001", sString.GetASCII());

		sString = "";
		sString += -65.0;
		CHECK_EQUAL("-65", sString.GetASCII());

		sString = "";
		sString += -65.1001;
		CHECK_EQUAL("-65.1001", sString.GetASCII());

		sString += 9.0;
		CHECK_EQUAL("-65.10019", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Operator_Add_Mixed_Types) {
		sString = "MyString";
		sString = 5 + sString + '_' + true + "HeHo" + 55.9;
		CHECK_EQUAL("5MyString_1HeHo55.9", sString.GetASCII());
	}

	TEST_FIXTURE(ConstructTest, Special_Test_Format_g_5_12f) {
		CHECK_EQUAL("5.12", String::Format("%g", 5.12f).GetASCII());

		PL_WARNING_PUSH
			PL_WARNING_DISABLE(4996)    // "warning C4996: 'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup. See online help for details."
			// Now, change the locale...
			char *pLocalSave = strdup(setlocale(LC_ALL, nullptr));
		PL_WARNING_POP

		// Get the current set locale, we REALLY need to backup the locale because it "may" be changed by "setlocale"
		#ifdef LINUX
			setlocale(LC_ALL, "de_DE"); // Set another local, now normally a ',' instead of '.'
		#else
			setlocale(LC_ALL, "German"); // Set another local, now normally a ',' instead of '.'
		#endif
									 // is used by printf and so on... (but our string class ignores that :)
		CHECK_EQUAL("5.12", String::Format("%g", 5.12f).GetASCII());

		setlocale(LC_ALL, pLocalSave); // Reset the local
		free(pLocalSave);
	} 

	TEST_FIXTURE(ConstructTest, Special_Test_String_5_12_GetFloat) {
		float fValue = String("5.12").GetFloat();
		CHECK_CLOSE(5.12, fValue, 0.00001);
		
		PL_WARNING_PUSH
			PL_WARNING_DISABLE(4996)    // "warning C4996: 'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup. See online help for details."
			// Now, change the locale...
			strdup(setlocale(LC_ALL, nullptr));
		PL_WARNING_POP

		// Get the current set locale, we REALLY need to backup the locale because it "may" be changed by "setlocale"
		#ifdef LINUX
			setlocale(LC_ALL, "de_DE"); // Set another local, now normally a ',' instead of '.'
		#else
			setlocale(LC_ALL, "German"); // Set another local, now normally a ',' instead of '.'
		#endif
									 // is used by printf and so on... (but our string class ignores that :)
		fValue = String("5,12").GetFloat();
		CHECK_CLOSE(5.0f, fValue, 0.00001);
	}
	
	TEST_FIXTURE(ConstructTest, Special_Test_String_5_12_GetDouble) {
		double dValue = String("5.12").GetDouble();
		CHECK_CLOSE(5.12, dValue, 0.00001);
		
		PL_WARNING_PUSH
			PL_WARNING_DISABLE(4996)    // "warning C4996: 'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup. See online help for details."
			// Now, change the locale...
			strdup(setlocale(LC_ALL, nullptr));
		PL_WARNING_POP

		// Get the current set locale, we REALLY need to backup the locale because it "may" be changed by "setlocale"
		#ifdef LINUX
			setlocale(LC_ALL, "de_DE"); // Set another local, now normally a ',' instead of '.'
		#else
			setlocale(LC_ALL, "German"); // Set another local, now normally a ',' instead of '.'
		#endif
									 // is used by printf and so on... (but our string class ignores that :)
		dValue = String("5,12").GetDouble();
		CHECK_CLOSE(5.0, dValue, 0.00001);
	}
}
