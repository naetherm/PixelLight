/*********************************************************\
 *  File: PLDynVarVector4TreeItem.cpp                    *
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
#include <PLMath/Vector4.h>
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector4TreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


PLDynVarVector4TreeItem::PLDynVarVector4TreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, PLDynVarTreeItemTypes::Vector4, parent)
{
	RemoveFlags(1,  Qt::ItemIsEditable);
	if (m_dynVar && m_dynVar->GetTypeID() == PLCore::Type<PLMath::Vector4>::TypeID) {
		new Vector4XValueTreeItem(this, this);
		new Vector4YValueTreeItem(this, this);
		new Vector4ZValueTreeItem(this, this);
		new Vector4WValueTreeItem(this, this);
	}
}

QVariant PLDynVarVector4TreeItem::data(const int column, const int role)
{
	UpdateData();
	return PLDynVarStringTreeItem::data(column, role);
}

void PLDynVarVector4TreeItem::UpdateData()
{
	m_Vector4.FromString(m_dynVar->GetString());
}

void PLDynVarVector4TreeItem::UpdateDataToVar()
{
	m_dynVar->SetString(m_Vector4.ToString());
}

PLDynVarVector4TreeItem::Vector4ValueBaseTreeItem::Vector4ValueBaseTreeItem(const QString &name, TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem) : TreeItemBase(2, parent),
	m_Name(name),
	m_parentItem(parentItem)
{
	SetFlags(1, Qt::ItemIsEditable);
}

QVariant PLDynVarVector4TreeItem::Vector4ValueBaseTreeItem::data(const int column, const int role)
{
	// Don't show tooltip for Vector sub Items in column 0
	if (!m_parentItem->IsInStandardRole(role) || (role == Qt::ToolTipRole && column == 0))
		return QVariant();

	if(m_parentItem == nullptr)
		return QVariant();

	if (column == 0) {
		return m_Name;
	} else if (column == 1) {
		return (double)GetVectorData(m_parentItem->m_Vector4);
	}

	return QVariant();
}

bool PLDynVarVector4TreeItem::Vector4ValueBaseTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role != Qt::EditRole || column != 1)
		return false;

	float floatVal = value.value<float>();
	SetVectorData(m_parentItem->m_Vector4, floatVal);
	m_parentItem->UpdateDataToVar();
	return true;
}

PLDynVarVector4TreeItem::Vector4XValueTreeItem::Vector4XValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem) : Vector4ValueBaseTreeItem("X", parent, parentItem)
{
}

float PLDynVarVector4TreeItem::Vector4XValueTreeItem::GetVectorData(const PLMath::Vector4 &vector)
{
	return vector.x;
}

void PLDynVarVector4TreeItem::Vector4XValueTreeItem::SetVectorData(PLMath::Vector4 &vector, float newValue)
{
	vector.SetX(newValue);
}

PLDynVarVector4TreeItem::Vector4YValueTreeItem::Vector4YValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem) : Vector4ValueBaseTreeItem("Y", parent, parentItem)
{
}

float PLDynVarVector4TreeItem::Vector4YValueTreeItem::GetVectorData(const PLMath::Vector4 &vector)
{
	return vector.y;
}

void PLDynVarVector4TreeItem::Vector4YValueTreeItem::SetVectorData(PLMath::Vector4 &vector, float newValue)
{
	vector.SetY(newValue);
}

PLDynVarVector4TreeItem::Vector4ZValueTreeItem::Vector4ZValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem) : Vector4ValueBaseTreeItem("Z", parent, parentItem)
{
}

float PLDynVarVector4TreeItem::Vector4ZValueTreeItem::GetVectorData(const PLMath::Vector4 &vector)
{
	return vector.z;
}

void PLDynVarVector4TreeItem::Vector4ZValueTreeItem::SetVectorData(PLMath::Vector4 &vector, float newValue)
{
	vector.SetZ(newValue);
}

PLDynVarVector4TreeItem::Vector4WValueTreeItem::Vector4WValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem) : Vector4ValueBaseTreeItem("W", parent, parentItem)
{
}

float PLDynVarVector4TreeItem::Vector4WValueTreeItem::GetVectorData(const PLMath::Vector4 &vector)
{
	return vector.w;
}

void PLDynVarVector4TreeItem::Vector4WValueTreeItem::SetVectorData(PLMath::Vector4 &vector, float newValue)
{
	vector.SetW(newValue);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
