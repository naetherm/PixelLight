/*********************************************************\
 *  File: ConstIterator.inl                              *
 *      Constant iterator template implementation
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
ConstIterator<ValueType>::ConstIterator(IteratorImpl<ValueType> &cIteratorImpl) :
	m_pIteratorImpl(&cIteratorImpl)
{
	// Add a reference to the used iterator implementation
	m_pIteratorImpl->AddReference();
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
ConstIterator<ValueType>::ConstIterator(const ConstIterator<ValueType> &cIterator) :
	m_pIteratorImpl(cIterator.m_pIteratorImpl)
{
	// Add a reference to the used iterator implementation
	m_pIteratorImpl->AddReference();
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
ConstIterator<ValueType>::~ConstIterator()
{
	// Release the used iterator implementation
	m_pIteratorImpl->Release();
}

/**
*  @brief
*    Copy operator
*/
template <class ValueType>
ConstIterator<ValueType> &ConstIterator<ValueType>::operator =(const ConstIterator<ValueType> &cIterator)
{
	// Do only go on if the source is NOT this object!
	if (&cIterator != this) {
		// Release the current used iterator implementation
		m_pIteratorImpl->Release();

		// Set the new iterator implementation
		m_pIteratorImpl = cIterator.m_pIteratorImpl;

		// Add a reference to the new used iterator implementation
		m_pIteratorImpl->AddReference();
	}

	// Return this instance
	return *this;
}

/**
*  @brief
*    Checks whether the iterator can return a next element
*/
template <class ValueType>
bool ConstIterator<ValueType>::HasNext() const
{
	return m_pIteratorImpl->HasNext();
}

/**
*  @brief
*    Returns the next element
*/
template <class ValueType>
const ValueType &ConstIterator<ValueType>::Next()
{
	// Make the implementation unique
	UniqueImplementation();

	// Return the next element
	return m_pIteratorImpl->Next();
}

/**
*  @brief
*    Returns the next element
*/
template <class ValueType>
const ValueType &ConstIterator<ValueType>::operator ++()
{
	// Make the implementation unique
	UniqueImplementation();

	// Return the next element
	return m_pIteratorImpl->Next();
}

/**
*  @brief
*    Checks whether the iterator can return a previous element
*/
template <class ValueType>
bool ConstIterator<ValueType>::HasPrevious() const
{
	return m_pIteratorImpl->HasPrevious();
}

/**
*  @brief
*    Returns the previous element
*/
template <class ValueType>
const ValueType &ConstIterator<ValueType>::Previous()
{
	// Make the implementation unique
	UniqueImplementation();

	// Return the previous element
	return m_pIteratorImpl->Previous();
}

/**
*  @brief
*    Returns the previous element
*/
template <class ValueType>
const ValueType &ConstIterator<ValueType>::operator --()
{
	// Make the implementation unique
	UniqueImplementation();

	// Return the previous element
	return m_pIteratorImpl->Previous();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Checks whether the current internal iterator implementation is 'shared' and splits it if so
*/
template <class ValueType>
void ConstIterator<ValueType>::UniqueImplementation()
{
	// Is the iterator implementation used more than once?
	if (m_pIteratorImpl->GetRefCount() > 1) {
		// Release the current used iterator implementation
		m_pIteratorImpl->Release();

		// Set the new iterator implementation
		m_pIteratorImpl = m_pIteratorImpl->Clone();

		// Add a reference to the new used iterator implementation
		m_pIteratorImpl->AddReference();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
