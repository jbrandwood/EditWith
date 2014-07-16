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
// Class ID = {36B40FAF-667E-4B19-8F41-DFEB32A370DC}
//

#define MY_COM_CLASS {0x36B40FAF,0x667E,0x4B19,{0x8F,0x41,0xDF,0xEB,0x32,0xA3,0x70,0xDC}}
#define MY_COM_TITLE L"EditWithSublimeText"

#define MY_WIN_CLASS_STR L"PX_WINDOW_CLASS"

// Sublime Text does not register itself like Microsoft recommends so try
// looking for it in the (64 or 32 bit) "Program Files" directory.

#define MY_APP_PATH_FIXED L"Sublime Text 3\\sublime_text.exe"

#define MY_EXE_NAME L"sublime_text.exe"

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
 #define MY_EXE_OPTS L"-n"
#endif

// Sublime Text opens 2 windows if started with "-n" when no instance is running.
#define SKIP_OPTS_IF_NO_INSTANCE

//
// ****************************************************************************
//
