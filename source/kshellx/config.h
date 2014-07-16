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
// Class ID = {7F38663E-04FA-4C75-8FAA-B5A73C8B9131}
//

#define MY_COM_CLASS {0x7F38663E,0x04FA,0x4C75,{0x8F,0xAA,0xB5,0xA7,0x3C,0x8B,0x91,0x31}}
#define MY_COM_TITLE L"EditWithKomodo"

// Komodo requires OLE instead of WM_DROPFILES.
#define USE_OLE_DODRAGDROP

// N.B. Komodo uses the same window class name as other Mozilla applications.
#define MY_WIN_CLASS_STR L"MozillaWindowClass"

// Distinguish Komodo Edit / Komodo IDE from other Mozilla applications.
#define MY_WIN_TITLE_STR L"Komodo "

// Final test to ensure that we've got the right existing session window.
#define MY_EXE_IMAGE_CHK

#define MY_EXE_NAME L"komodo.exe"

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

// Komodo opens 2 windows if started with "-n" when no instance is running.
#define SKIP_OPTS_IF_NO_INSTANCE

//
// ****************************************************************************
//
