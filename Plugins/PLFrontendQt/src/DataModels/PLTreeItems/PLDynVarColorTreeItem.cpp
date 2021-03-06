/*********************************************************\
 *  File: PLDynVarColorTreeItem.cpp                      *
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
#include <QtGui/QRgb>
#include <QtGui/QColor>
#include <PLCore/Base/Var/DynVar.h>
#include <PLGraphics/Color/Color3.h>
#include <PLGraphics/Color/Color4.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarColorTreeItem.h"


enum { ColorRole = 33 };


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


PLDynVarColorTreeItem::PLDynVarColorTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent) : PLDynVarStringTreeItem(dynVar, PLDynVarTreeItemTypes::Color, parent)
 , m_bHasAlpha(dynVar ? dynVar->GetTypeID() == PLCore::Type<PLGraphics::Color4>::TypeID : false)
{
	
}

QVariant PLDynVarColorTreeItem::data(const int column, const int role)
{
	if (column == 1) {
		if (role == ColorRole) {
			return GetQColorFromDynVar();
		}
		return QVariant();
	}

	return PLDynVarStringTreeItem::data(column, role);
}

bool PLDynVarColorTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role != Qt::EditRole || column != 1 || value.userType() != QMetaType::QColor)
		return false;

	if (!m_bHasAlpha)
	{
		QColor color = value.value<QColor>();
		PLGraphics::Color3 col((float)color.redF(), (float)color.greenF(), (float)color.blueF());
		m_dynVar->SetString(col.ToString());
	}
	else
	{
		QColor color = value.value<QColor>();
		PLGraphics::Color4 col((float)color.redF(), (float)color.greenF(), (float)color.blueF(), (float)color.alphaF());
		m_dynVar->SetString(col.ToString());
	}

	return true;
}

QColor PLDynVarColorTreeItem::GetQColorFromDynVar()
{
	if (!m_bHasAlpha)
	{
		PLGraphics::Color3 col;
		col.FromString(m_dynVar->GetString());
		QColor color(QColor::fromRgbF(col.GetR(), col.GetG(), col.GetB()));
		return color;
	}
	else
	{
		PLGraphics::Color4 col;
		col.FromString(m_dynVar->GetString());
		QColor color(QColor::fromRgbF(col.GetR(), col.GetG(), col.GetB(), col.GetA()));
		return color;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
