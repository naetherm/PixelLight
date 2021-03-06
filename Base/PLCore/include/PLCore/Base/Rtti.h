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
#include "PLCore/Base/Func/DescCreateStaticInstHelper.h"
#include "PLCore/Base/Func/FuncConstructorDynFunc.h"
#include "PLCore/Base/Event/Signal.h"
#include "PLCore/Base/Event/EventDynEvent.h"
#include "PLCore/Base/Event/Slot.h"
#include "PLCore/Base/Event/EventHandlerDynEventHandler.h"
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

#define pl_class_enum_metadata(CLASS, ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef int  _BaseType; \
			typedef CLASS::ENUM _Type; \
			\
			static bool GetEnumValue(int nIndex, CLASS::ENUM &nValue, PLCore::String &sName, PLCore::String &sDescription) { \
				int nCount = 0; \
				
#define pl_class_enum_metadata2(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef int  _BaseType; \
			typedef _Self::ENUM _Type; \
			\
			static bool GetEnumValue(int nIndex, _Self::ENUM &nValue, PLCore::String &sName, PLCore::String &sDescription) { \
				int nCount = 0; \

#define pl_class_flag_metadata2(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef PLCore::uint32  _BaseType; \
			typedef _Self::ENUM _Type; \
			\
			static bool GetEnumValue(int nIndex, _Self::ENUM &nValue, PLCore::String &sName, PLCore::String &sDescription) { \
				int nCount = 0; \

#define __pl_flag(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef PLCore::uint32  _BaseType; \
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

#define pl_class_enum_value(VALUE, DESCRIPTION) \
					 if (nIndex == nCount)										{ nValue = _Self::VALUE; sName = #VALUE; return true; } \
				else if (nIndex == -1 && !sName.GetLength() && nValue == _Self::VALUE)	{ sName = #VALUE; return true; } \
				else if (nIndex == -1 && sName == #VALUE)						{ nValue = _Self::VALUE; sDescription = DESCRIPTION; return true; } \
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
				static inline TYPE Get(PLCore::Object *pObject) { \
					return static_cast<CLASS*>(pObject)->Get##NAME(); \
				} \
				static inline void Set(PLCore::Object *pObject, const TYPE &Value) { \
					static_cast<CLASS*>(pObject)->Set##NAME(Value); \
				} \
		}; \
		\
		template <typename BASE> class NAME##_ModAttr { \
			public: \
				typedef PLCore::StorageModifyAttr						StorageType; \
				typedef typename BASE::NAME##_Storage::BaseStorageType	BaseStorageType; \
				static inline TYPE Get(PLCore::Object *pObject) { \
					return static_cast<BASE*>(pObject)->NAME.Get(); \
				} \
				static inline void Set(PLCore::Object *pObject, const TYPE &Value) { \
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
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*  @param[in] DESCRIPTION
*    Attribute description
*  @param[in] ANNOTATION
*    Attribute annotation
*/
#define __pl_attr_desc_metadata(NAME, TYPE, DEFAULT, ACCESS, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::VarDesc { \
			public: \
				NAME##_Desc() : PLCore::VarDesc(PLCore::Type<TYPE>::TypeID, PLCore::Type<TYPE>::GetTypeName(), #NAME, DESCRIPTION, ANNOTATION) { \
					Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::String GetDefault() const override { \
					return PLCore::Type<TYPE>::ConvertToString(DEFAULT); \
				} \
				virtual PLCore::DynVarPtr GetAttribute(const PLCore::Object &cObject) const override { \
					return new PLCore::AttributeWrapper<TYPE, PLCore::Access##ACCESS, _Self::NAME##_Attr::StorageType>(&reinterpret_cast<_Self&>(const_cast<PLCore::Object&>(cObject)).NAME, this); \
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
#define __pl_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, STORAGE) \
		class NAME##_Attr : public PLCore::Attribute<TYPE, PLCore::Access##ACCESS, STORAGE> { \
			public: \
				typedef STORAGE	StorageType; \
				NAME##_Attr(_Self *pObject) : PLCore::Attribute<TYPE, PLCore::Access##ACCESS, STORAGE>(DEFAULT, pObject) { \
				} \
				\
				NAME##_Attr &operator =(const TYPE &Value) { \
					return static_cast<NAME##_Attr&>(PLCore::Attribute<TYPE, PLCore::Access##ACCESS, STORAGE>::operator =(Value)); \
				} \
		}; \

/**
*  @brief
*    Create class for an attribute with DirectValue as Storage type
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define __pl_attr_attr_directvalue(NAME, TYPE, DEFAULT, ACCESS) \
		__pl_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, PLCore::StorageDirectValue)

/**
*  @brief
*    Create class and helper storage class for an attribute with GetSet as Storage type
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define __pl_attr_attr_getset(CLASSTYPE, NAME, TYPE, DEFAULT, ACCESS) \
		template <typename CLASS> class NAME##_GetSet { \
			public: \
				typedef PLCore::StorageGetSet StorageType; \
				typedef PLCore::StorageGetSet BaseStorageType; \
				static inline TYPE Get(PLCore::Object *pObject) { \
					return static_cast<CLASS*>(pObject)->Get##NAME(); \
				} \
				static inline void Set(PLCore::Object *pObject, const TYPE &Value) { \
					static_cast<CLASS*>(pObject)->Set##NAME(Value); \
				} \
		}; \
		\
		__pl_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, NAME##_GetSet<CLASSTYPE>)

/**
*  @brief
*    Create class and helper storage class for an attribute with ModifyAttr as Storage type
*
*  @param[in] BASETYPE
*    Name of the base class which contains the attribute to be modified by this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define __pl_attr_attr_modifyattr(BASETYPE, NAME, TYPE, DEFAULT, ACCESS) \
		template <typename BASE> class NAME##_ModAttr { \
			public: \
				typedef PLCore::StorageModifyAttr StorageType; \
				typedef typename BASE::NAME##_Attr::StorageType BaseStorageType; \
				static inline TYPE Get(PLCore::Object *pObject) { \
					return static_cast<BASE*>(pObject)->NAME.Get(); \
				} \
				static inline void Set(PLCore::Object *pObject, const TYPE &Value) { \
					static_cast<BASE*>(pObject)->NAME.Set(Value); \
				} \
		}; \
		\
		__pl_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, NAME##_ModAttr<BASETYPE>)

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
#define __pl_method_desc_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::FuncDesc { \
			public: \
				typedef _Self																																ClassType; \
				typedef PLCore::FunctorWrapper<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>													FuncWrapperType; \
				typedef PLCore::Signature<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>														SigType; \
				typedef PLCore::ClassTypelist< _Self, PLCore::Typelist<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15> >::TypeMethodSignature	MethType; \
			public: \
				NAME##_Desc() : PLCore::FuncDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
				virtual PLCore::DynFuncPtr GetMethod(PLCore::Object &cObject) const override { \
					return new NAME##_Method(reinterpret_cast<_Self&>(cObject)); \
				} \
		}; \

/**
*  @brief
*    Create class for a method
*
*  @param[in] NAME
*    Method name
*/
#define __pl_method_meth_metadata(NAME) \
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
#define __pl_signal_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::EventDesc { \
			public: \
				typedef PLCore::Signature<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	SigType; \
			public: \
				NAME##_Desc() : PLCore::EventDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::DynEventPtr GetSignal(const PLCore::Object &cObject) const override { \
					return new PLCore::EventWrapper<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>(&reinterpret_cast<_Self&>(const_cast<PLCore::Object&>(cObject)).NAME, this); \
				} \
		}; \

/**
*  @brief
*    Create class for a signal
*
*  @param[in] NAME
*    Signal name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the GetDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define __pl_signal_evnt_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
		typedef PLCore::Event<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	NAME##_EventType; \
		class NAME##_Signal : public PLCore::Signal<NAME##_EventType> { \
			public: \
				NAME##_Signal() : PLCore::Signal<NAME##_EventType>() { \
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
*  @param[in] CLASS
*    Name of class which contains the slot which is described by this descriptor
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event handler description
*  @param[in] ANNOTATION
*    Event handler annotation
*/
#define __pl_slot_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::EventHandlerDesc { \
			public: \
				typedef PLCore::Signature<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	SigType; \
			public: \
				NAME##_Desc() : PLCore::EventHandlerDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::DynEventHandlerPtr GetSlot(const PLCore::Object &cObject) const override { \
					return new PLCore::EventHandlerWrapper<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>(&reinterpret_cast<_Self&>(const_cast<PLCore::Object&>(cObject)).Slot##NAME, this); \
				} \
		}; \

/**
*  @brief
*    Create class for a slot
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this slot
*  @param[in] NAME
*    Slot name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the GetDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define __pl_slot_evth_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
		typedef PLCore::EventHandler<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>															NAME##_EventHandlerType; \
		typedef PLCore::ClassTypelist< CLASSTYPE, PLCore::Typelist<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15> >::TypeMethodSignature	NAME##_MethType; \
		\
		class NAME##_Slot : public PLCore::Slot<NAME##_EventHandlerType, CLASSTYPE, NAME##_MethType> { \
			public: \
				NAME##_Slot(CLASSTYPE *pObject) : PLCore::Slot<NAME##_EventHandlerType,CLASSTYPE, NAME##_MethType>(&CLASSTYPE::NAME, pObject) { \
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
#define __pl_constructor_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public PLCore::ConstructorDesc { \
			public: \
				typedef _Self																						ClassType; \
				typedef PLCore::Params<PLCore::Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>		ParamType; \
				typedef const PLCore::Params<PLCore::Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	ConstParamType; \
				typedef PLCore::FuncConstructorWrapper<_Self,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	ConstWrapperType; \
				typedef PLCore::Signature<PLCore::Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	SigType; \
			public: \
				NAME##_Desc() : PLCore::ConstructorDesc(SigType::GetSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) \
					, m_ConstructorWrapper(&m_Constructor) \
				{ \
					Register(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual PLCore::DynFunc *GetConstructor() const override { \
					return const_cast<ConstWrapperType*>(&m_ConstructorWrapper); \
				} \
				virtual PLCore::Object *Create(const PLCore::DynParams &cConstParams) override { \
					if (cConstParams.GetSignature() == m_ConstructorWrapper.GetSignature()) { \
						ParamType cParams = static_cast<ConstParamType&>(cConstParams); \
						m_ConstructorWrapper.Call(cParams); \
						return static_cast<ParamType&>(cParams).Return; \
					} else return nullptr; \
				} \
				virtual PLCore::Object *Create(const PLCore::String &sConstParams) override { \
					ParamType cParams = ConstParamType::FromString(sConstParams); \
					m_ConstructorWrapper.Call(cParams); \
					return cParams.Return; \
				} \
				ConstWrapperType::FuncConstructorType	m_Constructor; \
				ConstWrapperType 						m_ConstructorWrapper; \
		}; \

/**
*  @brief
*    Create class for an event handler
*
*  @param[in] NAME
*    Event handler name
*/
#define __pl_constructor_cons_metadata(NAME) \
		class NAME##_DescCreateStaticInstHelper : public PLCore::DescCreateStaticInstHelper<NAME##_Desc> { \
		public: \
			NAME##_DescCreateStaticInstHelper() : PLCore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
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
	
#define pl_flag(ENUM) \
	__pl_flag(ENUM) \

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
*    Declare accessor for the RTTI class for this class
*
*  @param[in] API_EXPORT
*    specifier if the GetClass Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define pl_class_def(API_EXPORT) \
	public: \
		__pl_getclass_def(API_EXPORT) \


/**
*  @brief
*    Begin class properties
*/
#define pl_properties \
		/* Class properties */ \
		static inline void _RegisterProperties(PLCore::ClassReal *pClass) { \

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
			static_cast<_Class*>(pClass)->AddProperty(NAME, VALUE); \

/**
*  @brief
*    End class properties
*/
#define pl_properties_end \
		} \

/**
*  @brief
*    Declare attribute with DirectValue as storage type
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define pl_attribute_directvalue(NAME, TYPE, DEFAULT, ACCESS) \
	__pl_attr_attr_directvalue(NAME, TYPE, DEFAULT, ACCESS) \
	__pl_attr_decl(NAME, TYPE) \

/**
*  @brief
*    Declare attribute with GetSet as storage type
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define pl_attribute_getset(CLASSTYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__pl_attr_attr_getset(CLASSTYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__pl_attr_decl(NAME, TYPE) \

/**
*  @brief
*    Declare attribute with ModifyAttr as storage type
*
*  @param[in] BASETYPE
*    Name of the base class which contains the attribute to be modified by this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define pl_attribute_modifyattr(BASETYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__pl_attr_attr_modifyattr(BASETYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__pl_attr_decl(NAME, TYPE) \

/**
*  @brief
*    Declare descriptor for an attribute
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
#define pl_attribute_metadata(NAME, TYPE, DEFAULT, ACCESS, DESCRIPTION, ANNOTATION) \
	__pl_attr_desc_metadata(NAME, TYPE, DEFAULT, ACCESS, DESCRIPTION, ANNOTATION) \
	class NAME##_DescCreateStaticInstHelper : public PLCore::DescCreateStaticInstHelper<NAME##_Desc> { \
	public: \
		NAME##_DescCreateStaticInstHelper() : PLCore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
	}; \

/**
*  @brief
*    Declare an enum type for an attribute
*
*  @param[in] ENUM
*    Name of enum type
*/
#define pl_enum_type(ENUM) PLCore::EnumType< ENUM##__plcore_enum__ >

#define pl_enum_type_def(ENUM) PLCore::EnumType< _Self::ENUM##__plcore_enum__ >

#define pl_enum_type_def2(CLASS, ENUM) PLCore::EnumType< CLASS##_ENUM##__plcore_enum__ >

#define pl_enum_type_def3(CLASS, ENUM) PLCore::EnumType< CLASS::ENUM##__plcore_enum__ >

#define pl_enum_type_def4(CLASS, ENUM) PLCore::EnumType< RTTI_##CLASS::ENUM##__plcore_enum__ >

/**
*  @brief
*    Declare a flags type for an attribute
*
*  @param[in] ENUM
*    Name of enum type
*/
#define pl_flag_type(ENUM) PLCore::FlagType< ENUM##__plcore_enum__ >

#define pl_flag_type_def3(CLASS, ENUM) PLCore::FlagType< CLASS::ENUM##__plcore_enum__ >

#define pl_flag_type_def4(CLASS, ENUM) PLCore::FlagType< RTTI_##CLASS::ENUM##__plcore_enum__ >


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
#define pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	/* RTTI class methods are private to avoid misuse */ \
	__pl_method_desc_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_method_meth_metadata(NAME) \

#define pl_method_15_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_14_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_13_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_12_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_11_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_10_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_9_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_8_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_7_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_6_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_5_metadata(NAME, RET, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_4_metadata(NAME, RET, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_3_metadata(NAME, RET, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_2_metadata(NAME, RET, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_1_metadata(NAME, RET, T0, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_method_0_metadata(NAME, RET, DESCRIPTION, ANNOTATION) \
	pl_method_16_metadata(NAME, RET, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare signal
*
*  @param[in] NAME
*    Event name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the GetDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__pl_signal_evnt_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__pl_signal_decl(NAME) \

#define pl_signal_15_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) \
	pl_signal_16_def(CLASS, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType)

#define pl_signal_14_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType)

#define pl_signal_13_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_12_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_11_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_10_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_9_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_8_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_7_def(NAME, T0, T1, T2, T3, T4, T5, T6) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_6_def(NAME, T0, T1, T2, T3, T4, T5) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_5_def(NAME, T0, T1, T2, T3, T4) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_4_def(NAME, T0, T1, T2, T3) \
	pl_signal_16_def(NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_3_def(NAME, T0, T1, T2) \
	pl_signal_16_def(NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_2_def(NAME, T0, T1) \
	pl_signal_16_def(NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_1_def(NAME, T0) \
	pl_signal_16_def(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_signal_0_def(NAME) \
	pl_signal_16_def(NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

/**
*  @brief
*    Declare metadata for a signal
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
#define pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_signal_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	class NAME##_DescCreateStaticInstHelper : public PLCore::DescCreateStaticInstHelper<NAME##_Desc> { \
	public: \
		NAME##_DescCreateStaticInstHelper() : PLCore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
	}; \

#define pl_signal_15_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_14_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_13_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_12_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_11_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_10_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_9_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_8_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_7_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_6_metadata(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_5_metadata(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_4_metadata(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_3_metadata(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_2_metadata(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_signal_0_metadata(NAME, DESCRIPTION, ANNOTATION) \
	pl_signal_16_metadata(NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare slot
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this slot
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the GetDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__pl_slot_evth_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__pl_slot_decl(NAME) \

#define pl_slot_15_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType)

#define pl_slot_14_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType)

#define pl_slot_13_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_12_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_11_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_10_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_9_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_8_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_7_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_6_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_5_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_4_def(CLASSTYPE, NAME, T0, T1, T2, T3) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_3_def(CLASSTYPE, NAME, T0, T1, T2) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_2_def(CLASSTYPE, NAME, T0, T1) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_1_def(CLASSTYPE, NAME, T0) \
	pl_slot_16_def(CLASSTYPE, NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

#define pl_slot_0_def(CLASSTYPE, NAME) \
	pl_slot_16_def(CLASSTYPE, NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType)

/**
*  @brief
*    Declare metadata for a slot
*
*  @param[in] CLASS
*    Name of class which contains the slot which is described by this metadata
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event handler description
*  @param[in] ANNOTATION
*    Event handler annotation
*/
#define pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_slot_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	class NAME##_DescCreateStaticInstHelper : public PLCore::DescCreateStaticInstHelper<NAME##_Desc> { \
	public: \
		NAME##_DescCreateStaticInstHelper() : PLCore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
	}; \

#define pl_slot_15_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_14_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_13_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_12_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_11_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_10_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_9_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_8_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_7_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_6_metadata(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_5_metadata(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_4_metadata(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_3_metadata(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_2_metadata(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_0_metadata(NAME, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_slot_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	pl_slot_16_metadata(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

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
#define pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_constructor_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__pl_constructor_cons_metadata(NAME) \

#define pl_constructor_15_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_14_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_13_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_12_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_11_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_10_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_9_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_8_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_7_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_6_metadata(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_5_metadata(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_4_metadata(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, T3, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_3_metadata(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, T2, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_2_metadata(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, T1, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, T0, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

#define pl_constructor_0_metadata(NAME, DESCRIPTION, ANNOTATION) \
	pl_constructor_16_metadata(NAME, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, PLCore::NullType, DESCRIPTION, ANNOTATION)

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

// Needed to avoid that every class have to include
// pl_properties
// pl_properties_end
// even a class which doesn't define properties
class RTTI_MetadataBase
{
	protected:
		static inline void _RegisterProperties(PLCore::ClassReal *pClass) { }
};


//[-------------------------------------------------------]
//[ Internal macros                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create virtual function GetClass()
*/
#define __pl_getclass_def(API_EXPORT) \
		/* Public virtual PLCore::Object function */ \
		virtual API_EXPORT PLCore::Class *GetClass() const override; \

#define __pl_getclass_impl_def(CLASS) \
		PLCore::Class * CLASS::GetClass() const \
		{ \
			return RTTI_##CLASS::_Class::GetSingleton()->GetClass(); \
		} \

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
#define __pl_class_metadata(CLASS, NAMESPACE, BASECLASS, BASECLASSNAME, DESCRIPTION) \
class RTTI_##CLASS : public PLCore::RTTI_MetadataBase { \
	/* All RTTI members are public */ \
	public: \
		/* Class type */ \
		typedef CLASS     _Self; \
		typedef BASECLASS _Base; \
		\
		/* Class description */ \
		class _Class : public PLCore::ClassReal { \
			friend class CLASS; \
			friend class RTTI_##CLASS; \
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
							RTTI_##CLASS::_RegisterProperties(MyInstance); \
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


//[-------------------------------------------------------]
//[ RTTI macros                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Declare metadata class
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
#define pl_class_metadata(CLASS, NAMESPACE, BASECLASS, DESCRIPTION) \
	__pl_class_metadata(CLASS, NAMESPACE, BASECLASS, #BASECLASS, DESCRIPTION) \
	__pl_guard(CLASS) \

#define pl_class_def_end \
	private:

/**
*  @brief
*    Declare metadata class - internal (only for PLCore::Object)
*
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
#define pl_class_def_internal(CLASS, API_EXPORT) \
	public: \
		typedef CLASS     _Self; \
		typedef PLCore::ObjectBase _Base; \
		__pl_getclass_def(API_EXPORT) \

#define pl_class_metadata_internal(CLASS, NAMESPACE, BASECLASS, DESCRIPTION) \
	__pl_class_metadata(CLASS, NAMESPACE, PLCore::ObjectBase, #BASECLASS, DESCRIPTION) \
	__pl_guard(CLASS) \

/**
*  @brief
*    End metadata class declaration
*/
#define pl_class_metadata_end(CLASS) \
	}; \
	\
	__pl_getclass_impl_def(CLASS) \
	/* Guard */ \
	RTTI_##CLASS::_Guard cGuard_##CLASS; \


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_RTTI_H__
