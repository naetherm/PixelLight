/*********************************************************\
 *  File: ClassReal.cpp                                  *
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
#include "PLCore/Base/Class.h"
#include "PLCore/Base/ClassManager.h"
#include "PLCore/Base/Var/VarDesc.h"
#include "PLCore/Base/Func/FuncDesc.h"
#include "PLCore/Base/Func/ConstructorDesc.h"
#include "PLCore/Base/Event/EventDesc.h"
#include "PLCore/Base/Event/EventHandlerDesc.h"
#include "PLCore/Base/ClassReal.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ClassReal::ClassReal(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass) : ClassImpl(nModuleID, sName, sDescription, sNamespace, sBaseClass)
{
	// Register at class manager
	ClassManager::GetInstance()->RegisterClass(nModuleID, this);
}

/**
*  @brief
*    Destructor
*/
ClassReal::~ClassReal()
{
	// Unregister at class manager, please note that the class manager may have already been destroyed (random static de-initialization order)
	if (ClassManager::HasInstance())
		ClassManager::GetInstance()->UnregisterClass(m_nModuleID, this);

	// De-initialize class
	if (m_bInitialized)
		DeInitClass();
}


//[-------------------------------------------------------]
//[ Protected virtual ClassImpl functions                 ]
//[-------------------------------------------------------]
bool ClassReal::IsDummy() const
{
	// This is the real thing!
	return false;
}

/**
*  @brief
*    Initialize class and class members
*/
void ClassReal::InitClass() const
{
	// Check if class has already been initialized
	if (!m_bInitialized) {
		// Get base class
		if (m_sBaseClass.GetLength())
			m_pBaseClass = ClassManager::GetInstance()->GetClass(m_sBaseClass);

		// Check if a valid base class has been found
		if (m_pBaseClass || !m_sBaseClass.GetLength()) {
			// Do we have a base class? (only Object doesn't, but that must count, too)
			if (m_pBaseClass) {
				// Initialize base class
				static_cast<ClassImpl*>(m_pBaseClass->m_pClassImpl)->InitClass();

				{ // Add properties from base class
					ClassImpl *pBaseClassReal = static_cast<ClassImpl*>(m_pBaseClass->m_pClassImpl);
					Iterator<String> cIterator = pBaseClassReal->m_mapProperties.GetKeyIterator();
					while (cIterator.HasNext()) {
						const String sName  = cIterator.Next();
						const String sValue = pBaseClassReal->m_mapProperties.Get(sName);
						m_mapProperties.Set(sName, sValue);
					}
				}

				// Add attributes from base class
				m_lstAttributes = m_pBaseClass->GetAttributes();
				for (uint32 i=0; i<m_lstAttributes.GetNumOfElements(); i++) {
					VarDesc *pAttr = m_lstAttributes[i];
					m_mapMembers.Add(pAttr->GetName(), pAttr);
				}

				// Add methods from base class
				m_lstMethods = m_pBaseClass->GetMethods();
				for (uint32 i=0; i<m_lstMethods.GetNumOfElements(); i++) {
					FuncDesc *pMethod = m_lstMethods[i];
					m_mapMembers.Add(pMethod->GetName(), pMethod);
				}

				// Add signals from base class
				m_lstSignals = m_pBaseClass->GetSignals();
				for (uint32 i=0; i<m_lstSignals.GetNumOfElements(); i++) {
					EventDesc *pSignal = m_lstSignals[i];
					m_mapMembers.Add(pSignal->GetName(), pSignal);
				}

				// Add slots from base class
				m_lstSlots = m_pBaseClass->GetSlots();
				for (uint32 i=0; i<m_lstSlots.GetNumOfElements(); i++) {
					EventHandlerDesc *pSlot = m_lstSlots[i];
					m_mapMembers.Add(pSlot->GetName(), pSlot);
				}

				// Constructors are not copied from base classes, only the own constructors can be used!
			}

			// Add own properties
			Iterator<String> cIterator = m_mapOwnProperties.GetKeyIterator();
			while (cIterator.HasNext()) {
				const String sName  = cIterator.Next();
				const String sValue = m_mapOwnProperties.Get(sName);
				m_mapProperties.Set(sName, sValue);
			}

			// Add own members
			for (uint32 i=0; i<m_lstOwnMembers.GetNumOfElements(); i++) {
				// Get member
				MemberDesc *pMember = m_lstOwnMembers[i];

				// Add to hash map and overwrite variables from base classes that are already there (having the same name!)
				MemberDesc *pOverwriteMember = m_mapMembers.Get(pMember->GetName());
				if (pOverwriteMember) {
					// Overwrite base class member
					m_mapMembers.Set(pMember->GetName(), pMember);
				} else {
					// Add new member
					m_mapMembers.Add(pMember->GetName(), pMember);
				}

				// Check type and add to respective list
				switch (pMember->GetMemberType()) {
					// Attribute
					case MemberAttribute:
						if (pOverwriteMember)
							m_lstAttributes.Replace(static_cast<VarDesc*>(pOverwriteMember), static_cast<VarDesc*>(pMember));
						else
							m_lstAttributes.Add(static_cast<VarDesc*>(pMember));
						break;

					// Method
					case MemberMethod:
						if (pOverwriteMember)
							m_lstMethods.Replace(static_cast<FuncDesc*>(pOverwriteMember), static_cast<FuncDesc*>(pMember));
						else
							m_lstMethods.Add(static_cast<FuncDesc*>(pMember));
						break;

					// Event
					case MemberEvent:
						if (pOverwriteMember)
							m_lstSignals.Replace(static_cast<EventDesc*>(pOverwriteMember), static_cast<EventDesc*>(pMember));
						else
							m_lstSignals.Add(static_cast<EventDesc*>(pMember));
						break;

					// Event handler
					case MemberEventHandler:
						if (pOverwriteMember)
							m_lstSlots.Replace(static_cast<EventHandlerDesc*>(pOverwriteMember), static_cast<EventHandlerDesc*>(pMember));
						else
							m_lstSlots.Add(static_cast<EventHandlerDesc*>(pMember));
						break;

					// Constructor
					case MemberConstructor:
						m_lstConstructors.Add(static_cast<ConstructorDesc*>(pMember));
						break;
				}
			}

			// Done
			m_bInitialized = true;
		} else {
			// Error! Could not find base class
		}
	}
}

/**
*  @brief
*    De-Initialize class and class members
*/
void ClassReal::DeInitClass() const
{
	// Clear lists
	m_mapProperties.Clear();
	m_mapMembers.Clear();
	m_lstAttributes.Clear();
	m_lstMethods.Clear();
	m_lstSignals.Clear();
	m_lstSlots.Clear();
	m_lstConstructors.Clear();

	// Remove base class
	m_pBaseClass = nullptr;

	// Class de-initialized
	m_bInitialized = false;

	// De-initialize derived classes, please note that the class manager may have already been destroyed (random static de-initialization order)
	if (ClassManager::HasInstance()) {
		List<const Class*> lstClasses;
		ClassManager::GetInstance()->GetClasses(lstClasses, m_sClassName, NonRecursive, NoBase, IncludeAbstract);
		for (uint32 i=0; i<lstClasses.GetNumOfElements(); i++)
			lstClasses[i]->m_pClassImpl->DeInitClass();
	}
}

const List<VarDesc*> &ClassReal::GetAttributes() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Return attributes
	return m_lstAttributes;
}

const VarDesc *ClassReal::GetAttribute(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Get attribute
	MemberDesc *pMember = m_mapMembers.Get(sName);
	if (pMember && pMember->GetMemberType() == MemberAttribute)
		return static_cast<VarDesc*>(pMember);

	// Attribute could not be found
	return nullptr;
}

const List<FuncDesc*> &ClassReal::GetMethods() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Return methods
	return m_lstMethods;
}

const FuncDesc *ClassReal::GetMethod(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Get method
	MemberDesc *pMember = m_mapMembers.Get(sName);
	if (pMember && pMember->GetMemberType() == MemberMethod)
		return static_cast<FuncDesc*>(pMember);

	// Method could not be found
	return nullptr;
}

const List<EventDesc*> &ClassReal::GetSignals() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Return signals
	return m_lstSignals;
}

const EventDesc *ClassReal::GetSignal(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Get signal
	MemberDesc *pMember = m_mapMembers.Get(sName);
	if (pMember && pMember->GetMemberType() == MemberEvent)
		return static_cast<EventDesc*>(pMember);

	// Signal could not be found
	return nullptr;
}

const List<EventHandlerDesc*> &ClassReal::GetSlots() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Return slots
	return m_lstSlots;
}

const EventHandlerDesc *ClassReal::GetSlot(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Get slot
	MemberDesc *pMember = m_mapMembers.Get(sName);
	if (pMember && pMember->GetMemberType() == MemberEventHandler)
		return static_cast<EventHandlerDesc*>(pMember);

	// Slot could not be found
	return nullptr;
}

bool ClassReal::HasConstructor() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Check if there is at least one constructor
	return (m_lstConstructors.GetNumOfElements() > 0);
}

bool ClassReal::HasDefaultConstructor() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Loop through list
	Iterator<ConstructorDesc*> cIterator = m_lstConstructors.GetIterator();
	while (cIterator.HasNext()) {
		// Get constructor
		ConstructorDesc *pConstructor = cIterator.Next();

		// Default constructor?
		if (pConstructor->IsDefaultConstructor()) {
			// We found a default constructor
			return true;
		}
	}

	// No default constructor found
	return false;
}

const List<ConstructorDesc*> &ClassReal::GetConstructors() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Return constructors
	return m_lstConstructors;
}

const ConstructorDesc *ClassReal::GetConstructor(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Get constructor
	MemberDesc *pMember = m_mapMembers.Get(sName);
	if (pMember && pMember->GetMemberType() == MemberConstructor)
		return static_cast<ConstructorDesc*>(pMember);

	// Constructor could not be found
	return nullptr;
}

Object *ClassReal::Create() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Loop through constructors
	for (uint32 i=0; i<m_lstConstructors.GetNumOfElements(); i++) {
		// Get constructor
		ConstructorDesc *pConstructor = m_lstConstructors[i];

		// Check if this constructor is a default constructor
		if (pConstructor->GetSignature() == "Object*()") {
			// Call constructor
			return pConstructor->Create(Params<Object*>());
		}
	}

	// Error, no value constructor found
	return nullptr;
}

Object *ClassReal::Create(const DynParams &cParams) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Loop through constructors
	for (uint32 i=0; i<m_lstConstructors.GetNumOfElements(); i++) {
		// Get constructor
		ConstructorDesc *pConstructor = m_lstConstructors[i];

		// Check if this constructor has a matching signature
		if (pConstructor->GetSignature() == cParams.GetSignature()) {
			// Call constructor
			return pConstructor->Create(cParams);
		}
	}

	// Error, no value constructor found
	return nullptr;
}

Object *ClassReal::Create(const String &sName, const DynParams &cParams) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Loop through constructors
	for (uint32 i=0; i<m_lstConstructors.GetNumOfElements(); i++) {
		// Get constructor
		ConstructorDesc *pConstructor = m_lstConstructors[i];

		// Check if this constructor has a matching name and signature
		if (pConstructor->GetName() == sName && pConstructor->GetSignature() == cParams.GetSignature()) {
			// Call constructor
			return pConstructor->Create(cParams);
		}
	}

	// Error, no value constructor found
	return nullptr;
}

Object *ClassReal::Create(const String &sName, const String &sParams) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Loop through constructors
	for (uint32 i=0; i<m_lstConstructors.GetNumOfElements(); i++) {
		// Get constructor
		ConstructorDesc *pConstructor = m_lstConstructors[i];

		// Check if this constructor has a matching name (good luck with the signature)
		if (pConstructor->GetName() == sName) {
			// Call constructor
			return pConstructor->Create(sParams);
		}
	}

	// Error, no value constructor found
	return nullptr;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add member
*/
void ClassReal::AddMember(MemberDesc *pMemberDesc)
{
	// Check if pointer is valid
	if (pMemberDesc) {
		// De-initialize class
		if (m_bInitialized)
			DeInitClass();

		// Add member to list
		m_lstOwnMembers.Add(pMemberDesc);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
