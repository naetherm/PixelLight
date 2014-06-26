/*********************************************************\
 *  File: MainAndroid.cpp                                *
 *    Program entry point for Android
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
#include <PLCore/Frontend/FrontendMain.h>
#include <PLCore/Frontend/FrontendApplication.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
extern int UnitTests(const String &sExecutableFilename, const Array<String> &lstArguments);


//[-------------------------------------------------------]
//[ Module definition                                     ]
//[-------------------------------------------------------]
pl_module_application_frontend("PLUnitTests", "Application", "PLFrontendNull::Frontend")
	pl_module_vendor("Copyright (C) 2002-2013 by The PixelLight Team")
	pl_module_license("\"MIT License\" which is also known as \"X11 License\" or \"MIT X License\" (mit-license.org)")
	pl_module_description("PixelLight unit tests")
pl_module_end


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Application class... kind of first unit tests: Is our Android framework working?
*/
class Application : public PLCore::FrontendApplication {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, Application, "", PLCore::FrontendApplication, "Application class")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontend
		*    Frontend this application instance is running in
		*/
		Application(PLCore::Frontend &cFrontend) : FrontendApplication(cFrontend) { }

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Application() { }


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::AbstractLifecycle functions   ]
	//[-------------------------------------------------------]
	private:
		virtual bool OnStart() override
		{
			// Call base implementation
			if (FrontendApplication::OnStart()) {
				// Execute the unit tests
				Exit(UnitTests(GetApplicationContext().GetExecutableFilename(), GetApplicationContext().GetArguments()));

				// Done
				return true;
			}

			// Error
			return false;
		}


};


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Application)

