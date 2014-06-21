/*********************************************************\
 *  File: DynVar.h                                       *
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


#ifndef __PLCORE_DYNVAR_H__
#define __PLCORE_DYNVAR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class DynTypeInfo;
class VarDesc;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for variables
*
*  @remarks
*    This is the virtual base class to access variables and attributes dynamically.
*    It is a virtual interface that allows you to access a variable regardless
*    of it's actual type and storage.
*/
class DynVar {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API DynVar();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~DynVar();


	//[-------------------------------------------------------]
	//[ Public virtual DynVar functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get variable descriptor
		*
		*  @return
		*    Descriptor (can be a null pointer)
		*/
		PLCORE_API virtual const VarDesc *GetDesc() const;

		/**
		*  @brief
		*    Get type
		*
		*  @return
		*    Type info
		*/
		PLCORE_API virtual DynTypeInfo &GetType() const;

		/**
		*  @brief
		*    Get type ID
		*
		*  @return
		*    Type ID
		*/
		PLCORE_API virtual int GetTypeID() const;

		/**
		*  @brief
		*    Get type name
		*
		*  @return
		*    Type name (e.g. "int")
		*/
		PLCORE_API virtual String GetTypeName() const;

		/**
		*  @brief
		*    Check if variable is set to default value
		*
		*  @return
		*    'true' if default value is set, else 'false'
		*/
		PLCORE_API virtual bool IsDefault() const;

		/**
		*  @brief
		*    Set variable to default value
		*/
		PLCORE_API virtual void SetDefault();

		/**
		*  @brief
		*    Get default value as string
		*
		*  @return
		*    Default value as string
		*/
		PLCORE_API virtual String GetDefault() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] cValue
		*    Value as dynamic var
		*/
		PLCORE_API virtual void SetVar(const DynVar &cValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as bool
		*/
		PLCORE_API virtual bool GetBool() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] bValue
		*    Value as bool
		*/
		PLCORE_API virtual void SetBool(bool bValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int
		*/
		PLCORE_API virtual int GetInt() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int
		*/
		PLCORE_API virtual void SetInt(int nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int8
		*/
		PLCORE_API virtual int8 GetInt8() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int8
		*/
		PLCORE_API virtual void SetInt8(int8 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int16
		*/
		PLCORE_API virtual int16 GetInt16() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int16
		*/
		PLCORE_API virtual void SetInt16(int16 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int32
		*/
		PLCORE_API virtual int32 GetInt32() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int32
		*/
		PLCORE_API virtual void SetInt32(int32 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int64
		*/
		PLCORE_API virtual int64 GetInt64() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int64
		*/
		PLCORE_API virtual void SetInt64(int64 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint8
		*/
		PLCORE_API virtual uint8 GetUInt8() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint8
		*/
		PLCORE_API virtual void SetUInt8(uint8 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint16
		*/
		PLCORE_API virtual uint16 GetUInt16() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint16
		*/
		PLCORE_API virtual void SetUInt16(uint16 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint32
		*/
		PLCORE_API virtual uint32 GetUInt32() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint32
		*/
		PLCORE_API virtual void SetUInt32(uint32 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint64
		*/
		PLCORE_API virtual uint64 GetUInt64() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint64
		*/
		PLCORE_API virtual void SetUInt64(uint64 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint_ptr
		*/
		PLCORE_API virtual uint_ptr GetUIntPtr() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint_ptr
		*/
		PLCORE_API virtual void SetUIntPtr(uint_ptr nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as float
		*/
		PLCORE_API virtual float GetFloat() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] fValue
		*    Value as float
		*/
		PLCORE_API virtual void SetFloat(float fValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as double
		*/
		PLCORE_API virtual double GetDouble() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] dValue
		*    Value as double
		*/
		PLCORE_API virtual void SetDouble(double dValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as string
		*/
		PLCORE_API virtual String GetString() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] sValue
		*    Value as string
		*/
		PLCORE_API virtual void SetString(const String &sValue);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_DYNVAR_H__
