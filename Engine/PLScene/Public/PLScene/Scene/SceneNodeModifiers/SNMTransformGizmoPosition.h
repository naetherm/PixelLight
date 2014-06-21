/*********************************************************\
 *  File: SNMTransformGizmoPosition.h                    *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_TRANSFORMGIZMOPOSITION_H__
#define __PLSCENE_SCENENODEMODIFIER_TRANSFORMGIZMOPOSITION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneNodeModifiers/SNMTransformGizmo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract position transform gizmo scene node modifier base class
*/
class SNMTransformGizmoPosition : public SNMTransformGizmo {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMTransformGizmoPosition, "PLScene", PLScene::SNMTransformGizmo, "Abstract position transform gizmo scene node modifier base class")
		// Attributes
		pl_attribute(LineWidth,	float,	2.0f,	ReadWrite,	DirectValue,	"Line width",					"")
		pl_attribute(Tolerance,	float,	0.6f,	ReadWrite,	DirectValue,	"Tolerance for axis selection",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLS_API SNMTransformGizmoPosition(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMTransformGizmoPosition();


	//[-------------------------------------------------------]
	//[ Protected virtual SNMTransformGizmo functions         ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual PLCore::uint32 DetermineSelected(const PLMath::Ray &cRay) const override;
		PLS_API virtual void DrawGizmo(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMesh::MeshHandler *m_pMeshHandler;	/**< The 'cone' mesh for the axis arrow (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_TRANSFORMGIZMOPOSITION_H__
