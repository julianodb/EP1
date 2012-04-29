//
//	file: cBasicViewer.cpp
//	Thiago C. Martins 
//	Last Updated: 22/7/1998
//
//	Implementation of cBasicViewer methods
//

#include "cBasicViewer.h"	// Declaration of cBasicViewer

#include <windows.h>	// Win32 API main header.
#include <process.h>	// Thread control functions.
#include <string.h>		// strcpy

using namespace::BVThread;	// Uses the thread functions
							// declared in BVThread

// Static members initialization
const LPCTSTR cBasicViewer::ClassName = "cBasicViewerWindow";
bool cBasicViewer::Registered = false;
HINSTANCE cBasicViewer::ModuleInstance = NULL;

// cBasicViewer constructor
// Author: Thiago C. Martins
// last updated: 22/7/1998
//
//!	Creates a multi-thread viewer.
//!	This method only creates the thread. All the window creation stuff
//!	is actually made by the thread.
//
cBasicViewer::cBasicViewer(int Width, int Height, char *title)
{
	// Sets the width and height
	this->width = max(Width,120);	// Minimum size is 120x100
	this->height = max(Height,100);	// max() is a Win32 macro defined
									// in windows.h
	
	// Sets the title
	strcpy(this->title, title);
	
	// Creates the mutexes
	DCMutex=CreateMutex(NULL, false, NULL);
	RunMutex=CreateMutex(NULL, false, NULL);

	// Creates the thread
	_beginthread((void (*)(void *))ViewerThreadProcess, 0, this); 
	// Waits until the thread gets the RunMutex
	while(WaitForSingleObject(RunMutex,0)!=WAIT_TIMEOUT)
		ReleaseMutex(RunMutex);
	// Waits until the thread creates a message loop
	while(!PostThreadMessage(ThreadID, TH_MESSAGE, TH_CHECK, NULL))
		Sleep(0);
	// At this point, the thread should already has created
	// the window, the memory device context and 
	// started the message loop.
}

// cBasicViewer destructor
// Author: Thiago C. Martins
// last updated: 22/7/1998
//
//!	Delete the viewer.
//!	This method only sends TH_DIE to the thread and waits his death.
//!	All the clean-up is made by the thread.
//
cBasicViewer::~cBasicViewer()
{
	// Sends TH_DIE to the thread
	PostThreadMessage(ThreadID, TH_MESSAGE, TH_DIE, NULL);
	// Waits the thread to die
	WaitForSingleObject(RunMutex, INFINITE);
	// At this point, the thread should already has destroyed the 
	// window and the memory device context.
}

// BeginDraw Method
// Author: Thiago C. Martins
// last updated: 22/07/1998
//
//!	This method locks the memory device context
//!	in order to receive GDI calls from the user.
//!	Each call to BeginDraw must have a corresponding call to EndDraw.
//
HDC cBasicViewer::BeginDraw()
{
	// Gets the DCMutex
	WaitForSingleObject(DCMutex, INFINITE);
	// Now, the thread souldn't access the memory device context
	// until EndPaint is called.
	return hMemDC;
}

// EndDraw Method
// Author: Thiago C. Martins
// last updated: 22/07/1998
//
//!	This method Unlocks the memory device context that was locked
//!	by BeginPaint.
//!	it must ALWAYS be called after BeginPaint
//
void cBasicViewer::EndDraw()
{
	// Just releases the DCMutex
	ReleaseMutex(DCMutex);
	// Now, the thread can access the memory device context again
}

// Show method
// Author: Thiago C. Martins
// last updated: 22/07/1998
//
//!	This method asks the thread to show his window
//
void cBasicViewer::Show()
{
	// Sends TH_SHOW to the thread
	PostThreadMessage(ThreadID, TH_MESSAGE, TH_SHOW, NULL);
}

// Hide method
// Author: Thiago C. Martins
// last updated: 22/07/1998
//
//!	This method asks the thread to hide his window
//
void cBasicViewer::Hide()
{
	// Sends TH_HIDE to the thread
	PostThreadMessage(ThreadID, TH_MESSAGE, TH_HIDE, NULL);
}

// Update method
// Author: Thiago C. Martins
// last updated: 22/07/1998
//
//!	This method asks the thread to repaint his window
//
void cBasicViewer::Update()
{
	// Sends TH_UPDATE to the thread
	PostThreadMessage(ThreadID, TH_MESSAGE, TH_UPDATE, NULL);
}

// RegisterViewerClass Method
// Author: Thiago C. Martins
// last updated: 22/07/1998
//
//!	This method registers the Viewer window class
//
void cBasicViewer::RegisterViewerClass()
{
	WNDCLASS wc;
	if(!Registered) {	// If the class hasn't been registered...

		// Get the current module instance
		ModuleInstance = (HINSTANCE)GetModuleHandle(NULL);
		
		// Initializes the window class
		wc.style = CS_OWNDC | BV_CLASSSTYLE;		// Requires an own
													// device context
		wc.lpfnWndProc = (WNDPROC)ViewerWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = ModuleInstance;
		wc.hIcon = VIEWER_ICON();
		wc.hCursor = VIEWER_CURSOR();
		// Don't repaint the background
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = ClassName;
	
		// Registers the window class
		RegisterClass(&wc);

		Registered = true;
	}
}
