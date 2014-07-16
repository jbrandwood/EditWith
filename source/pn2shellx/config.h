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
// Class ID = {6BD977B9-96E3-478C-B4EE-1DE777375A1A}
//

#define MY_COM_CLASS {0x6BD977B9,0x96E3,0x478C,{0xB4,0xEE,0x1D,0xE7,0x77,0x37,0x5A,0x1A}}
#define MY_COM_TITLE L"EditWithPN2"

#define MY_WIN_CLASS_STR L"ATL:006AD5B8"

// Final test to ensure that we've got the right existing session window.
#define MY_EXE_IMAGE_CHK

// Programmer's Notepad does not register itself in "App Paths" so look
// for it in the older "Applications" key instead.

#define MY_APP_PATH_KEY L"Applications\\pn.exe\\shell\\edit\\command"

#define MY_EXE_NAME L"pn.exe"

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
 #define MY_EXE_OPTS L"--allowmulti"
#endif

//
// ****************************************************************************
//
