/*********************************************************\
 *  File: ClassReal.h                                    *
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


#ifndef __PLCORE_CLASS_REAL_H__
#define __PLCORE_CLASS_REAL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/ClassImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Real 'Class' implementation
*
*  @note
*    - The class instance is created and destroyed automatically by the RTTI system (see "pl_class"-macro)
*/
class ClassReal : public ClassImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MemberDesc;
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Protected virtual ClassImpl functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual bool IsDummy() const override;
		PLCORE_API virtual void InitClass() const override;
		PLCORE_API virtual void DeInitClass() const override;
		PLCORE_API virtual const List<VarDesc*> &GetAttributes() const override;
		PLCORE_API virtual const VarDesc *GetAttribute(const String &sName) const override;
		PLCORE_API virtual const List<FuncDesc*> &GetMethods() const override;
		PLCORE_API virtual const FuncDesc *GetMethod(const String &sName) const override;
		PLCORE_API virtual const List<EventDesc*> &GetSignals() const override;
		PLCORE_API virtual const EventDesc *GetSignal(const String &sName) const override;
		PLCORE_API virtual const List<EventHandlerDesc*> &GetSlots() const override;
		PLCORE_API virtual const EventHandlerDesc *GetSlot(const String &sName) const override;
		PLCORE_API virtual bool HasConstructor() const override;
		PLCORE_API virtual bool HasDefaultConstructor() const override;
		PLCORE_API virtual const List<ConstructorDesc*> &GetConstructors() const override;
		PLCORE_API virtual const ConstructorDesc *GetConstructor(const String &sName) const override;
		PLCORE_API virtual Object *Create() const override;
		PLCORE_API virtual Object *Create(const DynParams &cParams) const override;
		PLCORE_API virtual Object *Create(const String &sName, const DynParams &cParams) const override;
		PLCORE_API virtual Object *Create(const String &sName, const String &sParams) const override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nModuleID
		*    ID of owner module
		*  @param[in] sName
		*    Name
		*  @param[in] sDescription
		*    Description
		*  @param[in] sNamespace
		*    Namespace
		*  @param[in] sBaseClass
		*    Base class
		*/
		PLCORE_API ClassReal(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ClassReal();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Add member
		*
		*  @param[in] pMemberDesc
		*    Member descriptor
		*/
		PLCORE_API void AddMember(MemberDesc *pMemberDesc);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Own data (does not include data from base classes)
		List<MemberDesc*>						m_lstOwnMembers;	/**< List of members */
		// Member lists (also including the members from base classes)
		mutable HashMap<String, MemberDesc*>	m_mapMembers;		/**< Hash map of names -> members */
		mutable List<VarDesc*>					m_lstAttributes;	/**< List of attributes */
		mutable List<FuncDesc*>					m_lstMethods;		/**< List of methods */
		mutable List<EventDesc*>				m_lstSignals;		/**< List of signals */
		mutable List<EventHandlerDesc*>			m_lstSlots;			/**< List of slots */
		mutable List<ConstructorDesc*>			m_lstConstructors;	/**< List of constructors */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CLASS_REAL_H__