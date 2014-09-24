// ****************************************************************************
// ****************************************************************************
//
// config.h : Customization for this specific shell extension
//
//  Copyright John Brandwood 2014.
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#if _MSC_VER > 1000
#pragma once
#endif

//
// Class ID = {30DB95DC-3D27-4B74-9B8E-44E99A320F06}
//

#define MY_COM_CLASS {0x30DB95DC,0x3D27,0x4B74,{0x9B,0x8E,0x44,0xE9,0x9A,0x32,0x0F,0x06}}
#define MY_COM_TITLE L"EditWithBrackets"

// Brackets requires OLE instead of WM_DROPFILES.
#define USE_OLE_DODRAGDROP

// Brackets will only open a new session if there are no files on the command line.
#define ALWAYS_DROP_ON_2ND

// Brackets doesn't like a drag-and-drop too quickly after a new session starts.
// Give it 4000ms to get it's act together.
#define INITIAL_DROP_DELAY 4000

// N.B. Brackets uses the same window class name as other ChromiumEmbedded applications.
#define MY_WIN_CLASS_STR L"CEFCLIENT"

// Secondary Brackets window are opened up as "CefBrowserWindow"
#define MY_WIN_CLASS_STR_2ND L"CefBrowserWindow"

// Distinguish Brackets from other ChromiumEmbedded applications.
#define MY_WIN_TITLE_STR L"Brackets"

// Final test to ensure that we've got the right existing session window.
#define MY_EXE_IMAGE_CHK

// Brackets does not register itself like Microsoft recommends so try
// looking for it in the (64 or 32 bit) "Program Files" directory.

#define MY_APP_PATH_FIXED L"Brackets\\Brackets.exe"

#define MY_EXE_NAME L"Brackets.exe"

// Define this to disable multiple session support by default.
//
// This setting corresponds to this editor's default settings.
//
// It can be changed for the current user at any time in the registry.
//
// To do this, just run (double-click) on the "edit-single-session.js" or
// "edit-multi-sessions.js" files in this extensions's "build" directory.
//
// #define SINGLE_MY_APP_INSTANCE

#ifdef SINGLE_MY_APP_INSTANCE
 #define MY_EXE_OPTS L""
#else
 #define MY_EXE_OPTS L""
#endif

//
// ****************************************************************************
//
