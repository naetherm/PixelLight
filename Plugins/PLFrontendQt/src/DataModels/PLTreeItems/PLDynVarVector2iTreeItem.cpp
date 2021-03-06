/*********************************************************\
 *  File: PLDynVarVector2iTreeItem.cpp                   *
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
#include <PLMath/Vector2i.h>
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector2iTreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


PLDynVarVector2iTreeItem::PLDynVarVector2iTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, PLDynVarTreeItemTypes::Vector2i, parent)
{
	RemoveFlags(1,  Qt::ItemIsEditable);
	if (m_dynVar && m_dynVar->GetTypeID() == PLCore::Type<PLMath::Vector2i>::TypeID) {
		new Vector2iXValueTreeItem(this, this);
		new Vector2iYValueTreeItem(this, this);
	}
}

QVariant PLDynVarVector2iTreeItem::data(const int column, const int role)
{
	UpdateData();
	return PLDynVarStringTreeItem::data(column, role);
}

void PLDynVarVector2iTreeItem::UpdateData()
{
	m_Vector2i.FromString(m_dynVar->GetString());
}

void PLDynVarVector2iTreeItem::UpdateDataToVar()
{
	m_dynVar->SetString(m_Vector2i.ToString());
}

PLDynVarVector2iTreeItem::Vector2iValueBaseTreeItem::Vector2iValueBaseTreeItem(const QString &name, TreeItemBase *parent, PLDynVarVector2iTreeItem *parentItem) : TreeItemBase(2, parent),
	m_Name(name),
	m_parentItem(parentItem)
{
	SetFlags(1, Qt::ItemIsEditable);
}

QVariant PLDynVarVector2iTreeItem::Vector2iValueBaseTreeItem::data(const int column, const int role)
{
	// Don't show tooltip for Vector sub Items in column 0
	if (!m_parentItem->IsInStandardRole(role) || (role == Qt::ToolTipRole && column == 0))
		return QVariant();

	if(m_parentItem == nullptr)
		return QVariant();

	if (column == 0) {
		return m_Name;
	} else if (column == 1) {
		return (double)GetVectorData(m_parentItem->m_Vector2i);
	}

	return QVariant();
}

bool PLDynVarVector2iTreeItem::Vector2iValueBaseTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role != Qt::EditRole || column != 1)
		return false;

	int iloatVal = value.value<int>();
	SetVectorData(m_parentItem->m_Vector2i, iloatVal);
	m_parentItem->UpdateDataToVar();
	return true;
}

PLDynVarVector2iTreeItem::Vector2iXValueTreeItem::Vector2iXValueTreeItem(TreeItemBase* parent, PLFrontendQt::DataModels::PLDynVarVector2iTreeItem* parentItem) : Vector2iValueBaseTreeItem("X", parent, parentItem)
{
}

int PLDynVarVector2iTreeItem::Vector2iXValueTreeItem::GetVectorData(const PLMath::Vector2i &vector)
{
	return vector.x;
}

void PLDynVarVector2iTreeItem::Vector2iXValueTreeItem::SetVectorData(PLMath::Vector2i &vector, int newValue)
{
	vector.SetX(newValue);
}

PLDynVarVector2iTreeItem::Vector2iYValueTreeItem::Vector2iYValueTreeItem(TreeItemBase* parent, PLFrontendQt::DataModels::PLDynVarVector2iTreeItem* parentItem) : Vector2iValueBaseTreeItem("Y", parent, parentItem)
{
}

int PLDynVarVector2iTreeItem::Vector2iYValueTreeItem::GetVectorData(const PLMath::Vector2i& vector)
{
	return vector.y;
}

void PLDynVarVector2iTreeItem::Vector2iYValueTreeItem::SetVectorData(PLMath::Vector2i& vector, int newValue)
{
	vector.SetY(newValue);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
