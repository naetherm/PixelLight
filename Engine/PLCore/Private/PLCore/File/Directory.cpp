/*********************************************************\
 *  File: Directory.cpp                                  *
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
#include "PLCore/File/FileSearch.h"
#include "PLCore/File/Directory.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create a directory recursively
*/
bool Directory::CreateRecursive()
{
	// Does this directory already exist?
	if (Exists())
		return true; // Done
	else {
		// Get the first path
		String sNextPath;
		uint32 nPathPos = 0;
		String sPath    = GetUrl().GetProtocol() + GetUrl().GetRoot() + GetUrl().GetFirstPath(nPathPos);
		do {
			// Get the directory and check whether it's exist
			Directory cDirectory(sPath);
			if (!cDirectory.Exists()) {
				// Create the directory
				if (!cDirectory.Create())
					return false; // Error!
			}

			// Get the next path
			sNextPath = GetUrl().GetNextPath(nPathPos);
			if (sNextPath.GetLength())
				sPath +=  '/' + sNextPath;
		} while (sNextPath.GetLength());

		// Create this directory
		return Create();
	}
}

/**
*  @brief
*    Returns a file searcher for listing the content of a directory
*/
FileSearch *Directory::Search(const String &sFilter) const
{
	return new FileSearch(*this, sFilter);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Directory::Directory(const Directory &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Directory &Directory::operator =(const Directory &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
