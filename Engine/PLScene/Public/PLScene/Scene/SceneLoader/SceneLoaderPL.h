/*********************************************************\
 *  File: SceneLoaderPL.h                                *
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


#ifndef __PLSCENE_SCENELOADER_PL_H__
#define __PLSCENE_SCENELOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneLoader/SceneLoader.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class XmlElement;
}
namespace PLScene {
	class SceneNode;
	class SceneContainer;
	class SceneNodeModifier;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene loader implementation for the PixelLight scene XML file format
*
*  @note
*    - If the save function receives a boolean 'NoDefault'-parameter with 'true', (default setting)
*      variables with default values are not saved
*/
class SceneLoaderPL : public SceneLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneLoaderPL, "PLScene", PLScene::SceneLoader, "Scene loader implementation for the PixelLight scene XML file format")
		// Properties
		pl_properties
			pl_property("Formats",	"scene,SCENE")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	SceneContainer&,	PLCore::File&,			"Load method",												"")
		pl_method_2(Save,		pl_ret_type(bool),	SceneContainer&,	PLCore::File&,			"Save method",												"")
		pl_method_3(SaveParams,	pl_ret_type(bool),	SceneContainer&,	PLCore::File&,	bool,	"Save method. Parameters: No default values as 'bool'.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLS_API bool Load(SceneContainer &cContainer, PLCore::File &cFile);
		PLS_API bool Save(SceneContainer &cContainer, PLCore::File &cFile);
		PLS_API bool SaveParams(SceneContainer &cContainer, PLCore::File &cFile, bool bNoDefault);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal per instance data
		*/
		struct SInstance {
			SceneContainer *pContainer;				/**< Scene root container (the one we currently 'load in', always valid) */
			// Statistics
			PLCore::uint32  nTotalNumOfContainers;	/**< Total number of containers within the scene (without the root container itself) */
			PLCore::uint32  nTotalNumOfNodes;		/**< Total number of nodes (without containers) within the scene */
			PLCore::uint32  nTotalNumOfModifiers;	/**< Total number of modifiers within the scene */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SceneLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cContainer
		*    Scene container to load into
		*  @param[in] cSceneElement
		*    Scene XML element to read the data from
		*  @param[in] cFile
		*    File to load from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(SceneContainer &cContainer, const PLCore::XmlElement &cSceneElement, PLCore::File &cFile) const;

		/**
		*  @brief
		*    Recursive part of the load function
		*
		*  @param[in] sInstance
		*    Per instance data
		*  @param[in] cContainer
		*    Scene container object
		*  @param[in] cParent
		*    Parent XML element
		*  @param[in] nFirstSceneRow
		*    First scene Row
		*  @param[in] nLastSceneRow
		*    Last scene Row
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadRec(SInstance &sInstance, SceneContainer &cContainer, const PLCore::XmlElement &cParent, PLCore::uint32 nFirstSceneRow, PLCore::uint32 nLastSceneRow) const;

		/**
		*  @brief
		*    Loads a scene node and returns it
		*
		*  @param[in] sInstance
		*    Per instance data
		*  @param[in] cContainer
		*    Scene container object
		*  @param[in] cNode
		*    XML node
		*  @param[in] bContainer
		*    Is this a scene container?
		*
		*  @return
		*    The created scene node, a null pointer on error (maybe XML element has no class attribute?)
		*/
		SceneNode *LoadNode(SInstance &sInstance, SceneContainer &cContainer, const PLCore::XmlElement &cNode, bool bContainer) const;

		/**
		*  @brief
		*    Loads a scene node modifier and returns it
		*
		*  @param[in] sInstance
		*    Per instance data
		*  @param[in] cNode
		*    XML node
		*  @param[in] cSceneNode
		*    Scene node where to add the modifier
		*
		*  @return
		*    The created scene node modifier, a null pointer on error (maybe XML element has no class attribute?)
		*/
		SceneNodeModifier *LoadModifier(SInstance &sInstance, const PLCore::XmlElement &cNode, SceneNode &cSceneNode) const;

		/**
		*  @brief
		*    Recursive part of the save function
		*
		*  @param[in] sInstance
		*    Per instance data
		*  @param[in] cContainer
		*    Scene container object
		*  @param[in] cParent
		*    Parent XML element
		*  @param[in] bNoDefault
		*    If 'true', variables with default values will be ignored
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool SaveRec(SInstance &sInstance, const SceneContainer &cContainer, PLCore::XmlElement &cParent, bool bNoDefault = true) const;

		/**
		*  @brief
		*    Modifiers part of the save function
		*
		*  @param[in] sInstance
		*    Per instance data
		*  @param[in] cParent
		*    Parent XML element
		*  @param[in] cSceneNode
		*    Node to save the modifiers from
		*  @param[in] bNoDefault
		*    If 'true', variables with default values will be ignored
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool SaveModifiers(SInstance &sInstance, PLCore::XmlElement &cParent, const SceneNode &cSceneNode, bool bNoDefault = true) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENELOADER_PL_H__
