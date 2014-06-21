/*********************************************************\
 *  File: SQByClassName.h                                *
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


#ifndef __PLSCENE_SCENEQUERY_BYCLASSNAME_H__
#define __PLSCENE_SCENEQUERY_BYCLASSNAME_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/RegEx.h>
#include "PLScene/Scene/SceneQuery.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node enumeration by class name scene query class
*
*  @remarks
*    This scene query will enumerate scene nodes of the scene graph
*    if their class name matches a given regular expression.
*/
class SQByClassName : public SceneQuery {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SQByClassName, "PLScene", PLScene::SceneQuery, "Scene node enumeration by class name scene query class")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*   Flags (SceneQuery flags extension)
		*/
		enum EFlags {
			IgnoreBaseClasses = 1<<2	/**< Ignore base classes of scene node classes */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SQByClassName();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SQByClassName();

		/**
		*  @brief
		*    Returns the used regular expression
		*
		*  @return
		*    The used regular expression
		*/
		PLS_API const PLCore::RegEx &GetRegEx() const;

		/**
		*  @brief
		*    Sets the used regular expression
		*
		*  @param[in] cRegEx
		*    The regular expression to use
		*/
		PLS_API void SetRegEx(const PLCore::RegEx &cRegEx);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Recursive part of PerformQuery()
		*
		*  @param[in] cContainer
		*    Current scene container we work on
		*
		*  @return
		*    'true' to continue the query, 'false' if stop it right now
		*/
		bool PerformQueryRec(const SceneContainer &cContainer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::RegEx m_cRegEx;	/**< Regular expression used for the query */


	//[-------------------------------------------------------]
	//[ Public virtual SceneQuery functions                   ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool PerformQuery() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEQUERY_BYCLASSNAME_H__
