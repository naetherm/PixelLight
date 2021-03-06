/*********************************************************\
 *  File: PLDynVarVector3iTreeItem.cpp                   *
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
#include <PLMath/Vector3i.h>
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector3iTreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


PLDynVarVector3iTreeItem::PLDynVarVector3iTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, PLDynVarTreeItemTypes::Vector3i, parent)
{
	RemoveFlags(1,  Qt::ItemIsEditable);
	if (m_dynVar && m_dynVar->GetTypeID() == PLCore::Type<PLMath::Vector3i>::TypeID) {
		new Vector3iXValueTreeItem(this, this);
		new Vector3iYValueTreeItem(this, this);
		new Vector3iZValueTreeItem(this, this);
	}
}

QVariant PLDynVarVector3iTreeItem::data(const int column, const int role)
{
	UpdateData();
	return PLDynVarStringTreeItem::data(column, role);
}

void PLDynVarVector3iTreeItem::UpdateData()
{
	m_Vector3i.FromString(m_dynVar->GetString());
}

void PLDynVarVector3iTreeItem::UpdateDataToVar()
{
	m_dynVar->SetString(m_Vector3i.ToString());
}

PLDynVarVector3iTreeItem::Vector3iValueBaseTreeItem::Vector3iValueBaseTreeItem(const QString& name, PLFrontendQt::DataModels::TreeItemBase* parent, PLFrontendQt::DataModels::PLDynVarVector3iTreeItem* parentItem) : TreeItemBase(2, parent),
	m_Name(name),
	m_parentItem(parentItem)
{
	SetFlags(1, Qt::ItemIsEditable);
}

QVariant PLDynVarVector3iTreeItem::Vector3iValueBaseTreeItem::data(const int column, const int role)
{
	// Don't show tooltip for Vector sub Items in column 0
	if (!m_parentItem->IsInStandardRole(role) || (role == Qt::ToolTipRole && column == 0))
		return QVariant();

	if(m_parentItem == nullptr)
		return QVariant();

	if (column == 0) {
		return m_Name;
	} else if (column == 1) {
		return (double)GetVectorData(m_parentItem->m_Vector3i);
	}

	return QVariant();
}

bool PLDynVarVector3iTreeItem::Vector3iValueBaseTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role != Qt::EditRole || column != 1)
		return false;

	float floatVal = value.value<float>();
	SetVectorData(m_parentItem->m_Vector3i, floatVal);
	m_parentItem->UpdateDataToVar();
	return true;
}

PLDynVarVector3iTreeItem::Vector3iXValueTreeItem::Vector3iXValueTreeItem(TreeItemBase* parent, PLFrontendQt::DataModels::PLDynVarVector3iTreeItem* parentItem) : Vector3iValueBaseTreeItem("X", parent, parentItem)
{
}

int PLDynVarVector3iTreeItem::Vector3iXValueTreeItem::GetVectorData(const PLMath::Vector3i &vector)
{
	return vector.x;
}

void PLDynVarVector3iTreeItem::Vector3iXValueTreeItem::SetVectorData(PLMath::Vector3i &vector, int newValue)
{
	vector.SetX(newValue);
}

PLDynVarVector3iTreeItem::Vector3iYValueTreeItem::Vector3iYValueTreeItem(TreeItemBase* parent, PLFrontendQt::DataModels::PLDynVarVector3iTreeItem* parentItem) : Vector3iValueBaseTreeItem("Y", parent, parentItem)
{
}

int PLDynVarVector3iTreeItem::Vector3iYValueTreeItem::GetVectorData(const PLMath::Vector3i &vector)
{
	return vector.y;
}

void PLDynVarVector3iTreeItem::Vector3iYValueTreeItem::SetVectorData(PLMath::Vector3i &vector, int newValue)
{
	vector.SetY(newValue);
}

PLDynVarVector3iTreeItem::Vector3iZValueTreeItem::Vector3iZValueTreeItem(PLFrontendQt::DataModels::TreeItemBase* parent, PLFrontendQt::DataModels::PLDynVarVector3iTreeItem* parentItem) : Vector3iValueBaseTreeItem("Z", parent, parentItem)
{
}

int PLDynVarVector3iTreeItem::Vector3iZValueTreeItem::GetVectorData(const PLMath::Vector3i &vector)
{
	return vector.z;
}

void PLDynVarVector3iTreeItem::Vector3iZValueTreeItem::SetVectorData(PLMath::Vector3i &vector, int newValue)
{
	vector.SetZ(newValue);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
