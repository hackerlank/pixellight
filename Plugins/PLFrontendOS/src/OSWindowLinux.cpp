/*********************************************************\
 *  File: OSWindowLinux.cpp                              *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <string.h>
#include <signal.h>
#include "PLFrontendOS/Frontend.h"
#include "PLFrontendOS/OSWindowLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendOS {


//[-------------------------------------------------------]
//[ Ugly global variables                                 ]
//[-------------------------------------------------------]
bool g_bSignalSystemQuit = false;	/**< Does the OS asks us to shut down? */


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
OSWindowLinux::OSWindowLinux(Frontend &cFrontendOS) :
	m_pFrontendOS(&cFrontendOS),
	m_pDisplay(XOpenDisplay(nullptr)),
	m_wmDelete(XInternAtom(m_pDisplay, "WM_DELETE_WINDOW", True)),
	m_nNativeWindowHandle(NULL_HANDLE)
{
	// Tell the frontend about this instance at once because it may already be required during frontend lifecycle initialization
	m_pFrontendOS->m_pOSWindow = this;

	// Connect Linux signals
	signal(SIGINT,  OSWindowLinux::SignalHandler);
	signal(SIGTERM, OSWindowLinux::SignalHandler);

	{ // Create the native OS window
		const unsigned int  nWidth  = 640;
		const unsigned int  nHeight = 480;
		const int           nScreen = DefaultScreen(m_pDisplay);
			  Visual       *pVisual = DefaultVisual(m_pDisplay, nScreen);
		const int           nDepth  = DefaultDepth(m_pDisplay, nScreen);
	
		// X events
		XSetWindowAttributes sXSetWindowAttributes;
		sXSetWindowAttributes.event_mask = ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask | VisibilityChangeMask;

		// Create the native OS window instance
		m_nNativeWindowHandle = XCreateWindow(m_pDisplay, XRootWindow(m_pDisplay, nScreen), 0, 0, nWidth, nHeight, 0, nDepth, InputOutput, pVisual, CWEventMask, &sXSetWindowAttributes);
		XSetWMProtocols(m_pDisplay, m_nNativeWindowHandle, &m_wmDelete, 1);
		XTextProperty sXTextProperty;
		sXTextProperty.value    = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>("PLFrontendOS_OSWindowLinuxX11"));
		sXTextProperty.encoding = XA_STRING;
		sXTextProperty.format   = 8;
		sXTextProperty.nitems   = strlen(reinterpret_cast<const char*>(sXTextProperty.value));
		XSetWMName(m_pDisplay, m_nNativeWindowHandle, &sXTextProperty);
		XMapRaised(m_pDisplay, m_nNativeWindowHandle);
	}

	// Do the frontend lifecycle thing - start
	m_pFrontendOS->OnStart();
}

/**
*  @brief
*    Destructor
*/
OSWindowLinux::~OSWindowLinux()
{
	// Check if the window has already been destroyed
	if (m_nNativeWindowHandle) {
		// Send destroy message to window
		XDestroyWindow(m_pDisplay, m_nNativeWindowHandle);
	}
}


//[-------------------------------------------------------]
//[ Private virtual OSWindow functions                    ]
//[-------------------------------------------------------]
handle OSWindowLinux::GetNativeWindowHandle() const
{
	return static_cast<handle>(m_nNativeWindowHandle);
}

void OSWindowLinux::Redraw()
{
	if (m_nNativeWindowHandle) {
		// Send expose event
		XEvent sEvent;
		sEvent.type			 = Expose;
		sEvent.xany.window	 = m_nNativeWindowHandle;
		sEvent.xexpose.count = 0;
		XSendEvent(m_pDisplay, m_nNativeWindowHandle, False, 0, &sEvent);

		// Do it!
		XSync(m_pDisplay, False);
	}
}

bool OSWindowLinux::Ping()
{
	bool bQuit = false;

	// Look if messages are waiting (non-blocking)
	while (XPending(m_pDisplay) > 0) {
		// Get the waiting message
		XEvent sXEvent;
		XNextEvent(m_pDisplay, &sXEvent);

		// Process message
		switch (sXEvent.type) {
			case Expose:
				// There could be more then one Expose event currently in the event loop.
				// To avoid too many redraw calls, call OnDraw only when the current processed Expose event is the last one.
				if (!sXEvent.xexpose.count)
					m_pFrontendOS->OnDraw();
				break;

			case DestroyNotify:
				// Do the frontend lifecycle thing - stop
				m_pFrontendOS->OnStop();

				// Mark window destroyed
				m_nNativeWindowHandle = NULL_HANDLE;
				bQuit = true;
				break;

			case UnmapNotify:
			case FocusOut:
				m_pFrontendOS->OnPause();
				break;

			case MapNotify:
			case FocusIn:
				m_pFrontendOS->OnResume();
				break;

			case ClientMessage:
				if (sXEvent.xclient.data.l[0] == m_wmDelete)
					bQuit = true;
				break;
		}
	}

	// Let the frontend update it's states
	if (!bQuit && !g_bSignalSystemQuit)
		m_pFrontendOS->OnUpdate();

	// Done
	return (bQuit || g_bSignalSystemQuit);
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Signal handler callback
*/
void OSWindowLinux::SignalHandler(int nSignal)
{
	// Catch signal
	switch (nSignal) {
		// Interrupt (exit application by ctrl-c)
		case SIGINT:
			// The OS asks us to shut down
			g_bSignalSystemQuit = true;

			// Signal handler has done it's job, re-raise signal
			signal(nSignal, SIG_DFL);
			raise(nSignal);
			break;

		// Terminate (exit application)
		case SIGTERM:
			// The OS asks us to shut down
			g_bSignalSystemQuit = true;

			// Signal handler has done it's job, re-raise signal
			signal(nSignal, SIG_DFL);
			raise(nSignal);
			break;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendOS

