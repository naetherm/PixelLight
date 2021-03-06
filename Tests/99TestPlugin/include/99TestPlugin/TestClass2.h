/*********************************************************\
 *  File: TestClass2.h                                   *
 *      Test class demonstrating the RTTI features
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


#ifndef __99TESTPLUGIN_TESTCLASS2_H__
#define __99TESTPLUGIN_TESTCLASS2_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "99TestPlugin/TestClass1.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace TestPlugin {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class DummyClass {
	protected:
		int a, b, c, d;

		DummyClass()
		{
			a = b = 0;
			c = d = 1;
		}

		virtual ~DummyClass()
		{
			int h = a;
		}
};


class TestClass2 : public DummyClass, public TestClass1 {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Test flags #2
		enum TestFlags2 {
			Flag16  = 16,
			Flag32  = 32,
			Flag64  = 64,
			Flag128 = 128
		};

		// Define enum in RTTI
		pl_enum(TestFlags2)
			pl_enum_base(TestFlags)
			pl_enum_value(Flag16,  "")
			pl_enum_value(Flag32,  "")
			pl_enum_value(Flag64,  "")
			pl_enum_value(Flag128, "")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(TESTPLUGIN_RTTI_EXPORT, TestClass2, "TestPlugin", TestClass1, "Yet another test class demonstrating the RTTI features")
		// Properties
		pl_properties
			pl_property("TestInfo",	"This is class TestClass2")
		pl_properties_end
		// Attributes
		pl_attribute(Bool,	bool,						true,		ReadWrite,	DirectValue,	"Boolean variable",			"")
			// Overwritten TestClass1 attributes
		pl_attribute(Float,	float,						2.71828f,	ReadWrite,	ModifyAttr)
		pl_attribute(Flags,	pl_flag_type(TestFlags2),	32,			ReadWrite,	ModifyAttr,		"Flags test (extended)",	"")
		// Constructors
		pl_constructor_0(Create,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		TESTPLUGIN_API TestClass2();
		TESTPLUGIN_API virtual ~TestClass2();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // TestPlugin


#endif // __99TESTPLUGIN_TESTCLASS2_H__
