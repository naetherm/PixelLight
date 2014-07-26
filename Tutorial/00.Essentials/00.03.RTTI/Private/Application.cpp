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
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Application)



//TEMP
class A
{
	pl_rtti()

public:
	int a;

	virtual void f(int i) = 0;
};
pl_declare_type(A)

pl_begin_class(A)
	pl_class_method(f)
pl_end_class()

class B : public A
{
	pl_rtti()

public:
	int b;

	virtual void f(int i) override
	{
		i = 1;
	}
};
pl_declare_type(B)

pl_begin_class(B)
	pl_class_base(A)
pl_end_class()

class C
{
	pl_rtti()

public:
	C() : c(0) {}
	C(int cc) : c(cc) {}
	int c;
};
pl_declare_type(C)

pl_begin_class(C)
pl_end_class()

class D : public B, public C
{
	pl_rtti()

public:
	D() : d(0) {}
	D(int cc, int dd) : C(cc), d(dd) {}
	int d;

	virtual void f(int i) override
	{
		i = 2;
	}
};
pl_declare_type(D)

pl_begin_class(D)
	pl_class_base(B)
	pl_class_base(C)
pl_end_class()



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

	const PLRefl::Class *clss = PLRefl::ClassManager::GetInstance()->GetClass("MyClass");
	if (clss)
	{
		// Method
		const PLRefl::ClassMethod *meth = clss->GetMethod("Foo");
		if (meth)
		{
			// Indirect call
			PLCore::Array<PLCore::FunctionParam> params;
			params.Add(PLCore::FunctionParam(&c));
			params.Add(PLCore::FunctionParam(108));
			params.Add(PLCore::FunctionParam(10.8f));

			int ret = meth->Call(&params).Get<int>();
			
			// Direct call
			ret = meth->CallDirect<int>(&c, 108, 108.108f);
			ret=ret;
		}

		// Property
		const PLRefl::ClassProperty *prop = clss->GetProperty("PrivateInt");
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

	/*PLCore::Function<decltype(&MyClass::PublicInt), PLCore::Field> f(&MyClass::PublicInt);

	int* v = nullptr;
	f(&c, &v);
	*v = 2;
	
	PLCore::Array<PLCore::FunctionParam> params;
	params.Add(PLCore::FunctionParam(&c));
	params.Add(PLCore::FunctionParam(&v));

	f.DynInvoke(&params);
	*v = 108;*/

	D inst;
	clss = PLRefl::ClassManager::GetInstance()->GetClass("D");
	const PLRefl::ClassMethod *meth = clss->GetMethod("f");
	if (meth)
	{
		PLCore::Array<PLCore::FunctionParam> params;
		params.Add(PLCore::FunctionParam(&inst));
		params.Add(PLCore::FunctionParam(108));

		meth->Call(&params);
	}

	const char *id = (&inst)->_ClassId();
	id=id;

	PLCore::Array<PLCore::FunctionParam> params;
	params.Add(PLCore::FunctionParam(1008));
	params.Add(PLCore::FunctionParam(108));

	PLCore::ConstructorFunc<D> ctor1;
	PLCore::ConstructorFunc<D, int, int> ctor2;

	D* d1 = ctor1.DynInvoke(&params).Get<D*>();
	D* d2 = ctor2.DynInvoke(&params).Get<D*>();

	d1=d2;
}
