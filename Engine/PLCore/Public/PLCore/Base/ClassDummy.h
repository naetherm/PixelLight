/*********************************************************\
 *  File: ClassDummy.h                                   *
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


#ifndef __PLCORE_CLASS_DUMMY_H__
#define __PLCORE_CLASS_DUMMY_H__
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
*    Dummy 'Class' implementation
*/
class ClassDummy : public ClassImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Protected virtual ClassImpl functions                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool IsDummy() const override;
		virtual void InitClass() const override;
		virtual void DeInitClass() const override;
		virtual const List<VarDesc*> &GetAttributes() const override;
		virtual const VarDesc *GetAttribute(const String &sName) const override;
		virtual const List<FuncDesc*> &GetMethods() const override;
		virtual const FuncDesc *GetMethod(const String &sName) const override;
		virtual const List<EventDesc*> &GetSignals() const override;
		virtual const EventDesc *GetSignal(const String &sName) const override;
		virtual const List<EventHandlerDesc*> &GetSlots() const override;
		virtual const EventHandlerDesc *GetSlot(const String &sName) const override;
		virtual bool HasConstructor() const override;
		virtual bool HasDefaultConstructor() const override;
		virtual const List<ConstructorDesc*> &GetConstructors() const override;
		virtual const ConstructorDesc *GetConstructor(const String &sName) const override;
		virtual Object *Create() const override;
		virtual Object *Create(const DynParams &cParams) const override;
		virtual Object *Create(const String &sName, const DynParams &cParams) const override;
		virtual Object *Create(const String &sName, const String &sParams) const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
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
		*  @param[in] bHasConstructor
		*    Class has any constructors?
		*  @param[in] bHasDefaultConstructor
		*    Class has a default constructor?
		*/
		ClassDummy(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass, bool bHasConstructor, bool bHasDefaultConstructor);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ClassDummy();

		/*
		*  @brief
		*    Requests the real class implementation
		*
		*  @return
		*    The real class implementation if all went fine, else a null pointer
		*
		*  @note
		*    - Please note that it's not valid to use this dummy class implementation after this method was call because it may have been killed!
		*/
		ClassImpl *GetRealClassImpl() const;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		// Member lists (also including the members from base classes) - static because those are just dummies in here
		static HashMap<String, MemberDesc*> m_mapMembers;		/**< Hash map of names -> members */
		static List<VarDesc*>			    m_lstAttributes;	/**< List of attributes */
		static List<FuncDesc*>				m_lstMethods;		/**< List of methods */
		static List<EventDesc*>				m_lstSignals;		/**< List of signals */
		static List<EventHandlerDesc*>		m_lstSlots;			/**< List of slots */
		static List<ConstructorDesc*>		m_lstConstructors;	/**< List of constructors */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bHasConstructor;			/**< Class has any constructors? */
		bool m_bHasDefaultConstructor;	/**< Class has a default constructor? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CLASS_DUMMY_H__
