/*********************************************************\
 *  File: Class.h                                        *
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


#ifndef __PLCORE_CLASS_H__
#define __PLCORE_CLASS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/List.h"
#include "PLCore/Container/HashMap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Module;
class Object;
class VarDesc;
class FuncDesc;
class EventDesc;
class DynParams;
class ClassImpl;
class ConstructorDesc;
class EventHandlerDesc;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Description and interface for classes
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Class {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ClassImpl;
	friend class ClassReal;
	friend class ClassDummy;
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get module the class belongs to
		*
		*  @return
		*    Module (always valid, do not destroy the returned instance!)
		*/
		inline const Module *GetModule() const;

		/**
		*  @brief
		*    Get full class name (with namespace)
		*
		*  @return
		*    Name of class and namespace
		*/
		inline String GetClassName() const;

		/**
		*  @brief
		*    Get full name of base class (with namespace)
		*
		*  @return
		*    Name of base class and namespace
		*/
		inline String GetBaseClassName() const;

		/**
		*  @brief
		*    Get class name (without namespace)
		*
		*  @return
		*    Name of class
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Get class description
		*
		*  @return
		*    Description
		*/
		inline String GetDescription() const;

		/**
		*  @brief
		*    Get namespace
		*
		*  @return
		*    Namespace
		*/
		inline String GetNamespace() const;

		/**
		*  @brief
		*    Get base class
		*
		*  @return
		*    Pointer to base class (can be a null pointer, do not destroy the returned instance!)
		*/
		inline const Class *GetBaseClass() const;

		/**
		*  @brief
		*    Check if class is derived from another class
		*
		*  @param[in] cBaseClass
		*    Base class
		*
		*  @return
		*    'true' if class is derived from given base class, else 'false'
		*/
		inline bool IsDerivedFrom(const Class &cBaseClass) const;

		/**
		*  @brief
		*    Check if class is derived from another class
		*
		*  @param[in] sBaseClass
		*    Base class name (with namespace)
		*
		*  @return
		*    'true' if class is derived from given base class, else 'false'
		*/
		inline bool IsDerivedFrom(const String &sBaseClass) const;

		/**
		*  @brief
		*    Get derived classes
		*
		*  @param[out] lstClasses
		*    Receives the list of derived classes, given list is not cleared before adding new elements, do not destroy the returned instances!
		*
		*  @remarks
		*    This method always returns all sub-classes of a class.
		*    If you want to search for classes with more specific search criteria,
		*    have a look at ClassManager::GetClasses().
		*/
		PLCORE_API void GetDerivedClasses(List<const Class*> &lstClasses) const;

		/**
		*  @brief
		*    Get properties
		*
		*  @return
		*    Hash map of properties (name -> value)
		*
		*  @remarks
		*    A property is a name/value pair of strings, that can be assigned to a class. Use this to
		*    transport additional information for your class, e.g.
		*      "PluginType"  -> "Widget"
		*      "FileFormats" -> "avi mpg mp4"
		*/
		inline const HashMap<String, String> &GetProperties() const;

		/**
		*  @brief
		*    Get attributes
		*
		*  @return
		*    List of attribute descriptors, do not destroy the returned instances!
		*/
		inline const List<VarDesc*> &GetAttributes() const;

		/**
		*  @brief
		*    Get attribute
		*
		*  @param[in] sName
		*    Attribute name
		*
		*  @return
		*    Attribute descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const VarDesc *GetAttribute(const String &sName) const;

		/**
		*  @brief
		*    Get methods
		*
		*  @return
		*    List of method descriptors, do not destroy the returned instances!
		*/
		inline const List<FuncDesc*> &GetMethods() const;

		/**
		*  @brief
		*    Get method
		*
		*  @param[in] sName
		*    Method name
		*
		*  @return
		*    Method descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const FuncDesc *GetMethod(const String &sName) const;

		/**
		*  @brief
		*    Get signals
		*
		*  @return
		*    List of signal descriptors, do not destroy the returned instances!
		*/
		inline const List<EventDesc*> &GetSignals() const;

		/**
		*  @brief
		*    Get signal
		*
		*  @param[in] sName
		*    Signal name
		*
		*  @return
		*    Signal descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const EventDesc *GetSignal(const String &sName) const;

		/**
		*  @brief
		*    Get slot
		*
		*  @return
		*    List of slot descriptors, do not destroy the returned instances!
		*/
		inline const List<EventHandlerDesc*> &GetSlots() const;

		/**
		*  @brief
		*    Get slot
		*
		*  @param[in] sName
		*    Slot name
		*
		*  @return
		*    Slot descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const EventHandlerDesc *GetSlot(const String &sName) const;

		/**
		*  @brief
		*    Check if class has any constructors
		*
		*  @return
		*    'true' if class has at least one constructor, else 'false'
		*/
		inline bool HasConstructor() const;

		/**
		*  @brief
		*    Check if class has a default constructor
		*
		*  @return
		*    'true' if class has a default constructor, else 'false'
		*/
		inline bool HasDefaultConstructor() const;

		/**
		*  @brief
		*    Get constructors
		*
		*  @return
		*    List of constructor descriptors, do not destroy the returned instances!
		*/
		inline const List<ConstructorDesc*> &GetConstructors() const;

		/**
		*  @brief
		*    Get constructor
		*
		*  @param[in] sName
		*    Constructor name
		*
		*  @return
		*    Constructor descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const ConstructorDesc *GetConstructor(const String &sName) const;

		/**
		*  @brief
		*    Create object by using the default constructor
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will call the default constructor of the class.
		*    If the class has no default constructor, the function will fail and return a null pointer.
		*/
		inline Object *Create() const;

		/**
		*  @brief
		*    Create object by using typed constructor parameters in order to identity the constructor automatically
		*
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will search for a constructor that matches the signature of the given parameters.
		*    If no such constructor can be found, the function will fail and return a null pointer.
		*/
		inline Object *Create(const DynParams &cParams) const;

		/**
		*  @brief
		*    Create object by using a given constructor name and typed constructor parameters
		*
		*  @param[in] sName
		*    Constructor name
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will search for a constructor with the specified name. If no such constructor can be found, or
		*    the given parameters do not match the signature of the constructor, the function will fail and return a null pointer.
		*/
		inline Object *Create(const String &sName, const DynParams &cParams) const;

		/**
		*  @brief
		*    Create object by using a given constructor name and typeless constructor parameters
		*
		*  @param[in] sName
		*    Constructor name
		*  @param[in] sParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will search for a constructor with the specified name. If no such constructor can be found,
		*    the function will fail and return a null pointer.
		*/
		inline Object *Create(const String &sName, const String &sParams) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cClassImpl
		*    Reference to the class specific implementation (this class just shares the given data and doesn't destroy it)
		*/
		Class(ClassImpl &cClassImpl);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Class(const Class &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		~Class();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Class &operator =(const Class &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		mutable ClassImpl *m_pClassImpl;	/**< Pointer to the class specific implementation, just shared pointer (assumed to be never a null pointer!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Base/Class.inl"


#endif // __PLCORE_CLASS_H__
