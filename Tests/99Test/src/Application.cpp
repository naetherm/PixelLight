/*********************************************************\
 *  File: Application.cpp                                *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Log/Log.h>
#include <PLCore/Xml/Xml.h>
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/Base/ClassManager.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Base/Module.h>
#include <PLCore/Base/Var/Var.h>
#include <PLCore/Base/Func/Func.h>
#include <PLCore/Base/Func/Functor.h>
#include <PLCore/Base/Func/FuncConstructor.h>
#include <PLCore/Base/Event/Event.h>
#include <PLCore/Base/Event/EventHandler.h>
#include <PLCore/Base/Tools/ParamsParser.h>
#include <PLCore/Base/Tools/ParamsParserXml.h>
#include <PLCore/Base/Tools/TypeTraits.h>
#include <PLCore/Tools/Localization.h>
#include "TestObject.h"
#include "RttiObject.h"
#include "TrollType.h"
#include "Application.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Application)


//[-------------------------------------------------------]
//[ Test classes                                          ]
//[-------------------------------------------------------]
class A
{
};

class B : public A
{
};

// Sample enum type
enum EEnum { Value1, Value2, Value3 };
pl_enum(EEnum)
	pl_enum_value(Value1, "")
	pl_enum_value(Value2, "")
	pl_enum_value(Value3, "")
pl_enum_end


//[-------------------------------------------------------]
//[ Static functions                                      ]
//[-------------------------------------------------------]
int TestFunc(int a) {
	System::GetInstance()->GetConsole().Print(String("CALLED TestFunc(") + a + ")\n");
	return a*2;
}

void TestFunc2(int a, float f) {
	System::GetInstance()->GetConsole().Print(String("CALLED TestFunc2(") + a + ", " + f + ")\n");
}

void TestHandler(int a, int b) {
	System::GetInstance()->GetConsole().Print(String("EVENT(") + a + ", " + b + ")\n");
}

void TestHandler2(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o) {
	int num = a+b+c+d+e+f+g+h+i+j+k+l+m+n+o;
	System::GetInstance()->GetConsole().Print(String("EVENT2(") + num + ")\n");
}

void TestHandler3() {
	System::GetInstance()->GetConsole().Print("EVENT()\n");
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application::Application() : PLCore::CoreApplication()
{
	// Set application name and title
	SetName("99Test");
	SetTitle(PLT("PLSample 99 - Test"));
}

/**
*  @brief
*    Destructor
*/
Application::~Application()
{
}

/**
*  @brief
*    Test RTTI functionality
*/
void Application::TestModules()
{
	// Test RTTI: List modules
	System::GetInstance()->GetConsole().Print("PLCore: Modules\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Get module list
	System::GetInstance()->GetConsole().Print("List of modules:\n");
	const List<const PLCore::Module *> &lstModules = PLCore::ClassManager::GetInstance()->GetModules();
	for (uint32 i=0; i<lstModules.GetNumOfElements(); i++) {
		// Get module
		const PLCore::Module *pModule = lstModules[i];

		// Print module info
		System::GetInstance()->GetConsole().Print("- " + pModule->GetName() + '\n');
		System::GetInstance()->GetConsole().Print("  - Description: " + pModule->GetDescription() + '\n');
		System::GetInstance()->GetConsole().Print("  - Vendor:      " + pModule->GetVendor() + '\n');
		System::GetInstance()->GetConsole().Print("  - License:     " + pModule->GetLicense() + '\n');
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Get current module
	System::GetInstance()->GetConsole().Print("Current module:\n");
	const PLCore::Module *pModule = PLCore::ClassManager::GetInstance()->GetModuleByID(pl_current_module_id());
	System::GetInstance()->GetConsole().Print("- " + pModule->GetName() + '\n');
	System::GetInstance()->GetConsole().Print("  - Description: " + pModule->GetDescription() + '\n');
	System::GetInstance()->GetConsole().Print("  - Vendor:      " + pModule->GetVendor() + '\n');
	System::GetInstance()->GetConsole().Print("  - License:     " + pModule->GetLicense() + '\n');

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestClasses()
{
	// Test RTTI: List classes
	System::GetInstance()->GetConsole().Print("PLCore: Classes\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// List all classes
	{
		System::GetInstance()->GetConsole().Print("All classes:\n");
		const List<const PLCore::Class *> &lstClasses = PLCore::ClassManager::GetInstance()->GetClasses();
		for (uint32 i=0; i<lstClasses.GetNumOfElements(); i++) {
			// Get class
			const PLCore::Class *pClass = lstClasses[i];

			// Print class info
			System::GetInstance()->GetConsole().Print("- " + pClass->GetClassName() + " -> " + pClass->GetBaseClassName() + '\n');
		}
		System::GetInstance()->GetConsole().Print('\n');
	}

	// List classes derived from PLCore::LoaderImpl
	{
		System::GetInstance()->GetConsole().Print("All classes derived from PLCore::LoaderImpl:\n");
		List<const PLCore::Class *> lstClasses;
		PLCore::ClassManager::GetInstance()->GetClasses(lstClasses, "PLCore::LoaderImpl", PLCore::Recursive, PLCore::IncludeBase, PLCore::IncludeAbstract);
		for (uint32 i=0; i<lstClasses.GetNumOfElements(); i++) {
			// Get class
			const PLCore::Class *pClass = lstClasses[i];

			// Print class info
			System::GetInstance()->GetConsole().Print("- " + pClass->GetClassName() + " -> " + pClass->GetBaseClassName() + '\n');
		}
		System::GetInstance()->GetConsole().Print('\n');
	}

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestProperties()
{
	// Test RTTI: List classes
	System::GetInstance()->GetConsole().Print("PLCore: Properties\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Get class list
	const List<const PLCore::Class *> &lstClasses = PLCore::ClassManager::GetInstance()->GetClasses();
	for (uint32 i=0; i<lstClasses.GetNumOfElements(); i++) {
		// Get class
		const PLCore::Class *pClass = lstClasses[i];

		// Print class info
		System::GetInstance()->GetConsole().Print("Class " + pClass->GetClassName() + ":\n");

		// Get properties
		Iterator<String> cIterator = pClass->GetProperties().GetKeyIterator();
		while (cIterator.HasNext()) {
			String sName  = cIterator.Next();
			String sValue = pClass->GetProperties().Get(sName);
			System::GetInstance()->GetConsole().Print("- '" + sName + "' = '" + sValue + "'\n");
		}
		System::GetInstance()->GetConsole().Print('\n');
	}

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestParameters()
{
	// Test RTTI: Test parameters
	System::GetInstance()->GetConsole().Print("PLCore: Parameters\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// String
	String sInput = "one=eins	two=2   three=3.0 four=\"yon\" five='V'";
	System::GetInstance()->GetConsole().Print("ParseString(\"" + sInput + "\")\n");
	PLCore::ParamsParser cParams;
	cParams.ParseString(sInput);
	while (cParams.HasParam()) {
		System::GetInstance()->GetConsole().Print("- " + cParams.GetName() + " = '" + cParams.GetValue() + "'\n");
		cParams.Next();
	}
	System::GetInstance()->GetConsole().Print('\n');

	// XML
	sInput = "<?xml version=\"1.0\" ?><Call Param0=\"1\" Param1=\"2.5\"/>";
	System::GetInstance()->GetConsole().Print("ParseXml(\"" + sInput + "\")\n");
	XmlDocument cXml;
	cXml.Parse(sInput);
	PLCore::ParamsParserXml cParams2;
	cParams2.ParseXml(*(XmlElement*)cXml.GetFirstChildElement());
	while (cParams2.HasParam()) {
		System::GetInstance()->GetConsole().Print("- " + cParams2.GetName() + " = '" + cParams2.GetValue() + "'\n");
		cParams2.Next();
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestVars()
{
	// Test RTTI: Test vars
	System::GetInstance()->GetConsole().Print("PLCore: Vars\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Int Value = 10
	System::GetInstance()->GetConsole().Print("int Value = 10\n");
	PLCore::Var<int> AttrInt(0);
	AttrInt.SetInt(10);
	System::GetInstance()->GetConsole().Print("- Type = '" + AttrInt.GetTypeName() + "'\n");
	System::GetInstance()->GetConsole().Print("- Value[bool]   = " + String() + AttrInt.GetBool()   + '\n');
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrInt.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[float]  = " + String() + AttrInt.GetFloat()  + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrInt.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Int Value = 12.5f
	System::GetInstance()->GetConsole().Print("int Value = 12.5f\n");
	AttrInt.SetFloat(12.5f);
	System::GetInstance()->GetConsole().Print("- Type = '" + AttrInt.GetTypeName() + "'\n");
	System::GetInstance()->GetConsole().Print("- Value[bool]   = " + String() + AttrInt.GetBool()   + '\n');
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrInt.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[float]  = " + String() + AttrInt.GetFloat()  + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrInt.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Int Value = "101"
	System::GetInstance()->GetConsole().Print("int Value = \"101\"\n");
	AttrInt.SetString("101");
	System::GetInstance()->GetConsole().Print("- Type = '" + AttrInt.GetTypeName() + "'\n");
	System::GetInstance()->GetConsole().Print("- Value[bool]   = " + String() + AttrInt.GetBool()   + '\n');
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrInt.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[float]  = " + String() + AttrInt.GetFloat()  + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrInt.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// troll Value = 101
	System::GetInstance()->GetConsole().Print("troll Value = \"101\"\n");
	PLCore::Var<TrollType> AttrTroll = TrollType();
	AttrTroll.SetString("101");
	System::GetInstance()->GetConsole().Print("- Type = '" + AttrTroll.GetTypeName() + "'\n");
	System::GetInstance()->GetConsole().Print("- Value[bool]   = " + String() + AttrTroll.GetBool()   + '\n');
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrTroll.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[float]  = " + String() + AttrTroll.GetFloat()  + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrTroll.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// TestEnum Value = 0
	System::GetInstance()->GetConsole().Print("TestEnum Value = 0\n");
	PLCore::Var< pl_enum_type(TestEnum) > AttrEnum(Zero);
	System::GetInstance()->GetConsole().Print("- Type = '" + AttrEnum.GetTypeName() + "'\n");
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrEnum.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrEnum.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// TestEnum Value = 1
	System::GetInstance()->GetConsole().Print("TestEnum Value = 1\n");
	AttrEnum = (TestEnum)1;
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrEnum.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrEnum.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// TestEnum Value = 2
	System::GetInstance()->GetConsole().Print("TestEnum Value = 2\n");
	AttrEnum = (TestEnum)2;
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrEnum.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrEnum.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// TestEnum Value = 3
	System::GetInstance()->GetConsole().Print("TestEnum Value = 3\n");
	AttrEnum = (TestEnum)3;
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrEnum.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrEnum.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// TestEnum Value = 4
	System::GetInstance()->GetConsole().Print("TestEnum Value = 4\n");
	AttrEnum = (TestEnum)4;
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrEnum.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrEnum.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// TestEnum Value = 'Two'
	System::GetInstance()->GetConsole().Print("TestEnum Value = \"Two\"\n");
	AttrEnum.SetString("Two");
	System::GetInstance()->GetConsole().Print("- Value[int]    = " + String() + AttrEnum.GetInt()    + '\n');
	System::GetInstance()->GetConsole().Print("- Value[string] = " + String() + AttrEnum.GetString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Default values
	PLCore::Var<int> cVar(12);
	System::GetInstance()->GetConsole().Print("Var<int>:\n");
	System::GetInstance()->GetConsole().Print("- Default (direct ): '" + String() + cVar.Default() + "'\n");
	System::GetInstance()->GetConsole().Print("- Default (dynamic): '" + cVar.GetDefault() + "'\n");
	System::GetInstance()->GetConsole().Print("- cVar: '" + cVar.GetString() + "'\n");
	cVar = 1;
	System::GetInstance()->GetConsole().Print("- cVar = 1: '" + cVar.GetString() + "'\n");
	cVar.SetDefault();
	System::GetInstance()->GetConsole().Print("- cVar.SetDefault(): '" + cVar.GetString() + "'\n");
	System::GetInstance()->GetConsole().Print('\n');

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestFuncs()
{
	// Test RTTI: Test funcs
	System::GetInstance()->GetConsole().Print("PLCore: Funcs\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Create functoids and functors
	PLCore::Func<int, int> MyFunc;
	PLCore::Func<void, int> MyFunc2;
	PLCore::FuncConstructor<RttiObject> MyCreate;
	PLCore::FuncConstructor<RttiObject, int> MyCreate2;

	// List signatures
	System::GetInstance()->GetConsole().Print("Signatures:\n");
	PLCore::DynFunc *pFunc = &MyFunc;
	System::GetInstance()->GetConsole().Print("- MyFunc    [int(int)]:     " + pFunc->GetSignature() + '\n');
	pFunc = &MyFunc2;
	System::GetInstance()->GetConsole().Print("- MyFunc2   [void(int)]:    " + pFunc->GetSignature() + '\n');
	pFunc = &MyCreate;
	System::GetInstance()->GetConsole().Print("- MyCreate  [Object*()]:    " + pFunc->GetSignature() + '\n');
	pFunc = &MyCreate2;
	System::GetInstance()->GetConsole().Print("- MyCreate2 [Object*(int)]: " + pFunc->GetSignature() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Calling functions
	System::GetInstance()->GetConsole().Print("Calling functions:\n");

	System::GetInstance()->GetConsole().Print("- MyFunc(10): (empty functoid) \n");
	pFunc = &MyFunc;
	pFunc->Call(PLCore::Params<int, int>(10));

	System::GetInstance()->GetConsole().Print("- MyFunc2(10): (empty functoid) \n");
	pFunc = &MyFunc2;
	pFunc->Call(PLCore::Params<void, int>(10));

	System::GetInstance()->GetConsole().Print("- MyFunc2(10): (wrong parameters!)\n");
	pFunc = &MyFunc2;
	pFunc->Call(PLCore::Params<int, int>(10));	// Wrong parameter types!

	System::GetInstance()->GetConsole().Print('\n');

	// Clone constructor
	System::GetInstance()->GetConsole().Print("Cloning MyCreate2:\n");
	PLCore::FuncConstructor<RttiObject, int> *pMyCreate3 = (PLCore::FuncConstructor<RttiObject, int>*)MyCreate2.Clone();
	if (pMyCreate3) {
		// Display signature of cloned constructor
		System::GetInstance()->GetConsole().Print("- MyCreate3 [Object*(int)]: " + pMyCreate3->GetSignature() + '\n');

		// Delete cloned constructor
		delete pMyCreate3;
	} else {
		System::GetInstance()->GetConsole().Print("- ERROR: Could not create clone!\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Test functors
	System::GetInstance()->GetConsole().Print("Testing functors:\n");

	PLCore::Functor<int, int> cFunc(&TestFunc);
	System::GetInstance()->GetConsole().Print(String() + "- cFunc(10): " + cFunc(10) + " [20]\n");

	PLCore::Params<int, int> cParams = PLCore::Params<int, int>::FromString("a=23");
	cFunc.Call(cParams);
	System::GetInstance()->GetConsole().Print(String() + "- cFunc(23): " + cParams.Return + " [46]\n");

	PLCore::Functor<void, int, float> cFunc2(&TestFunc2);
	System::GetInstance()->GetConsole().Print("- cFunc(42, 3.1415): ");
	cFunc2.Call("MyFirstParameter='42' MySecondParameter='3.1415'");

	System::GetInstance()->GetConsole().Print('\n');

	// Test constructors
	System::GetInstance()->GetConsole().Print("Testing constructors:\n");

	System::GetInstance()->GetConsole().Print("- MyCreate: ");
	RttiObject *pObject = static_cast<RttiObject*>(MyCreate());
	if (pObject) System::GetInstance()->GetConsole().Print('\n');
	else		 System::GetInstance()->GetConsole().Print("ERROR: Could not create object!\n");

	System::GetInstance()->GetConsole().Print("- MyCreate2: ");
	pFunc = &MyCreate2;
	PLCore::Params<PLCore::Object*, int> cParams2(10);
	pFunc->Call(cParams2);
	PLCore::Object *pObject2 = cParams2.Return;
	if (pObject2) System::GetInstance()->GetConsole().Print('\n');
	else		  System::GetInstance()->GetConsole().Print("ERROR: Could not create object!\n");
	System::GetInstance()->GetConsole().Print('\n');

	// Delete objects
	System::GetInstance()->GetConsole().Print("Deleting objects:\n");
	delete pObject;
	delete pObject2;
	System::GetInstance()->GetConsole().Print('\n');

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestEvents()
{
	// Test RTTI: Test events
	System::GetInstance()->GetConsole().Print("PLCore: Events\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Declare events
	PLCore::Event<int, int> MyEvent;
	PLCore::Event<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int> MyEvent2;
	PLCore::EventHandler<int, int> MyHandler(&TestHandler);
	PLCore::EventHandler<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int> MyHandler2(&TestHandler2);
	MyEvent.Connect(MyHandler);
	MyEvent2.Connect(MyHandler2);

	// Call events
	System::GetInstance()->GetConsole().Print("Calling events:\n");

	System::GetInstance()->GetConsole().Print("- MyEvent(0, 0):        ");
	MyEvent(0, 0);

	System::GetInstance()->GetConsole().Print("- MyEvent(10, 20):      ");
	MyEvent(10, 20);

	System::GetInstance()->GetConsole().Print("- MyEvent(100, 1):      ");
	MyEvent(100, 1);

	System::GetInstance()->GetConsole().Print("- MyEvent(100, 101):    ");
	MyEvent.Emit(PLCore::Params<void, int, int>(100, 101));

	System::GetInstance()->GetConsole().Print("- MyEvent(100, 101.0f): ");
	MyEvent.Emit(PLCore::Params<void, int, float>(100, 101.0f));
	System::GetInstance()->GetConsole().Print('\n');

	System::GetInstance()->GetConsole().Print("- MyEvent2(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1): ");
	MyEvent2(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

	System::GetInstance()->GetConsole().Print('\n');

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestAttributes()
{
	// Test RTTI: Test vars
	System::GetInstance()->GetConsole().Print("PLCore: Attributes\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

void Application::TestObjects()
{
	// Test RTTI: Test vars
	System::GetInstance()->GetConsole().Print("PLCore: Objects\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Create object
	System::GetInstance()->GetConsole().Print("Creating cObject:\n");
	System::GetInstance()->GetConsole().Print("- ");
	RttiObject cObject;
	System::GetInstance()->GetConsole().Print('\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject
	System::GetInstance()->GetConsole().Print("cObject attributes:\n");
	const Container<PLCore::VarDesc*> &lstAttributes = cObject.GetClass()->GetAttributes();
	for (uint32 i=0; i<lstAttributes.GetNumOfElements(); i++) {
		PLCore::VarDesc *pDesc = lstAttributes[i];
		int    nType = pDesc->GetTypeID();
		String sType = pDesc->GetTypeName();
		String sName = pDesc->GetName();
		String sDesc = pDesc->GetDescription();
		String sAnnt = pDesc->GetAnnotation();
		System::GetInstance()->GetConsole().Print(
			"- Var " + sName + '(' + sType + "): " + sDesc + '\n'
		);
	}
	System::GetInstance()->GetConsole().Print('\n');
	System::GetInstance()->GetConsole().Print("cObject methods:\n");
	const Container<PLCore::FuncDesc*> &lstMethods = cObject.GetClass()->GetMethods();
	for (uint32 i=0; i<lstMethods.GetNumOfElements(); i++) {
		PLCore::FuncDesc *pDesc = lstMethods[i];
		String sSig  = pDesc->GetSignature();
		String sName = pDesc->GetName();
		String sDesc = pDesc->GetDescription();
		String sAnnt = pDesc->GetAnnotation();
		System::GetInstance()->GetConsole().Print(
			"- Method " + sName + '(' + sSig + "): " + sDesc + '\n'
		);
	}
	System::GetInstance()->GetConsole().Print('\n');
	System::GetInstance()->GetConsole().Print("cObject signals:\n");
	const Container<PLCore::EventDesc*> &lstSignals = cObject.GetClass()->GetSignals();
	for (uint32 i=0; i<lstSignals.GetNumOfElements(); i++) {
		PLCore::EventDesc *pDesc = lstSignals[i];
		String sSig  = pDesc->GetSignature();
		String sName = pDesc->GetName();
		String sDesc = pDesc->GetDescription();
		String sAnnt = pDesc->GetAnnotation();
		System::GetInstance()->GetConsole().Print(
			"- Signal " + sName + '(' + sSig + "): " + sDesc + '\n'
		);
	}
	System::GetInstance()->GetConsole().Print('\n');
	System::GetInstance()->GetConsole().Print("cObject slots:\n");
	const Container<PLCore::EventHandlerDesc*> &lstSlots = cObject.GetClass()->GetSlots();
	for (uint32 i=0; i<lstSlots.GetNumOfElements(); i++) {
		PLCore::EventHandlerDesc *pDesc = lstSlots[i];
		String sSig  = pDesc->GetSignature();
		String sName = pDesc->GetName();
		String sDesc = pDesc->GetDescription();
		String sAnnt = pDesc->GetAnnotation();
		System::GetInstance()->GetConsole().Print(
			"- Slot " + sName + '(' + sSig + "): " + sDesc + '\n'
		);
	}
	System::GetInstance()->GetConsole().Print('\n');
	System::GetInstance()->GetConsole().Print("cObject constructors:\n");
	const Container<PLCore::ConstructorDesc*> &lstConstructors = cObject.GetClass()->GetConstructors();
	for (uint32 i=0; i<lstConstructors.GetNumOfElements(); i++) {
		PLCore::ConstructorDesc *pDesc = lstConstructors[i];
		String sSig  = pDesc->GetSignature();
		String sName = pDesc->GetName();
		String sDesc = pDesc->GetDescription();
		String sAnnt = pDesc->GetAnnotation();
		System::GetInstance()->GetConsole().Print(
			"- Constructor " + sName + '(' + sSig + "): " + sDesc + '\n'
		);
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Sizes
	System::GetInstance()->GetConsole().Print("Sizes:\n");
	int nSize = sizeof(int);
	System::GetInstance()->GetConsole().Print("- sizeof(int):      " + String() + (int)nSize + '\n');
	int nSizeValue = sizeof(cObject.IntValue);
	System::GetInstance()->GetConsole().Print("- sizeof(IntValue): " + String() + (int)nSizeValue + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject values
	System::GetInstance()->GetConsole().Print("Values:\n");
	System::GetInstance()->GetConsole().Print("- BoolValue: " + String() + cObject.BoolValue.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue:  " + String() + cObject.IntValue.GetString()  + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue2: " + String() + cObject.IntValue2.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- Number:    " + String() + cObject.Number.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("* " + cObject.GetValues(PLCore::WithDefault) + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject.BoolValue = false
	cObject.BoolValue = false;
	System::GetInstance()->GetConsole().Print("BoolValue = false:\n");
	System::GetInstance()->GetConsole().Print("- BoolValue: " + String() + cObject.BoolValue.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue:  " + String() + cObject.IntValue.GetString()  + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue2: " + String() + cObject.IntValue2.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- Number:    " + String() + cObject.Number.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("* " + cObject.GetValues(PLCore::WithDefault) + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject.IntValue = 100
	cObject.IntValue.Set(100);
	System::GetInstance()->GetConsole().Print("IntValue = 100:\n");
	System::GetInstance()->GetConsole().Print("- BoolValue: " + String() + cObject.BoolValue.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue:  " + String() + cObject.IntValue.GetString()  + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue2: " + String() + cObject.IntValue2.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- Number:    " + String() + cObject.Number.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("* " + cObject.GetValues(PLCore::WithDefault) + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject.IntValue2 = 100
	cObject.IntValue2 = 100;
	System::GetInstance()->GetConsole().Print("IntValue2 = 100:\n");
	System::GetInstance()->GetConsole().Print("- BoolValue: " + String() + cObject.BoolValue.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue:  " + String() + cObject.IntValue.GetString()  + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue2: " + String() + cObject.IntValue2.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- Number:    " + String() + cObject.Number.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("* " + cObject.GetValues(PLCore::WithDefault) + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject.IntValue = 99, cObject.Number = 1
	cObject.SetAttribute("IntValue", "99");
	cObject.Number = RttiObject::Inside1;
	System::GetInstance()->GetConsole().Print("IntValue = 99, cObject.Number = 1:\n");
	System::GetInstance()->GetConsole().Print("- BoolValue: " + String() + cObject.BoolValue.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue:  " + String() + cObject.IntValue.GetString()  + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue2: " + String() + cObject.IntValue2.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- Number:    " + String() + cObject.Number.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("* " + cObject.GetValues(PLCore::WithDefault) + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject.BoolValue = true, cObject.IntValue = 5
	XmlDocument cXmlValues;
	cXmlValues.Parse("<?xml version=\"1.0\" ?><Object BoolValue=\"true\" IntValue=\"5\" Number=\"Three\"/>");
	cObject.SetValuesXml(*(XmlElement*)cXmlValues.GetFirstChildElement());
	System::GetInstance()->GetConsole().Print("BoolValue = true, IntValue = 5, Number = 'Three':\n");
	System::GetInstance()->GetConsole().Print("- BoolValue: " + String() + cObject.BoolValue.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue:  " + String() + cObject.IntValue.GetString()  + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue2: " + String() + cObject.IntValue2.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- Number:    " + String() + cObject.Number.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("* " + cObject.GetValues(PLCore::WithDefault) + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// cObject.BoolValue = false, cObject.IntValue = 7
	cObject.SetValues("BoolValue='false' IntValue='7'");
	System::GetInstance()->GetConsole().Print("BoolValue = true, IntValue = 5:\n");
	System::GetInstance()->GetConsole().Print("- BoolValue: " + String() + cObject.BoolValue.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue:  " + String() + cObject.IntValue.GetString()  + '\n');
	System::GetInstance()->GetConsole().Print("- IntValue2: " + String() + cObject.IntValue2.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("- Number:    " + String() + cObject.Number.GetString() + '\n');
	System::GetInstance()->GetConsole().Print("* " + cObject.GetValues(PLCore::WithDefault) + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Test flags
	System::GetInstance()->GetConsole().Print("Testing flags:\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + '\n');
	cObject.State = 1;
	System::GetInstance()->GetConsole().Print("->  State = 1\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 2;
	System::GetInstance()->GetConsole().Print("->  State = 2\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 3;
	System::GetInstance()->GetConsole().Print("->  State = 3\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 4;
	System::GetInstance()->GetConsole().Print("->  State = 4\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 5;
	System::GetInstance()->GetConsole().Print("->  State = 5\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 6;
	System::GetInstance()->GetConsole().Print("->  State = 6\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 7;
	System::GetInstance()->GetConsole().Print("->  State = 7\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 8;
	System::GetInstance()->GetConsole().Print("->  State = 8\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 9;
	System::GetInstance()->GetConsole().Print("->  State = 9\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State = 10;
	System::GetInstance()->GetConsole().Print("->  State = 10\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State.SetString("Ugly|Big");
	System::GetInstance()->GetConsole().Print("->  State = 'Ugly|Big'\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State.SetString("7");
	System::GetInstance()->GetConsole().Print("->  State = '7'\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State.SetString("Ugly|1|2");
	System::GetInstance()->GetConsole().Print("->  State = 'Ugly|1|2'\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	cObject.State.SetString("Ugly|10");
	System::GetInstance()->GetConsole().Print("->  State = 'Ugly|10'\n");
	System::GetInstance()->GetConsole().Print("- State: " + cObject.State.GetString() + " [" + cObject.State.GetInt() + "]\n");
	System::GetInstance()->GetConsole().Print('\n');

	// Get object attributes as XML
	XmlElement cXmlElement("Object");
	cObject.GetValuesXml(cXmlElement, PLCore::WithDefault);
	System::GetInstance()->GetConsole().Print("Object as XML:\n");
	System::GetInstance()->GetConsole().Print(cXmlElement.ToString() + '\n');
	System::GetInstance()->GetConsole().Print(cObject.ToXml().ToString() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Method: Test()
	System::GetInstance()->GetConsole().Print("Calling MethodTest (7 times):\n");
	cObject.CallMethod("Test", "Param0=200 Param1='0.753'");
	XmlDocument cXmlParams;
	cXmlParams.Parse("<?xml version=\"1.0\" ?><Call Param0=\"1\" Param1=\"2.5\"/>");
	cObject.CallMethod("Test", *(XmlElement*)cXmlParams.GetFirstChildElement());
	System::GetInstance()->GetConsole().Print('\n');

	// Events
		// Connect event handlers
	PLCore::EventHandler<int, int> cEventHandler(&TestHandler);
	cObject.Event1.Connect(cEventHandler);
	PLCore::EventHandler<> cEventHandler2(&TestHandler3);
	cObject.Event0.Connect(cEventHandler2);

		// Get signature
	System::GetInstance()->GetConsole().Print("Checking signature of cObject.Event1 (void(int, int)):\n");
	System::GetInstance()->GetConsole().Print("- " + cObject.GetSignal("Event1")->GetSignature() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

		// Emit directly (accessing the actual Signal)
	System::GetInstance()->GetConsole().Print("Emitting cObject.Event1(100, 200): ");
	cObject.Event1(100, 200);
	System::GetInstance()->GetConsole().Print('\n');

		// Emit dynamically (accessing the signal only indirectly by DynSignal)
	System::GetInstance()->GetConsole().Print("Emitting cObject.Event1(1, 2): ");
	cObject.GetSignal("Event1")->Emit( PLCore::Params<void, int, int>(1, 2) );
	System::GetInstance()->GetConsole().Print('\n');

		// Emit dynamically (accessing the signal only indirectly by DynSignal)
	System::GetInstance()->GetConsole().Print("Emitting cObject.Event0(): ");
	cObject.GetSignal("Event0")->Emit( PLCore::Params<void>() );
	System::GetInstance()->GetConsole().Print('\n');

		// Done
	System::GetInstance()->GetConsole().Print('\n');

	// Call base constructor
	System::GetInstance()->GetConsole().Print("Testing constructors:\n");
	PLCore::Class *pClass = cObject.GetClass();
	PLCore::Object *pObject = nullptr;

	pObject = pClass->Create();
	if (pObject) delete pObject;

	pObject = pClass->Create(PLCore::Params<PLCore::Object*>());
	if (pObject) delete pObject;

	pObject = pClass->Create(PLCore::Params<PLCore::Object*, int>(10));
	if (pObject) delete pObject;

	pObject = pClass->Create(PLCore::Params<PLCore::Object*, int, float>(10, 2.5f));
	if (pObject) delete pObject;

	pObject = pClass->Create(PLCore::Params<PLCore::Object*, int, int>(10, 10));
	if (pObject) delete pObject;

	// Done
	System::GetInstance()->GetConsole().Print("Done.\n\n");
}

void Application::TestEnums()
{
	// Test RTTI: Test vars
	System::GetInstance()->GetConsole().Print("PLCore: Enums\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Show enum values of TestEnum by directly accessing the enum-type
	int nValues = pl_enum_type(TestEnum)::GetNumOfEnumValues();
	System::GetInstance()->GetConsole().Print(String("TestEnum: ") + nValues + " values\n");
	for (int i=0; i<nValues; i++) {
		String sName  = pl_enum_type(TestEnum)::GetEnumName(i);
		String sDesc  = pl_enum_type(TestEnum)::GetEnumDescription(sName);
		int    nValue = pl_enum_type(TestEnum)::GetEnumValue(sName);
		System::GetInstance()->GetConsole().Print("- '" + sName + "' = " + nValue + " [" + sDesc + "]\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Show enum values of TestFlags by directly accessing the enum-type
	nValues = pl_enum_type(Hallo::TestFlags)::GetNumOfEnumValues();
	System::GetInstance()->GetConsole().Print(String("TestFlags: ") + nValues + " values\n");
	for (int i=0; i<nValues; i++) {
		String sName  = pl_enum_type(Hallo::TestFlags)::GetEnumName(i);
		String sDesc  = pl_enum_type(Hallo::TestFlags)::GetEnumDescription(sName);
		int    nValue = pl_enum_type(Hallo::TestFlags)::GetEnumValue(sName);
		System::GetInstance()->GetConsole().Print("- '" + sName + "' = " + nValue + " [" + sDesc + "]\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Show enum values of TestFlags2 by directly accessing the enum-type
	nValues = pl_enum_type(TestFlags2)::GetNumOfEnumValues();
	System::GetInstance()->GetConsole().Print(String("TestFlags2: ") + nValues + " values\n");
	for (int i=0; i<nValues; i++) {
		String sName  = pl_enum_type(TestFlags2)::GetEnumName(i);
		String sDesc  = pl_enum_type(TestFlags2)::GetEnumDescription(sName);
		int    nValue = pl_enum_type(TestFlags2)::GetEnumValue(sName);
		System::GetInstance()->GetConsole().Print("- '" + sName + "' = " + nValue + " [" + sDesc + "]\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Show enum values of FamousNumbers by directly accessing the enum-type
	nValues = pl_enum_type(FamousNumbers)::GetNumOfEnumValues();
	System::GetInstance()->GetConsole().Print(String("FamousNumbers: ") + nValues + " values\n");
	for (int i=0; i<nValues; i++) {
		String sName  = pl_enum_type(FamousNumbers)::GetEnumName(i);
		String sDesc  = pl_enum_type(FamousNumbers)::GetEnumDescription(sName);
		float  fValue = pl_enum_type(FamousNumbers)::GetEnumValue(sName);
		System::GetInstance()->GetConsole().Print("- '" + sName + "' = " + fValue + " [" + sDesc + "]\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Show enum values of TestFlags2 by accessing it's dynamic type info
	PLCore::Var< pl_enum_type(TestFlags2) > cVar;
	System::GetInstance()->GetConsole().Print("TestFlags2: ");
	if (cVar.GetType().IsEnumType()) {
		const List<String> &lstEnums = cVar.GetType().GetEnumValues();
		System::GetInstance()->GetConsole().Print(String() + (int)lstEnums.GetNumOfElements() + " values\n");
		for (uint32 i=0; i<lstEnums.GetNumOfElements(); i++) {
			String sEnum  = lstEnums[i];
			String sValue = cVar.GetType().GetEnumValue(sEnum);
			System::GetInstance()->GetConsole().Print("- '" + sEnum + "' = '" + sValue + "'\n");
		}
	} else {
		System::GetInstance()->GetConsole().Print('\n');
		System::GetInstance()->GetConsole().Print("  - ERROR: This is not a enum type!\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Show enum values of FamousNumbers by accessing it's dynamic type info
	PLCore::Var< pl_enum_type(FamousNumbers) > cVar2;
	System::GetInstance()->GetConsole().Print("FamousNumbers: ");
	if (cVar2.GetType().IsEnumType()) {
		const List<String> &lstEnums = cVar2.GetType().GetEnumValues();
		System::GetInstance()->GetConsole().Print(String() + (int)lstEnums.GetNumOfElements() + " values\n");
		for (uint32 i=0; i<lstEnums.GetNumOfElements(); i++) {
			String sEnum  = lstEnums[i];
			String sValue = cVar2.GetType().GetEnumValue(sEnum);
			System::GetInstance()->GetConsole().Print("- '" + sEnum + "' = '" + sValue + "'\n");
		}
	} else {
		System::GetInstance()->GetConsole().Print('\n');
		System::GetInstance()->GetConsole().Print("  - ERROR: This is not a enum type!\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Set overwritten variables
	/*
	B m_cB;
	m_cB.SetA(1);
	m_cB.SetB(2);
	m_cB.PrintA();
	m_cB.PrintB();
	*/

	// Done
	System::GetInstance()->GetConsole().Print("Done.\n\n");
}

void Application::TestOverwrite()
{
	// Test RTTI: Test vars
	System::GetInstance()->GetConsole().Print("PLCore: Overwriting attributes\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Create object of type "TestClass1"
	PLCore::Object *pObject1 = PLCore::ClassManager::GetInstance()->GetClass("TestClass1")->Create();
	System::GetInstance()->GetConsole().Print("TestClass1 attributes:\n");
	{
		const Container<PLCore::VarDesc*> &lstAttributes = pObject1->GetClass()->GetAttributes();
		for (uint32 i=0; i<lstAttributes.GetNumOfElements(); i++) {
			PLCore::VarDesc *pDesc = lstAttributes[i];
			int    nType = pDesc->GetTypeID();
			String sType = pDesc->GetTypeName();
			String sName = pDesc->GetName();
			String sDesc = pDesc->GetDescription();
			String sAnnt = pDesc->GetAnnotation();
			System::GetInstance()->GetConsole().Print(
				"- Var " + sName + '(' + sType + "): " + sDesc + '\n'
			);
		}
		System::GetInstance()->GetConsole().Print('\n');
	}

	// Show value of pObject1->Float
	System::GetInstance()->GetConsole().Print("TestClass1.Float:\n");
	pObject1->CallMethod("PrintFloat", PLCore::Params<void>());
	System::GetInstance()->GetConsole().Print('\n');

	// Create object of type "TestClass2"
	PLCore::Object *pObject2 = PLCore::ClassManager::GetInstance()->GetClass("TestPlugin::TestClass2")->Create();
	System::GetInstance()->GetConsole().Print("TestClass2 attributes:\n");
	{
		const Container<PLCore::VarDesc*> &lstAttributes = pObject2->GetClass()->GetAttributes();
		for (uint32 i=0; i<lstAttributes.GetNumOfElements(); i++) {
			PLCore::VarDesc *pDesc = lstAttributes[i];
			int    nType = pDesc->GetTypeID();
			String sType = pDesc->GetTypeName();
			String sName = pDesc->GetName();
			String sDesc = pDesc->GetDescription();
			String sAnnt = pDesc->GetAnnotation();
			System::GetInstance()->GetConsole().Print(
				"- Var " + sName + '(' + sType + "): " + sDesc + '\n'
			);
		}
		System::GetInstance()->GetConsole().Print('\n');
	}

	// Show value of pObject1->Float
	System::GetInstance()->GetConsole().Print("TestClass1.Float:\n");
	pObject2->CallMethod("PrintFloat", PLCore::Params<void>());
	System::GetInstance()->GetConsole().Print('\n');

	// Show value of pObject2->Float
	System::GetInstance()->GetConsole().Print("TestClass2.Float:\n");
	String sValue = pObject2->GetAttribute("Float")->GetString();
	System::GetInstance()->GetConsole().Print("- Float = " + sValue + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Show value of pObject2->Float
	System::GetInstance()->GetConsole().Print("TestClass2.Flags:\n");
	pObject2->GetAttribute("Flags")->SetInt(0);
	System::GetInstance()->GetConsole().Print("- Flags = 0: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(1);
	System::GetInstance()->GetConsole().Print("- Flags = 1: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(2);
	System::GetInstance()->GetConsole().Print("- Flags = 2: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(3);
	System::GetInstance()->GetConsole().Print("- Flags = 3: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(4);
	System::GetInstance()->GetConsole().Print("- Flags = 4: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(8);
	System::GetInstance()->GetConsole().Print("- Flags = 8: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(16);
	System::GetInstance()->GetConsole().Print("- Flags = 16: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(129);
	System::GetInstance()->GetConsole().Print("- Flags = 129: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	pObject2->GetAttribute("Flags")->SetInt(255);
	System::GetInstance()->GetConsole().Print("- Flags = 255: '" + pObject2->GetAttribute("Flags")->GetString() + "'\n");
	System::GetInstance()->GetConsole().Print('\n');

	// Done
	System::GetInstance()->GetConsole().Print("Done.\n\n");
}

void Application::TestConvertible()
{
	// Test RTTI: Test vars
	System::GetInstance()->GetConsole().Print("PLCore: Check convertible types\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Check convertible types
	bool bIsConvertible;
	System::GetInstance()->GetConsole().Print("Checking if types are convertible:\n");
	bIsConvertible = PLCore::IsConvertible<int, int>::Value;
	System::GetInstance()->GetConsole().Print("- int -> int:   " + String() + bIsConvertible + '\n');
	bIsConvertible = PLCore::IsConvertible<int, A>::Value;
	System::GetInstance()->GetConsole().Print("- int -> A:     " + String() + bIsConvertible + '\n');
	bIsConvertible = PLCore::IsConvertible<B, float>::Value;
	System::GetInstance()->GetConsole().Print("-   B -> float: " + String() + bIsConvertible + '\n');
	bIsConvertible = PLCore::IsConvertible<A, A>::Value;
	System::GetInstance()->GetConsole().Print("-   A -> A:     " + String() + bIsConvertible + '\n');
	bIsConvertible = PLCore::IsConvertible<A, B>::Value;
	System::GetInstance()->GetConsole().Print("-   A -> B:     " + String() + bIsConvertible + '\n');
	bIsConvertible = PLCore::IsConvertible<B, A>::Value;
	System::GetInstance()->GetConsole().Print("-   B -> A:     " + String() + bIsConvertible + '\n');
	bIsConvertible = PLCore::IsConvertible<B, B>::Value;
	System::GetInstance()->GetConsole().Print("-   B -> B:     " + String() + bIsConvertible + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Check convertible types
	bool bIsBaseClass;
	System::GetInstance()->GetConsole().Print("Checking baseclasses:\n");
	bIsBaseClass = PLCore::IsBaseClass<A, A>::Value;
	System::GetInstance()->GetConsole().Print("-            A <- A         : " + String() + bIsBaseClass + " (false)\n");
	bIsBaseClass = PLCore::IsBaseClass<B, A>::Value;
	System::GetInstance()->GetConsole().Print("-            A <- B         : " + String() + bIsBaseClass + " (true)\n");
	bIsBaseClass = PLCore::IsBaseClass<B, B>::Value;
	System::GetInstance()->GetConsole().Print("-            B <- B         : " + String() + bIsBaseClass + " (false)\n");
	bIsBaseClass = PLCore::IsBaseClass<A, B>::Value;
	System::GetInstance()->GetConsole().Print("-            B <- A         : " + String() + bIsBaseClass + " (false)\n");
	bIsBaseClass = PLCore::IsBaseClass<RttiObject, PLCore::Object>::Value;
	System::GetInstance()->GetConsole().Print("-       Object <- RttiObject: " + String() + bIsBaseClass + " (true)\n");
	bIsBaseClass = PLCore::IsBaseClass<PLCore::Object, RttiObject>::Value;
	System::GetInstance()->GetConsole().Print("-   RttiObject <- Object    : " + String() + bIsBaseClass + " (false)\n");
	bIsBaseClass = PLCore::IsBaseClass<PLCore::Object, PLCore::Object>::Value;
	System::GetInstance()->GetConsole().Print("-       Object <- Object    : " + String() + bIsBaseClass + " (false)\n");
	bIsBaseClass = PLCore::IsBaseClass<RttiObject, RttiObject>::Value;
	System::GetInstance()->GetConsole().Print("-   RttiObject <- RttiObject: " + String() + bIsBaseClass + " (false)\n");
	bIsBaseClass = PLCore::IsBaseClass<PLCore::Object, PLCore::ObjectBase>::Value;
	System::GetInstance()->GetConsole().Print("-   ObjectBase <- Object    : " + String() + bIsBaseClass + " (true)\n");
	System::GetInstance()->GetConsole().Print('\n');

	// Check enum types
	PLCore::Var<EEnum> cVar;
	System::GetInstance()->GetConsole().Print("Checking enum types:\n");
	System::GetInstance()->GetConsole().Print("- Var<EEnum>::Type = '" + cVar.GetType().GetTypeName() + "' (int)\n");
	System::GetInstance()->GetConsole().Print('\n');

	// Check events with enums
	PLCore::Event< pl_enum_type(EEnum) > cEvent;
	PLCore::Event< EEnum > cEvent2;

	// Done
	System::GetInstance()->GetConsole().Print("Done.\n\n");
}

void Application::TestStringRef()
{
	// Test RTTI: Test vars
	System::GetInstance()->GetConsole().Print("PLCore: Testing <String&> arguments\n");
	System::GetInstance()->GetConsole().Print("----------------------------------------\n");

	// Get RTTI class 'RttiObject'
	const PLCore::Class *pClass = PLCore::ClassManager::GetInstance()->GetClass("RttiObject");
	if (pClass) {
	    // Create an instance of the RTTI class
		RttiObject *pObject = static_cast<RttiObject*>(pClass->Create());

	    // Call RTTI method

		// This does (of course) not work:
//		pObject->CallMethod("MyMethod", "Param0=\"Bob\"");

		// This works:
		String sString = "Alice";
		pObject->CallMethod("MyMethod", "Param0=\"" + PLCore::Type<String&>::ConvertToString(sString) + '\"');

		// And this:
		pObject->CallMethod("MyMethod", String("Param0=\"") + (uint32)&sString + '\"');

	    // Cleanup
		delete pObject;
	}

	// Done
	System::GetInstance()->GetConsole().Print("Done.\n\n");
}

void Application::TestDebug()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void Application::Main()
{
	// Get all classes derived from PLCore::Object
	List<const PLCore::Class*> lstClasses;
	PLCore::ClassManager::GetInstance()->GetClasses(lstClasses, "PLCore::Object", PLCore::NonRecursive, PLCore::NoBase, PLCore::IncludeAbstract);

	// Iterate through the found classes
	Iterator<const PLCore::Class*> cIterator = lstClasses.GetIterator();
	while (cIterator.HasNext()) {
		// Get the class
		const PLCore::Class *pClass = cIterator.Next();

		// ...
	}

	// Run test
//	TestModules();
//	TestClasses();
//	TestProperties();
//	TestParameters();
//	TestVars();
//	TestFuncs();
//	TestEvents();
//	TestAttributes();
//	TestObjects();
//	TestEnums();
//	TestOverwrite();
	TestConvertible();
//	TestStringRef();
//	TestDebug();
}
