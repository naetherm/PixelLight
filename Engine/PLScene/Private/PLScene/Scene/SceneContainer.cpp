/*********************************************************\
 *  File: SceneContainer.cpp                             *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/Tools/Loader.h>
#include <PLCore/Container/Stack.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneHierarchy.h"
#include "PLScene/Scene/SceneHierarchyNode.h"
#include "PLScene/Scene/SceneQueryManager.h"
#include "PLScene/Scene/SceneLoader/SceneLoader.h"
#include "PLScene/Scene/SceneContainer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneContainer)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SceneContainer::GetHierarchy() const
{
	return m_sHierarchy;
}

void SceneContainer::SetHierarchy(const String &sValue)
{
	// Destroy the current hierarchy, but do NOT create the new one at once - this
	// is done if the new hierarchy is requested the first time  :)
	if (m_pHierarchy && m_pHierarchy->GetClass()->GetClassName() != sValue) {
		delete m_pHierarchy;
		m_pHierarchy = nullptr;
	}

	// Assign the new class hierarchy name
	m_sHierarchy = sValue;
}

void SceneContainer::SetFilename(const String &sValue)
{
	if (m_sFilename != sValue) {
		m_sFilename = sValue;

		// Load the container
		if (IsInitialized())
			LoadByFilename(m_sFilename);
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SceneContainer::SceneContainer() :
	Hierarchy(this),
	Flags(this),
	AABBMin(this),
	AABBMax(this),
	Filename(this),
	m_sHierarchy("PLScene::SHList"),
	m_pSceneContext(nullptr),
	m_pHierarchy(nullptr),
	m_pQueryManager(nullptr)
{
	// Overwritten SceneNode variables
	m_cAABoundingBox.vMin.SetXYZ(-10000.0f, -10000.0f, -10000.0f);
	m_cAABoundingBox.vMax.SetXYZ( 10000.0f,  10000.0f,  10000.0f);

	// Set the internal flag
	m_nInternalFlags |= ClassContainer;

	// Set draw function flags
	SetDrawFunctionFlags(UseDrawPre | UseDrawSolid | UseDrawTransparent | UseDrawDebug | UseDrawPost);
}

/**
*  @brief
*    Destructor
*/
SceneContainer::~SceneContainer()
{
	DeInit();
	if (m_pHierarchy)
		delete m_pHierarchy;
	if (m_pQueryManager)
		delete m_pQueryManager;
}

/**
*  @brief
*    Creates a new scene node at a certain index inside the scene node list
*/
SceneNode *SceneContainer::CreateAtIndex(const String &sClass, const String &sName, const String &sParameters, int nPosition)
{
	// Check parameter
	if (sClass.GetLength()) {
		// To keep things as fast as possible we store a pointer to the base class
		// (no hash map operation + quite efficient internal string comparison)
		static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLScene::SceneNode");

		// Get scene node plugin
		if (pBaseClass) {
			const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
			if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
				PL_LOG(Debug, "Create scene node '" + sName + "' of type '" + sClass + '\'')
				SceneNode *pNode = static_cast<SceneNode*>(pClass->Create());
				if (pNode) {

					// [TODO] Check/refactor the initialization/de-initialization process
					// Remove InitFunction()/DeInitFunction() if possible
					pNode->m_pManager = this;

					if (sParameters.GetLength())
						pNode->SetValues(sParameters);
					Add(*pNode, sName, true, nPosition);
				}

				// Return the created scene node
				return pNode;
			} else { // Plugin not found
				PL_LOG(Error, "Unknown scene node type: " + sClass)
			}
		}
	}

	// Error
	return nullptr;
}

/**
*  @brief
*    Calculates and sets the axis align bounding box in 'scene node space'
*/
void SceneContainer::CalculateAABoundingBox()
{
	AABoundingBox cAABoundingBox;

	// If there are any scene nodes...
	if (GetNumOfElements()) {
		// Set first bounding box
		cAABoundingBox = GetByIndex(0)->GetContainerAABoundingBox();

		// Combine all bounding boxes
		for (uint32 i=1; i<GetNumOfElements(); i++)
			cAABoundingBox.CombineAABoxes(GetByIndex(i)->GetContainerAABoundingBox());
	}

	// Set the final bounding box
	SetAABoundingBox(cAABoundingBox);
}

/**
*  @brief
*    Returns a transform matrix to bring a transformation from 'this scene container' into 'another scene container'
*/
bool SceneContainer::GetTransformMatrixTo(SceneContainer &cContainer, Matrix3x4 &mTransform)
{
	// Identity check
	if (this == &cContainer) {
		mTransform.SetIdentity();
	} else {
		// From this container space to the root space
		mTransform = GetTransform().GetMatrix();
		SceneContainer *pContainer = GetContainer();
		while (pContainer) {
			// Check if the job is already done and this container is just a child of the 'another scene container'
			if (pContainer == &cContainer)
				return true; // Done

			// Transform
			mTransform = pContainer->GetTransform().GetMatrix()*mTransform;
			pContainer = pContainer->GetContainer();
		}

		// From root space to target container space
		pContainer = &cContainer;
		Stack<SceneContainer*> lstStack;
		while (pContainer) {
			lstStack.Push(pContainer);
			pContainer = pContainer->GetContainer();
		}
		while (lstStack.GetNumOfElements()) {
			pContainer = lstStack.Top();
			lstStack.Pop();
			mTransform = pContainer->GetTransform().GetInverseMatrix()*mTransform;
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Hierarchy functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates the scene hierarchy of this scene container
*/
SceneHierarchy *SceneContainer::CreateHierarchy(const String &sClass)
{
	// Is this hierarchy type already used and is the given parameter valid?
	if ((!m_pHierarchy || m_sHierarchy != sClass) && sClass.GetLength()) {
		// To keep things as fast as possible we store a pointer to the base class
		// (no hash map operation + quite efficient internal string comparison)
		static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLScene::SceneHierarchy");

		// Get class and check it
		if (pBaseClass) {
			const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
			if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
				Object *pObject = pClass->Create();
				if (pObject) {
					// Destroy the hold hierarchy
					if (m_pHierarchy)
						delete m_pHierarchy;

					// Create new hierarchy
					m_pHierarchy = static_cast<SceneHierarchy*>(pObject);
					m_pHierarchy->m_pSceneContainer = this;

					// Initialize the hierarchy
					m_pHierarchy->Init(m_cAABoundingBox.vMin, m_cAABoundingBox.vMax);

					// Add scene nodes to the hierarchy
					for (uint32 i=0; i<GetNumOfElements(); i++)
						m_pHierarchy->AddSceneNode(*GetByIndex(i));

					// Touch recursive
					m_pHierarchy->GetRootNode().Touch(true);

					// Set the new hierarchy class name
					m_sHierarchy = sClass;
				}
			}
		}
	}

	// Return the hierarchy
	return m_pHierarchy;
}

/**
*  @brief
*    Returns the scene hierarchy of this scene container
*/
SceneHierarchy *SceneContainer::GetHierarchyInstance()
{
	// Hierarchy refresh
	if (m_pHierarchy) {
		while (!m_lstHierarchyRefresh.IsEmpty()) {
			SceneNode *pSceneNode = m_lstHierarchyRefresh.Get(0);
			m_lstHierarchyRefresh.RemoveAtIndex(0);
			pSceneNode->m_nInternalFlags &= ~SceneNode::RecalculateHierarchy;
			m_pHierarchy->RefreshSceneNode(*pSceneNode);
		}
	} else {
		// Create the hierarchy right now
		CreateHierarchy(m_sHierarchy);
	}

	// Return the hierarchy
	return m_pHierarchy;
}


//[-------------------------------------------------------]
//[ Query functions                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a scene query
*/
SceneQuery *SceneContainer::CreateQuery(const String &sClass)
{
	// Check parameter
	if (sClass.GetLength()) {
		// To keep things as fast as possible we store a pointer to the base class
		// (no hash map operation + quite efficient internal string comparison)
		static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLScene::SceneQuery");

		// Get class and check it
		if (pBaseClass) {
			const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
			if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
				Object *pObject = pClass->Create();
				if (pObject) {
					SceneQuery *pSceneQuery = static_cast<SceneQuery*>(pObject);
					pSceneQuery->m_pSceneContainer = this;

					// Add to query manager
					if (!m_pQueryManager)
						m_pQueryManager = new SceneQueryManager();
					m_pQueryManager->Add(*pSceneQuery);

					// Return the created scene query
					return pSceneQuery;
				}
			}
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Destroys a scene query
*/
bool SceneContainer::DestroyQuery(SceneQuery &cQuery)
{
	// Is there a query manager?
	if (m_pQueryManager) {
		// Check parameter
		if (m_pQueryManager->Unload(cQuery)) {
			// Check query manager
			if (!m_pQueryManager->GetNumOfElements()) {
				delete m_pQueryManager;
				m_pQueryManager = nullptr;
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a node
*/
bool SceneContainer::Add(SceneNode &cNode, const String &sName, bool bInitNode, int nPosition)
{
	// Add to list
	if (nPosition < 0 || nPosition >= static_cast<int>(m_lstElements.GetNumOfElements()))
		m_lstElements.Add(&cNode);
	else
		m_lstElements.AddAtIndex(&cNode, nPosition);
	cNode.m_pManager = this;

	// Set scene container
	if (cNode.IsContainer())
		static_cast<SceneContainer&>(cNode).m_pSceneContext = m_pSceneContext;

	// Emit signal
	cNode.SignalContainer();

	// Setup name
	String sNameT = sName;
	if (!sNameT.GetLength() && cNode.m_sName.GetLength())
		sNameT = cNode.m_sName; // Use it's default name

	// Is there already another scene node with the same name?
	bool bNameOccupied = false;
	if (sNameT.GetLength()) {
		SceneNode *pSceneNode = GetByName(sNameT);
		if (pSceneNode && pSceneNode != &cNode)
			bNameOccupied = true; // Sorry, this name is already in use
	}
	if (!bNameOccupied && sNameT.GetLength()) {
		cNode.m_sName = sNameT;
		m_mapElements.Add(sNameT, &cNode);
	} else { // Find an unused node name
		if (sNameT.GetLength()) {
			for (uint32 i=0; ; i++) {
				const String sNewName = sNameT + static_cast<int>(i);
				if (!GetByName(sNewName)) {
					cNode.m_sName = sNewName;
					m_mapElements.Add(sNewName, &cNode);
					break;
				}
			}
		} else {
			for (uint32 i=0; ; i++) {
				const String sNewName = cNode.GetClass()->GetClassName() + static_cast<int>(i);
				if (!GetByName(sNewName)) {
					cNode.m_sName = sNewName;
					m_mapElements.Add(sNewName, &cNode);
					break;
				}
			}
		}
	}

	// Initialize node
	if (bInitNode)
		cNode.InitSceneNode();

	// Add the scene node to the scene hierarchy
	if (m_pHierarchy)
		m_pHierarchy->AddSceneNode(cNode);

	// Done
	return true;
}

/**
*  @brief
*    Removes a node
*/
bool SceneContainer::Remove(SceneNode &cNode, bool bDeInitNode)
{
	// Check the given node
	if (!m_mapElements.Get(cNode.GetName()))
		return false; // Error!

	// Remove node
	if (bDeInitNode)
		cNode.DeInitSceneNode();
	m_mapElements.Remove(cNode.GetName());
	const bool bResult = m_lstElements.Remove(&cNode);
	cNode.m_pManager = nullptr;

	// Remove the scene node from the refresh list if required
	if (cNode.m_nInternalFlags & SceneNode::RecalculateHierarchy)
		m_lstHierarchyRefresh.Remove(&cNode);

	// Remove the scene node from the scene hierarchy
	if (m_pHierarchy)
		m_pHierarchy->RemoveSceneNode(cNode);

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SceneContainer::InitFunction()
{
	// Call base implementation
	SceneNode::InitFunction();

	// Load the container?
	if (m_sFilename.GetLength())
		LoadByFilename(m_sFilename);
}

void SceneContainer::DeInitFunction()
{
	// De-initialize all scene nodes
	for (uint32 i=0; i<GetNumOfElements(); i++)
		GetByIndex(i)->DeInitSceneNode();

	// Cleanup
	Clear();

	// Call base implementation
	SceneNode::DeInitFunction();
}

void SceneContainer::OnActivate(bool bActivate)
{
	// Call the base implementation
	SceneNode::OnActivate(bActivate);

	// Loop through all scene nodes
	for (uint32 i=0; i<GetNumOfElements(); i++)
		GetByIndex(i)->OnActivate(bActivate);
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::ElementManager functions       ]
//[-------------------------------------------------------]
bool SceneContainer::DeInit()
{
	// Cleanup
	Clear();

	// Done
	return true;
}

SceneNode *SceneContainer::GetByIndex(uint32 nIndex) const
{
	return m_lstElements[nIndex];
}

SceneNode *SceneContainer::GetByName(const String &sName) const
{
	// Name not empty and is '.' the first character?
	if (sName.GetLength() && sName[static_cast<uint32>(0)] != '.') {
		// Is the name 'This' at the beginning?
		static const String sThis = "This";
		if (sName.Compare(sThis, 0, 4)) {
			// Return this scene node?
			if (sName[static_cast<uint32>(4)] == '\0')
				return const_cast<SceneNode*>(reinterpret_cast<const SceneNode*>(this)); // [HACK] Make this not 'const'

			// Did a '.' follow?
			if (sName[static_cast<uint32>(4)] == '.')
				return GetByName(sName.GetSubstring(5));
		}

		// Is the name 'Root' at the beginning?
		static const String sRoot = "Root";
		if (sName.Compare(sRoot, 0, 4)) {
			// Return root scene node?
			if (sName[static_cast<uint32>(4)] == '\0')
				return m_pSceneContext->GetRoot();

			// Did a '.' follow?
			if (sName[static_cast<uint32>(4)] == '.')
				return m_pSceneContext->GetRoot() ? m_pSceneContext->GetRoot()->GetByName(sName.GetSubstring(5)) : nullptr;
		}

		// 'Parent' at the beginning?
		static const String sParent = "Parent";
		if (sName.Compare(sParent, 0, 6)) {
			// Return parent scene node?
			if (sName[static_cast<uint32>(6)] == '\0')
				return GetContainer();

			// Did a '.' follow?
			if (sName[static_cast<uint32>(6)] == '.') {
				// Is there a parent container?
				return GetContainer() ? GetContainer()->GetByName(sName.GetSubstring(7)) : nullptr;
			}
		}

		// Check for the character '.' within the name
		const int nIndex = sName.IndexOf('.');
		if (nIndex >= 0) {
			// Get the name of the scene container
			String sContainerName;
			sContainerName.Insert(sName, 0, static_cast<uint32>(nIndex));

			// Get the scene node and check whether it is a scene container
			SceneNode *pSceneNode = GetByName(sContainerName);
			if (pSceneNode && (pSceneNode->m_nInternalFlags & ClassContainer)) {
				// Change 'into' this scene container
				return static_cast<SceneContainer*>(pSceneNode)->GetByName(sName.GetSubstring(nIndex+1));
			}
		} else {
			// Search for a scene node with this name
			SceneNode *pNode = m_mapElements.Get(sName);
			if (pNode)
				return pNode;
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool SceneContainer::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Pause all to avoid timing problems
	const bool bIsPaused = Timing::GetInstance()->IsPaused();
	Timing::GetInstance()->Pause(true);

	// Call base implementation
	const bool bResult = Loadable::LoadByFilename(sFilename, sParams, sMethod);

	// Restore previous pause state
	Timing::GetInstance()->Pause(bIsPaused);

	// Done
	return bResult;
}

bool SceneContainer::LoadByFile(File &cFile, const String &sParams, const String &sMethod)
{
	// Pause all to avoid timing problems
	const bool bIsPaused = Timing::GetInstance()->IsPaused();
	Timing::GetInstance()->Pause(true);

	// Call base implementation
	const bool bResult = Loadable::LoadByFile(cFile, sParams, sMethod);

	// Restore previous pause state
	Timing::GetInstance()->Pause(bIsPaused);

	// Done
	return bResult;
}

bool SceneContainer::Unload()
{
	// Cleanup the container
	if (Clear()) {
		// Call base implementation
		return Loadable::Unload();
	} else {
		// Error!
		return false;
	}
}

String SceneContainer::GetLoadableTypeName() const
{
	static const String sString = "Scene";
	return sString;
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::Loadable functions          ]
//[-------------------------------------------------------]
bool SceneContainer::CallLoadable(File &cFile, Loader &cLoader, const String &sMethod, const String &sParams)
{
	// Get the loader implementation
	LoaderImpl *pLoaderImpl = cLoader.GetImpl();
	if (pLoaderImpl) {
		// Load
		if (sParams.GetLength()) {
			pLoaderImpl->CallMethod(sMethod, "Param0=\"" + Type<SceneContainer&>::ConvertToString(*this) + "\" Param1=\"" + Type<File&>::ConvertToString(cFile) + "\" " + sParams);
			return true;
		} else {
			Params<bool, SceneContainer&, File&> cParams(*this, cFile);
			pLoaderImpl->CallMethod(sMethod, cParams);
			return cParams.Return;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
SceneNode *SceneContainer::CreateElement(const String &sName)
{
	return Create(sName, "");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
