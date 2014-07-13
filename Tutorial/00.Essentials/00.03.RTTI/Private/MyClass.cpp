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
#include "MyClass.h"
#include <PLCore/Container/Array.h>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLRefl;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_begin_class(MyClass)
	pl_class_method(Foo)
//.Method("Foo", PLCore::Function<decltype(&_Clss::Foo)>(nullptr, &_Clss::Foo))
pl_end_class()

template <typename Func>
struct S
{
	S(Func _f) : f(_f) {}
	Func f;

	template <typename... Args>
	void Exec(Args... args)
	{
		f(args...);
	}
};

class B
{
public:
	virtual int f(int, float)
	{
		return 1;
	}
};

class D : public B
{
public:
	virtual int f(int ii, float ff) override
	{
		return 2;
	}
};

//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MyClass::MyClass()
{
	D inst;
	PLCore::Function<decltype(&B::f)> d(&inst, &B::f);
	d(1, 2.0f);

	//auto l = [this](int i, float f) {};
	//S<decltype(l)> s(l);
	//s.Exec(1, 1.0f);
	//PLCore::Function<decltype(l)> dl;

	PLCore::Tuple<int, float> t;
	int& i = PLCore::TupleGet<0>(t) = 2;
	float& f = PLCore::TupleGet<1>(t) = 2.0f;
	i = 108;
	f = 1.08f;
	d(t);

	PLCore::Array<PLCore::FunctionParam> params;
	params.Add(PLCore::FunctionParam(108));
	params.Add(PLCore::FunctionParam(10.8f));
	int ret = d.DynInvoke(&params).Get<int>();

	struct SmallStruct {
	
		int a;

		SmallStruct()
		{
			a=10;
		}
		~SmallStruct()
		{
			a=a;
		}
	};

	struct BigStruct {
	
		int a;
		int b;
		int c;
		int d;

		BigStruct()
		{
			a=b=c=d=10;
		}
		~BigStruct()
		{
			a=a;
		}
	};

	SmallStruct ss0;
	PLCore::FunctionParam p1(ss0);
	SmallStruct &ss = p1.Get<SmallStruct>();
	p1.Destroy<SmallStruct>();

	BigStruct bs0;
	PLCore::FunctionParam p2(bs0);
	BigStruct &bs = p2.Get<BigStruct>();
	p2.Destroy<BigStruct>();
}

/**
*  @brief
*    Destructor
*/
MyClass::~MyClass()
{
}

/**
*  @brief
*    Dummy function
*/
int MyClass::Foo(int i, float f)
{
	return 0;
}