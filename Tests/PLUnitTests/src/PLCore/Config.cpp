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
		pl_class_def()
			// Attributes
			pl_attribute_directvalue(Detail,	float,	1.0f,	ReadWrite)
			pl_attribute_directvalue(ShowFPS,	bool,	false,	ReadWrite)
		pl_class_def_end


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
		pl_class_def()
			// Attributes
			pl_attribute_directvalue(Players,	int,	1,	ReadWrite)
		pl_class_def_end


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
	pl_class_metadata(TestConfigTest, "", PLCore::ConfigGroup, "Configuration test class")
		// Constructors
		pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
		// Attributes
		pl_attribute_metadata(Detail,	float,	1.0f,	ReadWrite,	"Detail",					"")
		pl_attribute_metadata(ShowFPS,	bool,	false,	ReadWrite,	"Show Frames Per Second?",	"")
	pl_class_metadata_end(TestConfigTest)
		
	
	pl_class_metadata(TestConfigUser, "", PLCore::ConfigGroup, "Configuration user class")
		// Constructors
		pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
		// Attributes
		pl_attribute_metadata(Players,	int,	1,	ReadWrite,	"Number of players",	"")
	pl_class_metadata_end(TestConfigUser)


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