/*********************************************************\
 *  File: Object.cpp                                     *
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
#include "PLCore/Xml/Xml.h"
#include "PLCore/Base/Var/VarDesc.h"
#include "PLCore/Base/Var/DynVar.h"
#include "PLCore/Base/Func/FuncDesc.h"
#include "PLCore/Base/Event/EventDesc.h"
#include "PLCore/Base/Event/EventHandlerDesc.h"
#include "PLCore/Base/Event/DynEvent.h"
#include "PLCore/Base/Event/DynEventHandler.h"
#include "PLCore/Base/Tools/ParamsParser.h"
#include "PLCore/Base/Tools/ParamsParserXml.h"
#include "PLCore/Base/ClassManager.h"
#include "PLCore/Base/Class.h"
#include "PLCore/Base/Object.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Rtti interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Object)


//[-------------------------------------------------------]
//[ Public ObjectBase functions                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ObjectBase::ObjectBase() :
	m_nRefCount(1)	// Not 0, see class comment
{
}

/**
*  @brief
*    Destructor
*/
ObjectBase::~ObjectBase()
{
}

/**
*  @brief
*    Get class
*/
Class *ObjectBase::GetClass() const
{
	return nullptr;
}

/**
*  @brief
*    Get a pointer to the object
*/
const ObjectBase *ObjectBase::GetPointer() const
{
	return this;
}

ObjectBase *ObjectBase::GetPointer()
{
	return this;
}

/**
*  @brief
*    Increases the reference count
*/
uint32 ObjectBase::AddReference()
{
	// Increment reference count
	m_nRefCount++;

	// Return current reference count
	return m_nRefCount;
}

/**
*  @brief
*    Decreases the reference count
*/
uint32 ObjectBase::Release()
{
	// Decrement reference count
	if (m_nRefCount > 1) {
		m_nRefCount--;

		// Return current reference count
		return m_nRefCount;

	// Destroy object when no references are left
	} else {
		delete this;

		// This object is no longer
		return 0;
	}
}

/**
*  @brief
*    Gets the current reference count
*/
uint32 ObjectBase::GetRefCount() const
{
	// Return current reference count
	return m_nRefCount;
}

/**
*  @brief
*    Decreases the reference count without destroying this instance automatically
*/
uint32 ObjectBase::SoftRelease()
{
	// Decrement reference count
	if (m_nRefCount > 1) {
		m_nRefCount--;

		// Return current reference count
		return m_nRefCount;

	// Destroy object when no references are left
	} else {
		// Unlike "Release()", do not destroy this instance
		// delete this;
		m_nRefCount = 0;

		// This object is no longer referenced
		return 0;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Object::Object()
{
}

/**
*  @brief
*    Destructor
*/
Object::~Object()
{
	// Emit object destroyed signal
	SignalDestroyed();
}

/**
*  @brief
*    Check if object is instance of a given class by using a given class reference
*/
bool Object::IsInstanceOfByReference(const Class &cClass) const
{
	// Get class
	const Class *pClass = GetClass();
	if (pClass) {
		// Same class?
		if (pClass == &cClass) {
			return true;
		} else {
			// Check if class is derived from given class
			return pClass->IsDerivedFrom(cClass);
		}
	}

	// Class could not be found
	return false;
}

/**
*  @brief
*    Check if object is instance of a given class by using a class name
*/
bool Object::IsInstanceOf(const String &sClass) const
{
	// Get class
	const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
	if (pClass) {
		// Check class
		return IsInstanceOfByReference(*pClass);
	}

	// Class could not be found
	return false;
}

/**
*  @brief
*    Get attributes
*/
const List<DynVar*> Object::GetAttributes() const
{
	// Create attribute list
	List<DynVar*> lstAttributes;

	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor list
		const Container<VarDesc*> &lstDescriptors = pClass->GetAttributes();

		// Get all attributes for this instance and add them to the list
		for (uint32 i=0; i<lstDescriptors.GetNumOfElements(); i++)
			lstAttributes.Add(lstDescriptors[i]->GetAttribute(*this));
	}

	// Return attribute list
	return lstAttributes;
}

/**
*  @brief
*    Get attribute
*/
DynVar *Object::GetAttribute(const String &sName) const
{
	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor
		const VarDesc *pDescriptor = pClass->GetAttribute(sName);
		if (pDescriptor) {
			// Return attribute
			return pDescriptor->GetAttribute(*this);
		}
	}

	// Could not find attribute
	return nullptr;
}

/**
*  @brief
*    Get all callable methods
*/
void Object::GetMethods(List<DynFuncPtr> &lstMethods)
{
	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor list
		const Container<FuncDesc*> &lstDescriptors = pClass->GetMethods();

		// Get all callable methods for this instance and add them to the list
		for (uint32 i=0; i<lstDescriptors.GetNumOfElements(); i++)
			lstMethods.Add(lstDescriptors[i]->GetMethod(*this));
	}
}

/**
*  @brief
*    Get callable method by using a given method name
*/
DynFuncPtr Object::GetMethod(const String &sName)
{
	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor
		const FuncDesc *pDescriptor = pClass->GetMethod(sName);
		if (pDescriptor) {
			// Return callable method
			return pDescriptor->GetMethod(*this);
		}
	}

	// Could not find method
	return DynFuncPtr(nullptr);
}

/**
*  @brief
*    Get a list of all signals
*/
const List<DynEvent*> Object::GetSignals() const
{
	// Create signal list
	List<DynEvent*> lstSignals;

	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor list
		const Container<EventDesc*> &lstDescriptors = pClass->GetSignals();

		// Get all signals for this instance and add them to the list
		for (uint32 i=0; i<lstDescriptors.GetNumOfElements(); i++)
			lstSignals.Add(lstDescriptors[i]->GetSignal(*this));
	}

	// Return signal list
	return lstSignals;
}

/**
*  @brief
*    Get signal by using a given signal name
*/
DynEvent *Object::GetSignal(const String &sName) const
{
	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor
		const EventDesc *pDescriptor = pClass->GetSignal(sName);
		if (pDescriptor) {
			// Return signal
			return pDescriptor->GetSignal(*this);
		}
	}

	// Could not find signal
	return nullptr;
}

/**
*  @brief
*    Get a list of all slots
*/
const List<DynEventHandler*> Object::GetSlots() const
{
	// Create slot list
	List<DynEventHandler*> lstSlots;

	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor list
		const Container<EventHandlerDesc*> &lstDescriptors = pClass->GetSlots();

		// Get all slots for this instance and add them to the list
		for (uint32 i=0; i<lstDescriptors.GetNumOfElements(); i++)
			lstSlots.Add(lstDescriptors[i]->GetSlot(*this));
	}

	// Return slot list
	return lstSlots;
}

/**
*  @brief
*    Get slot by using a given slot name
*/
DynEventHandler *Object::GetSlot(const String &sName) const
{
	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Get descriptor
		const EventHandlerDesc *pDescriptor = pClass->GetSlot(sName);
		if (pDescriptor) {
			// Return slot
			return pDescriptor->GetSlot(*this);
		}
	}

	// Could not find slot
	return nullptr;
}

/**
*  @brief
*    Set attribute value by using a given string value
*/
void Object::SetAttribute(const String &sName, const String &sValue)
{
	// Get attribute
	DynVar *pAttribute = GetAttribute(sName);
	if (pAttribute) {
		// Set value
		pAttribute->SetString(sValue);
	}
}

/**
*  @brief
*    Set attribute value by using a given dynamic variable reference
*/
void Object::SetAttribute(const String &sName, const DynVar &cVar)
{
	// Get attribute
	DynVar *pAttribute = GetAttribute(sName);
	if (pAttribute) {
		// Set value
		pAttribute->SetVar(cVar);
	}
}

/**
*  @brief
*    Set attribute value by using a given dynamic variable pointer
*/
void Object::SetAttribute(const String &sName, const DynVar *pVar)
{
	// Is the given dynamic variable pointer valid?
	if (pVar) {
		// Get attribute
		DynVar *pAttribute = GetAttribute(sName);
		if (pAttribute) {
			// Set value
			pAttribute->SetVar(*pVar);
		}
	}
}

/**
*  @brief
*    Set attribute to it's default value
*/
void Object::SetAttributeDefault(const String &sName)
{
	// Get attribute
	DynVar *pAttribute = GetAttribute(sName);
	if (pAttribute) {
		// Set default value
		pAttribute->SetDefault();
	}
}

/**
*  @brief
*    Call method with given dynamic parameters
*/
void Object::CallMethod(const String &sName, DynParams &cParams)
{
	// Get callable method
	DynFuncPtr pMethod = GetMethod(sName);
	if (pMethod) {
		// Call method
		pMethod->Call(cParams);
	}
}

/**
*  @brief
*    Call method with given constant dynamic parameters
*/
void Object::CallMethod(const String &sName, const DynParams &cParams)
{
	// Get callable method
	DynFuncPtr pMethod = GetMethod(sName);
	if (pMethod) {
		// Call method
		pMethod->Call(cParams);
	}
}

/**
*  @brief
*    Call method with parameters given as string
*/
void Object::CallMethod(const String &sName, const String &sParams)
{
	// Get callable method
	DynFuncPtr pMethod = GetMethod(sName);
	if (pMethod) {
		// Call method
		pMethod->Call(sParams);
	}
}

/**
*  @brief
*    Call method with parameters given as XML element
*/
void Object::CallMethod(const String &sName, const XmlElement &cElement)
{
	// Get callable method
	DynFuncPtr pMethod = GetMethod(sName);
	if (pMethod) {
		// Call method
		pMethod->Call(cElement);
	}
}

/**
*  @brief
*    Get attribute values as a string
*/
String Object::GetValues(EDefaultValue nDefaultValue) const
{
	// Init string
	String sVars;

	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Loop through attributes
		const Container<VarDesc*> &lstDescriptors = pClass->GetAttributes();
		Iterator<VarDesc*> cIterator = lstDescriptors.GetIterator();
		while (cIterator.HasNext()) {
			// Get attribute descriptor
			VarDesc *pVarDesc = cIterator.Next();
			if (pVarDesc) {
				// Get attribute
				DynVar *pVar = pVarDesc->GetAttribute(*this);

				// Ignore variables with default values?
				if (nDefaultValue == WithDefault || !pVar->IsDefault()) {
					// Add attribute to string
					if (sVars.GetLength() > 0)
						sVars += ' ';
					sVars += pVarDesc->GetName();
					sVars += "=\"";
					sVars += pVar->GetString();
					sVars += '\"';
				}
			}
		}
	}

	// Return string
	return sVars;
}

/**
*  @brief
*    Set multiple attribute values as a string at once
*/
void Object::SetValues(const String &sVars)
{
	// Parse parameters
	ParamsParser cParams;
	cParams.ParseString(sVars);
	while (cParams.HasParam()) {
		// Set attribute
		SetAttribute(cParams.GetName(), cParams.GetValue());

		// Next parameter
		cParams.Next();
	}
}

/**
*  @brief
*    Get attribute values as XML
*/
void Object::GetValuesXml(XmlElement &cElement, EDefaultValue nDefaultValue) const
{
	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Loop through attributes
		const Container<VarDesc*> &lstDescriptors = pClass->GetAttributes();
		Iterator<VarDesc*> cIterator = lstDescriptors.GetIterator();
		while (cIterator.HasNext()) {
			// Get attribute descriptor
			VarDesc *pVarDesc = cIterator.Next();
			if (pVarDesc) {
				// Get attribute
				DynVar *pVar = pVarDesc->GetAttribute(*this);

				// Ignore variables with default values?
				if (nDefaultValue == WithDefault || !pVar->IsDefault()) {
					// Add attribute to XML element
					cElement.SetAttribute(pVarDesc->GetName(), pVar->GetString());
				}
			}
		}
	}
}

/**
*  @brief
*    Set attribute values from XML
*/
void Object::SetValuesXml(const XmlElement &cElement)
{
	// Parse parameters
	ParamsParserXml cParams;
	cParams.ParseXml(cElement);
	while (cParams.HasParam()) {
		// Set attribute
		SetAttribute(cParams.GetName(), cParams.GetValue());

		// Next parameter
		cParams.Next();
	}
}

/**
*  @brief
*    Set all attributes to default
*/
void Object::SetDefaultValues()
{
	// Get class
	Class *pClass = GetClass();
	if (pClass) {
		// Loop through attributes
		const Container<VarDesc*> &lstDescriptors = pClass->GetAttributes();
		Iterator<VarDesc*> cIterator = lstDescriptors.GetIterator();
		while (cIterator.HasNext()) {
			// Get attribute descriptor
			VarDesc *pVarDesc = cIterator.Next();
			if (pVarDesc) {
				// Get attribute
				DynVar *pVar = pVarDesc->GetAttribute(*this);

				// Set default value
				pVar->SetDefault();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual Object functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get object as string
*/
String Object::ToString() const
{
	// Get attributes
	return GetValues(WithDefault);
}

/**
*  @brief
*    Set object from string
*/
void Object::FromString(const String &sString)
{
	// Set attributes
	SetValues(sString);
}

/**
*  @brief
*    Get object as XML
*/
XmlElement Object::ToXml() const
{
	// Create XML element
	XmlElement cElement(GetClass()->GetClassName());
	GetValuesXml(cElement, WithDefault);
	return cElement;
}

/**
*  @brief
*    Set object from XML
*/
void Object::FromXml(const XmlElement &cElement)
{
	// Check if XML element has the class name as it's name
	if (cElement.GetValue() == GetClass()->GetClassName()) {
		// Set attributes from XML
		SetValuesXml(cElement);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
