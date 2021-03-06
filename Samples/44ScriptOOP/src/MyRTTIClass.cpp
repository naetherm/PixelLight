/*********************************************************\
 *  File: MyRTTIClass.cpp                                *
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
#include "MyRTTIClass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(MyRTTIClass, "", PLCore::Object, "Sample RTTI class, don't take it to serious")
	// Properties
	pl_properties
		pl_property("MyProperty",	"This is a property value")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_0_metadata(Return42,				pl_ret_type(int),					"Returns 42",							"")
	pl_method_1_metadata(IgnoreTheParameter,	pl_ret_type(void),			float,	"Ignores the provided parameter",		"")
	pl_method_0_metadata(SaySomethingWise,		pl_ret_type(void),					"Says something wise",					"")
	pl_method_0_metadata(GetSelf,				pl_ret_type(MyRTTIClass*),			"Returns a pointer to this instance",	"")
	// Attributes
	pl_attribute_metadata(Name,		PLCore::String,		"Bob",	ReadWrite,	"A name, emits MySignal after the name was changed",			"")
	pl_attribute_metadata(Level,	int,				1,		ReadWrite,	"Level, automatically increased on get/set name and OnMyEvent",	"")
	// Signals
	pl_signal_1_metadata(MySignal,	PLCore::String,	"My signal, automatically emitted after the name was changed, message as first parameter",	"")
	// Slots
	pl_slot_1_metadata(OnMyEvent,	PLCore::String,	"My slot, message as first parameter",	"")
pl_class_metadata_end(MyRTTIClass)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String MyRTTIClass::GetName()
{
	// Increase the level
	Level = Level + 1;

	// Return the name
	return m_sName;
}

void MyRTTIClass::SetName(const String &sName)
{
	// Increase the level
	Level = Level + 1;

	// Set the name
	m_sName = sName;

	// Emit the signal
	MySignal("C++");
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MyRTTIClass::MyRTTIClass() :
	Name(this),
	Level(this),
	SlotOnMyEvent(this),
	m_sName("Bob")
{
}

/**
*  @brief
*    Destructor
*/
MyRTTIClass::~MyRTTIClass()
{
}

/**
*  @brief
*    Returns 42
*/
int MyRTTIClass::Return42() const
{
	return 42;
}

/**
*  @brief
*    Ignores the provided parameter
*/
void MyRTTIClass::IgnoreTheParameter(float fValue) const
{
	System::GetInstance()->GetConsole().Print(String("Ignoring \"") + fValue + "\" ... D'OH!\n");
}

/**
*  @brief
*    Says something wise
*/
void MyRTTIClass::SaySomethingWise() const
{
	System::GetInstance()->GetConsole().Print(String(Return42()) + " - wise enough?\n");
}

/**
*  @brief
*    Returns a pointer to this instance
*/
MyRTTIClass *MyRTTIClass::GetSelf()
{
	return this;
}

/**
*  @brief
*    Called when my event was emitted
*/
void MyRTTIClass::OnMyEvent(String sMessage)
{
	// Increase the level
	Level = Level + 1;

	// If there's a message, write it into the console
	if (sMessage.GetLength())
		System::GetInstance()->GetConsole().Print("\"MyRTTIClass::OnMyEvent\" message: \"" + sMessage + "\"\n");
}
