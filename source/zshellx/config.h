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
// Class ID = {8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0}
//

#define MY_COM_CLASS {0x8A854BCF,0xE3A3,0x4173,{0xA1,0xEB,0xD2,0xFD,0x9B,0xE2,0x85,0xF0}}
#define MY_COM_TITLE L"EditWithZeus"

#define MY_WIN_CLASS_STR L"ZeusIDE"

#define MY_EXE_NAME L"zeus.exe"

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
 #define MY_EXE_OPTS L"-s -n -vn-"
#endif

// For Zeus 3.97s or below use ...
//
// #define MY_WINDOW_CLASS_STR L"xMDIFrame"
// #define MY_EXE_OPTS L"-s -n"

//
// ****************************************************************************
//
