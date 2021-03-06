/*********************************************************\
 *  File: PLIntrospectionModel.cpp                       *
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
#include <PLCore/PLCoreDefinitions.h>
#include <PLCore/Base/Var/DynVar.h>
#include <PLCore/Base/Func/DynFunc.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/DataModels/TreeItemBase.h"
#include "PLFrontendQt/DataModels/HeaderTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarTreeItemsFactory.h"
#include "PLFrontendQt/DataModels/PLIntrospectionModel.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


PLIntrospectionModel::PLIntrospectionModel(QObject *parent) : TreeModelBase(new HeaderTreeItem, parent)
{
	QStringList headerItems;
	headerItems << "Attribute" << "Value";

	HeaderTreeItem *header = static_cast<HeaderTreeItem*>(GetRootItem());
	header->setHeaderItems(headerItems);
}

PLIntrospectionModel::PLIntrospectionModel(DataModels::TreeItemBase *rootItem, QObject *parent) : TreeModelBase(rootItem, parent)
{
}

void PLIntrospectionModel::SetObject(PLCore::Object *Obj)
{
	beginResetModel();
	GetRootItem()->clearChildren();

	if (Obj) {
		PLCore::List<PLCore::DynFuncPtr> funcs;
		Obj->GetMethods(funcs);

		PLCore::List<PLCore::DynVarPtr> list = Obj->GetAttributes();
		for (PLCore::uint32 i=0; i<list.GetNumOfElements(); i++) {
			PLCore::DynVarPtr var = list[i];
			PLDynVarTreeItemsFactory::CreateDynVarTreeItem(var, GetRootItem());
		}
	}

	endResetModel();
}

void PLIntrospectionModel::UpdateModel()
{
	dataChanged(this->index(0,1), this->index(GetRootItem()->children().count(), 1));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
