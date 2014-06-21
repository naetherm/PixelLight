/*********************************************************\
 *  File: MimeTypeManager.cpp                            *
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
#include "PLCore/File/Url.h"
#include "PLCore/Network/Http/MimeTypeManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get mime type from filename
*/
String MimeTypeManager::GetMimeType(const String &sFilename)
{
	// Get filename extension
	const String sExtension = Url(sFilename).GetExtension();

	// Check known MIME types
	if (sExtension == "css")	return "text/css";
	if (sExtension == "html")	return "text/html";
	if (sExtension == "htm")	return "text/html";
	if (sExtension == "txt")	return "text/plain";
	if (sExtension == "xml")	return "text/xml";
	if (sExtension == "gif")	return "image/gif";
	if (sExtension == "ico")	return "image/vnd.microsoft.icon";
	if (sExtension == "jpg")	return "image/jpeg";
	if (sExtension == "png")	return "image/png";
	if (sExtension == "svg")	return "image/svg+xml";
	if (sExtension == "tif")	return "image/tiff";
	if (sExtension == "xhtml")	return "application/xhtml+xml";
	if (sExtension == "js")		return "application/x-javascript";
	if (sExtension == "pdf")	return "application/pdf";

	// Unknown MIME type, assume a text file
	return "text/plain";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
