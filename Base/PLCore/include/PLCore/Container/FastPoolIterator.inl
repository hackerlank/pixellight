/*********************************************************\
 *  File: FastPoolIterator.inl                           *
 *      Fast pool iterator template implementation
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
#include "PLCore/Container/FastPool.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
FastPoolIterator<ValueType>::FastPoolIterator(const FastPool<ValueType> &lstPool, uint32 nIndex) :
	m_plstPool(&lstPool)
{
	// Start at first element?
	if (!nIndex) {
		m_pNextElement     = m_plstPool->m_pFirstElement;
		m_pPreviousElement = nullptr;

	// Start at last element?
	} else if (nIndex >= m_plstPool->m_nNumOfElements-1) {
		m_pNextElement     = nullptr;
		m_pPreviousElement = m_plstPool->m_pLastElement;

	// Ok, we start somewhere between the start/end element... which search direction?
	} else {
		if (nIndex < m_plstPool->m_nNumOfElements/2) {
			// Start with the first element
			m_pPreviousElement = m_plstPool->m_pFirstElement;
			m_pNextElement     = m_pPreviousElement->m_pNextElement;
			uint32 nCurrent = 1;
			while (m_pNextElement && nCurrent != nIndex) {
				m_pPreviousElement = m_pNextElement;
				m_pNextElement     = m_pNextElement->m_pNextElement;
				nCurrent++;
			}
		} else {
			// Start with the last element
			m_pNextElement     = m_plstPool->m_pLastElement;
			m_pPreviousElement = m_pNextElement->m_pPreviousElement;
			uint32 nCurrent = m_plstPool->m_nNumOfElements-2;
			while (m_pPreviousElement && nCurrent != nIndex) {
				m_pNextElement     = m_pPreviousElement;
				m_pPreviousElement = m_pPreviousElement->m_pPreviousElement;
				nCurrent--;
			}
		}
	}
}

/**
*  @brief
*    Constructor
*/
template <class ValueType>
FastPoolIterator<ValueType>::FastPoolIterator(const FastPool<ValueType> &lstPool) :
	m_plstPool(&lstPool),
	m_pNextElement(nullptr),
	m_pPreviousElement(lstPool.m_pLastElement)
{
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
FastPoolIterator<ValueType>::FastPoolIterator(const FastPoolIterator<ValueType> &lstSource) :
	m_plstPool(lstSource.m_plstPool),
	m_pNextElement(lstSource.m_pNextElement),
	m_pPreviousElement(lstSource.m_pPreviousElement)
{
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
FastPoolIterator<ValueType>::~FastPoolIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class ValueType>
IteratorImpl<ValueType> *FastPoolIterator<ValueType>::Clone() const
{
	return new FastPoolIterator<ValueType>(*this);
}

template <class ValueType>
bool FastPoolIterator<ValueType>::HasNext() const
{
	return (m_pNextElement != nullptr);
}

template <class ValueType>
ValueType &FastPoolIterator<ValueType>::Next()
{
	if (!m_pNextElement)
		return FastPool<ValueType>::Null; // There's no next element
	m_pPreviousElement = m_pNextElement;
	m_pNextElement     = m_pNextElement->m_pNextElement;
	return *m_pPreviousElement;
}

template <class ValueType>
bool FastPoolIterator<ValueType>::HasPrevious() const
{
	return (m_pPreviousElement != nullptr);
}

template <class ValueType>
ValueType &FastPoolIterator<ValueType>::Previous()
{
	if (!m_pPreviousElement)
		return FastPool<ValueType>::Null; // There's no previous element
	m_pNextElement     = m_pPreviousElement;
	m_pPreviousElement = m_pPreviousElement->m_pPreviousElement;
	return *m_pNextElement;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
