/*********************************************************\
 *  File: LuaContext.inl                                 *
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
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Lua user data types                                   ]
//[-------------------------------------------------------]
// RTTIObjectPointer
inline RTTIObjectPointer &LuaContext::GetRTTIObjectPointer(Script &cScript, PLCore::Object *pRTTIObject)
{
	RTTIObjectPointer &cInstance = m_lstRTTIObjectPointer.Add();
	cInstance.InitializeInstance(cScript, pRTTIObject);
	return cInstance;
}

inline void LuaContext::ReleaseRTTIObjectPointer(RTTIObjectPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectPointer.RemoveElement(cInstance);
}

// RTTIObjectSlotPointer
inline RTTIObjectSlotPointer &LuaContext::GetRTTIObjectSlotPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynEventHandlerPtr pDynEventHandler)
{
	RTTIObjectSlotPointer &cInstance = m_lstRTTIObjectSlotPointer.Add();
	cInstance.InitializeInstance(cScript, pRTTIObject, pDynEventHandler);
	return cInstance;
}

inline void LuaContext::ReleaseRTTIObjectSlotPointer(RTTIObjectSlotPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectSlotPointer.RemoveElement(cInstance);
}

// RTTIObjectMethodPointer
inline RTTIObjectMethodPointer &LuaContext::GetRTTIObjectMethodPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynFuncPtr pDynFunc)
{
	RTTIObjectMethodPointer &cInstance = m_lstRTTIObjectMethodPointer.Add();
	cInstance.InitializeInstance(cScript, pRTTIObject, pDynFunc);
	return cInstance;
}

inline void LuaContext::ReleaseRTTIObjectMethodPointer(RTTIObjectMethodPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectMethodPointer.RemoveElement(cInstance);
}

// RTTIObjectSignalPointer
inline RTTIObjectSignalPointer &LuaContext::GetRTTIObjectSignalPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynEventPtr pDynEvent)
{
	RTTIObjectSignalPointer &cInstance = m_lstRTTIObjectSignalPointer.Add();
	cInstance.InitializeInstance(cScript, pRTTIObject, pDynEvent);
	return cInstance;
}

inline void LuaContext::ReleaseRTTIObjectSignalPointer(RTTIObjectSignalPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectSignalPointer.RemoveElement(cInstance);
}

// RTTIObjectSignalMethodPointer
inline RTTIObjectSignalMethodPointer &LuaContext::GetRTTIObjectSignalMethodPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynEventPtr pDynEvent, RTTIObjectSignalMethodPointer::EMethod nMethod)
{
	RTTIObjectSignalMethodPointer &cInstance = m_lstRTTIObjectSignalMethodPointer.Add();
	cInstance.InitializeInstance(cScript, pRTTIObject, pDynEvent, nMethod);
	return cInstance;
}

inline void LuaContext::ReleaseRTTIObjectSignalMethodPointer(RTTIObjectSignalMethodPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectSignalMethodPointer.RemoveElement(cInstance);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua
