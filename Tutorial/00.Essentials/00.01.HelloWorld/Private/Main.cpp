/*********************************************************\
 *  File: Main.cpp                                       *
 *      PixelLight Sample 01 - Application
 *      Simple 'Hello World'-Application using the PixelLight application framework
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
//[ Introduction                                          ]
//[-------------------------------------------------------]
/*

	PixelLight Tutorial 00.01 - Hello World

	This tutorial shows the most basic steps required to initialize and run a functional
	PixelLight application. Unline the following tutorials, this one has all the code in
	just a single file to keep things simple.

	The comments in the following code will explain the core concepts as you go through.

*/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]

// <PLCore/Main.h> includes the actual 'main' function for the application and should therefore
// only be included in executable projects, not libraries. Look for PLMain() down below for an
// actual implementation of PixelLight's entry point routine with full explanation.
#include <PLCore/Main.h>

// <PLCore/ModuleMain.h> should be included once per module (executable or library) as it contains
// everything necessary to define an engine module. See the 'pl_module' block below for the actual
// module definition.
#include <PLCore/ModuleMain.h>

// Very basic application base class. There are more advanced application base classes available
// and we will go through them in future excercises.
#include <PLCore/Application/CoreApplication.h>

// These two includes together provide access to the system console we'll use to print our obligatory
// message
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Module definition                                     ]
//[-------------------------------------------------------]

// The following is a very basic definition of PixelLight's engine module. This is a more advanced topic
// that will be explained later ([TODO] reference). For now, just note that it is a good practice to use
// this kind of definition in every PixelLight application which becomes a must in some use cases.
//pl_module("00.01.HelloWorld")
//	pl_module_vendor("Copyright (C) 2002-2014 by The PixelLight Team")
//	pl_module_license("\"MIT License\" which is also known as \"X11 License\" or \"MIT X License\" (mit-license.org)")
//	pl_module_description("PixelLight Tutorial 00.01 - Hello World")
//pl_module_end


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

// In this introductory tutorial, we define the application class directly here to keep things simple. In the
// following tutorials, look for Application.h and Application.cpp that will contain that tutorial's main
// application class

/**
*  @brief
*    Application class
*/
class Application : public PLCore::CoreApplication {

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Constructor
	*/
	Application()
	{
	}

	/**
	*  @brief
	*    Destructor
	*/
	virtual ~Application()
	{
	}


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::CoreApplication functions     ]
	//[-------------------------------------------------------]
private:

	// This function is called automatically by the engine when all the systems
	// are initialized
	virtual void Main() override
	{
		// Print some text
		System::GetInstance()->GetConsole().Print("Hello PixelLight :-)\n");
	}


};


//[-------------------------------------------------------]
//[ Program entry point                                   ]
//[-------------------------------------------------------]

// This is the real entry point for any PixelLight executable. You must define this function if you
// include <PLCore/Main.h> in your project. The parameters passed by the engine into this function
// are self-explanatory.
int PLMain(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// We just create an instance of our very basic application class and run it. This will result
	// in the engine being initialized into usable state and the above Application::Main method
	// being called. Note that only the very essential engine systems are intialized, such as log
	// and plugin system. We will explore the initialization of more advanced systems (such as renderer)
	// in future excercises.
	Application cApplication;
	return cApplication.Run(sExecutableFilename, lstArguments);
}
