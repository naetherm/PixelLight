/*********************************************************\
 *  File: Application.h                                  *
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


#ifndef __MYCLASS_H__
#define __MYCLASS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Reflection/Rtti.h"
#include "PLCore/Event/Signal.h"
#include "PLCore/Event/Slot.h"


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Test class using PixelLight's RTTI system
*/
class MyClass
{

	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_rtti()


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MyClass();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MyClass();

		/**
		*  @brief
		*    Sample function that will be bound to reflection
		*/
		int Foo(int i, float f);

		/**
		*  @brief
		*    Get private int
		*/
		const int &GetPrivateInt() const
		{
			return m_nPrivateInt;
		}

		/**
		*  @brief
		*    Set private int
		*/
		void SetPrivateInt(const int &i)
		{
			m_nPrivateInt = i;
		}

		MyClass &GetMyClass(MyClass &c)
		{
			return c;
		}

	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Public field
		*/
		int PublicInt;

		PLCore::Signal<int> OnInit;
		PLCore::Slot<int> InitializeSlot;

		void Initialize(int i)
		{
			int j = i;
			j=j;
 		}

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int m_nPrivateInt;
};

// Declare the reflected type
pl_declare_class(MyClass)


#endif // __MYCLASS_H__
