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
// Class ID = {7B245014-8540-4190-936A-D0B89838CC40}
//

#define MY_COM_CLASS {0x7B245014,0x8540,0x4190,{0x93,0x6A,0xD0,0xB8,0x98,0x38,0xCC,0x40}}
#define MY_COM_TITLE L"EditWithEmEditor"

#define MY_WIN_CLASS_STR L"EmEditorMainFrame3"

// EmEditor does not register itself in "App Paths" so look for it in the
// older "Applications" key instead.

#define MY_APP_PATH_KEY L"Applications\\EmEditor.exe\\shell\\edit\\command"

#define MY_EXE_NAME L"EmEditor.exe"

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
 #define MY_EXE_OPTS L"/sp"
#endif

#define GET_ICON_FROM_MY_EXE

//
// ****************************************************************************
//
