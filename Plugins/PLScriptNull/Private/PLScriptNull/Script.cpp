/*********************************************************\
 *  File: Script.cpp                                     *
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
#include "PLScriptNull/Script.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptNull {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Script)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Script::Script()
{
}

/**
*  @brief
*    Destructor
*/
Script::~Script()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Script functions               ]
//[-------------------------------------------------------]
bool Script::IsGlobalFunction(const String &sName, const String &sNamespace)
{
	// The global script function does not exist
	return false;
}

bool Script::AddGlobalFunction(const String &sFunction, const DynFunc &cDynFunc, const String &sNamespace)
{
	// Nothing to do in here

	// Done
	return true;
}

bool Script::RemoveAllGlobalFunctions()
{
	// Nothing to do in here

	// Done
	return true;
}

String Script::GetSourceCode() const
{
	return m_sSourceCode;
}

bool Script::SetSourceCode(const String &sSourceCode)
{
	// Backup the given source code
	m_sSourceCode = sSourceCode;

	// Nothing to do in here

	// Done
	return true;
}

void Script::GetAssociatedFilenames(Array<String> &lstFilenames)
{
	// Nothing to do in here
}

bool Script::Execute(const String &sSourceCode)
{
	// Nothing to do in here
	return true;
}

void Script::GetGlobalVariables(Array<String> &lstGlobalVariables, const String &sNamespace)
{
	// Nothing to do in here
}

bool Script::IsGlobalVariable(const String &sName, const String &sNamespace)
{
	// Nothing to do in here
	return false;
}

ETypeID Script::GetGlobalVariableTypeID(const String &sName, const String &sNamespace)
{
	// Nothing to do in here
	return TypeInvalid;
}

String Script::GetGlobalVariable(const String &sName, const String &sNamespace)
{
	// Nothing to do in here
	return "";
}

void Script::SetGlobalVariable(const String &sName, const DynVar &cValue, const String &sNamespace)
{
	// Nothing to do in here
}

bool Script::BeginCall(const String &sFunctionName, const String &sFunctionSignature, const String &sNamespace)
{
	// Nothing to do in here

	// Done
	return true;
}

void Script::PushArgument(bool bValue)
{
	// Nothing to do in here
}

void Script::PushArgument(float fValue)
{
	// Nothing to do in here
}

void Script::PushArgument(double fValue)
{
	// Nothing to do in here
}

void Script::PushArgument(int8 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(int16 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(int32 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(int64 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(uint8 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(uint16 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(uint32 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(uint64 nValue)
{
	// Nothing to do in here
}

void Script::PushArgument(const String &sString)
{
	// Nothing to do in here
}

void Script::PushArgument(Object *pObject)
{
	// Nothing to do in here
}

void Script::PushArgument(Object &cObject)
{
	// Nothing to do in here
}

bool Script::EndCall()
{
	// Nothing to do in here

	// Done
	return true;
}

bool Script::GetReturn(bool nValue)
{
	// Nothing to do in here
	return false;
}

float Script::GetReturn(float nValue)
{
	// Nothing to do in here
	return 0.0f;
}

double Script::GetReturn(double nValue)
{
	// Nothing to do in here
	return 0.0;
}

int8 Script::GetReturn(int8 nValue)
{
	// Nothing to do in here
	return 0;
}

int16 Script::GetReturn(int16 nValue)
{
	// Nothing to do in here
	return 0;
}

int32 Script::GetReturn(int32 nValue)
{
	// Nothing to do in here
	return 0;
}

int64 Script::GetReturn(int64 nValue)
{
	// Nothing to do in here
	return 0;
}

uint8 Script::GetReturn(uint8 nValue)
{
	// Nothing to do in here
	return 0;
}

uint16 Script::GetReturn(uint16 nValue)
{
	// Nothing to do in here
	return 0;
}

uint32 Script::GetReturn(uint32 nValue)
{
	// Nothing to do in here
	return 0;
}

uint64 Script::GetReturn(uint64 nValue)
{
	// Nothing to do in here
	return 0;
}

String Script::GetReturn(String sValue)
{
	// Nothing to do in here
	return "";
}

Object *Script::GetReturn(Object *nValue)
{
	// Nothing to do in here
	return nullptr;
}

Object &Script::GetReturn(Object &nValue)
{
	// ... please note that in here, we can't return a null pointer...
	Object *pObject = GetReturn(&nValue);
	return pObject ? *pObject : nValue;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Script::Script(const Script &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Script &Script::operator =(const Script &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptNull
