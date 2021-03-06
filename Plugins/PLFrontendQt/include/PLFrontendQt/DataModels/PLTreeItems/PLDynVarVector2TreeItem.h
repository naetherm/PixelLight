/*********************************************************\
 *  File: PLDynVarVector2TreeItem.h                      *
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


#ifndef __PLFRONTENDQT_PLDYNVARVECTOR2TREEITEM_H__
#define __PLFRONTENDQT_PLDYNVARVECTOR2TREEITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include "PLFrontendQt/DataModels//PLTreeItems/PLDynVarStringTreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dynamic variable tree item: Vector2
*/
class PLFRONTENDQT_API PLDynVarVector2TreeItem : public PLDynVarStringTreeItem {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		explicit PLDynVarVector2TreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent = nullptr);
		virtual QVariant data(const int column, const int role) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		void UpdateData();
		void UpdateDataToVar();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector2 m_Vector2;


	//[-------------------------------------------------------]
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		class Vector2ValueBaseTreeItem: public TreeItemBase {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector2ValueBaseTreeItem(const QString &name, TreeItemBase *parent, PLDynVarVector2TreeItem *parentItem);
				virtual QVariant data(const int column, const int role) override;
				bool setData(const int column, const QVariant &value, const int role);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector2ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				virtual float GetVectorData(const PLMath::Vector2 &vector) = 0;
				virtual void SetVectorData(PLMath::Vector2 &vector, float newValue) = 0;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				QString					 m_Name;
				PLDynVarVector2TreeItem *m_parentItem;


		};

		class Vector2XValueTreeItem : public Vector2ValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector2XValueTreeItem(TreeItemBase *parent, PLDynVarVector2TreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector2ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				virtual float GetVectorData(const PLMath::Vector2 &vector) override;
				virtual void SetVectorData(PLMath::Vector2 &vector, float newValue) override;


		};

		class Vector2YValueTreeItem : public Vector2ValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector2YValueTreeItem(TreeItemBase *parent, PLDynVarVector2TreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector2ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				virtual float GetVectorData(const PLMath::Vector2 &vector) override;
				virtual void SetVectorData(PLMath::Vector2 &vector, float newValue) override;


		};
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt


#endif // __PLFRONTENDQT_PLDYNVARVECTOR2TREEITEM_H__
