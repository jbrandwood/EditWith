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
// Class ID = {1D1E7DAD-E970-4D20-9843-908C9CF89CE2}
//

#define MY_COM_CLASS {0x1D1E7DAD,0xE970,0x4D20,{0x98,0x43,0x90,0x8C,0x9C,0xF8,0x9C,0xE2}}
#define MY_COM_TITLE L"EditWithNPP"

#define MY_WIN_CLASS_STR L"Notepad++"

#define MY_EXE_NAME L"notepad++.exe"

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
 #define MY_EXE_OPTS L"-multiInst -nosession"
#endif

// Notepad++ only accepts drag-n-drop within an actual edit window.
#define DROP_ON_CHILD_WINDOW L"Scintilla"

//
// ****************************************************************************
//
