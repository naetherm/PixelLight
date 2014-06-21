/*********************************************************\
 *  File: FrontendAndroid.cpp                            *
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
#include <PLCore/Runtime.h>
#include <PLCore/Log/Log.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/System/System.h>
#include <PLCore/System/SystemAndroid.h>
#include <PLCore/Frontend/Frontend.h>
#include <PLCore/Frontend/FrontendContext.h>
#include "PLFrontendOS/FrontendAndroid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendOS {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(FrontendAndroid)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendAndroid::FrontendAndroid(const FrontendContext &cFrontendContext, struct android_app &cAndroidApp) :
	m_pAndroidApp(&cAndroidApp),
	m_cFrontend(cFrontendContext, *this),
	m_bQuit(false),
	m_nTimeToWait(0),
	m_bAnimating(true),
	m_nMousePositionX(0),
	m_nMousePositionY(0),
	m_bMouseVisible(true)
{
	// Bring the PixelLight log into verbose mode so we can see the log output within the
	// Android in-kernel log buffer as well (use Androids "logcat" utility to access this system log)
	Log::GetInstance()->SetVerbose(true);

	// Do also show debug messages
	Log::GetInstance()->SetLogLevel(Log::Debug);

	// Write the used Android SDK version into the log
	PL_LOG(Info, String("Using Android SDK version ") + AConfiguration_getSdkVersion(cAndroidApp.config))

	// Configure the given native Android application instance
	cAndroidApp.userData     = this;
	cAndroidApp.onAppCmd     = onAppCmd;
	cAndroidApp.onInputEvent = onInputEvent;

	// Tell PixelLight about the native Android application
	SystemAndroid::SetAndroidApp(&cAndroidApp);

	// Tell PixelLight about the Android asset manager (this is required in order to be able to load files from the apk)
	SystemAndroid::SetAssetManager(cAndroidApp.activity->assetManager);

	// Write console messages into the Android in-kernel log buffer is used as well
	SystemAndroid::SetConsoleToKernelLog(true);

	// Scan PL-runtime directory for compatible plugins and load them in as well as scan for compatible data and register it
	Runtime::ScanDirectoryPluginsAndData();

	// Set parameters for the frontend RTTI class
	if (cFrontendContext.GetFrontendParameters().GetLength())
		m_cFrontend.SetValues(cFrontendContext.GetFrontendParameters());

	// Set parameters for the frontend implementation RTTI class
	if (cFrontendContext.GetFrontendImplementationParameters().GetLength())
		SetValues(cFrontendContext.GetFrontendImplementationParameters());

	// Do the frontend life cycle thing - let the world know that we have been created
	OnCreate();

	// Dummy command line arguments
	const String sExecutableFilename;
	const Array<String> lstArguments;

	// Let the world know that this frontend is now going to run
	OnRun(sExecutableFilename, lstArguments);

	// Action!
	Run(sExecutableFilename, lstArguments);
}

/**
*  @brief
*    Destructor
*/
FrontendAndroid::~FrontendAndroid()
{
	// Do the frontend life cycle thing - let the world know that we're going to die
	OnDestroy();
}


//[-------------------------------------------------------]
//[ Private "android_native_app_glue.h" callback functions ]
//[-------------------------------------------------------]
/**
*  @brief
*    Process the next main command
*/
void FrontendAndroid::onAppCmd(struct android_app* app, int32_t cmd)
{
	FrontendAndroid *pFrontendAndroid = static_cast<FrontendAndroid*>(app->userData);
	switch (cmd) {
		// Command from main thread: the AInputQueue has changed. Upon processing
		// this command, android_app->inputQueue will be updated to the new queue (or nullptr).
		case APP_CMD_INPUT_CHANGED:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_INPUT_CHANGED")
			break;

		// Command from main thread: a new ANativeWindow is ready for use. Upon
		// receiving this command, android_app->window will contain the new window surface.
		case APP_CMD_INIT_WINDOW:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_INIT_WINDOW")
			if (pFrontendAndroid->OnStart())
				pFrontendAndroid->OnResume();
			break;

		// Command from main thread: the existing ANativeWindow needs to be
		// terminated. Upon receiving this command, android_app->window still
		// contains the existing window; after calling android_app_exec_cmd
		// it will be set to nullptr.
		case APP_CMD_TERM_WINDOW:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_TERM_WINDOW")
			// Do the frontend life cycle thing - de-initialize
			pFrontendAndroid->OnPause();
			pFrontendAndroid->OnStop();
			break;

		// Command from main thread: the current ANativeWindow has been resized.
		// Please redraw with its new size.
		case APP_CMD_WINDOW_RESIZED:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_WINDOW_RESIZED")
			break;

		// Command from main thread: the system needs that the current ANativeWindow
		// be redrawn. You should redraw the window before handing this to
		// android_app_exec_cmd() in order to avoid transient drawing glitches.
		case APP_CMD_WINDOW_REDRAW_NEEDED:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_WINDOW_REDRAW_NEEDED")
			break;

		// Command from main thread: the content area of the window has changed,
		// such as from the soft input window being shown or hidden.  You can
		// find the new content rect in android_app::contentRect.
		case APP_CMD_CONTENT_RECT_CHANGED:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_CONTENT_RECT_CHANGED")
			break;

		// Command from main thread: the app's activity window has gained input focus.
		case APP_CMD_GAINED_FOCUS:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_GAINED_FOCUS")
			break;

		// Command from main thread: the app's activity window has lost input focus.
		case APP_CMD_LOST_FOCUS:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_LOST_FOCUS")
			break;

		// Command from main thread: the current device configuration has changed.
		case APP_CMD_CONFIG_CHANGED:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_CONFIG_CHANGED")
			break;

		// Command from main thread: the system is running low on memory.
		// Try to reduce your memory use.
		case APP_CMD_LOW_MEMORY:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_LOW_MEMORY")
			break;

		// Command from main thread: the app's activity has been started.
		case APP_CMD_START:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_START")
			// Do the frontend life cycle thing
//			pFrontendAndroid->OnStart();
			break;

		// Command from main thread: the app's activity has been resumed.
		case APP_CMD_RESUME:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_RESUME")
			// Do the frontend life cycle thing
//			pFrontendAndroid->OnResume();
			break;

		// Command from main thread: the app should generate a new saved state
		// for itself, to restore from later if needed.  If you have saved state,
		// allocate it with malloc and place it in android_app.savedState with
		// the size in android_app.savedStateSize.  The will be freed for you later.
		case APP_CMD_SAVE_STATE:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_SAVE_STATE")
			break;

		// Command from main thread: the app's activity has been paused.
		case APP_CMD_PAUSE:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_PAUSE")
			// Do the frontend life cycle thing
		//	pFrontendAndroid->OnPause();
			break;

		// Command from main thread: the app's activity has been stopped.
		case APP_CMD_STOP:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_STOP")
			// Do the frontend life cycle thing
		//	pFrontendAndroid->OnStop();
			break;

		// Command from main thread: the app's activity is being destroyed,
		// and waiting for the app thread to clean up and exit before proceeding.
		case APP_CMD_DESTROY:
			// [DEBUG]
			PL_LOG(Debug, "! APP_CMD_DESTROY")
			break;
	}
}

// [HACK] There's going something funny on in the NDK:
//   ANativeActivity_showSoftInput(activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);
// and
//   ANativeActivity_hideSoftInput(activity, ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);
// are not working.
// The following code is using jni to show/hide the soft keyboard.
// Source: http://stackoverflow.com/questions/5864790/how-to-show-the-soft-keyboard-on-native-activity
void displayKeyboard(ANativeActivity* activity, bool pShow) {
	// Attaches the current thread to the JVM.
	jint lResult;
	jint lFlags = 0;

	JavaVM* lJavaVM = activity->vm;
	JNIEnv* lJNIEnv = activity->env;

	JavaVMAttachArgs lJavaVMAttachArgs;
	lJavaVMAttachArgs.version = JNI_VERSION_1_6;
	lJavaVMAttachArgs.name = "NativeThread";
	lJavaVMAttachArgs.group = nullptr;

	lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
	if (lResult == JNI_ERR) {
		return;
	}

	// Retrieves NativeActivity.
	jobject lNativeActivity = activity->clazz;
	jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

	// Retrieves Context.INPUT_METHOD_SERVICE.
	jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
	jfieldID FieldINPUT_METHOD_SERVICE =
		lJNIEnv->GetStaticFieldID(ClassContext,
			"INPUT_METHOD_SERVICE", "Ljava/lang/String;");
	jobject INPUT_METHOD_SERVICE =
		lJNIEnv->GetStaticObjectField(ClassContext,
			FieldINPUT_METHOD_SERVICE);
	//jniCheck(INPUT_METHOD_SERVICE);

	// Runs getSystemService(Context.INPUT_METHOD_SERVICE).
	jclass ClassInputMethodManager = lJNIEnv->FindClass(
		"android/view/inputmethod/InputMethodManager");
	jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(
		ClassNativeActivity, "getSystemService",
		"(Ljava/lang/String;)Ljava/lang/Object;");
	jobject lInputMethodManager = lJNIEnv->CallObjectMethod(
		lNativeActivity, MethodGetSystemService,
		INPUT_METHOD_SERVICE);

	// Runs getWindow().getDecorView().
	jmethodID MethodGetWindow = lJNIEnv->GetMethodID(
		ClassNativeActivity, "getWindow",
		"()Landroid/view/Window;");
	jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity,
		MethodGetWindow);
	jclass ClassWindow = lJNIEnv->FindClass(
		"android/view/Window");
	jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(
		ClassWindow, "getDecorView", "()Landroid/view/View;");
	jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow,
		MethodGetDecorView);

	if (pShow) {
		// Runs lInputMethodManager.showSoftInput(...).
		jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID(
			ClassInputMethodManager, "showSoftInput",
			"(Landroid/view/View;I)Z");
		jboolean lResult = lJNIEnv->CallBooleanMethod(
			lInputMethodManager, MethodShowSoftInput,
			lDecorView, lFlags);
	} else {
		// Runs lWindow.getViewToken()
		jclass ClassView = lJNIEnv->FindClass(
			"android/view/View");
		jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(
			ClassView, "getWindowToken", "()Landroid/os/IBinder;");
		jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,
			MethodGetWindowToken);

		// lInputMethodManager.hideSoftInput(...).
		jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(
			ClassInputMethodManager, "hideSoftInputFromWindow",
			"(Landroid/os/IBinder;I)Z");
		jboolean lRes = lJNIEnv->CallBooleanMethod(
			lInputMethodManager, MethodHideSoftInput,
			lBinder, lFlags);
	}

	// Finished with the JVM.
	lJavaVM->DetachCurrentThread();
}

/**
*  @brief
*    Process the next input event
*/
int32_t FrontendAndroid::onInputEvent(struct android_app* app, struct AInputEvent* event)
{
	// Toggle soft input visibility by using the menu key (don't use the search key, not every device has such a key)
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY && AKeyEvent_getKeyCode(event) == AKEYCODE_MENU && AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN) {
		static bool bSoftKeyboardVisible = false;
		bSoftKeyboardVisible = !bSoftKeyboardVisible;
		displayKeyboard(app->activity, bSoftKeyboardVisible);

		// Done, input event processed
		return 1;
	} else {
		// Motion input event?
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) { 
			// Get the number of pointers of data contained in this event
			const size_t nAndroidPointerCount = AMotionEvent_getPointerCount(event);
			if (nAndroidPointerCount) {
				// Get this frontend instance
				FrontendAndroid *pFrontendAndroid = static_cast<FrontendAndroid*>(app->userData);
				if (pFrontendAndroid) {
					// Get the current X and Y coordinate of this event for the given pointer index
					pFrontendAndroid->m_nMousePositionX = static_cast<int>(AMotionEvent_getX(event, 0));
					pFrontendAndroid->m_nMousePositionY = static_cast<int>(AMotionEvent_getY(event, 0));
				}
			}
		}

		// Redirect the input event into the PixelLight system
		SystemAndroid::EmitInputEvent(*event);

		// Done, but do not mark input events as processed to allow e.g. important system keys to be processed as the user would expect
		return 0;
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::FrontendImpl functions        ]
//[-------------------------------------------------------]
int FrontendAndroid::Run(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// [TODO] Do the frontend life cycle thing - initialize
//	if (OnStart()) {
//		OnResume();

		// The frontend main loop
		m_bQuit = false;
		while (m_pFrontend && !m_bQuit) {
			// Time to wait?
			if (m_nTimeToWait) {
				// Let the system some time to process other system tasks etc.
				// If this isn't done the CPU usage is always up to 100%!!
				// Please note that there's no guaranty that the resulting FPS always reaches
				// exactly the maximum FPS limit.
				System::GetInstance()->Sleep(m_nTimeToWait);
			}

			// Redraw
			if (m_bAnimating) {
				// Drawing is throttled to the screen update rate, so there is no need to do timing here
				Redraw();
			}

			// Ping
			Ping();

			// Quit?
			// [TODO]
//			if (m_pFrontend->IsRunning())
//				m_bQuit = true;
		}

		// Do the frontend life cycle thing - de-initialize
//		OnPause();
//		OnStop();

		// Done
		return 0;
		/*
	} else {
		// Error!
		return -1;
	}
	*/
}

handle FrontendAndroid::GetNativeWindowHandle() const
{
	// There's no window handle!
	return reinterpret_cast<handle>(m_pAndroidApp->window);
}

void FrontendAndroid::Redraw()
{
	// Let the frontend draw into it's window
	OnDraw();
}

void FrontendAndroid::Ping()
{
	{ // Read all pending events
		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		int nEvents = 0;
		struct android_poll_source *pAndroidPollSource = nullptr;
		int nIdent = ALooper_pollAll(m_bAnimating ? 0 : -1, nullptr, &nEvents, reinterpret_cast<void**>(&pAndroidPollSource));
		while (nIdent >= 0) {
			// Process this event
			if (pAndroidPollSource)
				pAndroidPollSource->process(m_pAndroidApp, pAndroidPollSource);

			// Check if we are exiting
			if (m_pAndroidApp->destroyRequested)
				m_bQuit = true;

			// Read next event
			nIdent = ALooper_pollAll(m_bAnimating ? 0 : -1, nullptr, &nEvents, reinterpret_cast<void**>(&pAndroidPollSource));
		}
	}

	// Check if we're allowed to perform an update right now
	if (Timing::GetInstance()->Update(&m_nTimeToWait)) {
		// Let the frontend update it's states
		OnUpdate();
	}
}

String FrontendAndroid::GetTitle() const
{
	// That's nothing we have control in here
	return "";
}

void FrontendAndroid::SetTitle(const String &sTitle)
{
	// That's nothing we have control in here
}

int FrontendAndroid::GetX() const
{
	// Lookout! We need to use the current content rectangle of the window, not the window surface
	// because the OS may add something like a virtual back-key to the window surface.
	return m_pAndroidApp->contentRect.left;
}

int FrontendAndroid::GetY() const
{
	// Lookout! We need to use the current content rectangle of the window, not the window surface
	// because the OS may add something like a virtual back-key to the window surface.
	return m_pAndroidApp->contentRect.top;
}

uint32 FrontendAndroid::GetWidth() const
{
	// Lookout! We need to use the current content rectangle of the window, not the window surface
	// because the OS may add something like a virtual back-key to the window surface.
	return m_pAndroidApp->contentRect.right - m_pAndroidApp->contentRect.left;
}

uint32 FrontendAndroid::GetHeight() const
{
	// Lookout! We need to use the current content rectangle of the window, not the window surface
	// because the OS may add something like a virtual back-key to the window surface.
	return m_pAndroidApp->contentRect.bottom - m_pAndroidApp->contentRect.top;
}

void FrontendAndroid::GetWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const
{
	// That's nothing we have control in here

	// Set known default values
	nX = nY = nWidth = nHeight = 0;
}

void FrontendAndroid::SetWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
	// That's nothing we have control in here
}

bool FrontendAndroid::GetToggleFullscreenMode() const
{
	// That's nothing we have control in here
	return false;
}

void FrontendAndroid::SetToggleFullscreenMode(bool bToggleFullscreenMode)
{
	// That's nothing we have control in here
}

bool FrontendAndroid::GetFullscreenAltTab() const
{
	// That's nothing we have control in here
	return false;
}

void FrontendAndroid::SetFullscreenAltTab(bool bAllowed)
{
	// That's nothing we have control in here
}

bool FrontendAndroid::IsFullscreen() const
{
	// That's nothing we have control in here
	return true;
}

void FrontendAndroid::SetFullscreen(bool bFullscreen)
{
	// That's nothing we have control in here
}

void FrontendAndroid::RefreshFullscreen()
{
	// That's nothing we have control in here
}

bool FrontendAndroid::IsMouseOver() const
{
	// We're always in fullscreen, so, "the mouse" is always watching over us
	return true;
}

int FrontendAndroid::GetMousePositionX() const
{
	return m_nMousePositionX;
}

int FrontendAndroid::GetMousePositionY() const
{
	return m_nMousePositionY;
}

bool FrontendAndroid::IsMouseVisible() const
{
	return m_bMouseVisible;
}

void FrontendAndroid::SetMouseVisible(bool bVisible)
{
	m_bMouseVisible = bVisible;
}

void FrontendAndroid::SetTrapMouse(bool bTrap)
{
	// Android has no mouse *touchscreen*
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendOS
