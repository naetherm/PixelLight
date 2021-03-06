/*********************************************************\
 *  File: Gui.cpp                                        *
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
#include <PLCore/PLCore.h>
PL_WARNING_PUSH
	PL_WARNING_DISABLE(4127)	// "warning C4127: conditional expression is constant"
	#include <QtCore/qurl.h>
	#include <QtCore/qfilesystemwatcher.h>
	#include <QtGui/qevent.h>
	#include <QtGui/qlabel.h>
	#include <QtGui/qmenubar.h>
	#include <QtGui/qstatusbar.h>
	#include <QtGui/qfiledialog.h>
	#include <QtGui/qinputdialog.h>
	#include <QtGui/qdesktopservices.h>
	#include <QtGui/QMessageBox.h>
PL_WARNING_POP
#include <PLCore/Log/Log.h>
#include <PLCore/File/Url.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Script/Script.h>
#include <PLCore/System/System.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneQueries/SQByClassName.h>
#include <PLEngine/SceneCreator/SceneCreatorLoadableType.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLFrontendQt/Frontend.h>
#include <PLFrontendQt/QtStringAdapter.h>
#include <PLFrontendQt/FrontendMainWindow.h>
#include <PLFrontendQt/ConstructFileFilter.h>
#include <PLFrontendQt/DockWidget/DockWidget.h>
#include <PLFrontendQt/DockWidget/DockWidgetManager.h>
#include "ApplicationQt.h"
#include "GuiPicking.h"
#include "Gui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
using namespace PLEngine;
using namespace PLFrontendQt;
using namespace PLRenderer;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Gui::Gui(ApplicationQt &cApplication) :
	EventHandlerCameraFound(&Gui::OnCameraFound, this),
	m_pApplication(&cApplication),
	m_pGuiPicking(nullptr),
	m_pQFileSystemWatcher(new QFileSystemWatcher()),
	// Menu bar
	m_pQActionNew(nullptr),
	m_pQActionReload(nullptr),
	m_pQActionAutomaticReload(nullptr),
	m_pQActionSave(nullptr),
	m_pQActionSaveAs(nullptr),
	m_pQMenuCamera(nullptr),
	m_pQActionGroupCamera(nullptr),
	m_pQMenuWindow(nullptr),
	m_pQActionGroupWindow(nullptr),
	m_pQActionShowColorGradientBackground(nullptr),
	m_pQActionShowBackfacesAndSilhouettes(nullptr),
	m_pQActionReloadMaterials(nullptr),
	m_pQActionReloadScene(nullptr),
	// Status bar
	m_pQLabelStatusBar(nullptr)
{
	connect(m_pQFileSystemWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(QtSlotFileChanged(const QString&)));

	// Get the Qt main window
	FrontendMainWindow *pFrontendMainWindow = GetFrontendMainWindow();
	if (pFrontendMainWindow) {
		// Initialize the main window
		InitMainWindow(*pFrontendMainWindow);
	}
}

/**
*  @brief
*    Destructor
*/
Gui::~Gui()
{
	// Destroy the GUI picking instance
	if (m_pGuiPicking)
		delete m_pGuiPicking;

	// Destroy the Qt file system watcher instance
	delete m_pQFileSystemWatcher;
}

/**
*  @brief
*    Returns the owner application
*/
ApplicationQt &Gui::GetApplication() const
{
	return *m_pApplication;
}

/**
*  @brief
*    Get frontend main window
*/
FrontendMainWindow *Gui::GetFrontendMainWindow() const
{
	// Get Qt frontend implementation
	PLFrontendQt::Frontend *pFrontendQt = static_cast<PLFrontendQt::Frontend*>(m_pApplication->GetFrontend().GetImpl());
	if (pFrontendQt) {
		// Return the frontend main window
		return pFrontendQt->GetMainWindow();
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Sets whether or not the GUI is currently enabled
*/
void Gui::SetEnabled(bool bEnabled)
{
	// Get the Qt main window
	FrontendMainWindow *pFrontendMainWindow = GetFrontendMainWindow();
	if (pFrontendMainWindow) {
		// State change?
		if (pFrontendMainWindow->isEnabled() != bEnabled) {
			// Set new enabled state of the Qt main window
			pFrontendMainWindow->setEnabled(bEnabled);

			// Perform a dock widget manager broadcast
			pFrontendMainWindow->GetDockWidgetManager().CallDockWidgetsMethod("SetSceneContainer", Params<void, SceneContainer*>(bEnabled ? m_pApplication->GetScene() : nullptr));

			// Remove all files from the Qt file system watcher instance, but only when files where set
			// -> This avoids a warning printed by Qt into the console: "QFileSystemWatcher::removePaths: list is empty"
			if (m_pQFileSystemWatcher->files().count())
				m_pQFileSystemWatcher->removePaths(m_pQFileSystemWatcher->files());
		}

		// Is the GUI enabled?
		if (bEnabled) {
			// Create the GUI picking
			if (!m_pGuiPicking)
				m_pGuiPicking = new GuiPicking(*this);

			// Setup the update interval of the Qt main window (in milliseconds)
			pFrontendMainWindow->SetUpdateInterval(10);

			// Update reload and save Qt action
			m_pQActionReload->setEnabled(m_pApplication->GetResourceFilename().GetLength() != 0);
			m_pQActionSave->setEnabled(m_pApplication->GetResourceFilename().GetLength() != 0);

			// Reset and fill the Qt file system watcher instance
			ResetAndFillQFileSystemWatcher();

			// Automatically activate the GUI
			//pFrontendMainWindow->GetDockWidgetManager().CallDockWidgetsMethod("SelectObject", Params<void, Object*>(reinterpret_cast<Object*>(m_pApplication->GetScene())));
		} else {
			// Disable the timed update of the Qt main window
			pFrontendMainWindow->SetUpdateInterval(0);

			// Destroy the GUI picking
			if (m_pGuiPicking) {
				delete m_pGuiPicking;
				m_pGuiPicking = nullptr;
			}
		}
	}
}

/**
*  @brief
*    Sets the state text
*/
void Gui::SetStateText(const String &sText)
{
	// Update the Qt label shown in the status bar of the Qt main window
	if (m_pQLabelStatusBar)
		m_pQLabelStatusBar->setText(QtStringAdapter::PLToQt(sText));
}

/**
*  @brief
*    Opens a dialog in order to give the user a choice between multiple options
*/
String Gui::InputDialog(const String &sTitle, const String &sText, const Array<String> &lstOptions) const
{
	// Construct Qt string list
	QStringList cQStringList;
	for (uint32 i=0; i<lstOptions.GetNumOfElements(); i++)
		cQStringList << QtStringAdapter::PLToQt(lstOptions[i]);

	// Create Qt input dialog
	QInputDialog cQInputDialog(GetFrontendMainWindow(), Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	cQInputDialog.setLabelText(tr(sText));
	cQInputDialog.setComboBoxItems(cQStringList);

	{ // Set window title and size
		const int nMaxNumOfTitleCharacters = 100;
		const QFontMetrics &cQFontMetrics = cQInputDialog.fontMetrics();

		// Shorten title when the title has more then 100 characters
		QString sQTitle = tr(sTitle);
		if (sQTitle.length() > nMaxNumOfTitleCharacters)
			sQTitle = cQFontMetrics.elidedText(sQTitle, Qt::ElideMiddle, cQFontMetrics.averageCharWidth()*nMaxNumOfTitleCharacters);

		// Set window title
		cQInputDialog.setWindowTitle(sQTitle);

		// Usability: Set dialog width so we can see the complete title at once
		const int nWidth = cQFontMetrics.width(sQTitle) + 200;
		QSize cQSize = cQInputDialog.size();
		if (cQSize.width() < nWidth)
			cQSize.setWidth(nWidth);
		cQInputDialog.setFixedSize(cQSize);
	}

	// Open Qt input dialog
	return (cQInputDialog.exec() == QDialog::Accepted) ? QtStringAdapter::QtToPL(cQInputDialog.textValue()) : "";
}

/**
*  @brief
*    Shows a dock widget
*/
DockWidget *Gui::ShowDockWidget(const String &sClassName)
{
	// Get the Qt main window
	FrontendMainWindow *pFrontendMainWindow = GetFrontendMainWindow();
	if (pFrontendMainWindow) {
		// Show the requested dock widget
		return pFrontendMainWindow->GetDockWidgetManager().ShowDockWidget(sClassName);
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Updates the GUI
*/
void Gui::Update()
{
	// Perform the informative picking
	if (m_pGuiPicking)
		m_pGuiPicking->PerformInformativPicking();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes the main window
*/
void Gui::InitMainWindow(QMainWindow &cQMainWindow)
{
	{ // Menu bar
		{ // Setup the file menu
			QMenu *pQMenu = cQMainWindow.menuBar()->addMenu(tr("&File"));

			{ // Setup the new action
				m_pQActionNew = new QAction(tr("&New"), &cQMainWindow);
				connect(m_pQActionNew, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredNew()));
				m_pQActionNew->setShortcut(tr("Ctrl+T"));
				pQMenu->addAction(m_pQActionNew);
			}

			// Add a separator
			pQMenu->addSeparator();

			{ // Setup the load action
				QAction *pQAction = new QAction(tr("&Load"), &cQMainWindow);
				connect(pQAction, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredLoad()));
				pQAction->setShortcut(tr("Ctrl+L"));
				pQMenu->addAction(pQAction);
			}

			{ // Setup the reload action
				m_pQActionReload = new QAction(tr("&Reload"), &cQMainWindow);
				connect(m_pQActionReload, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredReload()));
				m_pQActionReload->setShortcut(tr("F5"));
				pQMenu->addAction(m_pQActionReload);
			}

			{ // Setup the automatic reload action
				m_pQActionAutomaticReload = new QAction(tr("&Automatic Reload"), &cQMainWindow);
				m_pQActionAutomaticReload->setCheckable(true);
				m_pQActionAutomaticReload->setChecked(true);
				connect(m_pQActionAutomaticReload, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredAutomaticReload()));
				pQMenu->addAction(m_pQActionAutomaticReload);
			}

			// Add a separator
			pQMenu->addSeparator();

			{ // Setup the save action
				m_pQActionSave = new QAction(tr("&Save"), &cQMainWindow);
				connect(m_pQActionSave, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredSave()));
				m_pQActionSave->setShortcut(tr("Ctrl+S"));
				pQMenu->addAction(m_pQActionSave);
			}

			{ // Setup the save dialog action
				m_pQActionSaveAs = new QAction(tr("Save &As..."), &cQMainWindow);
				connect(m_pQActionSaveAs, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredSaveAs()));
				m_pQActionSaveAs->setShortcut(tr("Ctrl+Shift+S"));
				pQMenu->addAction(m_pQActionSaveAs);
			}

			// Add a separator
			pQMenu->addSeparator();

			{ // Setup the exit action
				QAction *pQAction = new QAction(tr("E&xit"), &cQMainWindow);
				pQAction->setShortcuts(QKeySequence::Quit);
				connect(pQAction, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredExit()));
				pQAction->setShortcut(tr("Ctrl+Q"));
				pQMenu->addAction(pQAction);
			}
		}

		{ // Setup the camera menu
			m_pQMenuCamera = cQMainWindow.menuBar()->addMenu(tr("&Camera"));
			connect(m_pQMenuCamera, SIGNAL(aboutToShow()), this, SLOT(QtSlotAboutToShowMenuCamera()));

			// Menu is filled when it's about to show
		}

		{ // Setup the window menu
			m_pQMenuWindow = cQMainWindow.menuBar()->addMenu(tr("&Window"));
			connect(m_pQMenuWindow, SIGNAL(aboutToShow()), this, SLOT(QtSlotAboutToShowMenuWindow()));

			// Menu is filled when it's about to show
		}

		{ // Setup the tools menu
			QMenu *pQMenu = cQMainWindow.menuBar()->addMenu(tr("&Tools"));
			connect(pQMenu, SIGNAL(aboutToShow()), this, SLOT(QtSlotAboutToShowMenuTools()));

			{ // Setup the log action
				QAction *pQAction = new QAction(tr("Open Log File"), &cQMainWindow);
				connect(pQAction, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredOpenLogFile()));
				pQMenu->addAction(pQAction);
			}

			{ // Setup the show color gradient background action
				m_pQActionShowColorGradientBackground = new QAction(tr("Show Color Gradient Background"), &cQMainWindow);
				m_pQActionShowColorGradientBackground->setCheckable(true);
				connect(m_pQActionShowColorGradientBackground, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredShowColorGradientBackground()));
				pQMenu->addAction(m_pQActionShowColorGradientBackground);
			}

			{ // Setup the show backfaces and silhouettes action
				m_pQActionShowBackfacesAndSilhouettes = new QAction(tr("Show Backfaces and Silhouettes"), &cQMainWindow);
				m_pQActionShowBackfacesAndSilhouettes->setCheckable(true);
				connect(m_pQActionShowBackfacesAndSilhouettes, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredShowBackfacesAndSilhouettes()));
				pQMenu->addAction(m_pQActionShowBackfacesAndSilhouettes);
			}

			// Add a separator
			pQMenu->addSeparator();

			{ // Setup the reload materials action
				m_pQActionReloadMaterials = new QAction(tr("Reload only materials"), &cQMainWindow);
				connect(m_pQActionReloadMaterials, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredReloadMaterials()));
				m_pQActionReloadMaterials->setShortcut(tr("Ctrl+Shift+R"));
				pQMenu->addAction(m_pQActionReloadMaterials);
			}
			{ // Setup the reload scene action
				m_pQActionReloadScene = new QAction(tr("Reload only scene"), &cQMainWindow);
				connect(m_pQActionReloadScene, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredReloadScene()));
				m_pQActionReloadScene->setShortcut(tr("Alt+F5"));
				pQMenu->addAction(m_pQActionReloadScene);
			}
		}

		{ // Setup the help menu
			QMenu *pQMenu = cQMainWindow.menuBar()->addMenu(tr("&Help"));

			{ // Setup the open PixelLight website action
				QAction *pQAction = new QAction(tr("Open PixelLight Website"), &cQMainWindow);
				connect(pQAction, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredOpenPixelLightWebsite()));
				pQMenu->addAction(pQAction);
			}
		}
	}

	{ // Status bar
		// Create the Qt label shown in the status bar of the Qt main window and add it to Qt main window status bar
		m_pQLabelStatusBar = new QLabel();
		cQMainWindow.statusBar()->addWidget(m_pQLabelStatusBar);
	}
}

/**
*  @brief
*    Called when a camera scene node was found
*/
void Gui::OnCameraFound(SceneQuery &cQuery, SceneNode &cSceneNode)
{
	// Camera Qt menu there?
	if (m_pQMenuCamera) {
		// Add camera to the menu
		QAction *pQAction = m_pQMenuCamera->addAction(QtStringAdapter::PLToQt(cSceneNode.GetAbsoluteName()));
		pQAction->setCheckable(true);
		if (reinterpret_cast<SceneNode*>(m_pApplication->GetCamera()) == &cSceneNode)
			pQAction->setChecked(true);
		m_pQActionGroupCamera->addAction(pQAction);
	}
}

/**
*  @brief
*    Fills the window menu recursively
*/
uint32 Gui::FillMenuWindowRec(QMenu &cQMenu, const String &sBaseClass)
{
	// Number of checked items
	uint32 nCheckedItems = 0;

	// Get a list of RTTI classes derived from the current base class
	List<const Class*> lstClasses;
	ClassManager::GetInstance()->GetClasses(lstClasses, sBaseClass, NonRecursive, NoBase, IncludeAbstract);
	ConstIterator<const Class*> cIterator = lstClasses.GetConstIterator();
	while (cIterator.HasNext()) {
		// Get the current RTTI class
		const Class *pClass = cIterator.Next();

		// Abstract class?
		if (pClass->HasConstructor()) {
			// Class can be instanced

			// Add action
			QAction *pQAction = cQMenu.addAction(tr(pClass->GetProperties().Get("Title")));
			pQAction->setCheckable(true);
			pQAction->setData(QtStringAdapter::PLToQt(pClass->GetClassName()));
			m_pQActionGroupWindow->addAction(pQAction);

			// One can also derive from classes which can be instanced, but by convention this should not be done for RTTI dock widgets

			// Get the Qt main window
			FrontendMainWindow *pFrontendMainWindow = GetFrontendMainWindow();

			// Is there currently an instance of this dock widget already visible?
			DockWidget *pDockWidget = pFrontendMainWindow ? pFrontendMainWindow->GetDockWidgetManager().GetFirstDockWidget(pClass->GetClassName()) : nullptr;
			if (pDockWidget && pDockWidget->IsQDockWidgetVisible()) {
				pQAction->setChecked(true);
				nCheckedItems++;
			} else {
				pQAction->setChecked(false);
			}
		} else {
			// Abstract class will result in a sub-menu, but only if at least one none abstract class is derived from it
			List<const Class*> lstDerivedClasses;
			ClassManager::GetInstance()->GetClasses(lstDerivedClasses, pClass->GetClassName(), Recursive, NoBase, NoAbstract);
			if (lstDerivedClasses.GetNumOfElements()) {
				// Add sub-menu
				QMenu *pQMenuSub = cQMenu.addMenu(tr(pClass->GetProperties().Get("Title")));

				// Automatically fill the Qt window menu by using RTTI information
				nCheckedItems += FillMenuWindowRec(*pQMenuSub, pClass->GetClassName());
			}
		}
	}

	// Return the number of checked items
	return nCheckedItems;
}

/**
*  @brief
*    Resets and fills the Qt file system watcher instance
*/
void Gui::ResetAndFillQFileSystemWatcher()
{
	// Remove all files from the Qt file system watcher instance, but only when files where set
	// -> This avoids a warning printed by Qt into the console: "QFileSystemWatcher::removePaths: list is empty"
	if (m_pQFileSystemWatcher->files().count())
		m_pQFileSystemWatcher->removePaths(m_pQFileSystemWatcher->files());

	// Update the Qt file system watcher instance
	if (m_pQActionAutomaticReload && m_pQActionAutomaticReload->isChecked()) {
		// Add files to the Qt file system watcher instance
		const String sResourceFilename = m_pApplication->GetResourceFilename();
		if (sResourceFilename.GetLength()) {
			// Lookout!
			// -> The current resource filename (e.g. "Data\Scripts\Lua\Main.lua") can be relative to the base directory received by
			//    "PLEngine::ScriptApplication::GetBaseDirectory()" (e.g. "C:\MyApplication\")
			// -> Concatenating the two filenames results in the valid absolute path (e.g. "C:\MyApplication\Data\Scripts\Lua\Main.lua")
			// -> We can't provide "QFileSystemWatcher::addPath()" just with the relative filename because this filename is considered to be relative
			//    to the base directory (e.g. "C:\MyApplication\"), not the current system directory (e.g. "C:\MyApplication\x86") you can query by
			//    using "PLCore::System::GetCurrentDir()". Qt can only know the current system directory and the result would be potentially
			//    wrong (e.g. "C:\MyApplication\x86\Data\Scripts\Lua\Main.lua").
			// -> Solution: Feed "QFileSystemWatcher::addPath()" only with absolute filenames!
			const Url cUrl = Url(sResourceFilename);
			const QString cRessourceFile = QtStringAdapter::PLToQt(cUrl.IsAbsolute() ? cUrl.GetNativePath() : (GetApplication().GetBaseDirectory() + cUrl.GetNativePath()));

			// Add the resource file itself
			m_pQFileSystemWatcher->addPath(cRessourceFile);

			// In case the resource is a script, do also take the associated filenames into account
			Script *pScript = m_pApplication->GetScript();
			if (pScript) {
				// Get a list of associated filenames
				Array<String> lstFilenames;
				pScript->GetAssociatedFilenames(lstFilenames);

				// Add the filenames to the Qt file system watcher instance
				for (uint32 i=0; i<lstFilenames.GetNumOfElements(); i++)
					m_pQFileSystemWatcher->addPath(QtStringAdapter::PLToQt(Url(lstFilenames[i]).GetNativePath()));
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private Qt slots (MOC)                                ]
//[-------------------------------------------------------]
void Gui::QtSlotFileChanged(const QString &path)
{
	// Resetting materials
	RendererContext *pMaterials = m_pApplication->GetRendererContext();
	pMaterials->GetMaterialManager().Clear();

	// Reload the resource, not the provided file
	// -> In case of scripts, the main script may be "Main.lua" but the included "Application.lua" may have just been changed
	const String sResourceFilename = m_pApplication->GetResourceFilename();
	if (sResourceFilename.GetLength())
		m_pApplication->LoadResource(sResourceFilename);
}

void Gui::QtSlotTriggeredLoad()
{
	// Fill the file filter (filter example: "All Files (*);;Scene (*.scene *.SCENE);;Script (*.lua *.LUA)")
	const String sAllFiles = QtStringAdapter::QtToPL(tr("All Files"));
	String sFileFilter = sAllFiles + " (*)";
	sFileFilter += ";;";
	sFileFilter += ConstructFileFilter::ByLoadableType(QtStringAdapter::QtToPL(tr("Scene")), "Scene");
	sFileFilter += ";;";
	sFileFilter += ConstructFileFilter::ByScriptLanguages(QtStringAdapter::QtToPL(tr("Script")));

	{ // Fill the file filter - the highly dynamic part
		// Get a list of loadable types were it's possible to use a scene creator in order to
		// configure a scene, everything else is not interesting in this viewer
		Array<String> lstLoadableTypes;
		SceneCreatorLoadableType::GetLoadableTypes(lstLoadableTypes);

		// Fill the file filter
		if (lstLoadableTypes.GetNumOfElements()) {
			sFileFilter += ";;";
			for (uint32 i=0; i<lstLoadableTypes.GetNumOfElements(); i++) {
				// Get the current loadable type...
				const String &sType = lstLoadableTypes[i];

				// ... and add it to the file filter
				sFileFilter += ConstructFileFilter::ByLoadableType(QtStringAdapter::QtToPL(tr(sType)), sType);

				// Will another file filter follow?
				if (i < lstLoadableTypes.GetNumOfElements()-1)
					sFileFilter += ";;";
			}
		}
	}

	// Open a file dialog were the user can choose a filename
	QString sQSelectedFilter;
	const QString sQFilename = QFileDialog::getOpenFileName(GetFrontendMainWindow(), "", "", QtStringAdapter::PLToQt(sFileFilter), &sQSelectedFilter);

	// Filename chosen?
	if (sQFilename.length()) {
		// Get the loadable type
		String sType;
		String sSelectedFilter = QtStringAdapter::QtToPL(sQSelectedFilter);
		const int nIndex = sSelectedFilter.IndexOf(" (");
		if (nIndex > -1)
			sType = sSelectedFilter.GetSubstring(0, nIndex);

		// Load the resource
		m_pApplication->LoadResource(QtStringAdapter::QtToPL(sQFilename), (sType != sAllFiles) ? sType : "");
	}
}

void Gui::QtSlotTriggeredReload()
{
	//Reload materials
	RendererContext *pMaterials = m_pApplication->GetRendererContext();
	pMaterials->GetMaterialManager().ReloadMaterials();

	//Reload scene
	const String sResourceFilename = m_pApplication->GetResourceFilename();
	m_pApplication->LoadResource(sResourceFilename);
}

void Gui::QtSlotTriggeredAutomaticReload()
{
	// Reset and fill the Qt file system watcher instance
	ResetAndFillQFileSystemWatcher();
}

void Gui::QtSlotTriggeredSave()
{
	// Get the scene container with our 'concrete scene'
	SceneContainer *pSceneContainer = m_pApplication->GetScene();

	// Hack(?) :(
	pSceneContainer->SetAttribute("Filename", "");

	// Get filename
	const String sResourceFilename = m_pApplication->GetResourceFilename();

	// Save scene (not reloading the scene when saving even if the "Automatic Reload" tick is checked)
	if (m_pQActionAutomaticReload->isChecked()) {
		m_pQActionAutomaticReload->setChecked(false);
		ResetAndFillQFileSystemWatcher();
		pSceneContainer->SaveByFilename(sResourceFilename);
		m_pQActionAutomaticReload->setChecked(true);
		ResetAndFillQFileSystemWatcher();
	} else {
		pSceneContainer->SaveByFilename(sResourceFilename);
	}

}

void Gui::QtSlotTriggeredSaveAs()
{
	// Fill the file filter Scene (*.scene *.SCENE)")
	//String sFileFilter = ConstructFileFilter::ByLoadableType(QtStringAdapter::QtToPL(tr("Scene")), "Scene");
	String sFileFilter = "Scene (*.scene)";

	// Open a file dialog were the user can choose a filename
	const QString sQFilename = QFileDialog::getSaveFileName(GetFrontendMainWindow(), "", "", QtStringAdapter::PLToQt(sFileFilter));

	// Get the scene container with our 'concrete scene'
	SceneContainer *pSceneContainer = m_pApplication->GetScene();

	// Hack(?) :(
	pSceneContainer->SetAttribute("Filename", "");

	// Save scene (not reloading the scene when saving even if the "Automatic Reload" tick is checked)
	if (m_pQActionAutomaticReload->isChecked()) {
		m_pQActionAutomaticReload->setChecked(false);
		ResetAndFillQFileSystemWatcher();
		pSceneContainer->SaveByFilename(QtStringAdapter::QtToPL(sQFilename));
		m_pQActionAutomaticReload->setChecked(true);
		ResetAndFillQFileSystemWatcher();
	} else {
		pSceneContainer->SaveByFilename(QtStringAdapter::QtToPL(sQFilename));
	}	
}

void Gui::QtSlotTriggeredNew()
{
	// Get filename
	const String sResourceFilename = m_pApplication->GetResourceFilename();

	// To display a dialogue with warning
	QMessageBox msgBox;
	msgBox.setWindowTitle("PLViewerQt");
	msgBox.setText(
		"You're going to create a new scene.\n"
		"All changes of the current scene will be lost."
	);
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setInformativeText("Would you like to save the current scene?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int dialog = msgBox.exec(); 
	switch (dialog) {
		case QMessageBox::Save:
			// To display a dialogue "save as"
			Gui::QtSlotTriggeredSaveAs();
			// Then create an empty scene
			m_pApplication->OnCreateRootScene();
			// And unload old scene
			m_pApplication->LoadResource("");
			break;
		case QMessageBox::Discard:
			// Create an empty scene
			m_pApplication->OnCreateRootScene();
			// And unload old scene (if the scene has already been unloaded, do not make unnecessary actions)
			if (sResourceFilename.GetLength())
				m_pApplication->LoadResource("");
			break;
		default:
			// nothing
			break;
	}
}

void Gui::QtSlotTriggeredExit()
{
	// Shut down the application
	m_pApplication->Exit(0);
}

void Gui::QtSlotAboutToShowMenuCamera()
{
	// Camera Qt menu there?
	if (m_pQMenuCamera) {
		// Clear the previous content - this is not performance critical so there's no reason to implement a
		// more complex solution like e.g. updating the menu entries as soon as there's a change within the scene
		m_pQMenuCamera->clear();
		if (m_pQActionGroupCamera)
			delete m_pQActionGroupCamera;
		m_pQActionGroupCamera = new QActionGroup(GetFrontendMainWindow());
		connect(m_pQActionGroupCamera, SIGNAL(selected(QAction*)), this, SLOT(QtSlotSelectedCamera(QAction*)));

		{ // Setup the select current camera action
			QAction *pQActionHideAll = new QAction(tr("Select Current Camera"), GetFrontendMainWindow());
			connect(pQActionHideAll, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredCameraSelectCurrentCamera()));
			m_pQMenuCamera->addAction(pQActionHideAll);
		}

		// Add a separator
		m_pQMenuCamera->addSeparator();

		// Automatically fill the Qt camera menu by using the cameras which are within the scene

		// Get the scene container with our 'concrete scene'
		SceneContainer *pSceneContainer = m_pApplication->GetScene();
		if (pSceneContainer) {
			// Create a scene query instance
			SQByClassName *pSceneQuery = static_cast<SQByClassName*>(pSceneContainer->CreateQuery("PLScene::SQByClassName"));
			if (pSceneQuery) {
				// Setup the regular expression of the scene query
				pSceneQuery->SetRegEx(RegEx("PLScene::SNCamera"));

				// Connect event handler
				pSceneQuery->SignalSceneNode.Connect(EventHandlerCameraFound);

				// Perform the scene query
				pSceneQuery->PerformQuery();

				// Destroy scene query instance
				pSceneContainer->DestroyQuery(*pSceneQuery);
			}
		}
	}
}

void Gui::QtSlotTriggeredCameraSelectCurrentCamera()
{
	// Get the Qt main window
	FrontendMainWindow *pFrontendMainWindow = GetFrontendMainWindow();
	if (pFrontendMainWindow) {
		// Get the camera scene node currently used by the application and make it to the currently selected object
		pFrontendMainWindow->GetDockWidgetManager().CallDockWidgetsMethod("SelectObject", Params<void, Object*>(reinterpret_cast<Object*>(m_pApplication->GetCamera())));
	}
}

void Gui::QtSlotSelectedCamera(QAction *pQAction)
{
	// Get the scene container with our 'concrete scene'
	SceneContainer *pSceneContainer = m_pApplication->GetScene();
	if (pSceneContainer) {
		// Get the chosen camera scene node
		SceneNode *pSceneNode = pSceneContainer->GetByName(QtStringAdapter::QtToPL(pQAction->text()));
		if (pSceneNode && pSceneNode->IsCamera()) {
			// Set the new application camera
			m_pApplication->SetCamera(reinterpret_cast<SNCamera*>(pSceneNode));
		}
	}
}

void Gui::QtSlotAboutToShowMenuWindow()
{
	// Window Qt menu there?
	if (m_pQMenuWindow) {
		// Get the Qt main window
		FrontendMainWindow *pFrontendMainWindow = GetFrontendMainWindow();

		// Clear the previous content - this is not performance critical so there's no reason to implement a
		// more complex solution like e.g. updating the menu entries as soon as there's a change within the scene
		m_pQMenuWindow->clear();
		if (m_pQActionGroupWindow)
			delete m_pQActionGroupWindow;
		m_pQActionGroupWindow = new QActionGroup(pFrontendMainWindow);
		m_pQActionGroupWindow->setExclusive(false);
		connect(m_pQActionGroupWindow, SIGNAL(selected(QAction*)), this, SLOT(QtSlotSelectedWindow(QAction*)));

		// Setup the hide all action
		QAction *pQActionHideAll = new QAction(tr("Hide all"), pFrontendMainWindow);
		connect(pQActionHideAll, SIGNAL(triggered()), this, SLOT(QtSlotTriggeredWindowHideAll()));
		pQActionHideAll->setShortcut(tr("Ctrl+H"));
		m_pQMenuWindow->addAction(pQActionHideAll);

		// Add a separator
		m_pQMenuWindow->addSeparator();

		// Automatically fill the Qt window menu by using RTTI information
		if (!FillMenuWindowRec(*m_pQMenuWindow, "PLFrontendQt::DockWidget"))
			pQActionHideAll->setEnabled(false);	// Disable "Hide all" menu entry if there's currently nothing to hide
	}
}

void Gui::QtSlotTriggeredWindowHideAll()
{
	// Get the Qt main window
	FrontendMainWindow *pFrontendMainWindow = GetFrontendMainWindow();
	if (pFrontendMainWindow) {
		// All registered dock widgets
		pFrontendMainWindow->GetDockWidgetManager().HideDockWidgets();

		// Usability: When hiding all windows at once, do also unselect the current object (if one is selected)
		// -> A kind of "Disable edit mode and switch back to business as usual"
		// -> Perform a dock widget manager broadcast
		pFrontendMainWindow->GetDockWidgetManager().CallDockWidgetsMethod("SelectObject", Params<void, Object*>(nullptr));
	}
}

void Gui::QtSlotSelectedWindow(QAction *pQAction)
{
	// Show the requested dock widget
	ShowDockWidget(QtStringAdapter::QtToPL(pQAction->data().toString()));
}

void Gui::QtSlotAboutToShowMenuTools()
{
	// Get the configuration and scene renderer tool instance
	Config &cConfig = m_pApplication->GetConfig();
	SceneRendererTool &cSceneRendererTool = m_pApplication->GetSceneRendererTool();

	// Update actions according to the configuration or disable them completely in case the option would have no effect in the first place
	m_pQActionShowColorGradientBackground->setVisible(cSceneRendererTool.GetPassByName("BackgroundColorGradient") != nullptr);
	if (m_pQActionShowColorGradientBackground->isVisible())
		m_pQActionShowColorGradientBackground->setChecked(cConfig.GetVarInt("PLViewerQtConfig", "ShowColorGradientBackground") != 0);
	m_pQActionShowBackfacesAndSilhouettes->setVisible(cSceneRendererTool.GetPassByName("DebugWireframes") != nullptr);
	if (m_pQActionShowBackfacesAndSilhouettes->isVisible())
		m_pQActionShowBackfacesAndSilhouettes->setChecked(cConfig.GetVarInt("PLViewerQtConfig", "ShowBackfacesAndSilhouettes") != 0);
}

void Gui::QtSlotTriggeredOpenLogFile()
{
	// Use "PLCore::System::Execute()" to open the log file which is usually a simple text file
	// -> "QDesktopServices::openUrl(QtStringAdapter::PLToQt(Log::GetInstance()->GetFilename()));" didn't work for me
	System::GetInstance()->Execute(Log::GetInstance()->GetFilename(), "");
}

void Gui::QtSlotTriggeredShowColorGradientBackground()
{
	// Update the configuration
	if (m_pQActionShowColorGradientBackground)
		m_pApplication->GetConfig().SetVar("PLViewerQtConfig", "ShowColorGradientBackground", m_pQActionShowColorGradientBackground->isChecked());
}

void Gui::QtSlotTriggeredShowBackfacesAndSilhouettes()
{
	// Update the configuration
	if (m_pQActionShowBackfacesAndSilhouettes)
		m_pApplication->GetConfig().SetVar("PLViewerQtConfig", "ShowBackfacesAndSilhouettes", m_pQActionShowBackfacesAndSilhouettes->isChecked());
}

void Gui::QtSlotTriggeredReloadMaterials()
{
	//Reload only materials
	RendererContext *pMaterials = m_pApplication->GetRendererContext();
	pMaterials->GetMaterialManager().ReloadMaterials();
}

void Gui::QtSlotTriggeredReloadScene()
{
	//Reload only scene
	const String sResourceFilename = m_pApplication->GetResourceFilename();
	m_pApplication->LoadResource(sResourceFilename);
}

void Gui::QtSlotTriggeredOpenPixelLightWebsite()
{
	QDesktopServices::openUrl(QUrl("http://www.pixellight.org"));
}
