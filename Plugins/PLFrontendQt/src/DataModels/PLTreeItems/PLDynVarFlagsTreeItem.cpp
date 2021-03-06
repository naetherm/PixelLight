/*********************************************************\
 *  File: PLDynVarFlagsTreeItem.cpp                      *
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
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarFlagsTreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {
namespace DataModels {


PLDynVarFlagsTreeItem::PLDynVarFlagsTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, PLDynVarTreeItemTypes::Flags, parent)
{
	RemoveFlags(1, Qt::ItemIsEditable);

	PLCore::DynTypeInfo &typeInfo = m_dynVar->GetType();

	if (typeInfo.IsFlagType()) {
		// Get a list containing the names of all available flags
		const List<String> &enumList = typeInfo.GetEnumValues();

		// Loop thru all flags
		for (uint32 i=0; i<enumList.GetNumOfElements(); i++) {
			// Get the name of the current flag
			const String flagName = enumList[i];

			// Get the value of the current flag
			const uint32 flagValue = typeInfo.GetEnumValue(flagName).GetUInt32();

			// Add this flag item
			new FlagValueTreeItem(this, QtStringAdapter::PLToQt(flagName), flagValue, this);
		}
	}
}

QVariant PLDynVarFlagsTreeItem::data(const int column, const int role)
{
	UpdateData();
	return PLDynVarStringTreeItem::data(column, role);
}

void PLDynVarFlagsTreeItem::UpdateData()
{
	m_flagValues = m_dynVar->GetUInt32();
}

void PLDynVarFlagsTreeItem::UpdateDataToVar()
{
	m_dynVar->SetUInt32(m_flagValues);
}

PLDynVarFlagsTreeItem::FlagValueTreeItem::FlagValueTreeItem(TreeItemBase *parent, QString flagName, uint32 flagValue, PLDynVarFlagsTreeItem *parentItem) : TreeItemBase(2, parent),
	m_flagValue(flagValue),
	m_flagName(flagName),
	m_parentItem(parentItem)
{
	SetFlags(1, Qt::ItemIsUserCheckable);
}

QVariant PLDynVarFlagsTreeItem::FlagValueTreeItem::data(const int column, const int role)
{
	// Don't show tooltip for Flag sub Items in column 0
	if (m_parentItem == nullptr || column == 0 && role == Qt::ToolTipRole)
		return QVariant();

	if (column == 0 && m_parentItem->IsInStandardRole(role))
		return m_flagName;
	else if (column == 1 && role == Qt::CheckStateRole)
		return (m_parentItem->m_flagValues & m_flagValue) == m_flagValue ? Qt::Checked : Qt::Unchecked;
	else if (column == 1 && role == Qt::ToolTipRole)
		return (m_parentItem->m_flagValues & m_flagValue) == m_flagValue;
	return QVariant();
}

bool PLDynVarFlagsTreeItem::FlagValueTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role != Qt::CheckStateRole || column != 1)
		return false;

	bool boolVal = value.value<bool>();

	if (boolVal)
		m_parentItem->m_flagValues |= m_flagValue;
	else
		m_parentItem->m_flagValues &= ~m_flagValue;

	m_parentItem->UpdateDataToVar();
	return true;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
