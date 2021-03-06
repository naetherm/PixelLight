/*********************************************************\
 *  File: PLDynVarTreeItemsFactory.cpp                   *
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
#include <PLMath/Vector3i.h>
#include <PLGraphics/Color/Color3.h>
#include <PLGraphics/Color/Color4.h>
#include <PLCore/Base/TypeInfo/DynTypeInfo.h>
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarStringTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarFlagsTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector2TreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector3TreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector4TreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector2iTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarVector3iTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarNativNumericTypeTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarColorTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarEnumTreeItem.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarTreeItemsFactory.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {
using namespace PLCore;


TreeItemBase *PLDynVarTreeItemsFactory::CreateDynVarTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent)
{
	if (dynVar == nullptr)
		return nullptr;
	
	// Check Flags and Enums before BaseType because Flags and Enums are internally represented as int32
	if (dynVar->GetType().IsFlagType())
		return new PLDynVarFlagsTreeItem(dynVar, parent);
	else if(dynVar->GetType().IsEnumType())
		return new PLDynVarEnumTreeItem(dynVar, parent);
	else if (IsBaseType(*dynVar))
		return new PLDynVarNativNumericTypeTreeItem(dynVar, parent);
	else if (dynVar->GetTypeID() == PLCore::Type<PLMath::Vector2>::TypeID)
		return new PLDynVarVector2TreeItem(dynVar, parent);
	else if (dynVar->GetTypeID() == PLCore::Type<PLMath::Vector3>::TypeID)
		return new PLDynVarVector3TreeItem(dynVar, parent);
	else if (dynVar->GetTypeID() == PLCore::Type<PLMath::Vector4>::TypeID)
		return new PLDynVarVector4TreeItem(dynVar, parent);
	else if (dynVar->GetTypeID() == PLCore::Type<PLMath::Vector2i>::TypeID)
		return new PLDynVarVector2iTreeItem(dynVar, parent);
	else if (dynVar->GetTypeID() == PLCore::Type<PLMath::Vector3i>::TypeID)
		return new PLDynVarVector3iTreeItem(dynVar, parent);
	else if (dynVar->GetTypeID() == PLCore::Type<PLGraphics::Color3>::TypeID || dynVar->GetTypeID() == PLCore::Type<PLGraphics::Color4>::TypeID)
		return new PLDynVarColorTreeItem(dynVar, parent);
	else
		return new PLDynVarStringTreeItem(dynVar, PLFrontendQt::DataModels::PLDynVarTreeItemTypes::String, parent);

	return nullptr;
}

bool PLDynVarTreeItemsFactory::IsBaseType(const PLCore::DynVar &dynVar)
{
	int typeId = dynVar.GetTypeID();
	return (typeId == PLCore::TypeBool || typeId == PLCore::TypeFloat || typeId == PLCore::TypeDouble
			|| typeId == PLCore::TypeInt || typeId == PLCore::TypeInt32 || typeId == PLCore::TypeInt16 || typeId == PLCore::TypeInt64  || typeId == PLCore::TypeInt8
		   || typeId == PLCore::TypeUInt32 || typeId == PLCore::TypeUInt16 || typeId == PLCore::TypeUInt64  || typeId == PLCore::TypeUInt8);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
