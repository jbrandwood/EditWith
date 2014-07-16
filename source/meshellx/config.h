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
// Class ID = {762DB251-9E8E-44F5-B505-1770CEFBEB5D}
//

#define MY_COM_CLASS {0x762DB251,0x9E8E,0x44F5,{0xB5,0x05,0x17,0x70,0xCE,0xFB,0xEB,0x5D}}
#define MY_COM_TITLE L"EditWithMultiEdit"

#define MY_WIN_CLASS_STR L"MeXMain"

#define MY_EXE_NAME L"Mew32.exe"

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
 #define MY_EXE_OPTS L"/NoSplash"
#else
 #define MY_EXE_OPTS L"/NI /NS /NoSplash"
#endif

//
// ****************************************************************************
//
