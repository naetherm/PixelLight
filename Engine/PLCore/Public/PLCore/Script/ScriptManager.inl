/*********************************************************\
 *  File: ScriptManager.inl                              *
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
*    Returns a list of the names of the supported script languages
*/
inline const Array<String> &ScriptManager::GetScriptLanguages()
{
	RegisterClasses();
	return m_lstScriptLanguages;
}

/**
*  @brief
*/
inline String ScriptManager::GetScriptLanguageByExtension(const String &sExtension)
{
	RegisterClasses();
	return m_mapScriptLanguagesByExtension.Get(sExtension);
}

/**
*  @brief
*    Checks whether or not a given script language is supported
*/
inline bool ScriptManager::IsSupported(const String &sScriptLanguage)
{
	RegisterClasses();
	return (m_mapScriptLanguages.Get(sScriptLanguage) != nullptr);
}

/**
*  @brief
*    Returns a list of all script binding instances
*/
inline const Array<ScriptBinding*> &ScriptManager::GetScriptBindings()
{
	RegisterClasses();
	return m_lstScriptBindings;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Register a class
*/
inline void ScriptManager::OnClassLoaded(const Class *pClass)
{
	// Just put the new class on the queue - because we can't assume that this class is or can be initialized
	// already, we can't even expect "IsDerivedFrom" to work, so, just REALLY note this class in here!
	m_lstNewClasses.Add(pClass);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
