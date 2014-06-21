/*********************************************************\
 *  File: Rtti.h                                         *
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


#ifndef __PLCORE_RTTI_H__
#define __PLCORE_RTTI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/ClassReal.h"
#include "PLCore/Base/ClassManager.h"
#include "PLCore/Base/Var/Var.h"
#include "PLCore/Base/Var/Attribute.h"
#include "PLCore/Base/Func/Method.h"
#include "PLCore/Base/Func/Constructor.h"
#include "PLCore/Base/Event/Signal.h"
#include "PLCore/Base/Event/Slot.h"
#include "PLCore/Base/Tools/TypeTraits.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Debug mode
*/
#ifdef _DEBUG
	#define PLCORE_IS_DEBUGMODE true
#else
	#define PLCORE_IS_DEBUGMODE false
#endif


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class to give each module a unique ID
*
*  @remarks
*    This is implemented as a template to ensure, that the compiler will create a *unique* class
*    in each module (e.g. library or application) that is using that class. So, e.g. calling
*    ModuleID<int> from project A will result in another class being used than calling it
*    from project B, which allows the class to request a unique identifier in each module.
*    The class is accessed using ModuleID<int>.
*/
template <typename T> class ModuleID
{


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get module ID
		*
		*  @return
		*    Unique module ID
		*/
		static uint32 GetModuleID();

		/**
		*  @brief
		*    Register module
		*
		*  @param[in] sName
		*    Module name
		*  @param[in] sVendor
		*    Vendor name
		*  @param[in] sLicense
		*    Module license
		*  @param[in] sDescription
		*    Module description
		*/
		static void RegisterModule(const String &sName, const String &sVendor, const String &sLicense, const String &sDescription);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		static uint32 m_nModuleID;	/**< Unique module ID */


};

// Get module ID
template <typename T> uint32 ModuleID<T>::GetModuleID()
{
	// Request module ID from ClassManager
	if (m_nModuleID == 0)
		m_nModuleID = ClassManager::GetInstance()->GetUniqueModuleID();

	// Return module ID
	return m_nModuleID;
}

template <typename T> void ModuleID<T>::RegisterModule(const String &sName, const String &sVendor, const String &sLicense, const String &sDescription)
{
	// Ensure we have a valid module ID
	GetModuleID();

	// Register module, and provide a pointer to our static module ID variable ("memory anchor")
	ClassManager::GetInstance()->RegisterModule(&m_nModuleID, sName, sVendor, sLicense, sDescription);
}

// Module ID
template <typename T> uint32	ModuleID<T>::m_nModuleID = 0;


//[-------------------------------------------------------]
//[ Internal macros                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create class for an enumeration type
*
*  @param[in] ENUM
*    Enumeration name
*/
#define __pl_enum(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef int  _BaseType; \
			typedef ENUM _Type; \
			\
			static bool GetEnumValue(int nIndex, ENUM &nValue, PLCore::String &sName, PLCore::String &sDescription) { \
				int nCount = 0; \

/**
*  @brief
*    Create class for an direct enumeration type (not using enum{}, e.g. for float 'enums')
*
*  @param[in] ENUM
*    Enumeration name
*  @param[in] TYPE
*    Enumeration type
*/
#define __pl_enum_direct(ENUM, TYPE) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef TYPE _BaseType; \
			typedef TYPE _Type; \
			\
			static bool GetEnumValue(int nIndex, TYPE &nValue, PLCore::String &sName, PLCore::String &sDescription) { \
				int nCount = 0; \

/**
*  @brief
*    Declare base enumeration type (add all values of an existing enumeration type)
*
*  @param[in] ENUM
*    Enumeration name of base data type
*/
#define __pl_enum_base(ENUM) \
				nCount = PLCore::EnumType<ENUM##__plcore_enum__>::GetNumOfEnumValues(); \
				if (nIndex >= 0 && nIndex < nCount) { \
					return ENUM##__plcore_enum__::GetEnumValue(nIndex, reinterpret_cast<ENUM&>(nValue), sName, sDescription); \
				} else if (nIndex == -1) { \
					if (ENUM##__plcore_enum__::GetEnumValue(-1, reinterpret_cast<ENUM&>(nValue), sName, sDescription)) \
						return true; \
				} \

/**
*  @brief
*    Add enumeration value
*
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define __pl_enum_value(VALUE, DESCRIPTION) \
					 if (nIndex == nCount)										{ nValue = VALUE; sName = #VALUE; return true; } \
				else if (nIndex == -1 && !sName.GetLength() && nValue == VALUE)	{ sName = #VALUE; return true; } \
				else if (nIndex == -1 && sName == #VALUE)						{ nValue = VALUE; sDescription = DESCRIPTION; return true; } \
				nCount++; \

/**
*  @brief
*    Add enumeration value by directly specifying the value
*
*  @param[in] NAME
*    Enumeration name
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define __pl_enum_value_direct(NAME, VALUE, DESCRIPTION) \
					 if (nIndex == nCount)										{ nValue = VALUE; sName = #NAME; return true; } \
				else if (nIndex == -1 && !sName.GetLength() && nValue == VALUE)	{ sName = #NAME;  return true; } \
				else if (nIndex == -1 && sName == #NAME)						{ nValue = VALUE; sDescription = DESCRIPTION; return true; } \
				nCount++; \

/**
*  @brief
*    End enumeration class
*/
#define __pl_enum_end \
				return false; \
			} \
	}; \

/**
*  @brief
*    Create class
*
*  @param[in] CLASS
*    Class name (without namespace)
*  @param[in] NAMESPACE
*    Namespace
*  @param[in] BASECLASS
*    Base class name (with namespace)
*  @param[in] DESCRIPTION
*    Class description
*/
#define __pl_class(CLASS, NAMESPACE, BASECLASS, BASECLASSNAME, DESCRIPTION) \
	/* All RTTI members are public */ \
	public: \
		/* Class type */ \
		typedef CLASS     _Self; \
		typedef BASECLASS _Base; \
		\
		/* Class description */ \
		class _Class : public PLCore::ClassReal { \
			friend class CLASS; \
			public: \
				/* Check base class */ \
				static void Error() { \
					PLCore::CheckBaseClass<CLASS, BASECLASS>::Type::Error(); \
				} \
				\
				/* Singleton */ \
				static _Class *GetSingleton(bool bGet = true) {\
					static bool    MyShutdown = false; \
					static _Class *MyInstance = nullptr; \
					if (bGet) { \
						/* Get or create instance */ \
						if (!MyInstance && !MyShutdown) { \
							MyInstance = new _Class(); \
							CLASS::_RegisterProperties(MyInstance); \
						} \
					} else { \
						/* Destroy instance and make sure that it won't be recreated */ \
						MyShutdown = true; \
						if (MyInstance) { \
							delete MyInstance; \
							MyInstance = nullptr; \
						} \
					} \
					return MyInstance; \
				} \
				\
				/* Constructor */ \
				_Class() : PLCore::ClassReal(PLCore::ModuleID<int>::GetModuleID(), #CLASS, DESCRIPTION, NAMESPACE, BASECLASSNAME) { \
				}; \
				\
				/* Destructor */ \
				virtual ~_Class() { \
				}; \
		}; \

/**
*  @brief
*    Mark export flag
*
*  @param[in] EXPORT
*    Export definition (must be defined as either 1 or 0)
*/
#define __pl_rtti_export(EXPORT) \
		enum { \
			_RttiExport = EXPORT \
		}; \
		\

/**
*  @brief
*    Create guard for class
*
*  @param[in] CLASS
*    Class name (without namespace)
*/
#define __pl_guard(CLASS) \
		/* Creation and destruction guard */ \
		class _Guard { \
			public: \
				/* Constructor */ \
				_Guard() { \
					_Class::GetSingleton(); \
				} \
				\
				/* Destructor */ \
				~_Guard() { \
					/* Make sure that class instance is removed on shutdown */ \
					_Class::GetSingleton(false); \
				} \
		}; \

/**
*  @brief
*    Create virtual function GetClass()
*/
#define __pl_getclass() \
		/* Public virtual PLCore::Object function */ \
		virtual PLCore::Class *GetClass() const override \
		{ \
			return _Class::GetSingleton()->GetClass(); \
		} \

/**
*  @brief
*    Create class implementation (static variables)
*
*  @param[in] CLASS
*    Class name (without namespace)
*/
#define __pl_class_impl(CLASS) \
	/* Guard */ \
	CLASS::_Guard cGuard_##CLASS; \

/**
*  @brief
*    Create class for properties
*/
#define __pl_prop_meth \
		/* Class properties */ \
		static inline void _RegisterProperties(PLCore::ClassReal *pClass) { \

/**
*  @brief
*    Add property
*
*  @param[in] NAME
*    Property name
*  @param[in] VALUE
*    Property value
*/
#define __pl_prop_prop(NAME, VALUE) \
			static_cast<_Class*>(pClass)->AddProperty(NAME, VALUE); \

/**
*  @brief
*    End class for properties
*/
#define __pl_prop_class_end \
		} \

/**
*  @brief
*    Create get/set storage class for an attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] STORAGE
*    Attribute storage type (DirectValue/GetSet/ModifyAttr)
*/
#define __pl_attr_stor(NAME, TYPE, STORAGE) \
		template <typename CLASS> class NAME##_GetSet { \
			public: \
				typedef PLCore::StorageGetSet StorageType; \
				typedef PLCore::StorageGetSet BaseStorageType; \
				static inline PLCore::Type<TYPE>::_Type Get(PLCore::Object *pObject) { \
					return static_cast<CLASS*>(pObject)->Get##NAME(); \
				} \
				static inline void Set(PLCore::Object *pObject, const PLCore::Type<TYPE>::_Type &Value) { \
					static_cast<CLASS*>(pObject)->Set##NAME(Value); \
				} \
		}; \
		\
		template <typename BASE> class NAME##_ModAttr { \
			public: \
				typedef PLCore::StorageModifyAttr						StorageType; \
				typedef typename BASE::NAME##_Storage::BaseStorageType	BaseStorageType; \
				static inline PLCore::Type<TYPE>::_Type Get(PLCore::Object *pObject) { \
					return static_cast<BASE*>(pObject)->NAME.Get(); \
				} \
				static inline void Set(PLCore::Object *pObject, const PLCore::Type<TYPE>::_Type &Value) { \
					static_cast<BASE*>(pObject)->NAME.Set(Value); \
				} \
		}; \

/**
*  @brief
*    Create descriptor class for an attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] DESCRIPTION
*    Attribute description
*  @param[in] ANNOTATION
*    Attribute annotation
*/
#define __pl_attr_desc(NAME, TYPE, DEFAULT, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::VarDesc { \
			public: \
				NAME##_Desc() : PLCore::VarDesc(PLCore::Type<TYPE>::TypeID, PLCore::Type<TYPE>::GetTypeName(), #NAME, DESCRIPTION, ANNOTATION) { \
					bool bRegister = static_cast<bool>(_Self::_RttiExport); \
					if (bRegister) \
						Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::String GetDefault() const override { \
					return PLCore::Type<TYPE>::ConvertToString(DEFAULT); \
				} \
				virtual PLCore::DynVar *GetAttribute(const Object &cObject) const override { \
					return &reinterpret_cast<_Self&>(const_cast<Object&>(cObject)).NAME; \
				} \
		}; \

/**
*  @brief
*    Create class for an attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*  @param[in] STORAGE
*    Attribute storage type (DirectValue/GetSet/ModifyAttr)
*/
#define __pl_attr_attr(NAME, TYPE, DEFAULT, ACCESS, STORAGE) \
		typedef PLCore::SpecializeIfEqual<PLCore::Storage##STORAGE, PLCore::StorageGetSet,     NAME##_GetSet,  _Self>::Type	NAME##_ClassGetSet; \
		typedef PLCore::SpecializeIfEqual<PLCore::Storage##STORAGE, PLCore::StorageModifyAttr, NAME##_ModAttr, _Base>::Type	NAME##_ClassModAttr; \
		typedef PLCore::StorageChooser<PLCore::Storage##STORAGE, NAME##_ClassGetSet, NAME##_ClassModAttr>::Storage			NAME##_Storage; \
		\
		class NAME##_Attr : public PLCore::Attribute<TYPE, PLCore::Access##ACCESS, NAME##_Storage, NAME##_Desc> { \
			public: \
				typedef PLCore::Type< TYPE >::_Type _Type; \
				NAME##_Attr(_Self *pObject) : PLCore::Attribute<TYPE, PLCore::Access##ACCESS, NAME##_Storage, NAME##_Desc>(DEFAULT, pObject) { \
				} \
				\
				NAME##_Attr &operator =(const _Type &Value) { \
					return static_cast<NAME##_Attr&>(PLCore::Attribute<TYPE, PLCore::Access##ACCESS, NAME##_Storage, NAME##_Desc>::operator =(Value)); \
				} \
		}; \

/**
*  @brief
*    Create attribute variable
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*/
#define __pl_attr_decl(NAME, TYPE) \
		NAME##_Attr NAME; \

/**
*  @brief
*    Create descriptor class for a method
*
*  @param[in] NAME
*    Method name
*  @param[in] RET
*    Return type
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Method description
*  @param[in] ANNOTATION
*    Method annotation
*/
#define __pl_method_desc(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::FuncDesc { \
			public: \
				typedef _Self																																ClassType; \
				typedef PLCore::Functor<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>															FuncType; \
				typedef PLCore::Signature<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>														SigType; \
				typedef PLCore::ClassTypelist< _Self, PLCore::Typelist<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15> >::TypeMethodSignature	MethType; \
			public: \
				NAME##_Desc() : PLCore::FuncDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					bool bRegister = static_cast<bool>(_Self::_RttiExport); \
					if (bRegister) \
						Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
				virtual PLCore::DynFuncPtr GetMethod(Object &cObject) const override { \
					return new NAME##_Method(reinterpret_cast<_Self&>(cObject)); \
				} \
			private: \
				FuncType m_cFunctor; \
		}; \

/**
*  @brief
*    Create class for a method
*
*  @param[in] NAME
*    Method name
*/
#define __pl_method_meth(NAME) \
		class NAME##_Method : public PLCore::Method<NAME##_Desc> { \
			public: \
				/* Cast away the const within the method pointer using a good old C-style cast to be as flexible as possible in here, if this is not done, only non-const methods can be exposed to the RTTI which isn't that comfortable */ \
				NAME##_Method() : PLCore::Method<NAME##_Desc>((NAME##_Desc::MethType::MemFuncType)(&_Self::NAME), nullptr) { \
					/* There are no automatic RTTI class method instances per RTTI class instance because there's no need for it and this safes RTTI class instance memory */ \
				} \
				/* Cast away the const within the method pointer using a good old C-style cast to be as flexible as possible in here, if this is not done, only non-const methods can be exposed to the RTTI which isn't that comfortable */ \
				NAME##_Method(_Self &cObject) : PLCore::Method<NAME##_Desc>((NAME##_Desc::MethType::MemFuncType)(&_Self::NAME), &cObject) { \
				} \
		}; \

/**
*  @brief
*    Create descriptor class for a signal
*
*  @param[in] NAME
*    Signal name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Signal description
*  @param[in] ANNOTATION
*    Signal annotation
*/
#define __pl_signal_desc(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::EventDesc { \
			public: \
				typedef _Self																			ClassType; \
				typedef PLCore::Event<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>			EventType; \
				typedef PLCore::Signature<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	SigType; \
			public: \
				NAME##_Desc() : PLCore::EventDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					bool bRegister = static_cast<bool>(_Self::_RttiExport); \
					if (bRegister) \
						Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::DynEvent *GetSignal(const Object &cObject) const override { \
					return &reinterpret_cast<_Self&>(const_cast<Object&>(cObject)).NAME; \
				} \
		}; \

/**
*  @brief
*    Create class for a signal
*
*  @param[in] NAME
*    Signal name
*/
#define __pl_signal_evnt(NAME) \
		class NAME##_Signal : public PLCore::Signal<NAME##_Desc> { \
			public: \
				NAME##_Signal() : PLCore::Signal<NAME##_Desc>() { \
				} \
		}; \

/**
*  @brief
*    Create signal variable
*
*  @param[in] NAME
*    Signal name
*/
#define __pl_signal_decl(NAME) \
		NAME##_Signal NAME; \

/**
*  @brief
*    Create descriptor class for a slot
*
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event handler description
*  @param[in] ANNOTATION
*    Event handler annotation
*/
#define __pl_slot_desc(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::EventHandlerDesc { \
			public: \
				typedef _Self																																ClassType; \
				typedef PLCore::EventHandler<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>															EventHandlerType; \
				typedef PLCore::Signature<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>														SigType; \
				typedef PLCore::ClassTypelist< _Self, PLCore::Typelist<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15> >::TypeMethodSignature	MethType; \
			public: \
				NAME##_Desc() : PLCore::EventHandlerDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					bool bRegister = static_cast<bool>(_Self::_RttiExport); \
					if (bRegister) \
						Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::DynEventHandler *GetSlot(const Object &cObject) const override { \
					return &reinterpret_cast<_Self&>(const_cast<Object&>(cObject)).Slot##NAME; \
				} \
		}; \

/**
*  @brief
*    Create class for a slot
*
*  @param[in] NAME
*    Slot name
*/
#define __pl_slot_evth(NAME) \
		class NAME##_Slot : public PLCore::Slot<NAME##_Desc> { \
			public: \
				NAME##_Slot(_Self *pObject) : PLCore::Slot<NAME##_Desc>(&_Self::NAME, pObject) { \
				} \
		}; \

/**
*  @brief
*    Create slot variable
*
*  @param[in] NAME
*    Event handler name
*/
#define __pl_slot_decl(NAME) \
		NAME##_Slot Slot##NAME; \

/**
*  @brief
*    Create descriptor class for a constructor
*
*  @param[in] NAME
*    Constructor name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Constructor description
*  @param[in] ANNOTATION
*    Constructor annotation
*/
#define __pl_constructor_desc(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::ConstructorDesc { \
			public: \
				typedef _Self																					ClassType; \
				typedef PLCore::Params<Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>			ParamType; \
				typedef const PLCore::Params<Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>		ConstParamType; \
				typedef PLCore::FuncConstructor<_Self,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	ConstType; \
				typedef PLCore::Signature<Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>		SigType; \
			public: \
				NAME##_Desc() : PLCore::ConstructorDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					bool bRegister = static_cast<bool>(_Self::_RttiExport); \
					if (bRegister) \
						Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::DynFunc *GetConstructor() const override { \
					return const_cast<PLCore::DynFunc*>(reinterpret_cast<const PLCore::DynFunc*>(&m_Constructor)); \
				} \
				virtual PLCore::Object *Create(const PLCore::DynParams &cConstParams) override { \
					if (cConstParams.GetSignature() == m_Constructor.GetSignature()) { \
						ParamType cParams = static_cast<ConstParamType&>(cConstParams); \
						m_Constructor.Call(cParams); \
						return static_cast<ParamType&>(cParams).Return; \
					} else return nullptr; \
				} \
				virtual PLCore::Object *Create(const PLCore::String &sConstParams) override { \
					ParamType cParams = ConstParamType::FromString(sConstParams); \
					m_Constructor.Call(cParams); \
					return cParams.Return; \
				} \
				ConstType m_Constructor; \
		}; \

/**
*  @brief
*    Create class for an event handler
*
*  @param[in] NAME
*    Event handler name
*/
#define __pl_constructor_cons(NAME) \
		class NAME##_Constructor : public PLCore::Constructor<NAME##_Desc> { \
			public: \
				NAME##_Constructor() { \
				} \
		}; \


//[-------------------------------------------------------]
//[ RTTI macros                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get current module ID
*/
#define pl_current_module_id() ( PLCore::ModuleID<int>::GetModuleID() )

/**
*  @brief
*    Declare enumeration type
*
*  @param[in] ENUM
*    Enumeration name
*/
#define pl_enum(ENUM) \
	__pl_enum(ENUM) \

/**
*  @brief
*    Declare direct enumeration type (not using enum{}, e.g. for float 'enums')
*
*  @param[in] ENUM
*    Enumeration name
*  @param[in] TYPE
*    Enumeration type
*/
#define pl_enum_direct(ENUM, TYPE) \
	__pl_enum_direct(ENUM, TYPE) \

/**
*  @brief
*    Declare base enumeration type (add all values of an existing enumeration type)
*
*  @param[in] ENUM
*    Enumeration name of base data type
*/
#define pl_enum_base(ENUM) \
	__pl_enum_base(ENUM) \

/**
*  @brief
*    Declare enumeration value
*
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define pl_enum_value(VALUE, DESCRIPTION) \
	__pl_enum_value(VALUE, DESCRIPTION) \

/**
*  @brief
*    Add enumeration value by directly specifying the value
*
*  @param[in] NAME
*    Enumeration name
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define pl_enum_value_direct(NAME, VALUE, DESCRIPTION) \
	__pl_enum_value_direct(NAME, VALUE, DESCRIPTION) \

/**
*  @brief
*    End enumeration
*/
#define pl_enum_end \
	__pl_enum_end \

/**
*  @brief
*    Declare class
*
*  @param[in] EXPORT
*    Export definition (must be defined as either 1 or 0)
*  @param[in] CLASS
*    Class name (without namespace)
*  @param[in] NAMESPACE
*    Namespace
*  @param[in] BASECLASS
*    Base class name (with namespace)
*  @param[in] DESCRIPTION
*    Class description
*/
#define pl_class(EXPORT, CLASS, NAMESPACE, BASECLASS, DESCRIPTION) \
	__pl_class(CLASS, NAMESPACE, BASECLASS, #BASECLASS, DESCRIPTION) \
	__pl_guard(CLASS) \
	__pl_getclass() \
	__pl_rtti_export(EXPORT) \

/**
*  @brief
*    Export RTTI class (use in macro pl_class)
*
*  @remarks
*    Use pl_rtti_export to mark a class as being exported to other libraries/applications. Usually, you should
*    define this in a global header file for your project to either pl_rtti_export(1) or pl_rtti_import(0),
*    depending on whether the project is currently built itself or imported into another project (just as the
*    dllimport/dllexport macros). However, if your project is an application that is not included anywhere else, you can
*    directly pass pl_rtti_export to pl_class, as your classes are only exported and never imported.
*/
#define pl_rtti_export 1

/**
*  @brief
*    Export RTTI class (use in macro pl_class)
*
*  @remarks
*    Use pl_rtti_import to mark a class as being imported by other libraries/applications (see pl_rtti_export).
*/
#define pl_rtti_import 0

/**
*  @brief
*    Declare class - internal (only for PLCore::Object)
*
*  @param[in] EXPORT
*    Export definition (must be defined as either 1 or 0)
*  @param[in] CLASS
*    Class name (without namespace)
*  @param[in] NAMESPACE
*    Namespace
*  @param[in] BASECLASS
*    Base class name (with namespace)
*  @param[in] DESCRIPTION
*    Class description
*
*  @remarks
*    This macro is only used for PLCore::Object. As this is the base class for all
*    RTTI classes, the virtual function GetClass() can not be overwritten here.
*/
#define pl_class_internal(EXPORT, CLASS, NAMESPACE, BASECLASS, DESCRIPTION) \
	__pl_class(CLASS, NAMESPACE, PLCore::ObjectBase, #BASECLASS, DESCRIPTION) \
	__pl_guard(CLASS) \
	__pl_getclass() \
	__pl_rtti_export(EXPORT) \

/**
*  @brief
*    Begin class properties
*/
#define pl_properties \
	__pl_prop_meth \

/**
*  @brief
*    Declare class property
*
*  @param[in] NAME
*    Property name
*  @param[in] VALUE
*    Property value
*/
#define pl_property(NAME, VALUE) \
	__pl_prop_prop(NAME, VALUE) \

/**
*  @brief
*    End class properties
*/
#define pl_properties_end \
	__pl_prop_class_end \

/**
*  @brief
*    Declare attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*  @param[in] STORAGE
*    Attribute storage type (DirectValue/GetSet/ModifyAttr)
*  @param[in] DESCRIPTION
*    Attribute description
*  @param[in] ANNOTATION
*    Attribute annotation
*/
#define pl_attribute(NAME, TYPE, DEFAULT, ACCESS, STORAGE, DESCRIPTION, ANNOTATION) \
	__pl_attr_stor(NAME, TYPE, STORAGE) \
	__pl_attr_desc(NAME, TYPE, DEFAULT, DESCRIPTION, ANNOTATION) \
	__pl_attr_attr(NAME, TYPE, DEFAULT, ACCESS, STORAGE) \
	__pl_attr_decl(NAME, TYPE) \

/**
*  @brief
*    Declare an enum type for an attribute
*
*  @param[in] ENUM
*    Name of enum type
*/
#define pl_enum_type(ENUM) PLCore::EnumType< ENUM##__plcore_enum__ >

/**
*  @brief
*    Declare a flags type for an attribute
*
*  @param[in] ENUM
*    Name of enum type
*/
#define pl_flag_type(ENUM) PLCore::FlagType< ENUM##__plcore_enum__ >

/**
*  @brief
*    Declare method
*
*  @param[in] NAME
*    Method name
*  @param[in] RET
*    Return type
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Method description
*  @param[in] ANNOTATION
*    Method annotation
*/
#define pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	private: /* RTTI class methods are private to avoid misuse */ \
	__pl_method_desc(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_method_meth(NAME) \
	public: \

#define pl_method_15(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_14(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_13(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_12(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_11(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_10(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_9(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_8(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_7(NAME, RET, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_6(NAME, RET, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_5(NAME, RET, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_4(NAME, RET, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_3(NAME, RET, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_2(NAME, RET, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_1(NAME, RET, T0, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_0(NAME, RET, DESCRIPTION, ANNOTATION) \
	pl_method_16(NAME, RET, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare signal
*
*  @param[in] NAME
*    Event name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event description
*  @param[in] ANNOTATION
*    Event annotation
*/
#define pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_signal_desc(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_signal_evnt(NAME) \
	__pl_signal_decl(NAME) \

#define pl_signal_15(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_14(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_13(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_12(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_11(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_10(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_9(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_8(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_7(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_6(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_5(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_4(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_3(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_2(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_1(NAME, T0, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_0(NAME, DESCRIPTION, ANNOTATION) \
	pl_signal_16(NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare slot
*
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event handler description
*  @param[in] ANNOTATION
*    Event handler annotation
*/
#define pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_slot_desc(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_slot_evth(NAME) \
	__pl_slot_decl(NAME) \

#define pl_slot_15(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_14(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_13(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_12(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_11(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_10(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_9(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_8(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_7(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_6(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_5(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_4(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_3(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_2(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_1(NAME, T0, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_0(NAME, DESCRIPTION, ANNOTATION) \
	pl_slot_16(NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare constructor
*
*  @param[in] NAME
*    Constructor name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Constructor  description
*  @param[in] ANNOTATION
*    Constructor  annotation
*/
#define pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_constructor_desc(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_constructor_cons(NAME) \

#define pl_constructor_15(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_14(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_13(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_12(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_11(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_10(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_9(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_8(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_7(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_6(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_5(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_4(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_3(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_2(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_1(NAME, T0, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_0(NAME, DESCRIPTION, ANNOTATION) \
	pl_constructor_16(NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare return type for a method
*
*  @param[in] RET
*    Return type
*
*  @remarks
*    This macro is there to provide a better readability of the RTTI declaration macros by marking the first type
*    explicitly as a return type. Actually you don't need to use it, but it is recommended to do so :-)
*/
#define pl_ret_type(RET) RET

/**
*  @brief
*    End class declaration
*/
#define pl_class_end

/**
*  @brief
*    Define class
*
*  @param[in] CLASS
*    Class name (without namespace)
*/
#define pl_implement_class(CLASS) \
	__pl_class_impl(CLASS) \


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_RTTI_H__
