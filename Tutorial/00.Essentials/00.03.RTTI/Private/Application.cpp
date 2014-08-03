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
#include <PLCore/Container/Array.h>
#include <PLCore/Typebase/Function.h>
#include "Application.h"
#include "MyClass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


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
	MyClass c;

	c.OnInit.Emit(108);

	const PLCore::Class *clss = PLCore::TypeRegistry::GetInstance()->GetClassType("MyClass")->GetClass();
	if (clss)
	{
		// Method
		const PLCore::ClassMethod *meth = clss->GetMethod("Foo");
		if (meth)
		{
			// Indirect call
			PLCore::Array<PLCore::FunctionParam> params;
			params.Add(&c);
			params.Add(108);
			params.Add(10.8f);

			int ret = meth->Call(&params).Get<int>();
			
			// Direct call
			ret = meth->CallDirect<int>(&c, 108, 108.108f);
			ret=ret;
		}

		// Property
		const PLCore::ClassProperty *prop = clss->GetProperty("PrivateInt");
		if (prop)
		{
			// Indirect set/get
			PLCore::Array<PLCore::FunctionParam> params;
			params.Add(PLCore::FunctionParam(&c));
			params.Add(PLCore::FunctionParam(108));

			prop->Set(&params);
			int ret = prop->Get(&params).Get<int>();

			// Direct set/get
			prop->SetDirect<const int&>(&c, 1008);
			ret = prop->GetDirect<const int&>(&c);
			ret=ret;
		}
	}
}
