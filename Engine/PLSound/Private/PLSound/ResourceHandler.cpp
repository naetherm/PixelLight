/*********************************************************\
 *  File: ResourceHandler.cpp                            *
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
#include "PLSound/Resource.h"
#include "PLSound/ResourceHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSound {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ResourceHandler::ResourceHandler() :
	m_pResource(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
ResourceHandler::ResourceHandler(const ResourceHandler &cSource) :
	m_pResource(nullptr)
{
	if (cSource.m_pResource) {
		m_pResource = cSource.m_pResource;
		m_pResource->AddResourceHandler(*this);
	}
}

/**
*  @brief
*    Destructor
*/
ResourceHandler::~ResourceHandler()
{
	// Inform resource
	if (m_pResource)
		m_pResource->RemoveResourceHandler(*this);
}

/**
*  @brief
*    Copy operator
*/
ResourceHandler &ResourceHandler::operator =(const ResourceHandler &cSource)
{
	// Inform the old resource
	if (m_pResource)
		m_pResource->RemoveResourceHandler(*this);

	// Set new one
	m_pResource = cSource.m_pResource;
	if (m_pResource)
		m_pResource->AddResourceHandler(*this);

	// Return this
	return *this;
}

/**
*  @brief
*    Returns the resource handlers resource
*/
Resource *ResourceHandler::GetResource() const
{
	return m_pResource;
}

/**
*  @brief
*    Sets the resource handlers resource
*/
void ResourceHandler::SetResource(Resource *pResource)
{
	// Inform the old resource
	if (m_pResource)
		m_pResource->RemoveResourceHandler(*this);

	// Set new one
	m_pResource = pResource;
	if (pResource)
		pResource->AddResourceHandler(*this);
}


//[-------------------------------------------------------]
//[ Public virtual ResourceHandler functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Is called when the resource was lost
*/
void ResourceHandler::OnLost()
{
	// Nothing to do by default
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound
