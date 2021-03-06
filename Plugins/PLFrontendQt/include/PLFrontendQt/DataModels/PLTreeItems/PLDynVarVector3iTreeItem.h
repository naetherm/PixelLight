/*********************************************************\
 *  File: PLDynVarVector3iTreeItem.h                     *
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


#ifndef __PLFRONTENDQT_PLDYNVARVECTOR3ITREEITEM_H__
#define __PLFRONTENDQT_PLDYNVARVECTOR3ITREEITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3i.h>
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
*    Dynamic variable tree item: Vector3i
*/
class PLFRONTENDQT_API PLDynVarVector3iTreeItem : public PLDynVarStringTreeItem {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		explicit PLDynVarVector3iTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent = nullptr);
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
		PLMath::Vector3i m_Vector3i;


	//[-------------------------------------------------------]
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		class Vector3iValueBaseTreeItem: public TreeItemBase {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector3iValueBaseTreeItem(const QString &name, TreeItemBase *parent, PLDynVarVector3iTreeItem *parentItem);
				virtual QVariant data(const int column, const int role) override;
				bool setData(const int column, const QVariant &value, const int role);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector3iValueBaseTreeItem functions ]
			//[-------------------------------------------------------]
			protected:
				virtual int GetVectorData(const PLMath::Vector3i& vector) = 0;
				virtual void SetVectorData(PLMath::Vector3i &vector, int newValue) = 0;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				QString					 m_Name;
				PLDynVarVector3iTreeItem *m_parentItem;


		};

		class Vector3iXValueTreeItem : public Vector3iValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector3iXValueTreeItem(TreeItemBase *parent, PLDynVarVector3iTreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector3iValueBaseTreeItem functions ]
			//[-------------------------------------------------------]
			protected:
				int GetVectorData(const PLMath::Vector3i &vector);
				void SetVectorData(PLMath::Vector3i &vector, int newValue);


		};

		class Vector3iYValueTreeItem : public Vector3iValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector3iYValueTreeItem(TreeItemBase *parent, PLDynVarVector3iTreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector3iValueBaseTreeItem functions ]
			//[-------------------------------------------------------]
			protected:
				virtual int GetVectorData(const PLMath::Vector3i &vector) override;
				virtual void SetVectorData(PLMath::Vector3i &vector, int newValue) override;


		};

		class Vector3iZValueTreeItem : public Vector3iValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector3iZValueTreeItem(TreeItemBase *parent, PLDynVarVector3iTreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector3iValueBaseTreeItem functions ]
			//[-------------------------------------------------------]
			protected:
				virtual int GetVectorData(const PLMath::Vector3i &vector) override;
				virtual void SetVectorData(PLMath::Vector3i &vector, int newValue) override;


		};


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt


#endif // __PLFRONTENDQT_PLDYNVARVECTOR3ITREEITEM_H__
