/*********************************************************\
 *  File: IOSystem.h                                     *
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


#ifndef __PLASSIMP_IOSYSTEM_H__
#define __PLASSIMP_IOSYSTEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Assimp/IOSystem.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class File;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Assimp IO system implementation
*/
class IOSystem : public Assimp::IOSystem {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char *MagicDefaultFilename;		/**< Magic filename (concept based on "Assimp::MemoryIOSystem") */
		static const int   MagicDefaultFilenameLength;	/**< Length of the magic filename */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFile
		*    File to load from, must be opened and readable
		*  @param[in] pszMagicDefaultFilename
		*    Magic filename (concept based on "Assimp::MemoryIOSystem")
		*  @param[in] nMagicDefaultFilenameLength
		*    Length of the magic filename
		*/
		IOSystem(PLCore::File &cFile, const char *pszMagicDefaultFilename, int nMagicDefaultFilenameLength);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IOSystem();


	//[-------------------------------------------------------]
	//[ Public virtual Assimp::IOSystem functions             ]
	//[-------------------------------------------------------]
	public:
		virtual bool Exists(const char *pFile) const override;
		virtual char getOsSeparator() const override;
		virtual Assimp::IOStream *Open(const char *pFile, const char *pMode = "rb") override;
		virtual void Close(Assimp::IOStream *pFile) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		IOSystem();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		IOSystem(const IOSystem &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		IOSystem &operator =(const IOSystem &cSource);

		/**
		*  @brief
		*    Returns a real filename
		*
		*  @param[in] pFile
		*    Filename as given by Assimp
		*
		*  @return
		*    The constructed real filename (... and hopefully a correct one...)
		*/
		PLCore::String GetRealFilename(const char *pszAssimpFile) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::File *m_pFile;							/**< File to load from, must be always valid, opened and readable (just shared, not closed or even destroyed in here) */
		const char   *m_pszMagicDefaultFilename;		/**< Magic filename (concept based on "Assimp::MemoryIOSystem") - just shared, do not destroy it */
		const int     m_nMagicDefaultFilenameLength;	/**< Length of the magic filename */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp


#endif // __PLASSIMP_IOSYSTEM_H__
