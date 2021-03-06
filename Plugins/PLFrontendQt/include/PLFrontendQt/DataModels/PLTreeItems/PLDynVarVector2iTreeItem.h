/*********************************************************\
 *  File: PLDynVarVector2iTreeItem.h                     *
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


#ifndef __PLFRONTENDQT_PLDYNVARVECTOR2ITREEITEM_H__
#define __PLFRONTENDQT_PLDYNVARVECTOR2ITREEITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
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
*    Dynamic variable tree item: Vector2i
*/
class PLFRONTENDQT_API PLDynVarVector2iTreeItem : public PLDynVarStringTreeItem {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		explicit PLDynVarVector2iTreeItem(PLCore::DynVarPtr dynVar, TreeItemBase *parent = nullptr);
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
		PLMath::Vector2i m_Vector2i;


	//[-------------------------------------------------------]
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		class Vector2iValueBaseTreeItem: public TreeItemBase {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector2iValueBaseTreeItem(const QString &name, TreeItemBase *parent, PLDynVarVector2iTreeItem *parentItem);
				virtual QVariant data(const int column, const int role) override;
				bool setData(const int column, const QVariant &value, const int role);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector2iValueBaseTreeItem functions ]
			//[-------------------------------------------------------]
			protected:
				virtual int GetVectorData(const PLMath::Vector2i &vector) = 0;
				virtual void SetVectorData(PLMath::Vector2i &vector, int newValue) = 0;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				QString					 m_Name;
				PLDynVarVector2iTreeItem *m_parentItem;


		};

		class Vector2iXValueTreeItem : public Vector2iValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector2iXValueTreeItem(TreeItemBase *parent, PLDynVarVector2iTreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector2iValueBaseTreeItem functions ]
			//[-------------------------------------------------------]
			protected:
				virtual int GetVectorData(const PLMath::Vector2i &vector) override;
				virtual void SetVectorData(PLMath::Vector2i &vector, int newValue) override;


		};

		class Vector2iYValueTreeItem : public Vector2iValueBaseTreeItem {


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				Vector2iYValueTreeItem(TreeItemBase *parent, PLDynVarVector2iTreeItem *parentItem);


			//[-------------------------------------------------------]
			//[ Protected virtual Vector2iValueBaseTreeItem functions ]
			//[-------------------------------------------------------]
			protected:
				virtual int GetVectorData(const PLMath::Vector2i &vector) override;
				virtual void SetVectorData(PLMath::Vector2i &vector, int newValue) override;


		};
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt


#endif // __PLFRONTENDQT_PLDYNVARVECTOR2ITREEITEM_H__
