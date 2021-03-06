/*********************************************************\
 *  File: PLDynVarVector4TreeItem.h                      *
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


#ifndef __PLFRONTENDQT_PLDYNVARVECTOR4TREEITEM_H__
#define __PLFRONTENDQT_PLDYNVARVECTOR4TREEITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector4.h>
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
*    Dynamic variable tree item: Vector4
*/
class PLFRONTENDQT_API PLDynVarVector4TreeItem : public PLDynVarStringTreeItem {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		explicit PLDynVarVector4TreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent = nullptr);
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
		PLMath::Vector4 m_Vector4;


	//[-------------------------------------------------------]
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		class Vector4ValueBaseTreeItem: public TreeItemBase {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector4ValueBaseTreeItem(const QString &name, TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem);
				virtual QVariant data(const int column, const int role) override;
				bool setData(const int column, const QVariant &value, const int role);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector4ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				virtual float GetVectorData(const PLMath::Vector4 &vector) = 0;
				virtual void SetVectorData(PLMath::Vector4 &vector, float newValue) = 0;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				QString					 m_Name;
				PLDynVarVector4TreeItem *m_parentItem;


		};

		class Vector4XValueTreeItem : public Vector4ValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector4XValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector4ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				float GetVectorData(const PLMath::Vector4 &vector);
				void SetVectorData(PLMath::Vector4 &vector, float newValue);


		};

		class Vector4YValueTreeItem : public Vector4ValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector4YValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector4ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				virtual float GetVectorData(const PLMath::Vector4 &vector) override;
				virtual void SetVectorData(PLMath::Vector4 &vector, float newValue) override;


		};

		class Vector4ZValueTreeItem : public Vector4ValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector4ZValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector4ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				virtual float GetVectorData(const PLMath::Vector4 &vector) override;
				virtual void SetVectorData(PLMath::Vector4 &vector, float newValue) override;


		};

		class Vector4WValueTreeItem : public Vector4ValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector4WValueTreeItem(TreeItemBase *parent, PLDynVarVector4TreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector4ValueBaseTreeItem functions  ]
			//[-------------------------------------------------------]
			protected:
				virtual float GetVectorData(const PLMath::Vector4 &vector) override;
				virtual void SetVectorData(PLMath::Vector4 &vector, float newValue) override;


		};


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt


#endif // __PLFRONTENDQT_PLDYNVARVECTOR4TREEITEM_H__
