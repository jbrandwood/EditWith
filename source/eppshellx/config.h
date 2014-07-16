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
// Class ID = {E8D5FF73-D41E-41CD-9779-E3B05DC5BA1F}
//

#define MY_COM_CLASS {0xE8D5FF73,0xD41E,0x41CD,{0x97,0x79,0xE3,0xB0,0x5D,0xC5,0xBA,0x1F}}
#define MY_COM_TITLE L"EditWithEditPad"

// EditPad Pro requires OLE instead of WM_DROPFILES.
#define USE_OLE_DODRAGDROP

#define MY_WIN_CLASS_STR L"TFormEditPadPro7"

#define MY_EXE_NAME L"EditPadPro7.exe"

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
 #define MY_EXE_OPTS L"/newinstance"
#endif

//
// ****************************************************************************
//
