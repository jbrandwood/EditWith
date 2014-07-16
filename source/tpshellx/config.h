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
// Class ID = {4A169362-D7BC-4428-9E21-D998BD37807F}
//

#define MY_COM_CLASS {0x4A169362,0xD7BC,0x4428,{0x9E,0x21,0xD9,0x98,0xBD,0x37,0x80,0x7F}}
#define MY_COM_TITLE L"EditWithTextPad"

#define MY_WIN_CLASS_STR L"TextPad7"

#define MY_EXE_NAME L"TextPad.exe"

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
 #define MY_EXE_OPTS L"-q"
#else
 #define MY_EXE_OPTS L"-q -m"
#endif

//
// ****************************************************************************
//
