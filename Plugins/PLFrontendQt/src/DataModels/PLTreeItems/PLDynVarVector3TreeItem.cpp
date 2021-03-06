/*********************************************************\
 *  File: PLDynVarVector3TreeItem.cpp                    *
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
#include <PLMath/Vector3.h>
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector3TreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


PLDynVarVector3TreeItem::PLDynVarVector3TreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, PLDynVarTreeItemTypes::Vector3, parent)
{
	RemoveFlags(1,  Qt::ItemIsEditable);
	if (m_dynVar && m_dynVar->GetTypeID() == PLCore::Type<PLMath::Vector3>::TypeID) {
		new Vector3XValueTreeItem(this, this);
		new Vector3YValueTreeItem(this, this);
		new Vector3ZValueTreeItem(this, this);
	}
}

QVariant PLDynVarVector3TreeItem::data(const int column, const int role)
{
	UpdateData();
	return PLDynVarStringTreeItem::data(column, role);
}

void PLDynVarVector3TreeItem::UpdateData()
{
	m_Vector3.FromString(m_dynVar->GetString());
}

void PLDynVarVector3TreeItem::UpdateDataToVar()
{
	m_dynVar->SetString(m_Vector3.ToString());
}

PLDynVarVector3TreeItem::Vector3ValueBaseTreeItem::Vector3ValueBaseTreeItem(const QString &name, TreeItemBase *parent, PLDynVarVector3TreeItem *parentItem) : TreeItemBase(2, parent),
	m_Name(name),
	m_parentItem(parentItem)
{
	SetFlags(1, Qt::ItemIsEditable);
}

QVariant PLDynVarVector3TreeItem::Vector3ValueBaseTreeItem::data(const int column, const int role)
{
	// Don't show tooltip for Vector sub Items in column 0
	if (!m_parentItem->IsInStandardRole(role) || (role == Qt::ToolTipRole && column == 0))
		return QVariant();

	if(m_parentItem == nullptr)
		return QVariant();

	if (column == 0) {
		return m_Name;
	} else if (column == 1) {
		return (double)GetVectorData(m_parentItem->m_Vector3);
	}

	return QVariant();
}

bool PLDynVarVector3TreeItem::Vector3ValueBaseTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role != Qt::EditRole || column != 1)
		return false;

	float floatVal = value.value<float>();
	SetVectorData(m_parentItem->m_Vector3, floatVal);
	m_parentItem->UpdateDataToVar();
	return true;
}

PLDynVarVector3TreeItem::Vector3XValueTreeItem::Vector3XValueTreeItem(TreeItemBase *parent, PLDynVarVector3TreeItem *parentItem) : Vector3ValueBaseTreeItem("X", parent, parentItem)
{
}

float PLDynVarVector3TreeItem::Vector3XValueTreeItem::GetVectorData(const PLMath::Vector3 &vector)
{
	return vector.x;
}

void PLDynVarVector3TreeItem::Vector3XValueTreeItem::SetVectorData(PLMath::Vector3 &vector, float newValue)
{
	vector.SetX(newValue);
}

PLDynVarVector3TreeItem::Vector3YValueTreeItem::Vector3YValueTreeItem(TreeItemBase *parent, PLDynVarVector3TreeItem *parentItem) : Vector3ValueBaseTreeItem("Y", parent, parentItem)
{
}

float PLDynVarVector3TreeItem::Vector3YValueTreeItem::GetVectorData(const PLMath::Vector3 &vector)
{
	return vector.y;
}

void PLDynVarVector3TreeItem::Vector3YValueTreeItem::SetVectorData(PLMath::Vector3 &vector, float newValue)
{
	vector.SetY(newValue);
}

PLDynVarVector3TreeItem::Vector3ZValueTreeItem::Vector3ZValueTreeItem(TreeItemBase *parent, PLDynVarVector3TreeItem *parentItem) : Vector3ValueBaseTreeItem("Z", parent, parentItem)
{
}

float PLDynVarVector3TreeItem::Vector3ZValueTreeItem::GetVectorData(const PLMath::Vector3 &vector)
{
	return vector.z;
}

void PLDynVarVector3TreeItem::Vector3ZValueTreeItem::SetVectorData(PLMath::Vector3 &vector, float newValue)
{
	vector.SetZ(newValue);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
