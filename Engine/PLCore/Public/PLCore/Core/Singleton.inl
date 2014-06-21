/*********************************************************\
 *  File: Singleton.inl                                  *
 *      Singleton template implementation
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
//[ Debugging options for Microsoft Visual Studio         ]
//[-------------------------------------------------------]
// <<nostepinto>> PLCore::Singleton::.*


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Static data                                           ]
//[-------------------------------------------------------]
template <class AType> typename Singleton<AType>::__Destruction_Guard Singleton<AType>::__Guard;
template <class AType> AType *Singleton<AType>::__Instance  = nullptr;
template <class AType> bool   Singleton<AType>::__Destroyed = false;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the class instance
*/
template <class AType> AType *Singleton<AType>::GetInstance()
{
	// [HACK] This is necessary for the compiler to instantiate __Guard
	__Guard.init();

	// Check if the instance must be created
	if (!__Instance && !__Destroyed) {
		// Create instance
		return new AType();
	} else {
		// Return instance
		return __Instance;
	}
}

/**
*  @brief
*    Returns whether or not there's an instance
*/
template <class AType> bool Singleton<AType>::HasInstance()
{
	return (__Instance != nullptr);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class AType> Singleton<AType>::Singleton()
{
	// Register instance - Don't use reinterpret_cast in here or hell breaks loose when starting programs
	__Instance = static_cast<AType*>(this);
}

/**
*  @brief
*    Destructor
*/
template <class AType> Singleton<AType>::~Singleton()
{
	// Unregister instance
	if (__Instance == this) {
		__Instance  = nullptr;
		__Destroyed = true;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
