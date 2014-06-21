/*********************************************************\
 *  File: CoreApplication.cpp                            *
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
#ifdef LINUX
	#include <signal.h>
#endif
#include "PLCore/Runtime.h"
#include "PLCore/Log/Log.h"
#include "PLCore/File/Directory.h"
#include "PLCore/System/System.h"
#include "PLCore/System/Console.h"
#include "PLCore/Tools/Timing.h"
#include "PLCore/Tools/LoadableManager.h"
#include "PLCore/Tools/Localization.h"
#include "PLCore/Tools/LocalizationGroup.h"
#include "PLCore/Application/CoreApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(CoreApplication)


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
CoreApplication *CoreApplication::g_pApplication = nullptr;	/**< Pointer to the current application instance */


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
CoreApplication::CoreApplication() :
	m_bMultiUser(true),
	m_bRunning(false),
	m_nResult(0)
{
	// Set global application pointer
	g_pApplication = this;

	// Set default application name and title
	m_sName  = Url(Url(System::GetInstance()->GetExecutableFilename()).GetFilename()).CutExtension();
	m_sTitle = m_sName;

	// Set default config and log filenames
	m_sConfigName = m_sName + ".cfg";
	m_sLogName = m_sName + ".log";

	// Set default application data subdir to "PixelLight" (Windows) respectively ".pixellight" (Linux)
	// -> Why this setting? At first, this was "m_sAppDataSubdir = System::GetInstance()->GetDataDirName(m_sName);" resulting in e.g.
	//    "C:\Users\COfenberg\AppData\Roaming\50RendererTriangleD", "C:\Users\COfenberg\AppData\Roaming\50RendererTriangle", "C:\Users\COfenberg\AppData\Roaming\XTest42"
	//    and so on. Worst case is, that in the end, "Roaming" is full of spam files and it's hard to figure out what's trash and what's important. So, by default, everything
	//    PixelLight based will be thrown into a common "PixelLight"-directory. More experienced users may then, explicitly, change this into an individual directory.
	m_sAppDataSubdir = System::GetInstance()->GetDataDirName("PixelLight");

	// Add standard command line options
	m_cCommandLine.AddFlag(		"Help",			"-h", "--help",			"Display help");
	m_cCommandLine.AddFlag(		"About",		"-V", "--version",		"Display version");
	m_cCommandLine.AddFlag(		"Verbose",		"-v", "--verbose",		"Write log messages onto the console");
	m_cCommandLine.AddParameter("LogFile",		"-l", "--logfile",		"Log filename",		"");
	m_cCommandLine.AddParameter("ConfigFile",	"-c", "--configfile",	"Config filename",	"");
}

/**
*  @brief
*    Destructor
*/
CoreApplication::~CoreApplication()
{
	// Reset global application pointer
	g_pApplication = nullptr;
}

/**
*  @brief
*    Set application name
*/
void CoreApplication::SetName(const String &sName)
{
	// Semi-hack: Adjust config and log filenames, if they are set to their default values
	if (m_sConfigName == m_sName + ".cfg")
		m_sConfigName = sName + ".cfg";
	if (m_sLogName == m_sName + ".log")
		m_sLogName = sName + ".log";

	// Set new name
	m_sName = sName;
}

/**
*  @brief
*    Run the application
*/
int CoreApplication::Run(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// Connect Linux signals
	#ifdef LINUX
		signal(SIGINT,  CoreApplication::SignalHandler);
		signal(SIGTERM, CoreApplication::SignalHandler);
	#endif

	// Fill application context
	m_cApplicationContext.SetStartupDirectory(System::GetInstance()->GetCurrentDir());
	m_cApplicationContext.SetExecutableFilename(sExecutableFilename);
	m_cApplicationContext.SetArguments(lstArguments);

	// Do the life cycle thing - let the world know that we have been created (must be done in here instead of within the constructor)
	OnCreate();

	// The application is now running
	m_bRunning = true;

	// Initialize application
	m_nResult  = 0;
	if (OnStart()) {
		OnResume();

		// Call application-specific initialization routine
		OnInit();

		// Run application
		Main();

		// Call application-specific de-initialization routine
		OnDeInit();

		// Do the frontend life cycle thing - de-initialize
		OnPause();
		OnStop();
	}

	// The application is no longer running
	m_bRunning = false;

	// Do the life cycle thing - let the world know that we're going to die (should be done in here instead of within the destructor)
	OnDestroy();

	// Exit
	return m_nResult;
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called directly after the object has been created
*/
void CoreApplication::OnCreate()
{
	// No default implementation
}

/**
*  @brief
*    Called directly before a stopped object is going to start again (always followed by "OnStart()")
*/
void CoreApplication::OnRestart()
{
	// No default implementation
}

/**
*  @brief
*    Initialization function that is called prior to OnInit()
*/
bool CoreApplication::OnStart()
{
	// Parse command line
	m_cCommandLine.ParseCommandLine(GetApplicationContext().GetArguments());

	// Create log
	OnInitLog();
	if (!m_bRunning)
		return false;

	// Check command line
	OnInitCmdLine();
	if (!m_bRunning)
		return false;

	// Load configuration
	OnInitConfig();
	if (!m_bRunning)
		return false;

	// Init plugins
	OnInitPlugins();
	if (!m_bRunning)
		return false;

	// Init data
	OnInitData();
	if (!m_bRunning)
		return false;

	// No error
	return true;
}

/**
*  @brief
*    Called when the object has the focus (keep the implementation lightweight)
*/
void CoreApplication::OnResume()
{
	// No default implementation
}

/**
*  @brief
*    Called when the object has no longer the focus (keep the implementation lightweight)
*/
void CoreApplication::OnPause()
{
	// No default implementation
}

/**
*  @brief
*    De-initialization function that is called after OnDeInit()
*/
void CoreApplication::OnStop()
{
	// Save configuration
	const String sFilename = m_cApplicationContext.GetConfigFilename();
	if (sFilename.GetLength())
		m_cConfig.SaveByFilename(sFilename);

	// Close log
	Log::GetInstance()->Close();
}

/**
*  @brief
*    Called before the object is going to be finally destroyed
*/
void CoreApplication::OnDestroy()
{
	// No default implementation
}


//[-------------------------------------------------------]
//[ Protected virtual CoreApplication functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Main function
*/
void CoreApplication::Main()
{
	// No default implementation
}

/**
*  @brief
*    Called when application should initialize it's log
*/
void CoreApplication::OnInitLog()
{
	// Create log
	Log *pLog = Log::GetInstance();
	if (pLog) {
		// Check if a log file has been given on the command line
		String sLog = m_cCommandLine.GetValue("--logfile");
		if (sLog.GetLength()) {
			// Try to open log, do nothing if that fails - command line is command line ;-)
			pLog->Open(sLog);
		} else {
			// Could not open log, try standard locations
			if (m_sLogName.GetLength()) {
				// Create user data directory, if it does not yet exist
				Directory cDir(System::GetInstance()->GetUserDataDir() + '/' + m_sAppDataSubdir);
				if (!cDir.Exists())
					cDir.Create();

				// Try user directory first, then use current directory
				sLog = System::GetInstance()->GetUserDataDir() + '/' + m_sAppDataSubdir + '/' + m_sLogName;
				if (!(m_bMultiUser && pLog->Open(sLog))) {
					// Not successful, so try current directory instead
					sLog = System::GetInstance()->GetCurrentDir() + '/' + m_sLogName;
					if (!pLog->Open(sLog)) {
						// Error: Could not open log
						sLog = "";
					}
				}
			}
		}

		// Log opened?
		if (sLog.GetLength()) {
			// Save log filename in application context
			m_cApplicationContext.SetLogFilename(sLog);

			// Set verbose mode
			if (m_cCommandLine.IsValueSet("--verbose"))
				pLog->SetVerbose(true);
		}
	}

	{ // Write some general information into the log
		// Create the log header:
		PL_LOG(Info, "Log-system started")
		PL_LOG(Quiet, '\n')
		PL_LOG(Info, "< " + Runtime::GetVersion().ToString() + " >")
		PL_LOG(Info, "PLCore build: " __DATE__ " " __TIME__ "")
		PL_LOG(Info, "Application start time: " + System::GetInstance()->GetTime().ToString())
		PL_LOG(Info, "\nPLCore information:\n" + System::GetInstance()->GetInfo() + '\n')
		PL_LOG(Quiet, '\n')

		{ // Print out directory information:
			// System directories
			PL_LOG(Info, "System directories:")
			PL_LOG(Info, "- Executable: " + System::GetInstance()->GetExecutableFilename())
			PL_LOG(Info, "- Home:       " + System::GetInstance()->GetUserHomeDir())
			PL_LOG(Info, "- Data:       " + System::GetInstance()->GetUserDataDir())
			PL_LOG(Info, "- Current:    " + System::GetInstance()->GetCurrentDir())
			PL_LOG(Quiet, '\n')

			// PixelLight directories
			PL_LOG(Info, "PixelLight directories:")
			PL_LOG(Info, "- Runtime:    " + Runtime::GetDirectory())
			PL_LOG(Quiet, '\n')

			// Application context
			PL_LOG(Info, "Application context:")
			PL_LOG(Info, "- Executable: " + GetApplicationContext().GetExecutableFilename())
			PL_LOG(Info, "- Startup:    " + GetApplicationContext().GetStartupDirectory())
			PL_LOG(Info, "- AppDir:     " + GetApplicationContext().GetAppDirectory())
			PL_LOG(Quiet, '\n')
		}

		// Print out the general system info:
		PL_LOG(Info, String::Format("CPU: %d MHz", System::GetInstance()->GetCPUMhz()))

		// Start enumerate messages
		PL_LOG(Quiet, "\n\n")
		PL_LOG(Info, "Messages:")
	}
}

/**
*  @brief
*    Called when application should check command line options
*/
void CoreApplication::OnInitCmdLine()
{
	// Check command line
	if (m_cCommandLine.HasErrors() || m_cCommandLine.IsValueSet("--help")) {
		// Display help and exit
		OnPrintHelp();
		Exit(1);
	} else if (m_cCommandLine.IsValueSet("--version")) {
		// Display about and exit
		OnPrintVersion();
		Exit(0);
	}
}

/**
*  @brief
*    Called when application should initialize it's configuration
*/
void CoreApplication::OnInitConfig()
{
	// Check if a config file has been given on the command line
	String sFilename = m_cCommandLine.GetValue("--configfile");
	if (sFilename.GetLength()) {
		// Try to load, do nothing if that fails - command line is command line ;-)
		m_cConfig.LoadByFilename(sFilename);
	} else {
		// Could not open config file, try standard locations
		if (m_sConfigName.GetLength()) {
			// Create user data directory, if it does not yet exist
			Directory cDir(System::GetInstance()->GetUserDataDir() + '/' + m_sAppDataSubdir);
			if (!cDir.Exists())
				cDir.Create();

			// Try user directory first, then use application (!) directory
			sFilename = System::GetInstance()->GetUserDataDir() + '/' + m_sAppDataSubdir + '/' + m_sConfigName;
			if (!(m_bMultiUser && m_cConfig.LoadByFilename(sFilename))) {
				// Not successful, so try application directory instead
				sFilename = m_cApplicationContext.GetAppDirectory() + '/' + m_sConfigName;
				if (!m_cConfig.LoadByFilename(sFilename)) {
					// Error: Could not open config file
					sFilename = "";
				}
			}
		}
	}

	// If no config file could be found, set a filename anyway so that the config can be saved later
	if (!sFilename.GetLength()) {
		// Only if we want a config at all
		if (m_sConfigName.GetLength()) {
			// Try user directory first, then use application directory
			Directory cDir(System::GetInstance()->GetUserDataDir() + '/' + m_sAppDataSubdir);
			if (m_bMultiUser && cDir.Exists())
				sFilename = System::GetInstance()->GetUserDataDir() + '/' + m_sAppDataSubdir + '/' + m_sConfigName;
			else
				sFilename = m_cApplicationContext.GetAppDirectory() + '/' + m_sConfigName;
		}
	}

	// Save config filename in application context
	m_cApplicationContext.SetConfigFilename(sFilename);

	// Analyze configuration
	if (m_sConfigName.GetLength()) {
		// Is this the first program start?
		if (m_cConfig.GetVar("PLCore::CoreConfig", "FirstRun").GetBool()) {
			// Call virtual function
			OnFirstProgramStart();

			// Reset flag
			m_cConfig.SetVar("PLCore::CoreConfig", "FirstRun", "0");
		}
	}
}

/**
*  @brief
*    Called when application should load it's plugins
*/
void CoreApplication::OnInitPlugins()
{
	// Scan PL-runtime directory for compatible plugins and load them in
	Runtime::ScanDirectoryPlugins();

	// Scan for plugins in the application executable directory, but not recursively, please. This is
	// quite useful for shipping applications and putting all plugins inside the application root directory
	// (which is necessary due to VC manifest policy)
	ClassManager::GetInstance()->ScanPlugins(m_cApplicationContext.GetExecutableDirectory(), NonRecursive);

	// There's no guarantee that the application executable directory is the same as the application startup directory
	// -> If the application executable directory is not the same as the application startup directory, do also scan the application startup directory
	// -> This behavior is quite useful because it allows development of plugins which can be tested within e.g. PLViewer without copying files around,
	//    just set the current directory to your plugin directory when launching the viewer application
	const String sStartupDirectory = m_cApplicationContext.GetStartupDirectory();
	if (m_cApplicationContext.GetExecutableDirectory() != sStartupDirectory) {
		// Scan for plugins in the application startup directory, but not recursively, please
		ClassManager::GetInstance()->ScanPlugins(sStartupDirectory, NonRecursive);
	}
}

/**
*  @brief
*    Called when application should set it's data paths
*/
void CoreApplication::OnInitData()
{
	// Scan PL-runtime directory for compatible data and register it
	Runtime::ScanDirectoryData();

	// Is '.' (= the current directory) already a base directory? If not, add it right now...
	LoadableManager *pLoadableManager = LoadableManager::GetInstance();
	if (!pLoadableManager->IsBaseDir('.'))
		pLoadableManager->AddBaseDir('.');

	// Is the application directory already a base directory? If not, add it right now...
	if (!pLoadableManager->IsBaseDir(m_cApplicationContext.GetAppDirectory()))
		pLoadableManager->AddBaseDir(m_cApplicationContext.GetAppDirectory());

	// The application and current directories should have the highest priority of all base directories because the application data is most likely in those directories
	pLoadableManager->SetBaseDirPriority(m_cApplicationContext.GetAppDirectory(), pLoadableManager->GetBaseDir(0));
	pLoadableManager->SetBaseDirPriority('.', pLoadableManager->GetBaseDir(1));

	// Scan for packages in current "Data" directory
	const String sCurrentDir = System::GetInstance()->GetCurrentDir();
	pLoadableManager->ScanPackages(sCurrentDir + "/Data");

	// Scan for packages in application's "Data" directory
	if (sCurrentDir != m_cApplicationContext.GetAppDirectory())
		pLoadableManager->ScanPackages(m_cApplicationContext.GetAppDirectory() + "/Data");

	// Get localization language (from config or from default)
	String sLanguage = m_cConfig.GetVar("PLCore::CoreConfig", "Language");
	if (!sLanguage.GetLength()) {
		// Use always English instead of the current program locale language so that we have a known default behavior
		sLanguage = "English";
	}

	// Setup localization system
	Localization::GetInstance()->SetLanguage(sLanguage);
	if (sLanguage.GetLength()) {
		LocalizationGroup *pLocalizationGroup = Localization::GetInstance()->AddGroup(Localization::PixelLight);
		if (pLocalizationGroup)
			pLocalizationGroup->LoadByFilename("Data/Misc/" + Localization::PixelLight + '_' + sLanguage + ".loc");
	}
}

/**
*  @brief
*    Called when application should initialize itself
*/
void CoreApplication::OnInit()
{
	// Reset timing class
	Timing::GetInstance()->Reset();
}

/**
*  @brief
*    Called when application should de-initialize itself
*/
void CoreApplication::OnDeInit()
{
	// No default implementation
}

/**
*  @brief
*    Function that is called when the program has been started for the first time
*/
void CoreApplication::OnFirstProgramStart()
{
	// Write message into log
	PL_LOG(Info, "First application start detected")
}

/**
*  @brief
*    Function that is called to display a help message about the application
*/
void CoreApplication::OnPrintHelp()
{
	// Print application title
	System::GetInstance()->GetConsole().Print(m_sTitle + "\n\n");

	// Print available command line options
	m_cCommandLine.PrintHelp(m_sName);
}

/**
*  @brief
*    Function that is called to display version information of the application
*/
void CoreApplication::OnPrintVersion()
{
	// Print application title and version
	System::GetInstance()->GetConsole().Print(m_sTitle + " - V" + m_cVersion.ToString() + '\n');
}

/**
*  @brief
*    Function that is called when a signal has arrive
*/
bool CoreApplication::OnSignal(ESignal nSignal)
{
	// Catch signal
	switch (nSignal) {
		// Interrupt (exit application by ctrl-c)
		case SignalInterrupt:
			// Exit application gracefully
			m_bRunning = false;
			return false;

		// Terminate (exit application)
		case SignalTerm:
			// Exit application gracefully
			m_bRunning = false;
			return false;
	}

	// Signal not handled, go on with default action
	return true;
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Signal handler callback
*/
void CoreApplication::SignalHandler(int nSignal)
{
	// Linux implementation
	#ifdef LINUX
		// Catch signal
		switch (nSignal) {
			// Interrupt (exit application by ctrl-c)
			case SIGINT:
				// Send signal to application
				if (!CoreApplication::GetApplication()->OnSignal(SignalInterrupt)) {
					// Ignore signal and restore handler
					signal(SIGINT, CoreApplication::SignalHandler);
				} else {
					// Signal handler has done it's job, re-raise signal
					signal(nSignal, SIG_DFL);
					raise(nSignal);
				}
				break;

			// Terminate (exit application)
			case SIGTERM:
				// Send signal to application
				if (!CoreApplication::GetApplication()->OnSignal(SignalTerm)) {
					// Ignore signal and restore handler
					signal(SIGTERM, CoreApplication::SignalHandler);
				} else {
					// Signal handler has done it's job, re-raise signal
					signal(nSignal, SIG_DFL);
					raise(nSignal);
				}
				break;
		}
	#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
