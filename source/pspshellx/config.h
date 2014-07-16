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
// Class ID = {ED90173A-3B4C-4E7E-B9CF-79714425D4B5}
//

#define MY_COM_CLASS {0xED90173A,0x3B4C,0x4E7E,{0xB9,0xCF,0x79,0x71,0x44,0x25,0xD4,0xB5}}
#define MY_COM_TITLE L"EditWithPSPad"

#define MY_WIN_CLASS_STR L"TfPSPad.UnicodeClass"

#define MY_EXE_NAME L"PSPad.exe"

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
 #define MY_EXE_OPTS L"/N"
#endif

#define MY_HEX_OPTS L" /H"

#define MY_DIFF_OPTS L" /D"
#define MAXIMUM_DIFF 2

// PSPad will not start a 2nd session with no files on the command line.
#define NO_INITIAL_DROP_ON_2ND

//
// ****************************************************************************
//
