/*********************************************************\
 *  File: SoundManager.cpp                               *
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
#include <PLCore/File/File.h>
#include <PLCore/Core/MemoryManager.h>
#include <PLCore/Log/Log.h>
#include <PLCore/Tools/Localization.h>
#include <PLCore/Tools/LoadableManager.h>
#include "PLSoundFMODEx/SoundManager.h"
#include "PLSoundFMODEx/Buffer.h"
#include "PLSoundFMODEx/Source.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLSoundFMODEx {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SoundManager, "PLSoundFMODEx", PLSound::SoundManager, "FMOD Ex sound backend")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(Driver,		PLCore::int32,								-1,							ReadWrite,	"Selects a sound card driver. It is used when an output mode has enumerated more than one output device, and you need to select between them.",		"")
	pl_attribute_metadata(Output,		pl_enum_type_def3(SoundManager, EOutput),	SoundManager::AUTODETECT,	ReadWrite,	"This function selects the output mode for the platform. This is for selecting different OS specific API's which might have different features.",	"")
	pl_attribute_metadata(Channels,		PLCore::uint32,								32,							ReadWrite,	"Maximum number of SOFTWARE channels available (HARDWARE channels are auto detected)",																"")
	pl_attribute_metadata(BufferLength,	PLCore::uint32,								1024,						ReadWrite,	"The mixer engine block size in samples. Use this to adjust mixer update granularity.",																"")
	pl_attribute_metadata(NumOfBuffers,	PLCore::uint32,								4,							ReadWrite,	"The mixer engine number of buffers used. Use this to adjust mixer latency.",																		"")
pl_class_metadata_end(SoundManager)


//[-------------------------------------------------------]
//[ Custom FMOD Ex file callbacks                         ]
//[-------------------------------------------------------]
FMOD_RESULT F_CALLBACK FSOpen(const char *pszName, int nUnicode, unsigned int *pFileSize, void **ppHandle, void **ppUserData)
{
	if (pszName) {
		// Create the file object
		File *pFile = new File();

		// Try to open the file
		if (LoadableManager::GetInstance()->OpenFile(*pFile, pszName, false)) {
			// Return opened file object
			*pFileSize	= pFile->GetSize();
			*ppHandle	= pFile;
			*ppUserData	= reinterpret_cast<void*>(0x12345678);

			// Done
			return FMOD_OK;
		}

		// Cleanup on error
		delete pFile;
	}

	// Error!
	return FMOD_ERR_FILE_NOTFOUND;
}

FMOD_RESULT F_CALLBACK FSClose(void *pHandle, void *pUserData)
{
	if (pHandle) {
		static_cast<File*>(pHandle)->Close();
		delete static_cast<File*>(pHandle);

		return FMOD_OK;
	} else {
		return FMOD_ERR_INVALID_PARAM;
	}
}

FMOD_RESULT F_CALLBACK FSRead(void *pHandle, void *pBuffer, unsigned int nSizeBytes, unsigned int *pnBytesRead, void *pUserData)
{
	if (pHandle) {
		if (pnBytesRead) {
			*pnBytesRead = static_cast<File*>(pHandle)->Read(pBuffer, 1, nSizeBytes);
			if (*pnBytesRead < nSizeBytes)
				return FMOD_ERR_FILE_EOF;
		}

		return FMOD_OK;
	} else {
		return FMOD_ERR_INVALID_PARAM;
	}
}

FMOD_RESULT F_CALLBACK FSSeek(void *pHandle, unsigned int nPos, void *pUserData)
{
	if (pHandle) {
		static_cast<File*>(pHandle)->Seek(nPos, File::SeekSet);

		return FMOD_OK;
	} else {
		return FMOD_ERR_INVALID_PARAM;
	}
}


//[-------------------------------------------------------]
//[ Custom FMOD Ex memory callbacks                       ]
//[-------------------------------------------------------]
void * F_CALLBACK FMODExAllocationFunction(unsigned int nSize, FMOD_MEMORY_TYPE nType, const char *pszSourceString)
{
	// 'nType' is not used (that's NO problem!)
	return MemoryManager::Allocator(MemoryManager::NewArray, nSize);
}

void * F_CALLBACK FMODExReallocationFunction(void *pAddress, unsigned int nSize, FMOD_MEMORY_TYPE nType, const char *pszSourceString)
{
	// 'nType' is not used (that's NO problem!)
	return MemoryManager::Reallocator(pAddress, nSize);
}

void F_CALLBACK FMODExDeallocationFunction(void *pAddress, FMOD_MEMORY_TYPE nType, const char *pszSourceString)
{
	// 'nType' is not used (that's NO problem!)
	if (pAddress)
		MemoryManager::Deallocator(MemoryManager::DeleteArray, pAddress);
}


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Checks for an FMOD Ex error
*/
bool SoundManager::ErrorCheck(FMOD_RESULT nResult)
{
	if (nResult != FMOD_OK) {
		PL_LOG(Error, String::Format("FMOD error! (%d) %s\n", nResult, FMOD_ErrorString(nResult)))

		// Error!
		return false;
	} else {
		return true;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SoundManager::SoundManager() :
	Driver(this),
	Output(this),
	Channels(this),
	BufferLength(this),
	NumOfBuffers(this),
	m_pSystem(nullptr),
	m_fVolume(1.0f),
	m_fPitch(1.0f),
	m_fDopplerFactor(1.0f)
{
	// Log message
	PL_LOG(Info, "Create sound manager: " + GetDescription())

	// Initialize listener attributes
	SetListenerAttribute(ListenerForward, Vector3::UnitZ);
	SetListenerAttribute(ListenerUpward,  Vector3::UnitY);
}

/**
*  @brief
*    Destructor
*/
SoundManager::~SoundManager()
{
	// Log message
	PL_LOG(Info, "Destroy sound manager: " + GetDescription())

	// De-initialize
	DeInit();
}

/**
*  @brief
*    Adds an active sound source to the sound manager
*/
bool SoundManager::AddActiveSource(PLSound::Source &cSource)
{
	return (!m_lstActiveSources.IsElement(&cSource) && m_lstActiveSources.Add(&cSource) != nullptr);
}

/**
*  @brief
*    Removes an active sound source from the sound manager
*/
bool SoundManager::RemoveActiveSource(PLSound::Source &cSource)
{
	return m_lstActiveSources.Remove(&cSource);
}

/**
*  @brief
*    Returns the FMOD Ex system instance
*/
FMOD::System *SoundManager::GetSystem() const
{
	return m_pSystem;
}


//[-------------------------------------------------------]
//[ Public virtual PLSound::SoundManager functions        ]
//[-------------------------------------------------------]
String SoundManager::GetDescription() const
{
	static const String sString = "FMOD Ex sound";
	return sString;
}

bool SoundManager::GetFormatList(List<Format> &lstList) const
{
	// The official list of supported FMOD file formats:
	static const char *szSupportedFormats[] = {
		// Extension	Description
		"AIFF",			"Audio Interchange File Format",
		"ASF",			"Advanced Streaming format, includes support for the audio tracks in video streams",
		"ASX",			"playlist format - contains links to other audio files. To access contents, the FMOD Ex tag API is used",
		"DLS",			"DownLoadable Sound format for midi playback. Can also be used as a stand alone container format in FMOD",
		"FLAC",			"Lossless compression codec",
		"FSB",			"FMOD sample bank format generated by FSBank and FMOD designer tool",
		"IT",			"Impulse tracker sequenced mod format. FMOD Ex also fully supports resonant filters in .IT files, and the per channel or per instrument echo effect send, that can be enabled in ModPlug Tracker. This is cross platform effect support and does not require DirectX like other libraries do",
		"M3U",			"playlist format - contains links to other audio files. To access contents, the FMOD Ex tag API is used",
		"MID",			"MIDI using operating system or custom DLS patches",
		"MOD",			"Protracker / Fasttracker and others sequenced mod format",
		"MP2",			"MPEG I/II Layer 2",
		"MP3",			"MPEG I/II Layer 3, including VBR support",
		"OGG",			"Ogg Vorbis format",
		"PLS",			"playlist format - contains links to other audio files. To access contents, the FMOD Ex tag API is used",
		"RAW",			"Raw file format support. The user can specify the number of channels, bit depth, format etc",
		"S3M",			"ScreamTracker 3 sequenced mod format",
		"VAG",			"PS2 / PSP format, supported on all platforms!",
		"WAV",			"Microsoft Wave files, including compressed wavs. PCM, MP3 and IMA ADPCM compressed wav files are supported across all platforms in FMOD Ex, and other compression formats are supported via windows codecs on that platform",
		"WAX",			"playlist format - contains links to other audio files. To access contents, the FMOD Ex tag API is used",
		"WMA",			"Windows Media Audio format",
		"XM",			"(FastTracker 2 sequenced format",
		"\0"			// Notes the end of the list
	};

	// Add all supported file formats to the given list
	for (uint32 i=0; *szSupportedFormats[i] != '\0'; i+=2) {
		{ // Lower case
			Format &cFormat = lstList.Add();
			if (&cFormat != &List<Format>::Null) {
				cFormat.sFormat = szSupportedFormats[i];
				cFormat.sFormat.ToLower();
				cFormat.sDescription = szSupportedFormats[i+1];
			}
		}

		{ // Upper case
			Format &cFormat = lstList.Add();
			if (&cFormat != &List<Format>::Null) {
				cFormat.sFormat = szSupportedFormats[i];
				cFormat.sFormat.ToUpper();
				cFormat.sDescription = szSupportedFormats[i+1];
			}
		}
	}

	// Done
	return true;
}

float SoundManager::GetVolume() const
{
	return m_fVolume;
}

void SoundManager::SetVolume(float fVolume)
{
	// No change = nothing to do :)
	if (m_fVolume != fVolume) {
		m_fVolume = fVolume;

		// Call FMOD Ex function
		if (m_pSystem) {
			FMOD::ChannelGroup *pChannelGroup;
			m_pSystem->getMasterChannelGroup(&pChannelGroup);
			if (pChannelGroup)
				pChannelGroup->setVolume(fVolume);
		}
	}
}

float SoundManager::GetPitch() const
{
	return m_fPitch;
}

void SoundManager::SetPitch(float fPitch)
{
	// Check and set pitch -> no change = nothing to do :)
	if (m_fPitch != fPitch) {
		m_fPitch = fPitch;

		// Call FMOD Ex function
		if (m_pSystem) {
			FMOD::ChannelGroup *pChannelGroup;
			m_pSystem->getMasterChannelGroup(&pChannelGroup);
			if (pChannelGroup)
				pChannelGroup->setPitch(fPitch);
		}
	}
}

float SoundManager::GetDopplerFactor() const
{
	return m_fDopplerFactor;
}

void SoundManager::SetDopplerFactor(float fFactor)
{
	// No change = nothing to do :)
	if (m_fDopplerFactor != fFactor) {
		m_fDopplerFactor = fFactor;

		// Call FMOD Ex function
		if (m_pSystem) {
			float fDopplerscale, fDistancefactor, fRolloffscale;
			if (m_pSystem->get3DSettings(&fDopplerscale, &fDistancefactor, &fRolloffscale) == FMOD_OK)
				m_pSystem->set3DSettings(fFactor, fDistancefactor, fRolloffscale);
		}
	}
}


//[-------------------------------------------------------]
//[ Create sound buffer/source                            ]
//[-------------------------------------------------------]
PLSound::Buffer *SoundManager::CreateSoundBuffer(const String &sFilename, bool bStream)
{
	// Is there already a sound buffer with this name?
	PLSound::Buffer *pSB = GetByName(sFilename);
	return pSB ? pSB : new Buffer(*this, sFilename, bStream);
}

PLSound::Source *SoundManager::CreateSoundSource(PLSound::Buffer *pSoundBuffer)
{
	// Create the FMOD Ex sound source
	PLSound::Source *pSS = new Source(*this);
	if (pSoundBuffer)
		pSS->Load(pSoundBuffer);

	// Return the created sound source
	return pSS;
}


//[-------------------------------------------------------]
//[ Listener                                              ]
//[-------------------------------------------------------]
Vector3 SoundManager::GetListenerAttribute(EListener nAttribute) const
{
	return m_vListenerAttributes[nAttribute];
}

bool SoundManager::SetListenerAttribute(EListener nAttribute, const Vector3 &vV)
{
	// Set attribute
	m_vListenerAttributes[nAttribute] = vV;

	// FMODEx uses a left handed coordinate system so we need to convert
	Vector3 vForward = m_vListenerAttributes[ListenerForward];
	vForward.z = -vForward.z;
	Vector3 vUpward = m_vListenerAttributes[ListenerUpward];
	vUpward.z = -vUpward.z;

	// Setup listener
	m_pSystem->set3DListenerAttributes(0,
									   reinterpret_cast<const FMOD_VECTOR*>(&m_vListenerAttributes[ListenerPosition].x),
									   reinterpret_cast<const FMOD_VECTOR*>(&m_vListenerAttributes[ListenerVelocity].x),
									   reinterpret_cast<const FMOD_VECTOR*>(&vForward.x),
									   reinterpret_cast<const FMOD_VECTOR*>(&vUpward.x));

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::ResourceManager functions      ]
//[-------------------------------------------------------]
bool SoundManager::Init()
{
	// Show FMOD Ex version
	PL_LOG(Info, String::Format("Initialize FMOD Ex (v%08x)", FMOD_VERSION))

	// Set new memory management functions
	FMOD_RESULT nResult = FMOD_Memory_Initialize(nullptr, 0, FMODExAllocationFunction, FMODExReallocationFunction, FMODExDeallocationFunction, FMOD_MEMORY_NORMAL);
	if (ErrorCheck(nResult)) {
		// Create a system object
		FMOD::System_Create(&m_pSystem);
		if (ErrorCheck(nResult)) {
			// Check the version
			unsigned int nVersion = 0;
			nResult = m_pSystem->getVersion(&nVersion);
			if (ErrorCheck(nResult)) {
				if (nVersion >= FMOD_VERSION) {
					// Set driver
					m_pSystem->setDriver(Driver);

					// Set output
					m_pSystem->setOutput(static_cast<FMOD_OUTPUTTYPE>(Output.GetInt()));

					// Set buffer size
					m_pSystem->setDSPBufferSize(BufferLength, NumOfBuffers);

					// Initialize the system object
					nResult = m_pSystem->init(Channels, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
					if (ErrorCheck(nResult)) {
						// Set file system callback functions
						nResult = m_pSystem->setFileSystem(FSOpen, FSClose, FSRead, FSSeek, nullptr, nullptr, 2048);
						if (ErrorCheck(nResult))
							return true; // Done
					}
				} else {
					PL_LOG(Error, String::Format("You are using an old version of FMOD %08x. This program requires %08x", nVersion, FMOD_VERSION))
				}
			}

			// Cleanup on error
			m_pSystem->release();
			m_pSystem = nullptr;
		}
	}

	// Error!
	return false;
}

bool SoundManager::DeInit()
{
	// Destroy all resources - the resources unregister automatically from the list, so, this while loop will terminate!
	while (m_lstResources.GetNumOfElements())
		delete m_lstResources[0];
	m_lstResources.Clear();

	// Call base implementation
	PLSound::SoundManager::DeInit();

	// Close FMOD Ex
	if (m_pSystem) {
		m_pSystem->release();
		m_pSystem = nullptr;
	}

	// Done
	return true;
}

bool SoundManager::Update()
{
	// Check system instance
	if (!m_pSystem)
		return false; // Error!

	// Update FMOD Ex
	m_pSystem->update();

	// Check active sources
	for (uint32 i=0; i<m_lstActiveSources.GetNumOfElements(); i++) {
		PLSound::Source *pSource = m_lstActiveSources[i];
		if (!pSource->IsPlaying()) {
			m_lstActiveSources.RemoveAtIndex(i);
			i--;
		}
	}

	// Update statistics
	m_sStatistics.nNumOfActiveSources = m_lstActiveSources.GetNumOfElements();

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ResourceManager functions     ]
//[-------------------------------------------------------]
PLSound::Buffer *SoundManager::CreateResource(const String &sName)
{
	return CreateSoundBuffer(sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundFMODEx
