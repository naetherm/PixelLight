/*********************************************************\
 *  File: PLDynVarEnumTreeItem.cpp                       *
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
#include <PLCore/Base/TypeInfo/DynTypeInfo.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarEnumTreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {
namespace DataModels {


PLDynVarEnumTreeItem::PLDynVarEnumTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, PLDynVarTreeItemTypes::Enum, parent)
{
	PLCore::DynTypeInfo &typeInfo = m_dynVar->GetType();

	if (typeInfo.IsEnumType()) {
		const List<String> &enumList = typeInfo.GetEnumValues();

		// Assumption that the order in the list equals to the bit which the flag represents
		for (uint32 i=0; i<enumList.GetNumOfElements(); i++) {
			QString str(QtStringAdapter::PLToQt(enumList[i]));
			QString sValue(QtStringAdapter::PLToQt(typeInfo.GetEnumValue(enumList[i])));
			m_enumValues.insert(str, sValue);
		}
	}
}

QVariant PLDynVarEnumTreeItem::data(const int column, const int role)
{
	if (column == 1 && role == PLDynVarTreeItemTypes::DynVarEnumValues)
		return m_enumValues;
	
	return PLDynVarStringTreeItem::data(column, role);
}

void PLDynVarEnumTreeItem::UpdateData()
{
	//m_flagValues = m_dynVar->GetUInt32();
}

void PLDynVarEnumTreeItem::UpdateDataToVar()
{
	//m_dynVar->SetUInt32(m_flagValues);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
