/*********************************************************\
 *  File: SNCellPortal.h                                 *
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


#ifndef __PLSCENE_CELLPORTAL_H__
#define __PLSCENE_CELLPORTAL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SNPortal.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SCCell;
class SceneNodeHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Cell-portal scene node
*
*  @remarks
*    A cell-portal allows the viewer to look from one into another cell.
*/
class SNCellPortal : public SNPortal {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoSeeThrough  = 1<<10,	/**< It's not possible to 'see through' this cell-portal into the target cell (example usage: closed door) */
			NoPassThrough = 1<<11	/**< It's not possible to 'pass through' this cell-portal into the target cell (example usage: window) */
		};
		pl_enum(EFlags)
			pl_enum_base(SNPortal::EFlags)
			pl_enum_value(NoSeeThrough,		"It's not possible to 'see through' this cell-portal into the target cell (example usage: closed door)")
			pl_enum_value(NoPassThrough,	"It's not possible to 'pass through' this cell-portal into the target cell (example usage: window)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNCellPortal, "PLScene", PLScene::SNPortal, "Cell-portal scene node")
		// Attributes
		pl_attribute(TargetCell,	PLCore::String,			"",	ReadWrite,	GetSet,	"Name of the cell this cell-portal links to (for instance 'Parent.Cell2')",	"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",																	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetTargetCell() const;
		PLS_API void SetTargetCell(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNCellPortal();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNCellPortal();

		/**
		*  @brief
		*    Returns the cell the cell-portal links to
		*
		*  @return
		*    The cell the cell-portal links to, a null pointer on error
		*    (maybe there's no node with the cell name or this node is no cell)
		*/
		PLS_API SCCell *GetTargetCellInstance();

		/**
		*  @brief
		*    Returns the warp matrix
		*
		*  @return
		*    The warp matrix
		*
		*  @remarks
		*    The warp matrix transforms from this cell space into target cell space.
		*
		*  @note
		*    - Is updated automatically
		*/
		PLS_API const PLMath::Matrix3x4 &GetWarpMatrix();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Flags which hold ínternal portal information (SNPortal flags extension)
		*/
		enum EInternalPortalFlags {
			// Recalculate
			RecalculateWarpMatrix = 1<<1	/**< Recalculation of warp matrix required */
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the target cell container, position, rotation or scale changed
		*/
		void OnCellContainerPositionRotationScale();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerCellContainerPositionRotationScale;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String	   m_sTargetCell;	/**< Name of the cell this cell-portal links to (for instance 'Parent.Cell2') */
		PLMath::Matrix3x4  m_mWarp;			/**< This cell space into target cell space */
		SceneNodeHandler  *m_pTargetCell;	/**< Target cell (ALWAYS valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_CELLPORTAL_H__
