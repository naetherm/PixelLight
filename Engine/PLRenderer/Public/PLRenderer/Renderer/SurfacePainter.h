/*********************************************************\
 *  File: SurfacePainter.h                               *
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


#ifndef __PLRENDERER_SURFACEPAINTER_H__
#define __PLRENDERER_SURFACEPAINTER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Surface;
class Renderer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract surface painter base class
*
*  @note
*    - Derived classes should use a 'SP' name prefix (example: SPPreview)
*/
class SurfacePainter : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Surface;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, SurfacePainter, "PLRenderer", PLCore::Object, "Abstract surface painter base class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the owner renderer
		*
		*  @return
		*    The owner renderer
		*/
		inline Renderer &GetRenderer() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Copy constructor (not implemented!)
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		SurfacePainter(const SurfacePainter &cSource);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		PLRENDERER_API SurfacePainter(Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Protected virtual SurfacePainter functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~SurfacePainter();

		/**
		*  @brief
		*    Is called when the owner surface is paint process begins
		*
		*  @param[in] cSurface
		*    Surface we're painting on
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API virtual bool OnPaintBegin(Surface &cSurface);

		/**
		*  @brief
		*    Is called when the owner surface is painted
		*
		*  @param[in] cSurface
		*    Surface we're painting on
		*/
		virtual void OnPaint(Surface &cSurface) = 0;

		/**
		*  @brief
		*    Is called when the owner surface is paint process ends
		*
		*  @param[in] cSurface
		*    Surface we're painting on
		*/
		PLRENDERER_API virtual void OnPaintEnd(Surface &cSurface);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		SurfacePainter &operator =(const SurfacePainter &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Renderer *m_pRenderer;	/**< The owner renderer (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/SurfacePainter.inl"


#endif // __PLRENDERER_SURFACEPAINTER_H__
