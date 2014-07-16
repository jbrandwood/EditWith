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
// Class ID = {2044570C-9710-4C0B-9107-87C082C37B6E}
//

#define MY_COM_CLASS {0x2044570C,0x9710,0x4C0B,{0x91,0x07,0x87,0xC0,0x82,0xC3,0x7B,0x6E}}
#define MY_COM_TITLE L"EditWithUltraEdit"

// N.B. UltraEdit uses a dynamic MFC window class name that can (and does) change.
// See MSDN article "TN070: MFC Window Class Names".
#define MY_WIN_CLASS_STR L"Afx:"

// The actual numbers can change with every new window, so limit the test length.
#define MY_WIN_CLASS_LEN 4

// Distinguish UltraEdit from UEStudio/EditPlus
#define MY_WIN_TITLE_STR L"UltraEdit"

// Final test to ensure that we've got the right existing session window.
#define MY_EXE_IMAGE_CHK

#define MY_EXE_NAME L"Uedit32.exe"

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
 #define MY_EXE_OPTS L"/fni"
#endif

//
// ****************************************************************************
//
