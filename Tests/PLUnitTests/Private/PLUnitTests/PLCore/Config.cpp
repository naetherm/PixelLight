//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <UnitTest++/UnitTest++.h>
#include <PLCore/System/System.h>
#include <PLMath/Math.h>
#include <PLCore/System/Console.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Config/Config.h>


//[-------------------------------------------------------]
//[ Namespaces                                            ]
//[-------------------------------------------------------]
using namespace PLCore;


/*
* Naming Convention for SUITE:
* CLASSNAME
*/
SUITE(Config) {
	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Configuration test class
	*/
	class TestConfigTest : public ConfigGroup {


		//[-------------------------------------------------------]
		//[ RTTI interface                                        ]
		//[-------------------------------------------------------]
		pl_class(pl_rtti_export, TestConfigTest, "", PLCore::ConfigGroup, "Configuration test class")
			// Attributes
			pl_attribute(Detail,	float,	1.0f,	ReadWrite,	DirectValue,	"Detail",					"")
			pl_attribute(ShowFPS,	bool,	false,	ReadWrite,	DirectValue,	"Show Frames Per Second?",	"")
			// Constructors
			pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
			pl_class_end


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
	public:
		TestConfigTest() :
		Detail(this),
			ShowFPS(this)
		{
		}

		virtual ~TestConfigTest()
		{
		}


	};

	/**
	*  @brief
	*    Configuration user class
	*/
	class TestConfigUser : public ConfigGroup {


		//[-------------------------------------------------------]
		//[ RTTI interface                                        ]
		//[-------------------------------------------------------]
		pl_class(pl_rtti_export, TestConfigUser, "", PLCore::ConfigGroup, "Configuration user class")
			// Attributes
			pl_attribute(Players,	int,	1,	ReadWrite,	DirectValue,	"Number of players",	"")
			// Constructors
			pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
			pl_class_end


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
	public:
		TestConfigUser() :
		Players(this)
		{
		}

		virtual ~TestConfigUser()
		{
		}


	};


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_implement_class(TestConfigTest)
	pl_implement_class(TestConfigUser)


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
		Config cConfig;
	};

	/*
	* Tests migrated from PLTestBase
	*/
	TEST_FIXTURE(ConstructTest, Constructors) {
		/* Show current settings
		System::GetInstance()->GetConsole().Print("\nCurrent Config settings:\n");
		List<const PLCore::Class*> lstClasses;
		PLCore::ClassManager::GetInstance()->GetClasses(lstClasses, "PLCore::ConfigGroup", Recursive, NoBase, NoAbstract);
		// Show current settings
		System::GetInstance()->GetConsole().Print(String("\n------------------------------------------------------------------\n"));
		for (uint32 i=0; i<lstClasses.GetNumOfElements(); i++) {
			const Class *pClass  = lstClasses[i];
			Object      *pObject = cConfig.GetClass(pClass->GetClassName());
			if (pObject)
				System::GetInstance()->GetConsole().Print(String("") + pClass->GetClassName().GetASCII() + " = '" + pObject->GetValues(WithDefault).GetASCII() + "'\n");
		}
		*/

		// Set TestConfigTest variables
		cConfig.SetVar("TestConfigTest", "Detail",  "0.5");
		cConfig.SetVar("TestConfigTest", "ShowFPS", "true");
		CHECK_CLOSE(0.5, cConfig.GetVar("TestConfigTest", "Detail").GetFloat(), PLMath::Math::Epsilon);
		CHECK(cConfig.GetVar("TestConfigTest", "ShowFPS").GetBool());

		// Set TestConfigUser variables
		cConfig.SetVar("TestConfigUser", "Players", "2");
		CHECK_EQUAL(2, cConfig.GetVarInt("TestConfigUser", "Players"));

		// Reset all to default
		cConfig.SetDefault();

		CHECK_CLOSE(1.0, cConfig.GetVar("TestConfigTest", "Detail").GetFloat(), PLMath::Math::Epsilon);
		CHECK(!cConfig.GetVar("TestConfigTest", "ShowFPS").GetBool());
		CHECK_EQUAL(1, cConfig.GetVarInt("TestConfigUser", "Players"));
	}
}