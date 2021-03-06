/*********************************************************\
 *  File: PLDynVarNativNumericTypeTreeItem.cpp           *
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
#include <PLCore/Base/Var/DynVar.h>
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarNativNumericTypeTreeItem.h"
#include <PLCore/PLCoreDefinitions.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


PLDynVarNativNumericTypeTreeItem::PLDynVarNativNumericTypeTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, GetNumericTypeFromDynVar(dynVar), parent)
	, m_basisTypeId(dynVar ? dynVar->GetTypeID() : -1)
{
	if (m_basisTypeId == PLCore::TypeBool)
		SetFlags(1, Qt::ItemIsUserCheckable);
}

QVariant PLDynVarNativNumericTypeTreeItem::data(const int column, const int role)
{
 	if (column == 1 && m_basisTypeId == PLCore::TypeBool)
	{
		if(role == Qt::CheckStateRole)
			return m_dynVar->GetBool() ? Qt::Checked : Qt::Unchecked;
		else
			return QVariant(); 
	}
	
	return (IsInStandardRole(role) && column == 1) ? getBaseTypeData() : PLDynVarStringTreeItem::data(column, role);
}

bool PLDynVarNativNumericTypeTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role == Qt::CheckStateRole && column == 1 && m_basisTypeId == PLCore::TypeBool)
		return setBaseTypeData(value);
	
	if (role == Qt::EditRole && column == 1 && m_basisTypeId != PLCore::TypeBool)
		return setBaseTypeData(value);

	
// 	float floatVal = value.value<float>();
// 	m_dynVar->SetFloat(floatVal);
 	return false;
}

QVariant PLDynVarNativNumericTypeTreeItem::getBaseTypeData()
{
	switch(m_basisTypeId)
	{
		case PLCore::TypeBool:
			return m_dynVar->GetBool();
		case PLCore::TypeDouble:
			return m_dynVar->GetDouble();
		case PLCore::TypeFloat:
			return (double)m_dynVar->GetFloat(); // return float values as double this makes it easier to display a working editor
		case PLCore::TypeInt8:
			return (qint8)m_dynVar->GetInt8();
		case PLCore::TypeInt16:
			return (qint16)m_dynVar->GetInt16();
		case PLCore::TypeInt32:
		case PLCore::TypeInt:
			return (int)m_dynVar->GetInt32();
		case PLCore::TypeInt64:
			return (qint64)m_dynVar->GetInt64();
		case PLCore::TypeUInt8:
			return (quint8)m_dynVar->GetUInt8();
		case PLCore::TypeUInt16:
			return (quint16)m_dynVar->GetUInt16();
		case PLCore::TypeUInt32:
			return (unsigned int)m_dynVar->GetUInt32();
		case PLCore::TypeUInt64:
			return (quint64)m_dynVar->GetUInt64();
		default:
			return QVariant();
	}
}

bool PLDynVarNativNumericTypeTreeItem::setBaseTypeData(const QVariant& value)
{
	bool bRet = true;
	switch(m_basisTypeId)
	{
		case PLCore::TypeBool:
			m_dynVar->SetBool(value.value<bool>());
			break;
		case PLCore::TypeDouble:
			m_dynVar->SetDouble(value.value<double>());
			break;
		case PLCore::TypeFloat:
			m_dynVar->SetFloat(value.value<float>());
			break;
		case PLCore::TypeInt8:
			m_dynVar->SetInt8(value.value<PLCore::int8>());
			break;
		case PLCore::TypeInt16:
			m_dynVar->SetInt16(value.value<PLCore::int16>());
			break;
		case PLCore::TypeInt32:
		case PLCore::TypeInt:
			m_dynVar->SetInt32(value.value<int>());
			break;
		case PLCore::TypeInt64:
			m_dynVar->SetInt64(value.value<PLCore::int64>());
			break;
		case PLCore::TypeUInt8:
			m_dynVar->SetUInt8(value.value<PLCore::uint8>());
			break;
		case PLCore::TypeUInt16:
			m_dynVar->SetUInt16(value.value<PLCore::uint16>());
			break;
		case PLCore::TypeUInt32:
			m_dynVar->SetUInt32(value.value<unsigned int>());
			break;
		case PLCore::TypeUInt64:
			m_dynVar->SetUInt64(value.value<PLCore::uint64>());
			break;
		default:
			bRet = false;
			break;
	}
	return bRet;
}

PLDynVarTreeItemTypes::DynVarTreeItemTypes PLDynVarNativNumericTypeTreeItem::GetNumericTypeFromDynVar(const PLCore::DynVarPtr dynVar)
{
	int typeId = dynVar ? dynVar->GetTypeID() : -1;
	switch(typeId)
	{
		case PLCore::TypeBool:
			return PLDynVarTreeItemTypes::Bool;
		case PLCore::TypeDouble:
			return PLDynVarTreeItemTypes::Double;
		case PLCore::TypeFloat:
			return PLDynVarTreeItemTypes::Float;
		case PLCore::TypeInt8:
			return PLDynVarTreeItemTypes::Int8;
		case PLCore::TypeInt16:
			return PLDynVarTreeItemTypes::Int16;
		case PLCore::TypeInt32:
		case PLCore::TypeInt:
			return PLDynVarTreeItemTypes::Int32;
		case PLCore::TypeInt64:
			return PLDynVarTreeItemTypes::Int64;
		case PLCore::TypeUInt8:
			return PLDynVarTreeItemTypes::UInt8;
		case PLCore::TypeUInt16:
			return PLDynVarTreeItemTypes::UInt16;
		case PLCore::TypeUInt32:
			return PLDynVarTreeItemTypes::UInt32;
		case PLCore::TypeUInt64:
			return PLDynVarTreeItemTypes::UInt64;
		default:
			return PLDynVarTreeItemTypes::String;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
