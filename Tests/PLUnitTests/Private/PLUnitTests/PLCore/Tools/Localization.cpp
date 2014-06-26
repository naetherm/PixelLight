//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/Tools/Localization.h>
#include <PLCore/Tools/LocalizationText.h>
#include <PLCore/Tools/LocalizationGroup.h>
#include <PLCore/String/String.h>
#include "UnitTest++AddIns/wchar_template.h"
#include "UnitTest++AddIns/PLCheckMacros.h"
#include "UnitTest++AddIns/PLChecks.h"


//[-------------------------------------------------------]
//[ Namespaces                                            ]
//[-------------------------------------------------------]
using namespace PLCore;


/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Localization) {
	/*
	* Naming Convention for METHOD:
	* METHODNAME_SCENARIO
	*/

	// Our Array Test Fixture :)
	struct ConstructTest
	{
		ConstructTest() :
	pLoc(Localization::GetInstance())
	{
		/* some setup */
	}
	~ConstructTest() {
		/* some teardown */
	}

	// Container for testing
	Localization *pLoc;
	};

	/*
	* Tests migrated from PLTestBase
	*/
	TEST_FIXTURE(ConstructTest, Translation) {
		// AddGroup(const String &sName)
		LocalizationGroup *pGroup = pLoc->AddGroup("LocalizationTest");

		// Set from & to group language
		pGroup->SetFromLanguage("English");
		pGroup->SetToLanguage("Deutsch");
		CHECK_EQUAL("English", pGroup->GetFromLanguage());
		CHECK_EQUAL("Deutsch", pGroup->GetToLanguage());

		// LocalizationGroup::AddText(const String &sName, const String &sTranslation)
		LocalizationText *pText1 = pGroup->AddText("Hello", "Hallo");
		LocalizationText *pText2 = pGroup->AddText("World", "Welt");
		LocalizationText *pText3 = pGroup->AddText("Nr.%d, The name: %s", "Nr.%d, Der Name: %s");
		LocalizationText *pTextT = pGroup->AddText("World", "Welt");

		CHECK_EQUAL(static_cast<uint32>(3), pGroup->GetNumOfTexts());
		CHECK(!pTextT);
		CHECK(pText1);
		CHECK(pText2);
		CHECK(pText3);
		CHECK_EQUAL("Hello", pText1->GetName());
		CHECK_EQUAL("Hallo", pText1->GetTranslation());
		CHECK_EQUAL("World", pText2->GetName());
		CHECK_EQUAL("Welt", pText2->GetTranslation());
		CHECK_EQUAL("Nr.%d, The name: %s", pText3->GetName());
		CHECK_EQUAL("Nr.%d, Der Name: %s", pText3->GetTranslation());

		// LocalizationGroup::Save(const String &sFilename)
		CHECK(pGroup->SaveByFilename(pGroup->GetName() + ".loc"));

		// LocalizationGroup::Load(const String &sFilename)
		CHECK(pGroup->LoadByFilename(pGroup->GetName() + ".loc"));
	}

	TEST_FIXTURE(ConstructTest, Get) {
		Localization *pLoc = Localization::GetInstance();

		// GetGroup(const String &sName)
		LocalizationGroup *pGroup = pLoc->GetGroup("LocalizationTest");
		CHECK_EQUAL(static_cast<uint32>(1), pLoc->GetNumOfGroups());
		CHECK(pGroup);
		CHECK_EQUAL("LocalizationTest",  pGroup->GetName());

		// Get(const String &sText, const String &sGroup)
		CHECK_EQUAL(pLoc->Get("Test").GetASCII(), "Test");
		CHECK_EQUAL(pLoc->Get("Hello").GetASCII(), "Hello");
		CHECK_EQUAL(pLoc->Get("Test", "LocalizationTest").GetASCII(), "Test");
		CHECK_EQUAL(pLoc->Get("Hello", "LocalizationTest").GetASCII(), "Hallo");
		CHECK_EQUAL(pLoc->Get("World", "LocalizationTest").GetASCII(), "Welt");

		// PLT(Text)
		CHECK_EQUAL(PLT("Test").GetASCII(), "Test");
		CHECK_EQUAL(PLT("Hello").GetASCII(), "Hello");
		CHECK_EQUAL(PLT("Nr.%d, The name: %s").GetASCII(), "Nr.%d, The name: %s");
		String sFormatTest = String::Format(PLT("Nr.%d, The name: %s").GetUnicode(), 1, L"Bob");
		CHECK_EQUAL(sFormatTest.GetASCII(), "Nr.1, The name: Bob");

		// PL_TEXT(Text, Group)
		CHECK_EQUAL(PL_TEXT("Test", "HeHo").GetASCII(), "Test");
		CHECK_EQUAL(PL_TEXT("Hello", "HeHo").GetASCII(), "Hello");
		CHECK_EQUAL(PL_TEXT("Test", "LocalizationTest").GetASCII(), "Test");
		CHECK_EQUAL(PL_TEXT("Hello", "LocalizationTest").GetASCII(), "Hallo");
		CHECK_EQUAL(PL_TEXT("World", "LocalizationTest").GetASCII(), "Welt");
	}
}