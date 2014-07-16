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
// Class ID = {6A06A71E-CFFA-444C-B347-1B7898C9AB7B}
//

#define MY_COM_CLASS {0x6A06A71E,0xCFFA,0x444C,{0xB3,0x47,0x1B,0x78,0x98,0xC9,0xAB,0x7B}}
#define MY_COM_TITLE L"EditWithSlickEdit"

// SlickEdit requires OLE instead of WM_DROPFILES.
#define USE_OLE_DODRAGDROP

// N.B. SlickEdit uses the same generic window class name as other QT applications.
#define MY_WIN_CLASS_STR L"QWidget"

// Distinguish SlickEdit from other QT applications.
#define MY_WIN_TITLE_STR L"SlickEdit"

// Final test to ensure that we've got the right existing session window.
#define MY_EXE_IMAGE_CHK

#define MY_EXE_NAME L"vs.exe"

// Define this to disable multiple session support by default.
//
// This setting corresponds to this editor's default settings.
//
// It can be changed for the current user at any time in the registry.
//
// To do this, just run (double-click) on the "edit-single-session.js" or
// "edit-multi-sessions.js" files in this extensions's "build" directory.
//
#define SINGLE_MY_APP_INSTANCE

#ifdef SINGLE_MY_APP_INSTANCE
 #define MY_EXE_OPTS L""
#else
 // This only works properly if the user disables SlickEdit's options
 // AutoRestoreFiles and AutoRestoreWorkspace.
 #define MY_EXE_OPTS L"+new"
#endif

// http://community.slickedit.com/index.php/topic,9835.0.html
//
// The SlickEdit Community Forum suggests using ...
//
// #define MY_EXE_OPTS L"+new -snorestore -snoconfig"
//
// But that set of options screws up drag-and-drop on my
// SlickEdit 2012.

// SlickEdit will not start a 2nd session with no files on the command line.
#define NO_INITIAL_DROP_ON_2ND

//
// ****************************************************************************
//
