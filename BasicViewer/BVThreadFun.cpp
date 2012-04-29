//
//	file: BVThreadFun.cpp
//	Thiago C. Martins 
//	Last Updated: 22/7/1998
//
//	Implementation of cBasicViewer thread functions
//
//	ViewerThreadProcess:	Creates the window and enters a message loop.
//							Controled by the TH_MESSAGE messages.
//
//	ViewerWndProc:	Handles	all messages sent to the viewer window
//

#include "cBasicViewer.h"	// Declaration of cBasicViewer and BVThread namespace
#include <windows.h>		// Win32 API main header

// ViewerThreadProcess function
// Author: Thiago C. Martins
// last updated: 22/7/98
//
//!	This is the main function of the thread created by cBasicViewer objects.
//!	Take as parameter a pointer to the object that created the thread
//!	It creates the viewer window and a memory device contex that receives the
//!	user GDI calls
//
void BVThread::ViewerThreadProcess(cBasicViewer *pViewer)
{
	int width, height;		// keeps the size of the window's client area
	int DeltaW, DeltaH;		// keeps the offset of the size of the window
							// required by the borders and caption
	HWND hwnd;		// Stores the handle of the viewer window
	MSG msg;		// Keeps a message to be handled
	HDC hdc;		// Device context of the window's clent area
	HBITMAP hbmp;	// Handle of the bitmap selected into the memory device context
	
	pViewer->ThreadID=GetCurrentThreadId();		// Retrieves our id in order
												// to allow the viewer to
												// send us messages.
	
	// Startup: Creates the window and the memory device context
	
	// Get the dimensions of the window's client area
	width  = pViewer->width;
	height = pViewer->height;
	
	// Calculates the offsets required for the borders and caption area
	DeltaW = 2*GetSystemMetrics(SM_CXFIXEDFRAME);
	DeltaH = 2*GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYCAPTION);

	// Checks if the window class has been registered. If not, register it
	if(!cBasicViewer::Registered) cBasicViewer::RegisterViewerClass(); 
	
	// Creates the window
	hwnd=CreateWindow(cBasicViewer::ClassName, pViewer->title, 
		BV_WNDSTYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		width+DeltaW, height+DeltaH, (HWND) NULL, 
		(HMENU) NULL, NULL, (LPVOID) NULL);
	pViewer->hwnd = hwnd;
	// Stores the pointer to the viewer into the GWL_USERDATA
	// of the window. Now, the ViewerWndProc can retrieve the viewer
	// associated with the window.
	SetWindowLong(hwnd, GWL_USERDATA, (LONG) pViewer);
	
	// Gets the device context of the window's client area
	// As the window syle have CS_OWNDC, we don't have to call
	// ReleaseDC()
	hdc=GetDC(hwnd);
	
	// Gets the DCMutex. Now nobody can draw on the memory device context
	WaitForSingleObject(pViewer->DCMutex, INFINITE);
	// Creates the memory device contex, compatible with the device context of
	// the window's client area
	pViewer->hMemDC=CreateCompatibleDC(hdc);
	// Creates a bitmap to stores the client area of the window
	hbmp=CreateCompatibleBitmap(hdc,width,height);
	// Selects the bitmap into the device context
	SelectObject(pViewer->hMemDC,hbmp);
	
	// Now, the DCMutex can be released. The object can draw on the memory device context
	ReleaseMutex(pViewer->DCMutex);
	
	// Startup finished!
	
	// Gets the RunMutex
	WaitForSingleObject(pViewer->RunMutex, INFINITE);
	
	// Enter the message loop
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);		
		if(msg.message==TH_MESSAGE) switch(msg.wParam) {
				// Boss has something to us, handle the message
			
				// TH_CHECK message: used for comunication checking
				case TH_CHECK:	
					// We're doing just fine boss, thanks		
					break;

				// TH_DIE message: used for shuting us down!
				case TH_DIE:	
					PostQuitMessage(0);	// uh-oh
					break;

				// TH_SHOW message: used for exhibition of the window
				case TH_SHOW:
					ShowWindow(pViewer->hwnd, SW_SHOW);	// Sets the show state of
														// the window to SW_SHOW
					break;

				// TH_HIDE message: used for hiding the window
				case TH_HIDE:
					ShowWindow(pViewer->hwnd, SW_HIDE);	// Sets the show state of
														// the window to SW_HIDE
					break;

				// TH_UPDATE message: used for repainting the window
				case TH_UPDATE: 
					{
						RECT rect;
						rect.top = rect.left = 0;
						rect.right = width;
						rect.bottom = height;
						InvalidateRect(hwnd,&rect, false);	// Invalidates the client area
					}	// System will now send a WM_PAINT message
					break;
		}
		else DispatchMessage(&msg);	// The message wasn't for us
	}
	
// Shutdown: Destroys the window and the memory device context
	
	// Destroys window
	if(pViewer->hwnd) DestroyWindow(pViewer->hwnd);
	// Gets the DCMutex for the last time
	WaitForSingleObject(pViewer->DCMutex, INFINITE);
	// Destroys the memory device context
	if(pViewer->hMemDC) DeleteDC(pViewer->hMemDC);
	// Destroys the bitmap
	if(hbmp) DeleteObject(hbmp);
	
	ReleaseMutex(pViewer->RunMutex); // Time to die
}

// ViewerWndProc function
// Author: Thiago C. Martins
// last updated: 22/7/98
//
//!	This is the viewer window function. It handles currently only
//!	the WM_PAINT and WM_CLOSE messages. All other messages are
//!	repassed to DefWndProc.
//!
//!	On WM_PAINT messages, the function will try to get the DCMutex
//!	to lock the memory device context. If it succeeds, the function
//!	redraws the client area of the window. If not, the function
//!	does nothig. The system will keep sending WM_PAINT until we can
//!	repaint the window
//!
//!	On WM_CLOSE messages, the function just hides the window. We don't
//!	want to destroy the window now, because a TH_SHOW message to the
//!	thread would requires the recreation of the window
//
LRESULT WINAPI BVThread::ViewerWndProc(HWND hWnd, UINT uMsg,\
									UINT uParam, LONG lParam)
{
	LONG lRet = 1;	// Return value of the function
					// zero if don't handled the message, nonzero otherwise
	
	cBasicViewer *Viewer;	// Points to the viewer associated with the window
	int width, height;		// Dimensions of the window
	
	PAINTSTRUCT ps;			// Struct filled by BeginPaint
	HDC hdc;				// Device context of the window's update region
	
	switch (uMsg) {
	
	// WM_PAINT messages:
	// We just copy the memory device context into the update region
	case WM_PAINT:
		// Retrieves the viewer associated with the window
		Viewer = (cBasicViewer *)GetWindowLong(hWnd, GWL_USERDATA);
		if(!Viewer) {	// If the pointer is invalid, repasses the message.
			lRet=DefWindowProc(hWnd, uMsg, uParam, lParam);
			break;
		}
		// Try to get the DCMutex
		// If we can't get it, there's nothing we can do here.
		// In this case, system will keep sending us WM_PAINT messages
		// until we get the DCMutex and repaint the window
		if(WaitForSingleObject(Viewer->DCMutex, 0)!=WAIT_TIMEOUT) {
			// If we can get it, repaint the window
			
			// First, get the window dimensions
			width = Viewer->width;
			height = Viewer->height;
			
			// Get the device context of the update region
			hdc=BeginPaint(hWnd, &ps);
			
			// Copy the memory DC into the update region
			BitBlt(hdc,0,0,width,height,Viewer->hMemDC,0,0,SRCCOPY);
			
			// Releases the device context of the update region
			EndPaint(hWnd, &ps);
			
			// Releases the DCMutex
			ReleaseMutex(Viewer->DCMutex);
		}
		break;

	// WM_CLOSE messages:
	// We don't want the window to be actually closed, so we just HIDE the window
	case WM_CLOSE:
		// Sets the show state of the window to SW_HIDE
		ShowWindow(hWnd, SW_HIDE);
		break;
		
	// Repasses all other messages to DefWindowProc
	default:
		lRet = DefWindowProc(hWnd, uMsg, uParam, lParam);
		break;
	}
	return lRet;
}
