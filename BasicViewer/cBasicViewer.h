//
//	file: cBasicViewer.h
//	Thiago C. Martins
//
//	Contains the declaration of class cBasicViewer and related stuff.
//
//	cBasicViewer is a class that encapsulates the Win32 API that
//	creates a graphic window and manages it. The window is created inside
//	a new thread. All the messages to the window are handled by the thread,
//	in order to prevent the main program to be 'locked' into a message 
//	loop. All the GDI calls from the main program are made on a memory
//	device context.
//
//		Last updated: 22/07/1998
//


#ifndef _BASICVIEWER_	// Avoids multiple inclusions
#define _BASICVIEWER_

class cBasicViewer;

#ifdef _MSC_VER			
#ifndef _MT				// Requires the multi-thread version
#define _MT				// of the run-time library
#endif					// (only under microsoft c)
#endif

#include <windows.h>	// Win32 API main header.

// Thread messages definition
#define TH_MESSAGE	0xC100		//! All messages from the viewer object 
								//! must have this number
// value of uParam:
#define TH_CHECK	0x0000		//! Checks comunication with thread
#define TH_DIE		0x0012		//! Asks the death to die
#define TH_SHOW		0x0013		//! Asks the thread to show his window
#define TH_HIDE		0x0014		//! Asks the thread to hide his window
#define TH_UPDATE	0x0015		//! Asks the thread to repaint his window

// Those macros defines the icon and cursor of the viewer window
#define VIEWER_ICON()  LoadIcon((HINSTANCE)NULL, IDI_WINLOGO)
#define VIEWER_CURSOR() LoadCursor((HINSTANCE)NULL, IDC_ARROW)

// Defines the window and window's class styles
#define BV_WNDSTYLE WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX
#ifdef BV_NOCLOSE
	#define BV_CLASSSTYLE CS_NOCLOSE
#else 
	#define BV_CLASSSTYLE 0
#endif

class cBasicViewer;

//! Declaration of the thread functions
namespace BVThread {
	//! The thread main function. Take as parameter a pointer to the
	//! object that created the thread.
	//! It creates a graphic window, then enters a message loop.
	//! Commands are sent to it by the TH_MESSAGE messages.
	void ViewerThreadProcess(cBasicViewer *pViewer);
	
	//! The window Procedure.
	//! Handles all messages sent to the window.
	LRESULT WINAPI ViewerWndProc(HWND hWnd, UINT uMsg,\
								UINT uParam, LONG lParam); 
}

//-----------22 - 7 - 1998-------------------------------------
// Thiago C. Martins
//
//	Class cBasicViewer
//
//!	cBasicViewer is a class that encapsulates the Win32 API that
//!	creates a graphic window and manages it. The window is created inside
//!	a new thread. All the messages to the window are handled by the thread,
//!	in order to prevent the main program to be 'locked' into a message 
//!	loop.
//
class cBasicViewer {

private:

    //! Window related atributes
	HWND hwnd;			//! Handle of the window
	HDC hMemDC;			//! Handle of the memory device context used to
						//! store the window's client area.
	
	const static LPCTSTR ClassName;	//! Keeps the window's class name

	int width, height;	//! Size of the window's client area.
	char title[60];		//! Keeps the title of the window.
	
    //! Thread related atributes
	DWORD ThreadID;		//! Keeps the thread ID.
	
    //! Synchronisation Mutexes
	HANDLE RunMutex;	//! Used by constructor and destructor.
						//! Signals the running state of the thread
	
	HANDLE DCMutex;		//! Synchronizes the access to 
						//! the memory device context

    //! Misc. private members
	static bool Registered;	//! True if the window class has already
							//! been registered.
	
	static HINSTANCE ModuleInstance;	//! Keeps the current module
										//! instance.
	
	static void RegisterViewerClass(void);	//! This method registers 
											//! the window class
protected:

	// Some access functions visible only for derivated classes
	// (we don't want user to have our hwnd)
	HWND GetWindow(void) const { return hwnd; };
	DWORD GetThreadID(void) const { return ThreadID; };
	
public:

	//! Constructor
	cBasicViewer(int Width, int Height, char *title);
	//! Destructor
	~cBasicViewer();

	HDC BeginDraw(void);//! Locks the memory device context
						//! in order to receive GDI calls.
						//! Each call to BeginDraw must have a
						//! corresponding call to EndDraw.

	void EndDraw(void);	//! Releases the memory device context.
						//! This method must ALWAYS be called after
						//! BeginDraw.

	void Show(void);	//! Shows the window.
	void Hide(void);	//! Hides the window.
	void Update(void);	//! Repaints the client area of the window

    // Thread friend functions.
	friend void BVThread::ViewerThreadProcess(cBasicViewer *pViewer);
	friend LRESULT WINAPI BVThread::ViewerWndProc(HWND hWnd, UINT uMsg,\
											UINT uParam, LONG lParam); 

     // Some access functions
	int GetWidth(void) { return width; };
	int GetHeight(void) { return height; };
};


#endif	// _BASICVIEWER_