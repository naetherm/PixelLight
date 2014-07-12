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
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Base/ClassManager.h>
#include "Application.h"
#include "MyClass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Application)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application::Application() : CoreApplication()
{
}

/**
*  @brief
*    Destructor
*/
Application::~Application()
{
}

//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void Application::Main()
{
	//// Create an instance of our RTTI-enabled class using a regular construction process
	MyClass *cInstance1 = new MyClass();

	//// We can now access the declared argumets as usual
	//cInstance1->Number = 108;
	//cInstance1->InnerObj = new MyClass();

	//// We can also create an instance of the class if we know its name
	//const Class *cMyClass = ClassManager::GetInstance()->GetClass("MyClass");
	//MyClass *cInstance2 = (MyClass*)cMyClass->Create();

	//// Attributes can be also accessed by name
	//cInstance2->GetAttribute("Number")->SetInt(108);
	//cInstance2->GetAttribute("InnerObj")->SetVar(Var<MyClass*>(new MyClass()));

	//// Cleanup
	//delete cInstance1->InnerObj;
	delete cInstance1;
	//delete cInstance2->InnerObj;
	//delete cInstance2;
}
